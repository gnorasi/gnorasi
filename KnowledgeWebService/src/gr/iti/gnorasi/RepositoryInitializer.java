package gr.iti.gnorasi;

import gr.iti.gnorasi.utils.Constants;

import java.sql.SQLException;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Set;
import java.util.Map.Entry;

import org.apache.commons.dbcp.BasicDataSource;
import org.apache.log4j.Logger;
import org.openrdf.repository.Repository;
import org.openrdf.repository.RepositoryConnection;
import org.openrdf.repository.RepositoryException;
import org.openrdf.repository.sail.SailRepository;
import org.openrdf.sail.Sail;

import com.useekm.indexing.IndexingSail;
import com.useekm.indexing.postgis.PostgisIndexMatcher;
import com.useekm.indexing.postgis.PostgisIndexerSettings;

public class RepositoryInitializer {
	BasicDataSource pgDatasource;
	RepositoryConnection conn;
	private static Logger logger = Logger.getLogger(RepositoryInitializer.class);
	
	public RepositoryInitializer() {
		pgDatasource = null;
		conn = null;
	}
	
	public void setPgDatasource(String url, String user, String pass) {
		pgDatasource = new BasicDataSource();
		pgDatasource.setDriverClassName("org.postgresql.Driver");
		pgDatasource.setUrl(url);
		pgDatasource.setUsername(user);
		pgDatasource.setPassword(pass);
	}
	
	public IndexingSail getIndexingSail(Sail sail, HashMap<String, String> matchers, boolean truncate) {
		PostgisIndexerSettings settings = new PostgisIndexerSettings();
		settings.setDataSource(pgDatasource);
		
		//specify statements that should be indexed:
		PostgisIndexMatcher[] postgisIndexMatcher = new PostgisIndexMatcher[matchers.size()];
		Set<Entry<String, String>> set = matchers.entrySet();
		Iterator<Entry<String, String>> iter = set.iterator();
		int counter = 0;
		while (iter.hasNext()) {
			Entry<String, String> entry = iter.next();
			
			PostgisIndexMatcher matcher1 = new PostgisIndexMatcher();
			matcher1.setPredicate(entry.getKey());
			
			if (entry.getValue().equals(Constants.TEXTINDEX))
				matcher1.setSearchConfig("simple");
			
			postgisIndexMatcher[counter] = matcher1;
			
			counter++;
		}
		
		// add matchers for each predicate for which statements need to be indexed:
		settings.setMatchers(Arrays.asList(postgisIndexMatcher));
		
		//truncate any tables
		//if (truncate)
		//	settings.initialize(truncate);
		
		//Initialize the IndexingSail that wraps your BigdataSail:
		IndexingSail idxSail = new IndexingSail(sail, settings);
		
		//truncate any tables
		if (truncate)
			settings.initialize(truncate);
		
		return idxSail;
	}
	
	public Repository setRepository(IndexingSail idxSail) {
		Repository repository = new SailRepository(idxSail);
		try {
			repository.initialize();
		} catch (RepositoryException e) {
			e.printStackTrace();
		}
		
		return repository;
	}
	
	public Repository setRepository(Sail sail) {
		Repository repository = new SailRepository(sail);
		try {
			repository.initialize();
		} catch (RepositoryException e) {
			e.printStackTrace();
		}
		
		return repository;
	}
	
	public void setRepositoryConnection(Repository rep) throws RepositoryException {
		conn = rep.getConnection();
		conn.setAutoCommit(false);
	}
	
	public BasicDataSource getDatasource() {
		return pgDatasource;
	}
	
	public RepositoryConnection getRepositoryConnection() {
		return conn;
	}
	
	public RepositoryConnection acquireRepositoryConnection(Sail sail, HashMap<String, String> matchers, boolean truncate) {
		try {
			IndexingSail idxSail = getIndexingSail(sail, matchers, truncate);
			Repository rep = setRepository(idxSail);
			setRepositoryConnection(rep);
		}catch (Exception e) {
			e.printStackTrace();
		}
		
		return conn;	
	}
	
	public RepositoryConnection acquireRepositoryConnection(Sail sail) {
		try {
			Repository rep = setRepository(sail);
			setRepositoryConnection(rep);
		}catch (Exception e) {
			e.printStackTrace();
		}
		
		return conn;
	}
	
	public void closeAll() {
		try {
			if (conn != null) {
				conn.clear();
				conn.commit();
				conn.close();
				conn = null;
				System.out.println("Repository connection is closed...");
				logger.info("Repository connection is closed...");
			}
			if (pgDatasource != null) {
				pgDatasource.close();
				pgDatasource = null;
				System.out.println("Datasource connection is closed...");
				logger.info("Datasource connection is closed...");
			}
		}catch (RepositoryException e) {
			System.out.println("Repository cannot be closed");
			logger.info("Repository connection cannot be closed...");
			e.printStackTrace();
		}catch (SQLException e) {
			System.out.println("Datasource cannot be closed");
			logger.info("Datasource connection cannot be closed...");
			e.printStackTrace();
		}
	}
}
