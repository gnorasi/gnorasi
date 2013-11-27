package gr.iti.gnorasi.service.rest;

import java.io.BufferedReader;
import java.io.FileReader;

import gr.iti.gnorasi.utils.Constants;
import gr.iti.gnorasi.utils.Utilities;

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
	
	@GET
	@Path("/test")
	@Produces(MediaType.TEXT_HTML)
	public String test() {
				
		return "<html><body>REST: test!!!</body></html>";
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
	
	//start multi-object classification
	@POST
	@Path("/processGnorasiFile")
	@Produces(MediaType.TEXT_XML)
	@Consumes(MediaType.APPLICATION_FORM_URLENCODED)
	public String processGnorasiFile(@FormParam("file") String inString) {

		String contents = Utilities.fileToString(inString);
		GeoRulesServiceSingleton.instance.getService().separateXMLinput(contents);

		return "REST: processGnorasiFile executed";
	}
	
	@POST
	@Path("/parseGnorasiHierarchyAndRules")
	@Produces(MediaType.TEXT_XML)
	@Consumes(MediaType.APPLICATION_FORM_URLENCODED)
	public String parseGnorasiHierarchyAndRules(@FormParam("contents") String contents) {

		//String contents = Utilities.fileToString(inString);
		GeoRulesServiceSingleton.instance.getService().separateXMLinput(contents);

		return "REST: processGnorasiFile executed";
	}
	
	@GET
	@Path("/processGnorasiUserOntology")
	@Consumes(MediaType.TEXT_PLAIN)
	@Produces(MediaType.TEXT_PLAIN)
	public String processGnorasiUserOntology(@QueryParam("context") String context) {
		
		GeoRulesServiceSingleton.instance.getService().addGnorasiUserOntology(context);
		System.out.println(context);
		
		return "REST: ontology added";
	}
	
	@GET
	@Path("/processGnorasiUserRules")
	@Consumes(MediaType.TEXT_PLAIN)
	@Produces(MediaType.TEXT_PLAIN)
	public String processGnorasiUserRules(@QueryParam("context") String context) {
		
		GeoRulesServiceSingleton.instance.getService().executeClassificationRules(context);
		System.out.println(context);
		
		return "REST: rules executed";
	}

	
	@POST
	@Path("/processFuzzyPropertiesFile")
	@Produces(MediaType.TEXT_XML)
	@Consumes(MediaType.APPLICATION_FORM_URLENCODED)
	public String processFuzzyPropertiesFile(@FormParam("file") String inString, 
											 @FormParam("context") String context) {

		String contents = Utilities.fileToString(inString);
		GeoRulesServiceSingleton.instance.getService().parseFuzzyPropertiesCSV(contents, context);

		return "REST: processFuzzyPropertiesFile executed";
	}
	
	@POST
	@Path("/processGnorasiFuzzyProperties")
	@Produces(MediaType.TEXT_XML)
	@Consumes(MediaType.APPLICATION_FORM_URLENCODED)
	public String processGnorasiFuzzyProperties(@FormParam("contents") String contents, 
											 @FormParam("context") String context) {

		GeoRulesServiceSingleton.instance.getService().parseFuzzyPropertiesCSV(contents, context);

		return "REST: processFuzzyPropertiesFile executed";
	}

	//end multi-object classification
	
	
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
	//@Consumes(MediaType.TEXT_PLAIN)
	@Consumes(MediaType.APPLICATION_FORM_URLENCODED)
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
		return results;
	}
	
	@POST
	@Path("/performClassification")
	@Produces(MediaType.TEXT_PLAIN)
	@Consumes(MediaType.APPLICATION_FORM_URLENCODED)
	public String performClassification(@FormParam("xmlcontents") String xmlContents, 
										@FormParam("csvcontents") String csvContents,
										@FormParam("context") String context) {

		GeoRulesServiceSingleton.instance.getService().separateXMLinput(xmlContents);
		GeoRulesServiceSingleton.instance.getService().addGnorasiUserOntology(context);
		GeoRulesServiceSingleton.instance.getService().parseFuzzyPropertiesCSV(csvContents, context);
		GeoRulesServiceSingleton.instance.getService().executeClassificationRules(context);
		String results = GeoRulesServiceSingleton.instance.getService().parseOntologyResults2CSV(getUserContext(context));

		System.out.println(results);
		
		return results;
		
		//return "REST: processFuzzyPropertiesFile executed";
	}

}
