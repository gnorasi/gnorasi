package gr.iti.gnorasi.rules.feature;

import gr.iti.gnorasi.service.rest.Main;
import gr.iti.gnorasi.utils.Constants;

import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.log4j.Logger;
import org.openrdf.query.MalformedQueryException;
import org.openrdf.query.QueryLanguage;
import org.openrdf.query.Update;
import org.openrdf.query.UpdateExecutionException;
import org.openrdf.repository.RepositoryConnection;
import org.openrdf.repository.RepositoryException;

/**
 * Feature rules are UPDATE queries. 
 * No special format. Temporary solution for assigning region classes to image regions
 * @author doulaver
 *
 */

public class FeatureRule {
	
	List<String> rules;
	final String delimeter = "\\-\\-\\-";
	private static Logger logger = Logger.getLogger(FeatureRule.class);
	
	public FeatureRule() {
		rules = new ArrayList<String>();
	}
	
	public void parseRules(String ruleString) {
		//ruleString = "oapoap aopa --- rgrtggrg --- rwerwewer ewrwer ewr ewr --- 32423224.432 --- ?><:{}";
		
		Pattern pattern = Pattern.compile("([^"+delimeter+"]+)");
		
		Matcher m = pattern.matcher(ruleString);
		
		while (m.find()) {
			rules.add(m.group().trim());
			//logger.info(m.group().trim());
		}
	}
	
	public void runRules(RepositoryConnection con, String context) {
		int counter = 1;
		for (String rule : rules) {
			try {
				if (!rule.trim().equals("")) {
					if (!context.trim().equals(""))
						rule = "WITH <"+context+">\n"+rule;
					
					logger.info("Executing rule: "+rule);
					Update update = con.prepareUpdate(QueryLanguage.SPARQL, Constants.queryPrefix+rule);
					update.execute();
					con.commit();
				}
			
			} catch (RepositoryException e) {
				e.printStackTrace();
			} catch (MalformedQueryException e) {
				logger.info("Syntax error in feature rule "+counter);
				System.out.println("Syntax error in feature rule "+counter);
				e.printStackTrace();
			} catch (UpdateExecutionException e) {
				logger.info("Execution error in feature rule "+counter);
				System.out.println("Execution error in feature rule "+counter);
				e.printStackTrace();
			}
			counter++;
		}
	}
	
	public static void main(String[] args) {
		FeatureRule f = new FeatureRule();
		f.parseRules("");
	}
}

