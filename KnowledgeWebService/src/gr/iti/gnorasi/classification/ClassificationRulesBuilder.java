package gr.iti.gnorasi.classification;

import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

import gr.iti.gnorasi.classification.ClassificationRules.RuleOperator;
import gr.iti.gnorasi.utils.Constants;
import gr.iti.gnorasi.utils.XMLDoc;

import org.apache.log4j.Logger;
import org.openrdf.model.URI;
import org.openrdf.model.ValueFactory;
import org.openrdf.model.impl.ValueFactoryImpl;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public class ClassificationRulesBuilder {

	private static Logger logger = Logger.getLogger(ClassificationRulesBuilder.class);
	static List<ClassificationRules> rulesList;
	
	public ClassificationRulesBuilder() {
		rulesList = new ArrayList<ClassificationRules>();
	}
	
	public List<ClassificationRules> getRulesList() {
		return rulesList;
	}
	
	public static ClassificationRules getRulesForClass(URI _class) {
		for (int i=0;i<rulesList.size();i++) {
			if (rulesList.get(i).getHead().toString().equals(_class.toString()))
				return rulesList.get(i);
		}
				
		return null;
	}

	public void setRulesList(List<ClassificationRules> rulesList) {
		this.rulesList = rulesList;
	}

	public void buildRules(String rules) {
		InputStream is = new ByteArrayInputStream(rules.getBytes());
		buildRules(new XMLDoc(is));
	}
	
	public void getParameters(Node node) {
		Node functionNode = node.getFirstChild();
		String functionName = functionNode.getAttributes().getNamedItem("name").getNodeValue();
		
		NodeList parameters = functionNode.getFirstChild().getChildNodes();
		for (int i=0;i<parameters.getLength();i++) {
			Node parameter = parameters.item(i);
			double parameterValue = Double.parseDouble(parameter.getAttributes().getNamedItem("value").getNodeValue());
		}
	}
	
	public void buildRules(XMLDoc doc){
		ValueFactory f = new ValueFactoryImpl();
		NodeList nodes = doc.getXPathNodes(Constants.rulesXmlXPath);

		for (int i=0;i<nodes.getLength();i++) {
			ClassificationRules cr = new ClassificationRules();
			Node classRule = nodes.item(i);
			cr.setHead(f.createURI(Constants.userNS + classRule.getAttributes().getNamedItem("id").getNodeValue()));
						
			Node fuzzyRulesTag = classRule.getFirstChild();
			if (fuzzyRulesTag != null) {
				while (!fuzzyRulesTag.getNodeName().equals("fuzzyRules"))
					fuzzyRulesTag = fuzzyRulesTag.getNextSibling();
				
				cr.setOp(RuleOperator.fromString(fuzzyRulesTag.getAttributes().getNamedItem("operator").getNodeValue()));
								
				NodeList rules = fuzzyRulesTag.getChildNodes();
				for (int j=0;j<rules.getLength();j++) {
					Node rule = rules.item(j);
					//do attribute rules
					if (rule.getNodeName().equals("attributeRule")) {
						AttributeRule ar = new AttributeRule();
						ar.setId(Integer.parseInt(rule.getAttributes().getNamedItem("id").getNodeValue()));
						ar.setFuzzyProperty(f.createURI(Constants.userNS+rule.getAttributes().getNamedItem("property").getNodeValue()));
						
						cr.getFuzzyRules().add(ar);
						//System.out.println("Attribute rule added");
					}
					//do shape rule
					else if (rule.getNodeName().equals("shapeRule")) {
						ShapeRule sr = new ShapeRule();
						sr.setId(Integer.parseInt(rule.getAttributes().getNamedItem("id").getNodeValue()));
						sr.setFuzzyProperty(f.createURI(Constants.userNS+rule.getAttributes().getNamedItem("property").getNodeValue()));
						
						cr.getFuzzyRules().add(sr);
						//System.out.println("Shape rule added");
					} 
					//do geo rules
					/*else if (rule.getNodeName().equals("spatialRule")) {
						GeoRule gr = new GeoRule();
						gr.setId(Integer.parseInt(rule.getAttributes().getNamedItem("id").getNodeValue()));
						gr.setTargetClass(f.createURI(Constants.userNS+rule.getAttributes().getNamedItem("class").getNodeValue()));
						gr.setProperty(GeoRule.createGeoFeature(rule.getAttributes().getNamedItem("feature").getNodeValue()));
						
						cr.getGeoRules().add(gr);
						//System.out.println("Geo rule added");
					}*/
				}
			}
			
			Node spatialRulesTag = classRule.getFirstChild();
			if (spatialRulesTag != null) {
				while (!spatialRulesTag.getNodeName().equals("spatialRules"))
					spatialRulesTag = spatialRulesTag.getNextSibling();
				
				NodeList rules = spatialRulesTag.getChildNodes();
				for (int j=0;j<rules.getLength();j++) {
					Node rule = rules.item(j);
					
					//do geo rules
					if (rule.getNodeName().equals("spatialRule")) {
						GeoRule gr = new GeoRule();
						gr.setId(Integer.parseInt(rule.getAttributes().getNamedItem("id").getNodeValue()));
						gr.setTargetClass(f.createURI(Constants.userNS+rule.getAttributes().getNamedItem("class").getNodeValue()));
						gr.setProperty(GeoRule.createGeoFeature(rule.getAttributes().getNamedItem("property").getNodeValue()));
						
						cr.getGeoRules().add(gr);
						//System.out.println("Geo rule added");
					}
				}
			}
			
			rulesList.add(cr);
		}
	}
}
