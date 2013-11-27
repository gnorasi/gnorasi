package gr.iti.gnorasi.owlim;

import gr.iti.gnorasi.utils.Constants;

import java.io.File;
import java.io.FileReader;
import java.net.URLDecoder;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import org.openrdf.model.Graph;
import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;
import org.openrdf.model.ValueFactory;
import org.openrdf.model.impl.GraphImpl;
import org.openrdf.model.impl.URIImpl;
import org.openrdf.model.impl.ValueFactoryImpl;
import org.openrdf.model.vocabulary.RDF;
import org.openrdf.repository.Repository;
import org.openrdf.repository.RepositoryConnection;
import org.openrdf.repository.RepositoryException;
import org.openrdf.repository.config.RepositoryConfig;
import org.openrdf.repository.manager.LocalRepositoryManager;
import org.openrdf.repository.manager.RepositoryManager;
import org.openrdf.rio.RDFFormat;
import org.openrdf.rio.RDFHandler;
import org.openrdf.rio.RDFHandlerException;
import org.openrdf.rio.RDFParser;
import org.openrdf.rio.Rio;
import org.openrdf.sail.Sail;

import com.ontotext.trree.owlim_ext.config.OWLIMSailConfig;
import com.ontotext.trree.owlim_ext.config.OWLIMSailFactory;


public class OWLIMRepository {
	
	Repository repository;
	
	OWLIMRepository() {
		repository = null;
	}
	
	public Repository getRepository() {
		return repository;
	}
	
	Graph parseFile(File configurationFile, RDFFormat format, String defaultNamespace) {
		final Graph graph = new GraphImpl();
		RDFParser parser = Rio.createParser(format);
		RDFHandler handler = new RDFHandler() {
			public void endRDF() throws RDFHandlerException {
			}

			public void handleComment(String arg0) throws RDFHandlerException {
			}

			public void handleNamespace(String arg0, String arg1) throws RDFHandlerException {
			}

			public void handleStatement(Statement statement) throws RDFHandlerException {
				graph.add(statement);
			}

			public void startRDF() throws RDFHandlerException {
			}
		};
		parser.setRDFHandler(handler);
		
		try {
			parser.parse(new FileReader(configurationFile), defaultNamespace);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return graph;
	}
	
	public void setNativeRepository(String configFilename, String mainNS, String repName) {
		File configFile = new File(configFilename);
		// Parse the configuration file, assuming it is in Turtle format
		final Graph graph = parseFile(configFile, RDFFormat.TURTLE, mainNS);

		// Look for the subject of the first matching statement for
		// "?s type Repository"
		Iterator<Statement> iter = graph.match(null, RDF.TYPE, new URIImpl(
				"http://www.openrdf.org/config/repository#Repository"));
		Resource repositoryNode = null;
		if (iter.hasNext()) {
			Statement st = iter.next();
			repositoryNode = st.getSubject();
		}

		// Create a manager for local repositories
		RepositoryManager repositoryManager = new LocalRepositoryManager(new File("RepManager"));

		try {
			// Initialize the repository manager
			repositoryManager.initialize();
	
			// Create a configuration object from the configuration file and add it
			// to the repositoryManager
			RepositoryConfig repositoryConfig = RepositoryConfig.create(graph, repositoryNode);
			repositoryManager.addRepositoryConfig(repositoryConfig);
			// Get the repository to use
			repository = repositoryManager.getRepository(repName);

		}catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public Sail getOWLIMLiteSail() {
		OWLIMSailConfig owlimConfig = new OWLIMSailConfig();
		Map<URI, String> configMap = new HashMap<URI, String>();
		ValueFactory f = new ValueFactoryImpl();
		try {
			String jarPath = URLDecoder.decode(OWLIMRepository.class.getProtectionDomain().getCodeSource().getLocation().getPath(), "UTF-8");
			//String path = (new File(jarPath)).getParentFile().getParentFile().getPath()+System.getProperty("file.separator");
			String path = (new File(jarPath)).getParentFile()+System.getProperty("file.separator");
			System.out.println("OWLIM path is: "+path);
			//configMap.put(new URIImpl("http://www.ontotext.com/trree/owlim#owlim-license"), "ITI_GR_OWLIM_SE_expires-01-04-2015_latest-01-04-2015_4cores.license");
			configMap.put(f.createURI("http://www.ontotext.com/trree/owlim#ruleset"), "owl-horst-optimized");
			configMap.put(f.createURI("http://www.ontotext.com/trree/owlim#noPersist"), "true");
			configMap.put(f.createURI("http://www.ontotext.com/trree/owlim#storage-folder"), path+"storage");
			configMap.put(f.createURI("http://www.ontotext.com/trree/owlim#base-URL"), Constants.mainNS);
			//configMap.put(f.createURI("http://www.ontotext.com/trree/owlim#num.threads.run"), "4");
			//configMap.put(f.createURI("http://www.ontotext.com/trree/owlim#entity-index-size"), "200000");
			//configMap.put(f.createURI("http://www.ontotext.com/trree/owlim#jobsize"), "200");
			//configMap.put(f.createURI("http://www.ontotext.com/trree/owlim#repository-type"), "in-memory-repository");
			//configMap.put(f.createURI("http://www.ontotext.com/trree/owlim#imports"), path+"ontology/gnorasi_v2.owl");
			configMap.put(f.createURI("http://www.ontotext.com/trree/owlim#imports"), path+"ontology/demo_gnorasi_v5.owl");
			configMap.put(f.createURI("http://www.ontotext.com/trree/owlim#defaultNS"), Constants.mainNS);
			
			owlimConfig.setConfigParams(configMap);
		
			return new OWLIMSailFactory().getSail(owlimConfig);
		
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return null;
	}

	public Sail getOWLIMSESail() {
/*		OWLIMParentSailConfig owlimConfig = new OWLIMParentSailConfig();
		Map<URI, String> configMap = new HashMap<URI, String>();

		configMap.put(new URIImpl("http://www.ontotext.com/trree/owlim#cache-memory"), "180m");
		configMap.put(new URIImpl("http://www.ontotext.com/trree/owlim#owlim-license"), "ITI_GR_OWLIM_SE_expires-01-04-2015_latest-01-04-2015_4cores.license");
		configMap.put(new URIImpl("http://www.ontotext.com/trree/owlim#ruleset"), "owl-horst-optimized");
		configMap.put(new URIImpl("http://www.ontotext.com/trree/owlim#storage-folder"), "storage");
		configMap.put(new URIImpl("http://www.ontotext.com/trree/owlim#base-URL"), "http://www.gnorasi.gr/ontology#");
		configMap.put(new URIImpl("http://www.ontotext.com/trree/owlim#entity-index-size"), "5000000");
		configMap.put(new URIImpl("http://www.ontotext.com/trree/owlim#repository-type"), "file-repository");
		configMap.put(new URIImpl("http://www.ontotext.com/trree/owlim#imports"), "./ontology/gnorasi.owl");
		configMap.put(new URIImpl("http://www.ontotext.com/trree/owlim#defaultNS"), "http://www.gnorasi.gr/ontology#");
		
		owlimConfig.setConfigParams(configMap);
		
		try {
			return new OWLIMParentSailFactory().getSail(owlimConfig);
		} catch (SailConfigException e) {
			e.printStackTrace();
		}
*/		
		return null;
}
	

	public RepositoryConnection getRepositoryConnection() {
		RepositoryConnection conn = null;
		try {
			if (repository != null)
				conn = repository.getConnection();
		} catch (RepositoryException e) {
			e.printStackTrace();
		}
		
		return conn;
	}
}
