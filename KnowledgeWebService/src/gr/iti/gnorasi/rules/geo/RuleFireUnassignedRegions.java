package gr.iti.gnorasi.rules.geo;

import gr.iti.gnorasi.utils.Constants;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import javax.sql.DataSource;

import org.apache.log4j.Logger;
import org.openrdf.model.Literal;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;
import org.openrdf.model.Value;
import org.openrdf.model.ValueFactory;
import org.openrdf.model.impl.ValueFactoryImpl;
import org.openrdf.model.vocabulary.RDF;
import org.openrdf.query.BindingSet;
import org.openrdf.query.BooleanQuery;
import org.openrdf.query.QueryLanguage;
import org.openrdf.query.TupleQuery;
import org.openrdf.query.TupleQueryResult;
import org.openrdf.repository.RepositoryConnection;
import org.openrdf.repository.RepositoryException;
import org.openrdf.repository.RepositoryResult;

public class RuleFireUnassignedRegions {

	private static Logger logger = Logger.getLogger(RuleFireUnassignedRegions.class);
	
	public int countStatements(RepositoryResult<Statement> st) throws RepositoryException {
		return st.asList().size();
	}
	
	public void fireAllIteration(List<GeoRule> rules, DataSource ds, RepositoryConnection con, URI context) {
		try {
			ValueFactory f = new ValueFactoryImpl();
			URI depictsObject = f.createURI(Constants.mainNS+Constants.depictsObject);
			int counter = 0;
			while (counter < 10) {
				long statementsNum = con.getStatements(null, depictsObject, null, false).asList().size();
				long start = System.currentTimeMillis();
				fireAll(rules,ds,con, context);
				long end = System.currentTimeMillis();
				System.out.println("Iteration "+(counter+1)+" took "+(end-start)+"ms");
				long newStatementsNum = con.getStatements(null, depictsObject, null, false).asList().size();
				if (newStatementsNum == statementsNum)
					break;
				counter++;
			}
		} catch (RepositoryException e) {
			e.printStackTrace();
		}
	}
	
	public void fireAll(List<GeoRule> rules, DataSource ds, RepositoryConnection con, URI context) {
		ValueFactory f = new ValueFactoryImpl();
		URI depictsObject = f.createURI(Constants.mainNS+Constants.depictsObject);
		URI depicts = f.createURI(Constants.mainNS+Constants.depicts);
		URI withConfidence = f.createURI(Constants.mainNS+Constants.withConfidence);
		List<URI> multiDepictionEntries = new ArrayList<URI>();
		
		
		for (GeoRule rule : rules) {
			try {
				reorderPredicateList(rule, con);
				Map<URI,Literal> uris = fire(rule, ds, con);
				URI region = null;
				RepositoryResult<Statement> rr = con.getStatements(null, RDF.TYPE, rule.getHead().getRegionDepiction(), false);
				
				if (rr.hasNext()) {
					region = (URI)rr.next().getSubject();
				}

				if (uris != null && region != null) {
					for (URI entry : uris.keySet()) {
						//String sparqlUpdate = "INSERT DATA (<"+entry+"> <"+Constants.mainNS+Constants.depictsObject+"> <"+region+">)";
						//con.add(entry, depictsObject, region);
						URI depiction = f.createURI(entry.toString()+"_depiction");
						Literal confidence = f.createLiteral((float)rule.getHead().getConfidence());
						if (context != null) {
							RepositoryResult<Statement> deps = con.getStatements(entry, depicts, null, true, context);
							int counter = 0;
							while (deps.hasNext()) {
								counter++;
								deps.next();
							}
							if (counter == 0)
								depiction = f.createURI(entry.toString()+"_depiction");
							else {
								depiction = f.createURI(entry.toString()+"_depiction"+counter);
								multiDepictionEntries.add(entry);
							}
							//con.add(entry, depictsObject, region, context);
							con.add(entry, depicts, depiction, context);
							con.add(depiction, depictsObject, region, context);
							con.add(depiction, withConfidence, confidence, context);
							con.add(depiction, f.createURI(Constants.mainNS+"assignedFrom"), f.createURI(Constants.mainNS+"geoRules"), context);
							con.add(depiction, RDF.TYPE, f.createURI(Constants.mainNS+"Depiction"), context);
						}
						else {
							RepositoryResult<Statement> deps = con.getStatements(entry, depicts, null, true);
							int counter = 0;
							while (deps.hasNext()) {
								counter++;
								deps.next();
							}
							if (counter == 0)
								depiction = f.createURI(entry.toString()+"_depiction");
							else {
								depiction = f.createURI(entry.toString()+"_depiction"+counter);
								multiDepictionEntries.add(entry);
							}
							//con.add(entry, depictsObject, region);
							con.add(entry, depicts, depiction);
							con.add(depiction, depictsObject, region);
							con.add(depiction, withConfidence, confidence);
							con.add(depiction, f.createURI(Constants.mainNS+"assignedFrom"), f.createURI(Constants.mainNS+"geoRules"));
							con.add(depiction, RDF.TYPE, f.createURI(Constants.mainNS+"Depiction"));
						}
					}
					con.commit();
				}
			} catch (RepositoryException e) {
				e.printStackTrace();
			}
		}
		
		for (URI entry : multiDepictionEntries) {
			String q = "SELECT ?depiction ?object ?conf WHERE {\n"+
						"<"+entry.toString()+"> gno:"+Constants.depicts+" ?depiction . \n"+
						"?depiction gno:"+Constants.depictsObject+" ?object . \n"+
						"?depiction gno:"+Constants.withConfidence+" ?conf . \n" +
						"} ORDER BY DESC(?conf)";
			
			try {
				TupleQuery tupleQuery = con.prepareTupleQuery(QueryLanguage.SPARQL, Constants.queryPrefix+q);
				TupleQueryResult result = tupleQuery.evaluate();
				int counter = 0;
				while (result.hasNext()) {
					BindingSet bindings = result.next();
					//keep the first entry, delete the rest
					if (counter > 0) {
						URI depiction = (URI)bindings.getValue("depiction");
						URI object = (URI)bindings.getValue("object");
						Literal conf = (Literal)bindings.getValue("conf");
						
						if (context == null) {
							con.remove(entry, Constants.depictsProperty, depiction);
							con.remove(depiction, Constants.depictsObjectProperty, object);
							con.remove(depiction, Constants.withConfidenceProperty, conf);
							con.remove(depiction, f.createURI(Constants.mainNS+"assignedFrom"), f.createURI(Constants.mainNS+"geoRules"));
							con.remove(depiction, RDF.TYPE, f.createURI(Constants.mainNS+"Depiction"));
						}
						else {
							con.remove(entry, Constants.depictsProperty, depiction, context);
							con.remove(depiction, Constants.depictsObjectProperty, object, context);
							con.remove(depiction, Constants.withConfidenceProperty, conf, context);
							con.remove(depiction, f.createURI(Constants.mainNS+"assignedFrom"), f.createURI(Constants.mainNS+"geoRules"), context);
							con.remove(depiction, RDF.TYPE, f.createURI(Constants.mainNS+"Depiction"), context);
						}
					}
					counter++;
				}
			}catch (Exception e) {
				e.printStackTrace();
			}
		}
		try {
			con.commit();
		} catch (RepositoryException e) {
			e.printStackTrace();
		}
		
	}
	
	public Map<URI,Literal> fire(GeoRule rule, DataSource ds, RepositoryConnection con ) {
		boolean firstPredicate = true;
		
		Map<URI,Literal> uris = null;
		
		for (GeoRulePredicate grp : rule.getPredicates()) {
			if (firstPredicate) {
				uris = geoQueryUnassigned(grp, rule.getHead(), con);
				firstPredicate = false;
			}
			else {
				uris = geoQueryFiltered(grp,ds,con,uris);
			}
		}
		
		return uris;
	}
	
	public void reorderPredicateList(GeoRule rule, RepositoryConnection con) {
		ValueFactory f = new ValueFactoryImpl();
		URI prop = f.createURI(Constants.mainNS+Constants.depictsObject);
		
		List<GeoRulePredicate> tempList = new ArrayList<GeoRulePredicate>(); 
		Map<URI,Integer> map = new HashMap<URI,Integer>();
		
		try {
			for(GeoRulePredicate grp : rule.getPredicates()) {
				//RepositoryResult<Statement> tempSts = con.getStatements(null, prop, grp.getRegionDepiction(), true);
				//?region gno:depictsObject ?obj . ?obj rdf:type grp.getRegionDepiction()
				RepositoryResult<Statement> depSt = con.getStatements(null, RDF.TYPE, grp.getRegionDepiction(), true);
				int num = 0;
				while (depSt.hasNext()) {
					Statement st = depSt.next();
					RepositoryResult<Statement> tempSts = con.getStatements(null, prop, st.getSubject(), true);
					num += countStatements(tempSts);
				}
				map.put(grp.getRegionDepiction(), Integer.valueOf(num));
				
				if (tempList.size() == 0) {
					tempList.add(grp);
				}
				else {
					int i;
					for (i=0;i<tempList.size();i++) {
						if (num <= map.get(tempList.get(i).getRegionDepiction()).intValue()) {
							break;
						}
					}
					if (i==tempList.size())
						tempList.add(grp);
					else
						tempList.add(i,grp);
				}
			}
		}catch (Exception e) {
			e.printStackTrace();
		}
		
		rule.clearPredicates();
		rule.setPredicates(tempList);
	}
	
	public Map<URI,Literal> geoQueryUnassigned(GeoRulePredicate grp, GeoRuleHead headClause, RepositoryConnection con) {
		Map<URI,Literal> bucket = new HashMap<URI,Literal>(500);
		String searchMethod = ", <opt:##byindex>";
		
		try {
			String regQuery = "SELECT ?reg ?regWKT WHERE {\n" +
									"?reg gno:"+Constants.depicts+" ?dep . \n" +
									"?dep gno:"+Constants.depictsObject+" ?obj . \n"+
									//segmentationLevelRestriction(grp, "reg") +
									//imageRestriction(grp, "reg")+
									"?obj rdf:type <"+grp.getRegionDepiction()+"> . \n" +
									"?reg geo:"+Constants.asWKT+" ?regWKT . \n" +
								"}";
			TupleQuery regTupleQuery = con.prepareTupleQuery(QueryLanguage.SPARQL, Constants.queryPrefix+regQuery);
			System.out.println(regQuery);
			
			String reg2Query = "SELECT ?reg2 ?reg2WKT WHERE {\n" +
									"{?reg2 geo:"+Constants.asWKT+" ?reg2WKT .\n"+ //select all the regions
									//segmentationLevelRestriction(grp, "reg2") + //that belong the seg. level
									//imageRestriction(grp, "reg2")+ //that belong to the image specified in the rule
									"FILTER (?reg != ?reg2) . \n" +
									"FILTER NOT EXISTS {" +
										"?reg2 gno:"+Constants.depicts+" ?dep2 . " +
									"} . \n"+
									"FILTER (search:"+grp.getSimpleFeature()+"(?regWKT, ?reg2WKT"+searchMethod+")) . }\n" +
									"UNION {\n"+
									"?reg2 gno:"+Constants.depicts+" ?dep22 . \n" +
									"?dep22 gno:assignedFrom gno:geoRules . \n"+
									"} \n"+
								"}";
			TupleQuery reg2TupleQuery = con.prepareTupleQuery(QueryLanguage.SPARQL, Constants.queryPrefix+reg2Query);
			System.out.println(reg2Query);
			TupleQueryResult regResult = regTupleQuery.evaluate();
			while (regResult.hasNext()) {
				BindingSet bindingSet = regResult.next();
				reg2TupleQuery.setBinding("regWKT", bindingSet.getValue("regWKT"));
				reg2TupleQuery.setBinding("reg", bindingSet.getValue("reg"));
				TupleQueryResult reg2Result = reg2TupleQuery.evaluate();
				while (reg2Result.hasNext()) {
					BindingSet bindingSet2 = reg2Result.next();
					if (!bucket.containsKey((URI)bindingSet2.getValue("reg2")))
						bucket.put((URI)bindingSet2.getValue("reg2"), (Literal)bindingSet2.getValue("reg2WKT"));
				}
				reg2Result.close();
			}
			regResult.close();
			logger.info("Printing first predicate bucket with queries:");
			logger.info(regQuery);
			logger.info(reg2Query);
		}catch (Exception e) {
			e.printStackTrace();
		}
		
		/*for (URI key : bucket.keySet()) {
			logger.info("geo rule result : "+key.toString());
		}*/
		System.out.println("Bucket size = "+bucket.size());
		
		return bucket;
	}
	
	public Map<URI,Literal> geoQueryFiltered(GeoRulePredicate grp, DataSource ds, RepositoryConnection con, Map<URI,Literal> regions) {
		if (regions == null)
			return null;
		
		if (regions.size() == 0)
			return null;
		
		Map<URI,Literal> bucket = new HashMap<URI,Literal>(regions.size()/4);
		ValueFactory f = new ValueFactoryImpl();
		URI wktLiteralType = f.createURI(Constants.geoNS+"wkt");
		
		try {
			StringBuffer regIN = new StringBuffer(regions.size()*Constants.mainNS.length());
			for (Entry<URI,Literal> region : regions.entrySet()) {
				regIN.append("'").append(region.getKey()).append("',");
			}
			regIN.deleteCharAt(regIN.length()-1);
			
			StringBuffer reg2IN = new StringBuffer(regions.size()*Constants.mainNS.length());
			String geoQuery = "SELECT ?reg2 WHERE {" +
												"?reg2 gno:"+Constants.depicts+" ?dep . " +
												"?dep gno:"+Constants.depictsObject+" ?obj . " +
												//segmentationLevelRestriction(grp, "reg2") +
												//imageRestriction(grp, "reg2") +
												"?obj rdf:type <"+grp.getRegionDepiction()+"> . " +
											"}";
			TupleQuery regTupleQuery = con.prepareTupleQuery(QueryLanguage.SPARQL, Constants.queryPrefix+geoQuery);
			TupleQueryResult regResult = regTupleQuery.evaluate();
			while (regResult.hasNext()) {
				BindingSet bindingSet = regResult.next();
				Value reg = bindingSet.getValue("reg2");
				reg2IN.append("'").append(reg).append("',");
			}
			reg2IN.deleteCharAt(reg2IN.length()-1);
			
			String sqlString = "SELECT DISTINCT regView.subject, regView.objectstring " +
							   "FROM idxst_0 AS regView, " +
							   "idxst_0 AS reg2View " +
							   	"WHERE " +
							   		"ST_"+grp.getSimpleFeature()+"(regView.objectspatial, reg2View.objectspatial) AND " +
							   		"regView.subject IN ("+regIN+") AND " +
							   		"reg2View.subject IN ("+reg2IN+") AND " + 
							   		"regView.subject != reg2View.subject";
			
			PreparedStatement ps = ds.getConnection().prepareStatement(sqlString);
			ResultSet results = ps.executeQuery();
			int counter = 0;
			while (results.next()) {
				URI uri = f.createURI(results.getString(1));
				if (!bucket.containsKey(uri))
					bucket.put(uri, f.createLiteral(results.getString(2), wktLiteralType));
				
				counter++;
			}
			//System.out.println("Row count = " + counter);
			
			results.close();
			ps.close();
		}
		catch (Exception e) {
			e.printStackTrace();
		}
		
		return bucket;
	}
	
	public String imageRestriction(GeoRuleTerm grt, String varName) {
		String ret = "";
		
		if (grt.gettImage() != null) {
			ret = "?"+varName+" gno:regionBelongsToImage gno:"+grt.gettImage().toString()+" . ";
		}
		else {
			ret = "?"+varName+" gno:regionBelongsToImage gno:CURRENTIMAGE . ";
		}
		
		return ret;
	}
	
	public String segmentationLevelRestriction(GeoRuleTerm grt, String varName) {
		String ret = "";
		if (grt.getSegmentationLevel() > 0)
			ret = "?"+varName+" gno:"+Constants.segmentationLevel+" gno:segmentationLevel_"+grt.getSegmentationLevel() +" . ";
		
		return ret;
	}
	
	public String superClassRestriction(GeoRuleTerm grt, String varName, RepositoryConnection con) {
		String ret = "";
		
		if (askSuperClasses(grt, con)) {
			ret = "?"+varName+" gno:"+Constants.depicts+" ?dep_"+varName+" . \n" +
				  "?dep_"+varName+" gno:"+Constants.depictsObject+" ?ins_"+varName+" . \n" +
				  "?ins_"+varName+" rdf:type ?class_"+varName+" . \n" +
				  //"<"+grt.getRegionDepiction()+"> rdfs:subClassOf ?class_"+varName+" . \n"+
				  "FILTER (?class_"+varName+" != <"+Constants.mainNS+"ObjectDepiction>) . \n";
		}
		
		return ret;
	}
	
	public boolean askSuperClasses(GeoRuleTerm grt, RepositoryConnection con) {
		try {
			String askQuery = "ASK {" +
									"?region gno:"+Constants.depicts+" ?dep . " +
									"?dep gno:"+Constants.depictsObject+" ?object . " +  
									"?object rdf:type ?class . " +
									"<"+grt.getRegionDepiction()+"> rdfs:subClassOf ?class . " +
									"FILTER (?class != <"+Constants.mainNS+"#ObjectDepiction>)"+
								"}";
			
			BooleanQuery booleanQuery = con.prepareBooleanQuery(QueryLanguage.SPARQL, Constants.queryPrefix+askQuery);
			return booleanQuery.evaluate();
		}catch (Exception e) {
			e.printStackTrace();
		}
		
		return false;
	}

	
}
