package gr.iti.gnorasi.rules.fuzzy;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

import gr.iti.gnorasi.rules.fuzzy.FuzzyRestriction.Operator;
import gr.iti.gnorasi.rules.geo.GeoRule;
import gr.iti.gnorasi.rules.geo.GeoRuleHead;
import gr.iti.gnorasi.rules.geo.GeoRulePredicate;
import gr.iti.gnorasi.rules.geo.RuleBuilder;
import gr.iti.gnorasi.rules.geo.GeoRuleTerm.TargetImage;
import gr.iti.gnorasi.utils.Constants;
import gr.iti.gnorasi.utils.XMLDoc;

import org.apache.log4j.Logger;
import org.openrdf.model.Literal;
import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;
import org.openrdf.model.ValueFactory;
import org.openrdf.model.impl.ValueFactoryImpl;
import org.openrdf.model.vocabulary.RDF;
import org.openrdf.query.BindingSet;
import org.openrdf.query.QueryLanguage;
import org.openrdf.query.TupleQuery;
import org.openrdf.query.TupleQueryResult;
import org.openrdf.repository.RepositoryConnection;
import org.openrdf.repository.RepositoryException;
import org.openrdf.repository.RepositoryResult;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public class FuzzyToConstruct {
	//class usage:
	//input: user-defined restrictions for class memberships, e.g. Region is Water if ndvi index is greater than 0.7 or
	//		 Region is Water if NDVI index is greater than 0.5 and Blue Band Mean is greater than 0.8	
	
	private static Logger logger = Logger.getLogger(FuzzyToConstruct.class);
	List<FuzzyRule> list;
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		FuzzyToConstruct f = new FuzzyToConstruct();
		try {
			f.parseRules(new FileInputStream("fuzzy/fuzzy_v2.xml"));
			//for (FuzzyRule fr : f.getList()) {
			//	System.out.println(fr.toString());
			//}
			f.exec(null, "");
			
		}
		catch (Exception e) {
			e.printStackTrace();
		}
	}

	public FuzzyToConstruct() {
		list = new ArrayList<FuzzyRule>();
	}
	
	public List<FuzzyRule> getList() {
		return list;
	}
	
	public void exec(RepositoryConnection con, String context) {
		URI uContext = null;
		if (!context.equals("")) {
			uContext = con.getValueFactory().createURI(context);
		}
		
		for (FuzzyRule fuzzyRule : list) {
			String values = "";
			for (int i=0;i<fuzzyRule.getRestrictionList().size();i++)
				values += "?value"+i+" ";
			
			String query = "SELECT ?region "+values+" WHERE {\n";
			int i=0;
			for (FuzzyRestriction fres : fuzzyRule.getRestrictionList()) {
				query += "?region <"+ fres.getProperty()+"> ?value"+i+" . \n";
				i++;
			}
			i=0;
			query += "FILTER(";
			for (FuzzyRestriction fres : fuzzyRule.getRestrictionList()) {
				if (i==0) {
					//query += "(?value"+i+" "+ fres.getOperator().getSymbol()+" \""+fres.getThreshold()+"\"^^<http://www.w3.org/2001/XMLSchema#float>) ";
					query += "(?value"+i+" "+ fres.getOperator().getSymbol()+" "+fres.getThreshold()+") ";
				}
				else {
					//query += "&& (?value"+i+" "+ fres.getOperator().getSymbol()+" \""+fres.getThreshold()+"\"^^<http://www.w3.org/2001/XMLSchema#float>) ";
					query += "&& (?value"+i+" "+ fres.getOperator().getSymbol()+" "+fres.getThreshold()+") ";
				}
				i++;
			}
			query += ")}";
			System.out.println(query);
			
			try {
				List<URI> listSuper = getSuperClassesInstances(con, fuzzyRule.getHead());
				TupleQuery tupleQuery = con.prepareTupleQuery(QueryLanguage.SPARQL, Constants.queryPrefix+query);
				TupleQueryResult result = tupleQuery.evaluate();
				while (result.hasNext()) {
					BindingSet bindingSet = result.next();
					for (URI entry : listSuper) {
						URI region = (URI)bindingSet.getValue("region");
						int count = con.getStatements(region, Constants.depictsProperty, null, false, uContext).asList().size();
						URI depiction = con.getValueFactory().createURI(Constants.mainNS+region.getLocalName()+"_depiction"+count);
						Literal conf = con.getValueFactory().createLiteral(setConfidence(bindingSet, fuzzyRule));
						if (uContext != null) {
							con.add(depiction, RDF.TYPE, con.getValueFactory().createURI(Constants.mainNS+"Depiction"), uContext);
							con.add(region, Constants.depictsProperty, depiction, uContext);
							con.add(depiction, Constants.depictsObjectProperty, entry, uContext);
							con.add(depiction, Constants.withConfidenceProperty, conf, uContext);
						}
						else {
							con.add(depiction, RDF.TYPE, con.getValueFactory().createURI(Constants.mainNS+"Depiction"));
							con.add(region, Constants.depictsProperty, depiction);
							con.add(depiction, Constants.depictsObjectProperty, entry);
							con.add(depiction, Constants.withConfidenceProperty, conf);
						}
					}
					con.commit();
				}
			}catch (Exception e) {
				e.printStackTrace();
			}
		}
	}
	
	public float setConfidence(BindingSet bindingSet, FuzzyRule fuzzyRule) {
		float ret = 0;
		float vals[] = new float[bindingSet.size()-1];
		try {
			for (int i=0;i<fuzzyRule.getRestrictionList().size();i++) {
				vals[i] = Float.parseFloat(bindingSet.getValue("value"+i).stringValue());
			}
			if (fuzzyRule.getOp().equals("min")) {
				ret = 2;
				for (int i=0;i<vals.length;i++) {
					if (vals[i] < ret)
						ret = vals[i];
				}
			}
			else { // good for "max" and any other
				ret = -1;
				for (int i=0;i<vals.length;i++) {
					if (vals[i] > ret)
						ret = vals[i];
				}
			}
		}catch (NumberFormatException e) {
			e.printStackTrace();
		}
		
		return ret;
	}
	
	public void depictSuperClasses(RepositoryConnection con, URI regionID, URI depictionClass) {
		try {
			//get the direct instance of the depiction class
			RepositoryResult<Statement> stmt = con.getStatements(null, RDF.TYPE, depictionClass, false);
			URI regionDepiction = null;
			if (stmt.hasNext()) {
				regionDepiction = (URI)stmt.next().getSubject();
			}
			if (regionDepiction != null) {
				RepositoryResult<Statement> stmts = con.getStatements(regionDepiction, RDF.TYPE, null, true);
				while (stmts.hasNext()) {
					Statement st = stmts.next();
					URI type = (URI)st.getObject();
					RepositoryResult<Statement> superClassInstances = con.getStatements(null, RDF.TYPE, type, false);
					while (superClassInstances.hasNext()) {
						Statement st1 = superClassInstances.next();
						con.add(regionID, Constants.depictsObjectProperty, st1.getSubject());
					}
				}
				con.commit();
			}
		} catch (RepositoryException e) {
			e.printStackTrace();
		}
	}
	
	public List<URI> getSuperClassesInstances(RepositoryConnection con, URI depictionClass) {
		List<URI> list = new ArrayList<URI>();
		try {
			if (depictionClass != null) {
				RepositoryResult<Statement> stmt = con.getStatements(null, RDF.TYPE, depictionClass, false);
				while (stmt.hasNext()) {
					Resource r = stmt.next().getSubject();
					System.out.println("super instance is: "+r+" from class "+depictionClass);
					if (r instanceof URI) {
						URI regionDepiction = (URI)r;
						list.add(regionDepiction);
					}
				}
			}
			/*
			//get the direct instance of the depiction class
			RepositoryResult<Statement> stmt = con.getStatements(null, RDF.TYPE, depictionClass, false);
			URI regionDepiction = null;
			if (stmt.hasNext()) {
				regionDepiction = (URI)stmt.next().getSubject();
			}
			if (regionDepiction != null) {
				RepositoryResult<Statement> stmts = con.getStatements(regionDepiction, RDF.TYPE, null, true);
				while (stmts.hasNext()) {
					Statement st = stmts.next();
					URI type = (URI)st.getObject();
					RepositoryResult<Statement> superClassInstances = con.getStatements(null, RDF.TYPE, type, false);
					while (superClassInstances.hasNext()) {
						Statement st1 = superClassInstances.next();
						list.add((URI)st1.getSubject());
					}
				}
				return list;
			}*/
		} catch (RepositoryException e) {
			e.printStackTrace();
		}
		
		return list;
	}
	
	
	public void parseRules(InputStream stream) {
		XMLDoc doc = new XMLDoc(stream);
		ValueFactory f = new ValueFactoryImpl();
		
		NodeList rulesList = doc.getXPathNodes("/fuzzy/fuzzyRule");
		String ruleID = "";
		
		for (int i=0;i<rulesList.getLength();i++) {
			try {
				Node rule = rulesList.item(i);
				
				FuzzyRule fuzzyRule = new FuzzyRule();
				if (rule.getAttributes().getNamedItem("id") != null) {
					ruleID = rule.getAttributes().getNamedItem("id").getNodeValue();
					fuzzyRule.setId(ruleID);
				}
				if (rule.getAttributes().getNamedItem("operator") != null) {
					String ruleOp = rule.getAttributes().getNamedItem("operator").getNodeValue();
					fuzzyRule.setOp(ruleOp);
				}
								
				//get body and head
				NodeList bodyAndHead = rule.getChildNodes();
				for (int j=0;j<bodyAndHead.getLength();j++) {
					Node node = bodyAndHead.item(j);
					if (node.getNodeName().equals("body")) {
						NodeList bodyNodes = node.getChildNodes();
						for (int k=0;k<bodyNodes.getLength();k++) {
							Node bodyNode = bodyNodes.item(k);
							if (bodyNode.getNodeName().equals("restriction")) {
								FuzzyRestriction fr = new FuzzyRestriction();
								fr.setProperty(f.createURI(Constants.userNS + getFuzzyProperty(bodyNode.getAttributes().getNamedItem("property").getNodeValue())));
								fr.setThreshold(Double.parseDouble(bodyNode.getAttributes().getNamedItem("value").getNodeValue()));
								fr.setOperator(Operator.valueOf(bodyNode.getAttributes().getNamedItem("operator").getNodeValue().toUpperCase()));
								
								fuzzyRule.getRestrictionList().add(fr);
							}
						}
					}
					else if (node.getNodeName().equals("head")) {
						if (node.getAttributes().getNamedItem("class") != null) {
							fuzzyRule.setHead(f.createURI(Constants.userNS+node.getAttributes().getNamedItem("class").getNodeValue()));
						}
						/*NodeList headNodes = node.getChildNodes();
						for (int k=0;k<headNodes.getLength();k++) {
							Node headNode = headNodes.item(k);
							if (headNode.getNodeName().equals("restriction")) {
								fuzzyRule.setHead(f.createURI(Constants.userNS+headNode.getAttributes().getNamedItem("class").getNodeValue()));
							}
						}*/
					}
				}
				list.add(fuzzyRule);
			}
			catch (IllegalStateException e) {
				logger.error("DEFINITON ERROR: "+e.getMessage() + ". Fuzzy rule '"+ruleID+"' skipped.");
			}
			catch (Exception e) {
				logger.error("PARSING ERROR: "+e.getMessage() + ". Fuzzy rule '"+ruleID+"' skipped.");
			}
		}
		
		//Utilities.printList(predicateList);
	}
	
	public String getFuzzyProperty(String nodeValue) {
		String ret = "";
		
		String parts[] = nodeValue.split("::");
		if (parts.length > 2) {
			ret = parts[2]+parts[1];
		}
		
		return ret;
	}

	
	/*public void parseRestrictions(InputStream stream) {
		try {
			XMLDoc doc = new XMLDoc(stream);
			ValueFactory f = new ValueFactoryImpl();
			
			NodeList restrictionsList = doc.getXPathNodes("/fuzzy/restriction");
			for (int i=0;i<restrictionsList.getLength();i++) {
				try {
					Node restriction = restrictionsList.item(i);
					
					FuzzyRestriction fuzzyR = new FuzzyRestriction();
					
					if (restriction.getAttributes().getNamedItem("class") != null) {
						String cl = restriction.getAttributes().getNamedItem("class").getNodeValue();
						fuzzyR.setClass_(f.createURI(Constants.userNS+cl));
					}
					if (restriction.getAttributes().getNamedItem("property") != null) {
						String prop = restriction.getAttributes().getNamedItem("property").getNodeValue();
						fuzzyR.setProperty(f.createURI(Constants.mainNS+prop));
					}
					if (restriction.getAttributes().getNamedItem("value") != null) {
						String val = restriction.getAttributes().getNamedItem("value").getNodeValue();
						fuzzyR.setThreshold(Double.valueOf(val).doubleValue());
					}
					if (restriction.getAttributes().getNamedItem("operator") != null) {
						String val = restriction.getAttributes().getNamedItem("operator").getNodeValue();
						fuzzyR.setOperator(Operator.valueOf(val.toUpperCase()));
					}
					
					list.add(fuzzyR);
				}
				catch (Exception e) {
					logger.error("Error in fuzzy restriction entry "+i);
					e.printStackTrace();
				}
			}
		} catch (Exception e) {
			logger.error("Fuzzy restrictions error");
			e.printStackTrace();
		}
	}*/
	
	
}
