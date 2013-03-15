package gr.iti.gnorasi.service.rest;

import com.sun.jersey.api.client.Client;
import com.sun.jersey.api.client.ClientResponse;
import com.sun.jersey.api.client.WebResource;
import com.sun.jersey.api.client.config.ClientConfig;
import com.sun.jersey.api.client.config.DefaultClientConfig;
import com.sun.jersey.api.container.grizzly2.GrizzlyServerFactory;
import com.sun.jersey.api.core.PackagesResourceConfig;
import com.sun.jersey.api.core.ResourceConfig;
import com.sun.jersey.api.representation.Form;

import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;
import org.glassfish.grizzly.http.server.HttpServer;
//import org.slf4j.Logger;
//import org.slf4j.LoggerFactory;
 
import gr.iti.gnorasi.utils.Constants;

import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.UriBuilder;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.PrintStream;
import java.net.URI;
 
public class Main {
	
	private static Logger logger = Logger.getLogger(Main.class);
	private static String polygon = "\"POLYGON ((35.33960634474316 25.13427050949077, 35.33960627246974 25.134278211699, 35.33959996130932 25.13427812349545, 35.33959974448613 25.1343012301182, 35.339606055646506 25.13430131832355, 35.339605983371115 25.13430902053167, 35.33959967221076 25.134308932325723, 35.339599599934886 25.134316634533228, 35.33958697761419 25.13431645812022, 35.33958690533786 25.134324160326504, 35.339580594177534 25.134324072119448, 35.339580666453834 25.13431636991376, 35.33958073872966 25.13430866770806, 35.33958704989002 25.134308755913924, 35.339587122165376 25.13430105370759, 35.339593433325746 25.134301141912882, 35.33959365014888 25.134278035291924, 35.33958733898845 25.13427794708843, 35.339587411261824 25.13427024488199, 35.33960634474316 25.13427050949077))\"^^<http://rdf.opensahara.com/type/geo/wkt>";
	 	 
    private static URI getBaseURI() {
        return UriBuilder.fromUri("http://localhost/").port(9998).build();
    }
 
    public static final URI BASE_URI = getBaseURI();
    //public static final String context = "http://www.gnorasi.gr/serviceData";
    public static final String context = "";
 
    protected static HttpServer startServer() throws IOException {
    	System.out.println("Starting grizzly...");
    	ResourceConfig rc = new PackagesResourceConfig("gr.iti.gnorasi.service.rest");
    	return GrizzlyServerFactory.createHttpServer(BASE_URI, rc);
    }
     
    public static void main(String[] args) throws IOException {
    	PropertyConfigurator.configure("log4j.properties");
    	//PrintStream printStream = new PrintStream(new FileOutputStream(new File("log/sysout.log")));
    	//System.setOut(printStream);
    	//PrintStream printStream1 = new PrintStream(new FileOutputStream(new File("log/syserr.log")));
    	//System.setErr(printStream1);
    	 
    	HttpServer httpServer = startServer();
        System.out.println(String.format("Jersey app started with WADL available at "
                + "%sapplication.wadl\nHit enter to stop it...",
               BASE_URI));
         
        /*ClientCalls call = new ClientCalls();
         
        //Launch the service
        logger.info(call.initConnection());
         
        //Send user ontology file
        logger.info(call.sendUserOntologyFromFile());
        long start = System.currentTimeMillis();
        logger.info(call.sendCSVFile());
        long end = System.currentTimeMillis();
        System.out.println("Data loaded in "+(end-start)+"ms");
        
        logger.info(call.sendFuzzyProperties());
        logger.info(call.execFuzzyRulesFromFile());
        //logger.info(call.getResults(q));
        
        //logger.info(call.sendFeatureRule(updateQuery));
        logger.info(call.sendGeoRule());
        //logger.info(call.getResults());
        logger.info(call.getResults(q));
         
        logger.info(call.closeConnection());
         */
        
        System.in.read(); //pause 
        httpServer.stop();
    }
     
     public static class ClientCalls {
    	 
    	 private WebResource service;
    	 
    	 private URI getBaseURI() {
    		 return UriBuilder.fromUri("http://localhost/").port(9998).build();
    	 }
    	 
    	 public ClientCalls() {
    		 ClientConfig config = new DefaultClientConfig();
    	 	 Client client = Client.create(config);
    	 	 service = client.resource(getBaseURI()); 
    	 }
    	 
    	 public String initConnection() {
    		 Form form = new Form();
    		 /*form.add("url", "jdbc:postgresql://localhost:5432/gnorasi_indexes");
    		 form.add("user", "postgres");
    		 form.add("pass", "gnorasi");*/
    		 form.add("truncate", "truncate");
    		    
    		 ClientResponse response = service.path("georulesservice").path("init")
    		            .type(MediaType.APPLICATION_FORM_URLENCODED)
    		            .post(ClientResponse.class, form);
    		    
    		 return response.getEntity(String.class); 
    	 }
    	 
    	 public String sendUserOntologyFromFile() {
    		 Form form = new Form();
    		 form.add("owlFile", "C:/Projects/GNORASI/Development/EclipseWorkspace/Sesame_uSeekM/ontology/userOntology2.owl");
    		 form.add("context", context);
    		    
    		 ClientResponse response = service.path("georulesservice").path("sendUserOntologyFromFile")
    		            .type(MediaType.APPLICATION_FORM_URLENCODED)
    		            .put(ClientResponse.class, form);
    		    
    		 return response.getEntity(String.class);
    	 }
    	 
    	 public String sendFeatureRule(String rule) {
    		 Form form = new Form();
    		 form.add("rules", rule);
    		 form.add("context", context);
    		 
    		 long start = System.currentTimeMillis();
    		 ClientResponse response = service.path("georulesservice").path("featureRuleString")
    		            .type(MediaType.APPLICATION_FORM_URLENCODED)
    		            .post(ClientResponse.class, form);
    		 long end = System.currentTimeMillis();
    		 System.out.println("Feature rule executed in "+(end-start)+"ms");
    		 
    		 return response.getEntity(String.class);
    	 }
    	 
    	 public String execFuzzyRulesFromFile() {
    		 Form form = new Form();
    		 form.add("ruleFile", "fuzzy/fuzzy_v2.xml");
    		 form.add("context", context);
    		 
    		 long start = System.currentTimeMillis();
    		 ClientResponse response = service.path("georulesservice").path("execFuzzyRulesFromFile")
    		            .type(MediaType.APPLICATION_FORM_URLENCODED)
    		            .post(ClientResponse.class, form);
    		 long end = System.currentTimeMillis();
    		 
    		 System.out.println("Fuzzy rules executed in "+(end-start)+"ms");
    		    
    		 return response.getEntity(String.class);
    	 }
    	 
    	 public String sendFuzzyProperties() {
    		 Form form = new Form();
    		 form.add("propertiesFile", "data/fuzzyValues.csv");
    		 form.add("context", context);
    		 
    		 long start = System.currentTimeMillis();
    		 ClientResponse response = service.path("georulesservice").path("parseFuzzyPropertiesFromFile")
    		            .type(MediaType.APPLICATION_FORM_URLENCODED)
    		            .put(ClientResponse.class, form);
    		 long end = System.currentTimeMillis();
    		 
    		 System.out.println("Fuzzy properties parsed in "+(end-start)+"ms");
    		    
    		 return response.getEntity(String.class);
    	 }
    	 
    	 public String sendGeoRule() {
    		 Form form = new Form();
    		 form.add("rulefile", "testRules/try7.xml");
    		 form.add("context", context);
    		 
    		 long start = System.currentTimeMillis();
    		 ClientResponse response = service.path("georulesservice").path("ruleFile")
    		            .type(MediaType.APPLICATION_FORM_URLENCODED)
    		            .post(ClientResponse.class, form);
    		 long end = System.currentTimeMillis();
    		 System.out.println("Geo rule executed in "+(end-start)+"ms");
    		    
    		 return response.getEntity(String.class);
    	 }
    	 
    	 public String closeConnection() {
    		 return service.path("georulesservice").path("close")
             .accept(MediaType.TEXT_XML).get(String.class);
    	 }
    	 
    	 public String sendCSVFile() {
    		 Form form = new Form();
    		 //form.add("csvfile", "C:/Projects/GNORASI/Development/EclipseWorkspace/Sesame_uSeekM/data/dataFile.csv");
    		 form.add("csvfile", "C:/Projects/GNORASI/Development/EclipseWorkspace/Sesame_uSeekM/data/feature+wkt_Data.csv");
    		 form.add("srid", "2100");
    		 form.add("context", context);
    		    
    		 ClientResponse response = service.path("georulesservice").path("sendCSVDataFile")
    		            .type(MediaType.APPLICATION_FORM_URLENCODED)
    		            .put(ClientResponse.class, form);
    		    
    		 return response.getEntity(String.class);
    	 }
    	 
    	 public String getResults() {
    		 
    		 /*String q = "SELECT * WHERE {" +
    		 				"?x gno:depicts ?y . " +
    		 				"?y gno:depictsObject ?obj. " +
    		 				"?y gno:withConfidence ?conf" +
    		 				"}";*/
    		 String q = "SELECT ?id ?obj (MAX(?conf) AS ?confidence) WHERE { \n"+
    				 		"?x gno:"+Constants.depicts+" ?y . \n" +
    				 		"?x gno:"+Constants.regionHasID+" ?id . \n"+
    				 		"?y gno:"+Constants.depictsObject+" ?obj. \n" +
    				 		"?y gno:"+Constants.withConfidence+" ?conf . \n" +
		 				"} GROUP BY ?x";
    		     		 
    		 String ret = service.path("georulesservice").path("getResultsQuery")
    				 	.queryParam("query", q)
    				 	.type(MediaType.APPLICATION_FORM_URLENCODED)
    				 	.accept(MediaType.TEXT_PLAIN)
    				 	.get(String.class);
    		 
    		 return ret;
    	 }
    	 
    	 public String getResults(String q) {
    		 
    		 String ret = service.path("georulesservice").path("getResultsQuery")
    				 	.queryParam("query", q)
    				 	.accept(MediaType.TEXT_PLAIN)
    				 	.get(String.class);
    		 
    		 return ret;
    	 }
     }
 }
 
 