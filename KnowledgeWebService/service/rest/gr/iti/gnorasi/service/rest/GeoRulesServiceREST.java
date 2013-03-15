package gr.iti.gnorasi.service.rest;

import gr.iti.gnorasi.utils.Constants;

import javax.ws.rs.Consumes;
import javax.ws.rs.DELETE;
import javax.ws.rs.FormParam;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.PUT;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.MediaType;

import org.apache.log4j.Logger;

@Path("/georulesservice")
public class GeoRulesServiceREST {

	private static Logger logger = Logger.getLogger(GeoRulesServiceREST.class);
	
	public String getUserContext(String context) {
		if (context.equals("")) {
			return GeoRulesServiceSingleton.instance.getService().getUserContext();
		}
		else {
			return context;
		}
	}
	
	@POST
	@Path("/init")
	@Produces(MediaType.TEXT_XML)
	@Consumes(MediaType.APPLICATION_FORM_URLENCODED)
	public String init(@FormParam("truncate") String truncate) {
		
		GeoRulesServiceSingleton.instance.getService().initializeRepository(truncate);
		
		return "REST: init started";
	}
	
	@GET
	@Path("/close")
	@Produces(MediaType.TEXT_XML)
	public String closeConnection() {
		GeoRulesServiceSingleton.instance.getService().closeConnections();
		
		return "REST: closed connections";
	}
	
	@DELETE
	@Path("/delete")
	public void deleteContextData(@QueryParam("context") String context) {
			
		GeoRulesServiceSingleton.instance.getService().deleteContextData(getUserContext(context));
		
		System.out.println(context);
	}
	
	@POST
	@Path("/featureRuleString")
	@Produces(MediaType.TEXT_XML)
	@Consumes(MediaType.APPLICATION_FORM_URLENCODED)
	public String featureRuleExecString(@FormParam("rules") String inString, 
										@FormParam("context") String context) {
		
		GeoRulesServiceSingleton.instance.getService().executeFeatureRules(inString, getUserContext(context));

		
		return "REST: feature rule string executed";
	}

	@POST
	@Path("/ruleString")
	@Produces(MediaType.TEXT_XML)
	@Consumes(MediaType.APPLICATION_FORM_URLENCODED)
	public String ruleExecString(@FormParam("rules") String inString, 
								 @FormParam("context") String context) {
		GeoRulesServiceSingleton.instance.getService().executeRules(inString, getUserContext(context));

		
		return "REST: geo rule string executed";
	}
	
	@POST
	@Path("/execFuzzyRules")
	@Produces(MediaType.TEXT_XML)
	@Consumes(MediaType.APPLICATION_FORM_URLENCODED)
	public String fuzzyExecString(@FormParam("rules") String inString, 
								  @FormParam("context") String context) {
		GeoRulesServiceSingleton.instance.getService().execFuzzyRules(inString, getUserContext(context));

		
		return "REST: fuzzy rule string executed";
	}

	@PUT
	@Path("/sendCSVDataString")
	@Produces(MediaType.TEXT_PLAIN)
	@Consumes(MediaType.APPLICATION_FORM_URLENCODED)
	public String sendCSVString(@FormParam("csvstring") String csvString, 
								@FormParam("srid") String srid, 
								@FormParam("parsefeatures") String parseFeatures,
								@FormParam("context") String context) {
		
		int srid_int;
		try {
			srid_int = Integer.valueOf(srid).intValue();
		}catch (NumberFormatException e) {
			srid_int = 2100; //default to GGRS
		}
		boolean parseFeatures_bool = Boolean.valueOf(parseFeatures);
		GeoRulesServiceSingleton.instance.getService().parseCSVString2Ontology(csvString, srid_int, getUserContext(context), parseFeatures_bool);
		
		return "REST: CSV string sent";
	}
	
	@PUT
	@Path("/sendUserOntology")
	@Produces(MediaType.TEXT_PLAIN)
	@Consumes(MediaType.APPLICATION_FORM_URLENCODED)
	public String sendUserOntology(@FormParam("ontology") String ontology, 
								   @FormParam("context") String context) {
		
		GeoRulesServiceSingleton.instance.getService().addUserOntology(ontology, getUserContext(context));
		System.out.println("Received ontology: "+ontology);
		
		return "REST: User ontology sent";
	}
	
	@GET
	@Path("/getResults")
	@Consumes(MediaType.TEXT_PLAIN)
	@Produces(MediaType.TEXT_PLAIN)
	public String getClassificationResults(@QueryParam("context") String context) {
		String results = GeoRulesServiceSingleton.instance.getService().parseOntologyResults2CSV(getUserContext(context));
		
		System.out.println(context);
		
		return results;
	}
	
	@GET
	@Path("/getResultsQuery")
	@Consumes(MediaType.APPLICATION_FORM_URLENCODED)
	@Produces(MediaType.TEXT_PLAIN)
	public String getQueryResults(@QueryParam("query") String query) {
		String results = GeoRulesServiceSingleton.instance.getService().parseQueryResults2CSV(query);
		
		System.out.println(query);
		System.out.println(results);
		
		return results;
	}
	
	@POST
	@Path("/performClassification")
	@Produces(MediaType.TEXT_XML)
	@Consumes(MediaType.APPLICATION_FORM_URLENCODED)
	public String performClassification(@FormParam("truncate") String truncate,
										@FormParam("ontology") String ontology,
										@FormParam("csvstring") String csvString, 
										@FormParam("srid") String srid, 
										@FormParam("parsefeatures") String parseFeatures,
										@FormParam("fuzzyrules") String fuzzyRules,
										@FormParam("georules") String geoRules,
										@FormParam("context") String context) {
		
		GeoRulesServiceSingleton.instance.getService().initializeRepository(truncate);
		System.out.println("init connection");
		
		GeoRulesServiceSingleton.instance.getService().addUserOntology(ontology, getUserContext(context));
		System.out.println("Received ontology");
		
		int srid_int = Integer.valueOf(srid).intValue();
		boolean parseFeatures_bool = Boolean.valueOf(parseFeatures);
		GeoRulesServiceSingleton.instance.getService().parseCSVString2Ontology(csvString, srid_int, getUserContext(context), parseFeatures_bool);
		System.out.println("Object data received");
		
		GeoRulesServiceSingleton.instance.getService().execFuzzyRules(fuzzyRules, getUserContext(context));
		System.out.println("Fuzzy rules executed");
		
		GeoRulesServiceSingleton.instance.getService().executeRules(geoRules, getUserContext(context));
		System.out.println("Geo rules executed");
				
		String q = "SELECT ?id ?obj (MAX(?conf) AS ?confidence) WHERE { \n"+
		 		"?x gno:"+Constants.depicts+" ?y . \n" +
		 		"?x gno:"+Constants.regionHasID+" ?id . \n"+
		 		"?y gno:"+Constants.depictsObject+" ?obj. \n" +
		 		"?y gno:"+Constants.withConfidence+" ?conf . \n" +
				"} GROUP BY ?x";
		String results = GeoRulesServiceSingleton.instance.getService().parseQueryResults2CSV(q);
		System.out.println("Results:\n"+results);
		
		return "REST: classification finished";
	}

}
