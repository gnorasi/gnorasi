package gr.iti.gnorasi.utils;

import java.io.InputStream;
import java.util.ArrayList;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathConstants;
import javax.xml.xpath.XPathExpression;
import javax.xml.xpath.XPathFactory;

import org.apache.log4j.Logger;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public class XMLDoc {
	private Document doc;
	private static Logger logger = Logger.getLogger(XMLDoc.class);
	
	public XMLDoc() {
		doc = null;
	}
	
	public XMLDoc(InputStream instream) {
		setDoc(instream);
	}
	
	public void setDoc(InputStream instream) {
		try {
			DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
		    DocumentBuilder db = dbf.newDocumentBuilder();
	    	doc = db.parse(instream);
		}catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public Document getDoc() {
		return doc;
	}
	
	public ArrayList<String> getXPathObj(String strXPath) {
		ArrayList<String> ret = new ArrayList<String>();
		try {
			XPathFactory factory = XPathFactory.newInstance();
			XPath xpath = factory.newXPath();
			XPathExpression expression = xpath.compile(strXPath);
			NodeList nodeList = (NodeList) expression.evaluate(doc, XPathConstants.NODESET);
			for (int i = 0; i < nodeList.getLength(); i++) {
				Node currentNode = nodeList.item(i);
				ret.add(currentNode.getTextContent());
			}
		} catch (Exception e) {
			System.out.println(e.getMessage());
		}
		
		return ret;
	}
	
	public NodeList getXPathNodes(String strXPath) {
		try {
			XPathFactory factory = XPathFactory.newInstance();
			XPath xpath = factory.newXPath();
			XPathExpression expression = xpath.compile(strXPath);
			return (NodeList) expression.evaluate(doc, XPathConstants.NODESET);
		}catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}
}
