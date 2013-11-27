package gr.iti.gnorasi;

import gr.iti.gnorasi.classification.ClassificationRulesBuilder;
import gr.iti.gnorasi.classification.ClassificationRulesFire;
import gr.iti.gnorasi.csvparser.Retrieval;
import gr.iti.gnorasi.csvparser.FuzzyCsvParser;
import gr.iti.gnorasi.ontology.UserOntology;
import gr.iti.gnorasi.owlim.OWLIMRepositoryFactory;
import gr.iti.gnorasi.service.rest.ServiceConfiguration;
import gr.iti.gnorasi.utils.Constants;
import gr.iti.gnorasi.utils.XMLDoc;

import java.io.ByteArrayInputStream;
import java.util.HashMap;

import org.apache.log4j.Level;
import org.apache.log4j.Logger;
import org.openrdf.model.Resource;
import org.openrdf.model.URI;
import org.openrdf.model.Value;
import org.openrdf.repository.RepositoryException;
import org.openrdf.sail.Sail;
import org.w3c.dom.NodeList;

public class GeoRulesService {
	RepositoryInitializer ri = null;
	String userContext = "";
	String hierarchyOntology = "";
	String classificationRules = "";
	private static Logger logger = Logger.getLogger(GeoRulesService.class);
	
	public String getUserContext() {
		return userContext;
	}
	
	public String getHierarchyOntology() {
		return hierarchyOntology;
	}
	
	public String getClassificationRules() {
		return classificationRules;
	}
	
	public void initializeRepository(String truncate) {
		/*com.useekm.indexing.internal.QueryEvaluatorUtil.setEvaluator(
				com.ontotext.trree.owlim_ext.SailImpl.class,
				com.useekm.indexing.internal.SesameNativeQueryEvaluator.INSTANCE
		);*/
		logger.setLevel(Level.INFO);
		Sail sail = OWLIMRepositoryFactory.getLiteSail();
		//Sail sail = new MemoryStore();
		//Sail sail = new ForwardChainingRDFSInferencer(new MemoryStore());
		
		HashMap<String, String> matchers = new HashMap<String, String>();
		matchers.put(Constants.geoNS+Constants.asWKT, Constants.POSTGISINDEX);
		//matchers.put("http://www.w3.org/2000/01/rdf-schema#label", Constants.TEXTINDEX);
		
		String user="",pass="",url="";
		url = ServiceConfiguration.getProperty("postgres.url");
		user = ServiceConfiguration.getProperty("postgres.user");
        pass = ServiceConfiguration.getProperty("postgres.password");
		
		if (url.equals(""))
			initializeRepository(sail);
		else if (truncate.equals("truncate"))
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
	
	public void initializeRepository(Sail sail) {
		ri = new RepositoryInitializer();
		ri.acquireRepositoryConnection(sail);
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
	
	/*public void separateXML(String inputXML) {
		ByteArrayInputStream inputXMLStream = new ByteArrayInputStream(inputXML.getBytes());
		XMLDoc doc = new XMLDoc(inputXMLStream);
		NodeList hierarchy = doc.getXPathNodes("output/hierarchy/RDF");
		xmlOntology = doc.nodeToString(hierarchy.item(0));
		
		NodeList classes = doc.getXPathNodes("output/classes");
		fuzzyRules = doc.nodeToString(classes.item(0));
	}*/
	
	///start multi object classification
	public void separateXMLinput(String xmlInput) {
		ByteArrayInputStream inStream = new ByteArrayInputStream(xmlInput.getBytes());
		XMLDoc doc = new XMLDoc(inStream);
		NodeList nodes = doc.getXPathNodes("root/hierarchy/RDF");
		hierarchyOntology = doc.nodeToString(nodes.item(0));
		nodes = doc.getXPathNodes("root/classes");
		classificationRules = doc.nodeToString(nodes.item(0));
		
		logger.info("Ontology:");
		logger.info(hierarchyOntology);
		logger.info("Rules");
		logger.info(classificationRules);
	}
	
	public void addGnorasiUserOntology(String context) {
		addUserOntology(hierarchyOntology, context);
	}
	
	public void parseFuzzyPropertiesCSV(String csvString, String context) {
		try {
			FuzzyCsvParser p = new FuzzyCsvParser(csvString);
			p.parse(ri.getRepositoryConnection());
		}catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public void executeClassificationRules(String context) {
		try {
			ClassificationRulesBuilder builder = new ClassificationRulesBuilder();
			builder.buildRules(classificationRules);
			ClassificationRulesFire fire = new ClassificationRulesFire();
			
			//fire.testGeospatialQuery(ri.getRepositoryConnection());
			fire.iterativeFireMethod2(builder.getRulesList(), ri.getRepositoryConnection());
			
			/*for (ClassificationRules rule : builder.getRulesList()) {
				fire.fireMethod1(rule, ri.getRepositoryConnection());
			}*/
		}catch (Exception e) {
			e.printStackTrace();
		}
	}
	///end multi object classification
	
	public String parseOntologyResults2CSV(String context) {
		Retrieval test = new Retrieval();
		String res = test.getResults2CSV(ri.getRepositoryConnection(), context);
		
		return res;
	}
	
	public String parseQueryResults2CSV(String query) {
		Retrieval test = new Retrieval();
		
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
	
	public void addUserOntology(String context) {
		UserOntology u = new UserOntology();
		userContext = u.assign(hierarchyOntology, ri.getRepositoryConnection(), context);
		u.closeConnection();
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
