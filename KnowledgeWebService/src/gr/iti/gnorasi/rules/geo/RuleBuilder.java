package gr.iti.gnorasi.rules.geo;

import gr.iti.gnorasi.service.rest.Main;
import gr.iti.gnorasi.utils.Constants;
import gr.iti.gnorasi.utils.XMLDoc;
import gr.iti.gnorasi.rules.geo.GeoRuleTerm.TargetImage;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

import javax.sql.DataSource;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathConstants;
import javax.xml.xpath.XPathExpression;
import javax.xml.xpath.XPathFactory;

import org.apache.log4j.Logger;
import org.openrdf.model.URI;
import org.openrdf.model.ValueFactory;
import org.openrdf.model.impl.ValueFactoryImpl;
import org.openrdf.repository.RepositoryConnection;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public class RuleBuilder {
	private XMLDoc doc;
	private List<GeoRule> rules;
	private static Logger logger = Logger.getLogger(RuleBuilder.class);
	
	public RuleBuilder() {
		doc = null;
		rules = new ArrayList<GeoRule>();
	}
	
	public RuleBuilder(InputStream instream) {
		doc = new XMLDoc(instream);
		rules = new ArrayList<GeoRule>();
	}
	
	public List<GeoRule> getRules() {
		return rules;
	}
	
	public void parseRules() {
		ValueFactory f = new ValueFactoryImpl();
		NodeList rulesList = doc.getXPathNodes("/rules/rule");
		String ruleID = "";
		
		for (int i=0;i<rulesList.getLength();i++) {
			try {
				Node rule = rulesList.item(i);
				
				GeoRule geoRule = new GeoRule();
				if (rule.getAttributes().getNamedItem("id") != null) {
					ruleID = rule.getAttributes().getNamedItem("id").getNodeValue();
					geoRule.setId(ruleID);
				}
				
				//get body and head
				NodeList bodyAndHead = rule.getChildNodes();
				for (int j=0;j<bodyAndHead.getLength();j++) {
					Node node = bodyAndHead.item(j);
					if (node.getNodeName().equals("body")) {
						NodeList bodyNodes = node.getChildNodes();
						for (int k=0;k<bodyNodes.getLength();k++) {
							Node bodyNode = bodyNodes.item(k);
							if (bodyNode.getNodeName().equals("region")) {
								GeoRulePredicate grp = new GeoRulePredicate();
								grp.setRegionDepiction(f.createURI(Constants.userNS + bodyNode.getAttributes().getNamedItem("class").getNodeValue()));
								grp.setSF(bodyNode.getAttributes().getNamedItem("property").getNodeValue());
								grp.setID(Integer.parseInt(bodyNode.getAttributes().getNamedItem("id").getNodeValue()));
								if (bodyNode.getAttributes().getNamedItem("level") != null)
									grp.setSegmentationLevel(Integer.parseInt(bodyNode.getAttributes().getNamedItem("level").getNodeValue()));
								if (bodyNode.getAttributes().getNamedItem("image") != null)
									grp.settImage(TargetImage.fromString(bodyNode.getAttributes().getNamedItem("image").getNodeValue()));
								
								geoRule.addPredicate(grp);
							}
						}
					}
					else if (node.getNodeName().equals("head")) {
						NodeList headNodes = node.getChildNodes();
						for (int k=0;k<headNodes.getLength();k++) {
							Node headNode = headNodes.item(k);
							if (headNode.getNodeName().equals("region")) {
								GeoRuleHead grh = new GeoRuleHead();
								grh.setRegionDepiction(f.createURI(Constants.userNS+headNode.getAttributes().getNamedItem("class").getNodeValue()));
								grh.setConfidence(Double.parseDouble(headNode.getAttributes().getNamedItem("confidence").getNodeValue()));
								if (headNode.getAttributes().getNamedItem("level") != null)
									grh.setSegmentationLevel(Integer.parseInt(headNode.getAttributes().getNamedItem("level").getNodeValue()));
								if (headNode.getAttributes().getNamedItem("image") != null)
									grh.settImage(TargetImage.fromString(headNode.getAttributes().getNamedItem("image").getNodeValue()));
								
								geoRule.setHead(grh);
							}
						}
					}
				}
				
				rules.add(geoRule);
			}
			catch (IllegalStateException e) {
				logger.error("DEFINITON ERROR: "+e.getMessage() + ". Rule '"+ruleID+"' skipped.");
			}
			catch (Exception e) {
				logger.error("PARSING ERROR: "+e.getMessage() + ". Rule '"+ruleID+"' skipped.");
			}
		}
		
		//Utilities.printList(predicateList);
	}

	public void fireAllRules(DataSource ds, RepositoryConnection con, URI context) {
		RuleFire rf = new RuleFire();
		
		rf.fireAll(rules, ds, con, context);
	}
	public void fireAllRulesIterative(DataSource ds, RepositoryConnection con, URI context) {
		RuleFire rf = new RuleFire();
		
		rf.fireAllIteration(rules, ds, con, context);
	}

}
