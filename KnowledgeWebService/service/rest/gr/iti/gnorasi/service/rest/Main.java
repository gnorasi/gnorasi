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
 
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.UriBuilder;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.net.URI;
import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
 
public class Main {
	
	private static Logger logger = Logger.getLogger(Main.class);
	private static String host = "locahost";
	private static int port = 9998;
	public static String context = "";
	
	static {
		ServiceConfiguration.readConfigurationFile("conf/service.properties");
		
		String temp = ServiceConfiguration.getProperty("service.host");
		if (temp != null)
    		host = temp;
		
		temp = ServiceConfiguration.getProperty("service.port");
		if (temp != null) {
    		try {
    			port = Integer.parseInt(temp);
    		}catch (NumberFormatException e) {
    			logger.error(e.getStackTrace());
    		}
    	}
	}
	
    private static URI getBaseURI() {
        return UriBuilder.fromUri("http://"+host+"/").port(port).build();
    }
 
    public static final URI BASE_URI = getBaseURI();
         
    protected static HttpServer startServer() throws IOException {
    	System.out.println("Starting grizzly...");
    	ResourceConfig rc = new PackagesResourceConfig("gr.iti.gnorasi.service.rest");
    	Map<String, Object> map = new HashMap<String, Object>(); 
    	map.put(ResourceConfig.FEATURE_DISABLE_WADL, true);
    	rc.setPropertiesAndFeatures(map);
    	return GrizzlyServerFactory.createHttpServer(BASE_URI, rc);
    }
     
    public static void main(String[] args) throws IOException {
    	PropertyConfigurator.configure("log4j.properties");
    	 
    	HttpServer httpServer = startServer();
        System.out.println(String.format("Jersey app started with WADL available at "
                + "%sapplication.wadl\nHit enter to stop it...",
               BASE_URI));
        
        /*
        ClientCalls call = new ClientCalls();
        
        //call.tempReplace("C:/Projects/GNORASI/Development/HgRepository/gnorasi-clone/data/bandStatistics+wkt_Data.csv", "C:/Projects/GNORASI/Development/HgRepository/gnorasi-clone/data/ontologyClassificationProcessor_2.csv");
        
        //Launch the service 
        logger.info(call.initConnection());
         
        logger.info(call.processGnorasiFile());
        logger.info(call.addGnorasiUserOntology());
        
        long start = System.currentTimeMillis();
        logger.info(call.processGnorasiFuzzyFeaturesFile());
        long end = System.currentTimeMillis();
        System.out.println("Data loaded in "+(end-start)+"ms");
        
        start = System.currentTimeMillis();
        logger.info(call.addGnorasiUserRules());
        end = System.currentTimeMillis();
        System.out.println("Rules executed in "+(end-start)+"ms");
        
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
    	 
    	 // start multi-object map classification
    	 public String processGnorasiFile() {
    		 Form form = new Form();
    		 //form.add("file", "C:/Projects/GNORASI/Development/HgRepository/gnorasi-clone/data/ontologyClassificationProcessor-data_2_Water.xml");
    		 form.add("file", "C:/Projects/GNORASI/Development/HgRepository/gnorasi-mklab2/data/csv2.csv");
    		 form.add("context", context);
    		 
    		 ClientResponse response = service.path("georulesservice").path("processGnorasiFile")
    		            .type(MediaType.APPLICATION_FORM_URLENCODED)
    		            .post(ClientResponse.class, form);
    		    
    		 return response.getEntity(String.class);
    	 }
    	 
    	 public String processGnorasiFuzzyFeaturesFile() {
    		 Form form = new Form();
    		 //form.add("file", "C:/Projects/GNORASI/Development/HgRepository/gnorasi-clone/data/ontologyClassificationProcessor_2.csv");
    		 //form.add("file", "C:/temp/tempReplace.csv");
    		 form.add("file", "C:/Projects/GNORASI/Development/HgRepository/gnorasi-mklab2/data/csv1.csv");
    		 form.add("context", context);
    		    
    		 ClientResponse response = service.path("georulesservice").path("processFuzzyPropertiesFile")
    		            .type(MediaType.APPLICATION_FORM_URLENCODED)
    		            .post(ClientResponse.class, form);
    		    
    		 return response.getEntity(String.class);
    	 }
    	 
    	 public String addGnorasiUserOntology() {
    		 
    		 String ret = service.path("georulesservice").path("processGnorasiUserOntology")
    				 	.queryParam("context", context)
    				 	.accept(MediaType.TEXT_PLAIN)
    				 	.get(String.class);
    		 
    		 return ret;
    	 }
    	 
    	 public String addGnorasiUserRules() {
    		 
    		 String ret = service.path("georulesservice").path("processGnorasiUserRules")
    				 	.queryParam("context", context)
    				 	.accept(MediaType.TEXT_PLAIN)
    				 	.get(String.class);
    		 
    		 return ret;
    	 }
    	// end multi-object map classification
    	 
    	 
    	 public String sendUserOntologyFromFile() {
    		 Form form = new Form();
    		 form.add("owlFile", "C:/Projects/GNORASI/Development/EclipseWorkspace/Sesame_uSeekM/ontology/userOntology2.owl");
    		 form.add("context", context);
    		    
    		 ClientResponse response = service.path("georulesservice").path("sendUserOntologyFromFile")
    		            .type(MediaType.APPLICATION_FORM_URLENCODED)
    		            .put(ClientResponse.class, form);
    		    
    		 return response.getEntity(String.class);
    	 }
    	 
    	 
    	 public String closeConnection() {
    		 return service.path("georulesservice").path("close")
             .accept(MediaType.TEXT_XML).get(String.class);
    	 }
    	 
    	 public String getResults(String q) {
    		 
    		 String ret = service.path("georulesservice").path("getResultsQuery")
    				 	.queryParam("query", q)
    				 	.accept(MediaType.TEXT_PLAIN)
    				 	.get(String.class);
    		 
    		 return ret;
    	 }
    	 
    	 public void tempReplace(String sourceFile, String targetFile) {
    		 try {
    			 String outFile = "C:/temp/tempReplace.csv";
    			 BufferedReader sourceReader = new BufferedReader(new FileReader(sourceFile));
    			 BufferedReader targetReader = new BufferedReader(new FileReader(targetFile));
    			 BufferedWriter targetWriter = new BufferedWriter(new FileWriter(outFile));
    			 
    			 String sourceLine, targetLine;
    			 while ((targetLine = targetReader.readLine()) != null) {
    				 sourceLine = sourceReader.readLine();
    				 if (sourceLine != null) {
    					 Pattern WKTPattern = Pattern.compile("POLYGON [^\\n]+");
    					 Matcher m = WKTPattern.matcher(sourceLine);
    					 if (m.find()) {
	    					 targetLine += ";\"" + m.group();
	    					 targetWriter.append(targetLine);
	    					 targetWriter.newLine();
    					 }
    				 }
    				 else
    					 break;
    			 }
    			 sourceReader.close();
    			 targetReader.close();
    			 targetWriter.close();
    		 }catch (Exception e) {
    			 e.printStackTrace();
    		 }
    	 }
     }
 }
 
 