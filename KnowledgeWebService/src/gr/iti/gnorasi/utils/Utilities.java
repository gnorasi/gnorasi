package gr.iti.gnorasi.utils;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.Map.Entry;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathConstants;
import javax.xml.xpath.XPathExpression;
import javax.xml.xpath.XPathFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public class Utilities {
	
	public void printMapKeyValue(Map m) {
		Set<Entry> set = m.entrySet();
		Iterator<Entry> iter = set.iterator();
		while (iter.hasNext()) {
			Entry entry = iter.next();
			System.out.println("Key: "+entry.getKey() + " , value: "+entry.getValue() );
		}
	}
	
	public static String getStringList(Object[] o) {
		String ret="";
		
		if (o.length == 1)
			return "<"+o[0].toString()+">";
		else if (o.length > 1){
			for (int i=0;i<o.length-1;i++) {
				ret += "<"+o[i]+">, ";
			}
			ret += "<"+o[o.length-1]+">";
		}
		
		return ret;
	}
	
	public static void printList(List l) {
		String print = "Size is "+l.size()+": ";
		for (int i=0;i<l.size();i++) {
			print += l.get(i).toString()+", ";
		}
		System.out.println(print);
	}
	
	public static String fileToString(String path) {
		String line = "";
		try {
			BufferedReader reader = new BufferedReader(new FileReader(path));
			
			StringBuffer lineBuffer = new StringBuffer(1000);
			while ((line = reader.readLine()) != null) {
				lineBuffer.append(line).append("\n");
			}
			line = lineBuffer.toString();
			reader.close();
		}catch (IOException e) {
			e.printStackTrace();
		}
		
		return line;
	}
	
	public static boolean contains(Object[] haystack, Object needle) {
		if (haystack instanceof String[]) {
			for (int i=0;i<haystack.length;i++) {
				if (((String)haystack[i]).equalsIgnoreCase((String)needle)) {
					return true;
				}
			}
		}
		else {
			for (int i=0;i<haystack.length;i++) {
				if ((haystack[i]).equals(needle)) {
					return true;
				}
			}
		}
		
		return false;
	}
	
	public static NodeList getXPathNodes(Document doc, String strXPath) {
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
	
	public static String getBaseNamespace(InputStream owlStream) {
	    String base = "";
		try {
			DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
		    factory.setNamespaceAware(true);
		    DocumentBuilder builder = factory.newDocumentBuilder();
		    Document doc = builder.parse(owlStream);
		    Element root = doc.getDocumentElement();
		    //get root name space
		    NamedNodeMap attributes = root.getAttributes();
		    if (attributes != null) {
		        for (int i = 0; i < attributes.getLength(); i++) {
		            Node node = attributes.item(i);
		            if (node != null) {
			            if (node.getNodeType() == Node.ATTRIBUTE_NODE) {
			                String name = node.getNodeName();
			                if (name.equals("xmlns"))
			                	base = node.getNodeValue();
			                else if (name.equals("xml:base"))
			                	base = node.getNodeValue()+"#";
			            }
		            }
		        }
		    }
	    }catch (Exception e) {
	    	e.printStackTrace();
	    }
		
		return base;
	}
}
