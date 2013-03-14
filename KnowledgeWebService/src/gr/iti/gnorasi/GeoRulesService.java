package gr.iti.gnorasi;

import gr.iti.gnorasi.csvparser.CSV2SPARQL;
import gr.iti.gnorasi.ontology.UserOntology;
import gr.iti.gnorasi.owlim.OWLIMRepositoryFactory;
import gr.iti.gnorasi.rules.feature.FeatureRule;
import gr.iti.gnorasi.rules.fuzzy.FuzzyCSV2SPARQL;
import gr.iti.gnorasi.rules.fuzzy.FuzzyRestriction;
import gr.iti.gnorasi.rules.fuzzy.FuzzyToConstruct;
import gr.iti.gnorasi.rules.geo.RuleBuilder;
import gr.iti.gnorasi.rules.geo.RuleFireAssignedRegions;
import gr.iti.gnorasi.rules.geo.RuleFireUnassignedRegions;
import gr.iti.gnorasi.service.rest.Main;
import gr.iti.gnorasi.utils.Constants;

import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.StringReader;
import java.util.HashMap;
import java.util.Properties;

import org.apache.log4j.Logger;
import org.openrdf.model.Resource;
import org.openrdf.model.URI;
import org.openrdf.model.Value;
import org.openrdf.repository.RepositoryException;
import org.openrdf.sail.Sail;
import org.openrdf.sail.memory.MemoryStore;
import org.openrdf.sail.inferencer.fc.ForwardChainingRDFSInferencer;

import com.useekm.indexing.postgis.PostgisIndexMatcher;

public class GeoRulesService {
	RepositoryInitializer ri = null;
	String userContext = "";
	private static Logger logger = Logger.getLogger(GeoRulesService.class);
	
	public String getUserContext() {
		return userContext;
	}
	
	public void initializeRepository(String truncate) {
		com.useekm.indexing.internal.QueryEvaluatorUtil.setEvaluator(
				com.ontotext.trree.owlim_ext.SailImpl.class,
				com.useekm.indexing.internal.SesameNativeQueryEvaluator.INSTANCE
		);
		
		Sail sail = OWLIMRepositoryFactory.getLiteSail();
		//Sail sail = new MemoryStore();
		//Sail sail = new ForwardChainingRDFSInferencer(new MemoryStore());
				
		HashMap<String, String> matchers = new HashMap<String, String>();
		matchers.put(Constants.geoNS+Constants.asWKT, Constants.POSTGISINDEX);
		//matchers.put("http://www.w3.org/2000/01/rdf-schema#label", Constants.TEXTINDEX);
		
		Properties prop = new Properties();
		String user="",pass="",url="";
		try {
	    	prop.load(new FileInputStream("conf/indexingSail.properties"));
	 
	    	url = prop.getProperty("postgres.url");
	        user = prop.getProperty("postgres.user");
	        pass = prop.getProperty("postgres.password");
    	}catch (IOException ex) {
    		ex.printStackTrace();
        }
		
		if (truncate.equals("truncate"))
			initializeRepository(url, user, pass, matchers, sail, true);
		else if (truncate.equals("append"))
			initializeRepository(url, user, pass, matchers, sail, false);
		else
			initializeRepository(url, user, pass, matchers, sail, true);
	}
	
	public void initializeRepository(String url, String user, String pass, boolean truncate) {
		Sail sail = OWLIMRepositoryFactory.getLiteSail();
		
		HashMap<String, String> matchers = new HashMap<String, String>();
		matchers.put(Constants.geoNS+Constants.asWKT, Constants.POSTGISINDEX);
		//matchers.put("http://www.w3.org/2000/01/rdf-schema#label", Constants.TEXTINDEX);
		
		initializeRepository(url, user, pass, matchers, sail, truncate);
	}
	
	public void initializeRepository(String url, String user, String pass, HashMap<String, String> matchers, Sail sail, boolean truncate) {
		ri = new RepositoryInitializer();
		ri.setPgDatasource(url, user, pass);
		ri.acquireRepositoryConnection(sail, matchers, truncate);
	}
	
	public void closeConnections() {
		try {
			/*if (ri.getDatasource() != null)
				ri.getDatasource().close();
			if (ri.getRepositoryConnection() != null)
				ri.getRepositoryConnection().close();*/
			ri.closeAll();
		}catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public void deleteContextData(String context) {
		try {
			ri.getRepositoryConnection().remove((Resource)null, null, null, ri.getRepositoryConnection().getValueFactory().createURI(context));
			ri.getRepositoryConnection().commit();
		} catch (RepositoryException e) {
			e.printStackTrace();
		}
	}
	
	public void executeRules(String inString, String context) {
		/*try {
			RuleBuilder rb = new RuleBuilder(new ByteArrayInputStream(inString.getBytes()));
			rb.parseRules();
			rb.fireAllRulesIterative(ri.getDatasource(), ri.getRepositoryConnection(), ri.getRepositoryConnection().getValueFactory().createURI(context));
		}catch (Exception e) {
			e.printStackTrace();
		}*/
		try {
			RuleBuilder rb = new RuleBuilder(new ByteArrayInputStream(inString.getBytes()));
			rb.parseRules();
			RuleFireUnassignedRegions rf = new RuleFireUnassignedRegions();
			rf.fireAll(rb.getRules(), ri.getDatasource(), ri.getRepositoryConnection(), ri.getRepositoryConnection().getValueFactory().createURI(context));
			
			RuleFireAssignedRegions rf2 = new RuleFireAssignedRegions();
			rf2.fireAll(rb.getRules(), ri.getDatasource(), ri.getRepositoryConnection(), ri.getRepositoryConnection().getValueFactory().createURI(context));
		}catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public void executeRulesFromFile(String inFile, String context) {
		/*try {
			if ((new File(inFile)).isFile()) {
				RuleBuilder rb = new RuleBuilder(new FileInputStream(inFile));
				rb.parseRules();
				rb.fireAllRulesIterative(ri.getDatasource(), ri.getRepositoryConnection(), ri.getRepositoryConnection().getValueFactory().createURI(context));
			}
		}catch (Exception e) {
			e.printStackTrace();
		}*/
		try {
			if ((new File(inFile)).isFile()) {
				RuleBuilder rb = new RuleBuilder(new FileInputStream(inFile));
				rb.parseRules();
				RuleFireUnassignedRegions rf = new RuleFireUnassignedRegions();
				rf.fireAll(rb.getRules(), ri.getDatasource(), ri.getRepositoryConnection(), ri.getRepositoryConnection().getValueFactory().createURI(context));
				RuleFireAssignedRegions rf2 = new RuleFireAssignedRegions();
				rf2.fireAll(rb.getRules(), ri.getDatasource(), ri.getRepositoryConnection(), ri.getRepositoryConnection().getValueFactory().createURI(context));
			}
		}catch (Exception e) {
			e.printStackTrace();
		}
	}

	
	public void parseCSVString2Ontology(String csvString, int srid, String context, boolean parseFeatures) {
		CSV2SPARQL test = new CSV2SPARQL();
		test.setMappingsFile("conf/mappings_v2.xml");
		
		BufferedReader reader = new BufferedReader(new StringReader(csvString));
		test.parseCSV(reader, ri.getRepositoryConnection(), context, srid, true, parseFeatures);
		try {
			reader.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public void parseCSVFile2Ontology(String csvFile, int srid, String context, boolean parseFeatures) {
		System.out.println("Inside 'parseCSVFile2Ontology'");
		CSV2SPARQL test = new CSV2SPARQL();
		test.setMappingsFile("conf/mappings_v2.xml");
		BufferedReader reader = null;
		try {
			if ((new File(csvFile)).isFile()) {
				System.out.println("Found '"+csvFile+"'.");
				reader = new BufferedReader(new FileReader(csvFile));
				test.parseCSV(reader, ri.getRepositoryConnection(), context, srid, false, parseFeatures);
				reader.close();
			}
			else {
				System.out.println("File '"+csvFile+"' not present or corrupted.");
			}
		}catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public String parseOntologyResults2CSV(String context) {
		CSV2SPARQL test = new CSV2SPARQL();
		String res = test.getResults2CSV(ri.getRepositoryConnection(), context);
		
		return res;
	}
	
	public String parseQueryResults2CSV(String query) {
		CSV2SPARQL test = new CSV2SPARQL();
		
		String res = test.getQueryResults2CSV(ri.getRepositoryConnection(), query);
		
		return res;
	}
	
	public void addUserOntologyFromFile(String file, String context) {
		UserOntology u = new UserOntology();
		userContext = u.assignFromFile(file, ri.getRepositoryConnection(), context);
		u.closeConnection();
	}
	
	public void addUserOntology(String ontology, String context) {
		UserOntology u = new UserOntology();
		userContext = u.assign(ontology, ri.getRepositoryConnection(), context);
		u.closeConnection();
	}
	
	public void addFuzzyPropertiesFromFile(String propertiesFile, String context) {
		FuzzyCSV2SPARQL f = new FuzzyCSV2SPARQL();
		BufferedReader reader = null;
		try {
			if ((new File(propertiesFile)).isFile()) {
				System.out.println("Found '"+propertiesFile+"'.");
				reader = new BufferedReader(new FileReader(propertiesFile));
				f.parseCSV(reader, ri.getRepositoryConnection(), context, false);
			}
			else {
				System.out.println("File '"+propertiesFile+"' not present or corrupted.");
			}
		}catch (Exception e) {
			System.out.println("File '"+propertiesFile+"' not present or corrupted.");
			e.printStackTrace();
		}
		finally {
			if (reader != null)
				try {
					reader.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
		}
	}
	
	public void execFuzzyRules(String rules, String context) {
		FuzzyToConstruct ftc = new FuzzyToConstruct();
		//ftc.parseRestrictions(new ByteArrayInputStream(rules.getBytes()));
		ftc.parseRules(new ByteArrayInputStream(rules.getBytes()));
		ftc.exec(ri.getRepositoryConnection(), context);
	}
	
	public void execFuzzyRulesFromFile(String file, String context) {
		try {
			FuzzyToConstruct ftc = new FuzzyToConstruct();
			ftc.parseRules(new FileInputStream(file));
			ftc.exec(ri.getRepositoryConnection(), context);
		}catch (Exception e) {
			e.printStackTrace();
		}
		
	}
	
	public void executeFeatureRules(String ruleString, String context) {
		logger.info("inside featureService");
		if (!ruleString.equals("")) {
			FeatureRule f = new FeatureRule();
			f.parseRules(ruleString);
			f.runRules(ri.getRepositoryConnection(), context);
		}
	}
	
	public void reset(String context) {
		if (!context.equals("")) {
			try {
				ri.getRepositoryConnection().remove((Resource)null, (URI)null, (Value)null, ri.getRepositoryConnection().getValueFactory().createURI(context));
			} catch (RepositoryException e) {
				e.printStackTrace();
			}
		}
	}
}
