package gr.iti.gnorasi.ontology;

import gr.iti.gnorasi.utils.Constants;
import gr.iti.gnorasi.utils.Utilities;

import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

import org.openrdf.model.Statement;
import org.openrdf.model.URI;
import org.openrdf.model.ValueFactory;
import org.openrdf.model.impl.ValueFactoryImpl;
import org.openrdf.repository.Repository;
import org.openrdf.repository.RepositoryConnection;
import org.openrdf.repository.RepositoryException;
import org.openrdf.repository.RepositoryResult;
import org.openrdf.repository.sail.SailRepository;
import org.openrdf.rio.RDFFormat;
import org.openrdf.sail.memory.MemoryStore;

public class UserOntology {
	
	RepositoryConnection userCon;
	
	public UserOntology() {}
	
	public String assign(String ontology, RepositoryConnection con, String context) {
		InputStream is = new ByteArrayInputStream(ontology.getBytes());
		is.mark(5000);
		String context1 = "";
		if (context != null) {
			if (!context.equals("")) {
				context1 = context;
			}
			else 
				context1 = Utilities.getBaseNamespace(is);
		}
		else 
			context1 = Utilities.getBaseNamespace(is);
		
		try {	
			is.reset();
			//Constants.userNS = Utilities.getBaseNamespace(is);
			Constants.userNS = context1;
			Constants.userNSURI = con.getValueFactory().createURI(Constants.userNS);
			
			is.reset();
			Repository myRepository = new SailRepository(new MemoryStore());
			myRepository.initialize();
			//URI uriContext = null;
			//uriContext = con.getValueFactory().createURI(context1);
			
			userCon = myRepository.getConnection();
			
			userCon.add(is, Constants.mainNS, RDFFormat.RDFXML);
			
			RepositoryResult<Statement> iter = userCon.getStatements(null, null, null, false);
			while (iter.hasNext()) {
				Statement s = iter.next();
				//if (uriContext != null)
				//	con.add(s, uriContext);
				if (Constants.userNSURI != null)
					con.add(s, Constants.userNSURI);
				else
					con.add(s);
			}
			con.commit();
		}catch (Exception e) {
			e.printStackTrace();
		}finally {
			try {
				is.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		
		if (!Constants.queryPrefix.contains("PREFIX gnoUser:"))
			Constants.queryPrefix += "PREFIX gnoUser: <"+context1+">\n";
		
		return context1;
		
	}
	
	public String assignFromFile(String file, RepositoryConnection con, String context) {
		String base = "";
		try {
			System.out.println("user file is "+file);
			BufferedReader br = new BufferedReader(new FileReader(file));
			String line = "";
			StringBuilder  stringBuilder = new StringBuilder(1000);
		    String ls = System.getProperty("line.separator");
			
		    while ((line = br.readLine()) != null) {
				stringBuilder.append(line).append(ls);
			}
		    stringBuilder.trimToSize();
		    base = assign(stringBuilder.toString(), con, context);
			br.close();
		}catch (Exception e) {
			e.printStackTrace();
		}
		
		return base;
	}
	
	public void closeConnection() {
		if (userCon != null) {
			try {
				userCon.close();
			} catch (RepositoryException e) {
				e.printStackTrace();
			}
		}
	}
	
}
