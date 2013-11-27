package gr.iti.gnorasi.classification;

import java.util.List;

import gr.iti.gnorasi.classification.ClassificationRules.RuleOperator;
import gr.iti.gnorasi.utils.Constants;

import org.apache.log4j.Level;
import org.apache.log4j.Logger;
import org.openrdf.model.Literal;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;
import org.openrdf.model.Value;
import org.openrdf.model.vocabulary.RDF;
import org.openrdf.model.vocabulary.RDFS;
import org.openrdf.query.BindingSet;
import org.openrdf.query.QueryLanguage;
import org.openrdf.query.TupleQuery;
import org.openrdf.query.TupleQueryResult;
import org.openrdf.repository.RepositoryConnection;
import org.openrdf.repository.RepositoryException;
import org.openrdf.repository.RepositoryResult;

import com.useekm.geosparql.UnitsOfMeasure;

public class ClassificationRulesFire {
	
	//test two versions: 
	//1. one with the attribute and rules executed in the same UPDATE query (simpler),
	//2. another one where the attribute queries are executed first, and spatial queries are executed second
	//   (more complex but should be faster as fewer regions will be spatially compared)
	
	private static Logger logger = Logger.getLogger(ClassificationRulesFire.class);
	
	public ClassificationRulesFire() {
		logger.setLevel(Level.INFO);
	}
	
	public void inheritanceDepictionsFromInstance(URI obj, URI depictionInstance, RepositoryConnection con) {
		try {
			RepositoryResult<Statement> sts = con.getStatements(obj, RDF.TYPE, null, false, Constants.userNSURI);
			if (sts.hasNext()) {
				URI headClass = (URI)sts.next().getObject();
				inheritanceDepictions(headClass, depictionInstance, con);
			}
			sts.close();
		}catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public void inheritanceDepictions(URI headClass, URI depictionInstance, RepositoryConnection con) {
		try {
			RepositoryResult<Statement> sts = con.getStatements(headClass, RDFS.SUBCLASSOF, null, true);
			while (sts.hasNext()) {
				URI superClass = (URI)sts.next().getObject();
				RepositoryResult<Statement> insSts = con.getStatements(null, RDF.TYPE, superClass, false, Constants.userNSURI);
				if (insSts.hasNext()) {
					URI classInstance = (URI)insSts.next().getSubject();
					if (!con.hasStatement(depictionInstance, Constants.depictsObjectProperty, classInstance, false, Constants.userNSURI))
						con.add(depictionInstance, Constants.depictsInheritedObjectProperty, classInstance, Constants.userNSURI);
				}
				insSts.close();
			}
			sts.close();
		}catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public Value getSegmentationLevel(URI _class, RepositoryConnection con) {
		Value ret = null;
		
		try {
			RepositoryResult<Statement> st = con.getStatements(null, RDF.TYPE, _class, false, Constants.userNSURI);
			if (st.hasNext()) {
				URI instance = (URI)st.next().getSubject();
				RepositoryResult<Statement> st2 = con.getStatements(instance, Constants.definedForSegmentationLevelProperty, null, false, Constants.userNSURI);
				if (st2.hasNext()) {
					ret = st2.next().getObject();
				}
				st2.close();
			}
			st.close();
			
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return ret;
	}
	
	private Literal getParentClassConfidence(BindingSet bindingSet, String bindingName) {
		/* Get parent class confidence */
		Value parentMembershipValue = bindingSet.getValue(bindingName);
		Literal parentMembershipLiteral = null;
		if (parentMembershipValue != null)
			parentMembershipLiteral = (Literal)parentMembershipValue;
		
		return parentMembershipLiteral;
	}
	
	public void testGeospatialQuery(RepositoryConnection con) {
		String query = "SELECT * " +
				   "WHERE {" +
				   "?region rdf:type gno:Region . \n"+
				   "?region geo:"+Constants.asWKT+" ?regionWKT . \n"+
				   "?region gno:"+Constants.belongsToSegmentationLevel+" <"+Constants.userNS+"segmentationLevel_0> . \n"+
				   "?region2 rdf:type gno:Region . \n"+
				   "?region2 geo:"+Constants.asWKT+" ?region2WKT . \n"+
				   "?region2 gno:"+Constants.belongsToSegmentationLevel+" <"+Constants.userNS+"segmentationLevel_1> . \n"+
				   "BIND (geof:distance(?regionWKT, ?region2WKT, <"+UnitsOfMeasure.URI_METRE.stringValue()+">) AS ?distance)"+
				"}";
		
		logger.info(query);
		try {
			TupleQuery tq = con.prepareTupleQuery(QueryLanguage.SPARQL, Constants.queryPrefix+query);
			TupleQueryResult tqr = tq.evaluate();
			System.out.println("Getting distances...");
			double distance = 0;
			while (tqr.hasNext()) {
				BindingSet bs = tqr.next();
				URI region = (URI)bs.getValue("region");
				URI region2 = (URI)bs.getValue("region2");
				distance = ((Literal)bs.getValue("distance")).doubleValue();
				logger.info(region+" "+region2+" : distance="+distance);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	private String composeInitialSelectionQuery(ClassificationRules allRules, RepositoryConnection con, URI rootClass) {
		String query = "SELECT * " +
				   "WHERE {" +
				   "?region rdf:type gno:Region . \n";
		
		if (rootClass != null) {
			query += "?region gno:"+Constants.depicts+" ?rootDepiction . \n"+
					"?rootDepiction gno:"+Constants.depictsObject+" ?rootObj . \n"+
					"?rootObj rdf:type <"+rootClass+"> . \n" + 
					"?rootDepiction gno:"+Constants.withConfidence+" ?rootConf . \n";
		}
		
		if (allRules.getGeoRules().size() > 0)
			query += "?region geo:"+Constants.asWKT+" ?regionWKT . \n";
		query += "?region gno:"+Constants.belongsToSegmentationLevel+" <" + 
			 getSegmentationLevel(allRules.getHead(), con).stringValue() + "> . \n";
		
		return query;
	}
	
	public void fireMethod1(ClassificationRules allRules, RepositoryConnection con, URI selectionClass) throws Exception {
		TupleQueryResult tqr;
		
		String query = composeInitialSelectionQuery(allRules, con, selectionClass);
		/*String query = "SELECT * " +
					   "WHERE {" +
					   "?region rdf:type gno:Region . \n";
		if (allRules.getGeoRules().size() > 0)
			query += "?region geo:"+Constants.asWKT+" ?regionWKT . \n";
		query += "?region gno:"+Constants.belongsToSegmentationLevel+" <" + 
				 getSegmentationLevel(allRules.getHead(), con).stringValue() + "> . \n";
		*/
		
		if (allRules.getFuzzyRules().size() > 0) {
			int counter = 0;
			for (FuzzyRule aRule : allRules.getFuzzyRules()) {
				query += "?region <"+aRule.getFuzzyProperty()+"> ?membershipValue"+(++counter)+" . \n";
			}
			query += "}";
			
			logger.info(query);
			TupleQuery tq = con.prepareTupleQuery(QueryLanguage.SPARQL, Constants.queryPrefix+query);
			tqr = tq.evaluate();
			int regionCounter = 0;
			while (tqr.hasNext()) {
				regionCounter++;
				BindingSet bindingSet = tqr.next();
				float membershipValue = getMembershipValue(allRules, bindingSet, "membershipValue", counter);
				Literal membershipValueLiteral = con.getValueFactory().createLiteral(membershipValue);

				URI region = (URI)bindingSet.getValue("region");
				
				/* Get parent class confidence */
				Literal parentMembershipLiteral = getParentClassConfidence(bindingSet, "rootConf");
				
				/* if there are no geospatial rules, then assign object depictions to ?region */
				if (allRules.getGeoRules().size() == 0) {
					 /* check if the depiction has been already inserted from a previous iteration */
					if (!con.hasStatement(region, Constants.depictsProperty, null, false, Constants.userNSURI)) {
						URI obj = getDepictionFromRule(allRules, con);
						addRegionDepiction(region, obj, membershipValueLiteral, parentMembershipLiteral, con);
					}
					else {
						RepositoryResult<Statement> sts = con.getStatements(region, Constants.depictsProperty, null, false, Constants.userNSURI);
						boolean found = false;
						while (sts.hasNext()) {
							Statement st = sts.next();
							if (st.getObject() instanceof URI) {
								URI depiction = (URI)st.getObject();
								RepositoryResult<Statement> stDepictions = con.getStatements(depiction, Constants.depictsObjectProperty, null, false, Constants.userNSURI);
								while (stDepictions.hasNext()) {
									Statement st2 = stDepictions.next();
									if (st2.getObject() instanceof URI) {
										URI obj = (URI)st2.getObject();
										if (con.hasStatement(obj, RDF.TYPE, allRules.getHead(), false, Constants.userNSURI)) {
											found = true;
											break;
										}
									}
								}
								if (found)
									break;
							}
						}
						if (!found) {
							URI obj = getDepictionFromRule(allRules, con);
							addRegionDepiction(region, obj, membershipValueLiteral, parentMembershipLiteral, con);
						}
					}
					/*String depictionAskQuery = "SELECT ?obj { " +
									"<"+region+"> gno:depicts ?depiction . " +
									"?depiction gno:depictsObject ?obj . }";
									
					TupleQueryResult objResults = con.prepareTupleQuery(QueryLanguage.SPARQL, Constants.queryPrefix + depictionAskQuery).evaluate();
					if (objResults.hasNext()) {
						URI obj = (URI)objResults.next().getValue("obj");
						if (!con.hasStatement(obj, RDF.TYPE, allRules.getHead(), false)) {
							//create the depiction instance
							addRegionDepiction(region, obj, membershipValueLiteral, con);
						}
					}
					else {
						URI obj = getDepictionFromRule(allRules, con);
						addRegionDepiction(region, obj, membershipValueLiteral, con);
					}*/
				}
				else {
					String geoQuery = "SELECT * \n" +
							"WHERE { \n";
					counter = 0;
					for (GeoRule gRule : allRules.getGeoRules()) {
						geoQuery += "?region"+counter+" gno:depicts ?depiction"+counter+" . \n" + 
								"?region"+counter+" geo:"+Constants.asWKT+" ?region"+counter+"WKT . \n" +
								"?depiction"+counter+" gno:depictsObject <" + gRule.getTargetClass() + "> . \n" +
								"FILTER (geof:"+gRule.geoFunction("?regionWKT", "?region"+counter) + "WKT) . \n";
					}
					geoQuery += "}\n";
					TupleQuery geoQueryTQ = con.prepareTupleQuery(QueryLanguage.SPARQL, Constants.queryPrefix+geoQuery);
					geoQueryTQ.setBinding("regionWKT", bindingSet.getValue("regionWKT"));
					TupleQueryResult geoQueryTQR = geoQueryTQ.evaluate();
					if (geoQueryTQR.hasNext()) {
						URI obj = getDepictionFromRule(allRules, con);
						addRegionDepiction(region, obj, membershipValueLiteral, parentMembershipLiteral, con);
					}
					geoQueryTQR.close();
				}
			}
			logger.info("Region counter = "+regionCounter);
			tqr.close();
			con.commit();
		}
		// if there are only geospatial rules...
		else if (allRules.getGeoRules().size() > 0) {
			//end "query" with appropriate }
			query += "}";
			TupleQuery tq = con.prepareTupleQuery(QueryLanguage.SPARQL, Constants.queryPrefix+query);
			tqr = tq.evaluate();
			
			String geoQuery = "SELECT * \n" +
					"WHERE { \n";
			int counter = 0;
			for (GeoRule gRule : allRules.getGeoRules()) {
				geoQuery += "?region"+counter+" gno:depicts ?depiction"+counter+" . \n" +
						"?region"+counter+" geo:"+Constants.asWKT+" ?region"+counter+"WKT . \n" +
						"?depiction"+counter+" gno:depictsObject <" + gRule.getTargetClass() + "> . \n" +
						"?depiction"+counter+" gno:withConfidence ?conf"+counter+" . \n" +
						"FILTER ("+gRule.geoFunction("?regionWKT", "?region"+counter+"WKT") + ") . \n";
				counter++;
			}
			geoQuery += "}\n";
			TupleQuery tq2 = con.prepareTupleQuery(QueryLanguage.SPARQL, Constants.queryPrefix+geoQuery);
			while (tqr.hasNext()) {
				BindingSet regionSet = tqr.next();
				
				URI region = (URI)regionSet.getValue("region");
				/* Get parent class confidence */
				Literal parentMembershipLiteral = getParentClassConfidence(regionSet, "rootConf");
				
				float membershipValue = getMembershipValue(allRules, regionSet, "conf", counter);
				Literal membershipValueLiteral = con.getValueFactory().createLiteral(membershipValue);
				
				tq2.setBinding("regionWKT", regionSet.getValue("regionWKT"));
				TupleQueryResult tq2r = tq2.evaluate();
				if (tq2r.hasNext()) {
					URI obj = getDepictionFromRule(allRules, con);
					addRegionDepiction(region, obj, membershipValueLiteral, parentMembershipLiteral, con);
				}
				tq2r.close();
			}
			tqr.close();
			con.commit();
		}
		con.commit();
	}
	
	//this method will go through each root class and hierarchically assign regions to objects
	//1. find root classes, 2. for each root class -> traverse hierarchy executing rules
	public void fireMethod2(RepositoryConnection con) throws Exception {
		RepositoryResult<Statement> res = con.getStatements(null, RDFS.SUBCLASSOF, Constants.objectDepictionClass, false, Constants.userNSURI);
		while (res.hasNext()) {
			Statement st1 = res.next();
			URI rootClass = (URI)st1.getSubject();
			recursiveRuleExecution(rootClass, con);
		}
	}
	
	public void recursiveRuleExecution(URI rootClass, RepositoryConnection con) {
		ClassificationRules allRules = ClassificationRulesBuilder.getRulesForClass(rootClass);
		//logger.info(allRules);
		if (allRules != null) {
			try {
				if (con.hasStatement(rootClass, RDFS.SUBCLASSOF, Constants.objectDepictionClass, false, Constants.userNSURI))
					fireMethod1(allRules, con, null);
				else {
					RepositoryResult<Statement> res = con.getStatements(rootClass, RDFS.SUBCLASSOF, null, false, Constants.userNSURI);
					while (res.hasNext()) {
						URI parentClass = (URI)res.next().getObject();
						fireMethod1(allRules, con, parentClass);
					}
				}
				
				RepositoryResult<Statement> res = con.getStatements(null, RDFS.SUBCLASSOF, rootClass, false, Constants.userNSURI);
				while (res.hasNext()) {
					recursiveRuleExecution((URI)res.next().getSubject(), con);
				}
				
			}catch (Exception e) {
				e.printStackTrace();
			}
		}
	}
	
	public void iterativeFireMethod1(List<ClassificationRules> rules, RepositoryConnection con) {
		long countStatements = 0;
		try {
			while(true) {
				if (countStatements != con.size(con.getValueFactory().createURI(Constants.userNS))) {
					countStatements = con.size(con.getValueFactory().createURI(Constants.userNS));
					for (ClassificationRules rule : rules) {
						try {
							fireMethod1(rule, con, null);
						} catch (Exception e) {
							e.printStackTrace();
						}
					}
				}
				else
					break;
			}
		}catch (RepositoryException e) {
			e.printStackTrace();
		}
	}
	
	public void iterativeFireMethod2(List<ClassificationRules> rules, RepositoryConnection con) {
		long countStatements = 0;
		try {
			int counter = 0;
			while(true) {
				
				if (countStatements != con.size(con.getValueFactory().createURI(Constants.userNS))) {
					
					countStatements = con.size(con.getValueFactory().createURI(Constants.userNS));
					fireMethod2(con);
					counter++;
					if (counter > 6)
						break;
				}
				else
					break;
				System.out.println("Iteration "+counter);
			}
		}catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public URI getDepictionFromRule(ClassificationRules rule, RepositoryConnection con) {
		try {
			RepositoryResult<Statement> sts = con.getStatements(null, RDF.TYPE, rule.getHead(), false, Constants.userNSURI);
			while (sts.hasNext()) {
				Statement st = sts.next();
				return (URI)st.getSubject();
			}
		}catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}
	
	public void addRegionDepiction(URI region, URI obj, Literal membershipValueLiteral, Literal parentMembershipLiteral, RepositoryConnection con) {
		try {
			Literal newMembershipValueLiteral = null;
			/* Modify depiction confidence according to parentConfidence*objectCondidence */
			if (parentMembershipLiteral != null) {
				float membershipValue = membershipValueLiteral.floatValue();
				float membershipParent = parentMembershipLiteral.floatValue();
				newMembershipValueLiteral = con.getValueFactory().createLiteral(membershipValue*membershipParent);
			}
			else {
				newMembershipValueLiteral = membershipValueLiteral;
			}
			
			if (newMembershipValueLiteral.floatValue() > Constants.membershipThreshold) {
				
				//URI depiction = con.getValueFactory().createURI(Constants.userNS + region.getLocalName()+"_depiction"+System.currentTimeMillis());
				URI depiction = con.getValueFactory().createURI(Constants.userNS + region.getLocalName()+"_depiction"+System.nanoTime());
				
				con.add(depiction, RDF.TYPE, Constants.depictionClass, Constants.userNSURI);
				con.add(depiction, Constants.depictsObjectProperty, obj, Constants.userNSURI);
				con.add(depiction, Constants.withConfidenceProperty, newMembershipValueLiteral, Constants.userNSURI);
				con.add(region, Constants.depictsProperty, depiction, Constants.userNSURI);
				
				/* assign inheritance depictions to the depiction instance */
				inheritanceDepictionsFromInstance(obj, depiction, con);
			}
		}catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public float getMembershipValue(ClassificationRules rule, BindingSet bindingSet, String variableName, int numValues) {
		float membershipValue = 0;
		if (rule.getOp().compareTo(RuleOperator.MAX) == 0) {
			membershipValue = 0;
			for (int i=1;i<=numValues;i++) {
				Literal val = (Literal)bindingSet.getValue(variableName+i);
				if (val.floatValue() > membershipValue)
					membershipValue = val.floatValue();
			}
		}
		else if (rule.getOp().compareTo(RuleOperator.MIN) == 0) {
			membershipValue = 1; 
			for (int i=1;i<=numValues;i++) {
				Literal val = (Literal)bindingSet.getValue(variableName+i);
				if (val.floatValue() < membershipValue)
					membershipValue = val.floatValue();
			}
		}
		
		return membershipValue;
	}
	
}
