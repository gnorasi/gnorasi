package gr.iti.gnorasi.csvparser;

import gr.iti.gnorasi.utils.CSV2RDF;
import gr.iti.gnorasi.utils.Constants;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.apache.log4j.Level;
import org.apache.log4j.Logger;
import org.geotools.geometry.jts.JTS;
import org.geotools.referencing.CRS;
import org.opengis.referencing.FactoryException;
import org.opengis.referencing.crs.CoordinateReferenceSystem;
import org.opengis.referencing.operation.MathTransform;
import org.openrdf.model.Literal;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;
import org.openrdf.model.Value;
import org.openrdf.model.ValueFactory;
import org.openrdf.model.vocabulary.RDF;
import org.openrdf.model.vocabulary.XMLSchema;
import org.openrdf.query.BindingSet;
import org.openrdf.query.QueryLanguage;
import org.openrdf.query.TupleQuery;
import org.openrdf.query.TupleQueryResult;
import org.openrdf.repository.RepositoryConnection;
import org.openrdf.repository.RepositoryException;
import org.openrdf.repository.RepositoryResult;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import com.vividsolutions.jts.geom.Geometry;
import com.vividsolutions.jts.io.WKTReader;

public class Retrieval {
	private static Logger logger = Logger.getLogger(Retrieval.class);
	
	public Retrieval() {
		/*try {
			String jarPath = URLDecoder.decode(OWLIMRepository.class.getProtectionDomain().getCodeSource().getLocation().getPath(), "UTF-8");
			String path = (new File(jarPath)).getParentFile().getParentFile().getPath()+System.getProperty("file.separator");
			setMappingsFile(path+"data/mappings.xml");
		}catch (Exception e) {
			e.printStackTrace();
		}*/
		logger.setLevel(Level.INFO);
	}
	
	
	public void printQuery(RepositoryConnection con) {
		String sparql = "SELECT * WHERE {"+
					"?x gno:depicts ?depiction . \n"+
					"?x gno:hasID ?id . \n"+
					"?depiction gno:depictsObject ?obj . \n" +
					//"?depiction gno:withConfidence ?conf . \n" +
					"FILTER (?id=1373)"+
				"}";
		
		try {
			TupleQuery q = con.prepareTupleQuery(QueryLanguage.SPARQL, Constants.queryPrefix+sparql);
			TupleQueryResult regResult = q.evaluate();
			while (regResult.hasNext()) {
				System.out.println(regResult.next());
			}
			regResult.close();
		}catch (Exception e) {
			e.printStackTrace();
		}
		
	}
	
	public String getResults2CSV(RepositoryConnection con, String context) {
		StringBuffer buffer = new StringBuffer(10000);
		buffer.append("ID;CLASSID;CLASSNAME;CONFIDENCE\n");
		try {
			String sparql = "SELECT ?regionID ?classID ?objectClass ?confidence \n";
			if (context != null) {
				if (!context.equals("")) {
					sparql += "FROM <"+context+"> ";
				}
			}
			
			sparql +=	"WHERE { {\n" +
								"{" +
									"SELECT ?x (MAX(?conf) AS ?confidence) WHERE { \n"+
										"?x gno:"+Constants.depicts+" ?y . \n" +
										"?y gno:"+Constants.depictsObject+" ?obj . \n" +
										"?y gno:"+Constants.withConfidence+" ?conf . \n"+
										"FILTER NOT EXISTS {\n"+
											"?x gno:"+Constants.depicts+" ?y2 . \n" +
											"?y2 gno:"+Constants.depictsInheritedObject+" ?obj . \n" +
											"FILTER (?y != ?y2) . \n" +
										"}\n"+
									"}\n" +
									"GROUP BY ?x \n" +
								"}\n"+
								"?x gno:"+Constants.depicts+" ?y . \n" +
								"?x gno:"+Constants.regionHasID+" ?regionID . \n" +
								"?y gno:"+Constants.depictsObject+" ?objectClass . \n" +
								"?objectClass gno:"+Constants.objectClassHasID +" ?classID . \n"+
								"?objectClass gno:"+Constants.definedForSegmentationLevel+" ?segLevel . \n"+
								"?y gno:"+Constants.withConfidence+" ?confidence . \n"+
							"} " +
							" UNION {\n" +
								"?x rdf:type gno:Region . \n" +
								"?x gno:"+Constants.regionHasID+" ?regionID . \n" +
								"FILTER NOT EXISTS {?x gno:"+Constants.depicts+" ?dep}"+
							"} }\n"+
							"ORDER BY ?regionID DESC(?confidence)";
			logger.info(sparql);
			TupleQuery q = con.prepareTupleQuery(QueryLanguage.SPARQL, Constants.queryPrefix+sparql);
			TupleQueryResult regResult = q.evaluate();
			//int currentRegionID = -1;
			//int currentClassID = -1;
			//float conf = -1;
			//String currentClassName = "";
			Literal previousRegionID = null;
			while (regResult.hasNext()) {
				BindingSet bindingSet = regResult.next();
				Literal regionID = (Literal)bindingSet.getValue("regionID");
				Literal classID = (Literal)bindingSet.getValue("classID");
				URI className = (URI)bindingSet.getValue("objectClass");
				Literal confidence = (Literal)bindingSet.getValue("confidence");
				
				//get class name
				String trueClassName = "";
				try {
					if (className != null) {
						RepositoryResult<Statement> res = con.getStatements(className, RDF.TYPE, null, false, Constants.userNSURI);
						if (res.hasNext()) {
							Statement st = res.next();
							trueClassName = ((URI)st.getObject()).getLocalName();
						}
						else
							trueClassName = "Unclassified";
						res.close();
					}
					else
						trueClassName = "Unclassified";
					
				} catch (RepositoryException e) {
					logger.error("Error getting class for: "+className);
				}
				
				if (!regionID.equals(previousRegionID)) {
					if (regionID != null) {
						buffer.append(regionID.intValue()).append(";");
						
						if (classID != null)
							buffer.append(classID.intValue()).append(";");
						else
							buffer.append("0").append(";");
						
						buffer.append(trueClassName).append(";");
						
						if (confidence != null)
							buffer.append(confidence.floatValue()).append("\n");
						else
							buffer.append("0.0").append("\n");
						
						previousRegionID = regionID;
					}
				}
			}
			
		}catch (Exception e) {
			e.printStackTrace();
		}
		
		//logger.info(buffer.toString());
		
		return buffer.toString();
	}
	
	public String getQueryResults2CSV(RepositoryConnection con, String query) {
		StringBuffer buffer = new StringBuffer(10000);
		
		try {
			int count = 1;
			TupleQuery q = con.prepareTupleQuery(QueryLanguage.SPARQL, Constants.queryPrefix+query);
			TupleQueryResult regResult = q.evaluate();
			List<String> bindingNames = regResult.getBindingNames();
			int bindingNamesSize = bindingNames.size();
			for (int i=0;i<bindingNamesSize-1;i++)
				buffer.append(bindingNames.get(i)).append(";");
			buffer.append(bindingNames.get(bindingNamesSize-1)).append("\n");
			
			while (regResult.hasNext()) {
				BindingSet bindingSet = regResult.next();
				for (int i=0;i<bindingNamesSize;i++) {
					Value v = bindingSet.getValue(bindingNames.get(i));
					if (v instanceof URI)
						buffer.append(v);
					else if (v instanceof Literal) {
						Literal lv = (Literal)v;
						URI datatype = lv.getDatatype();
						if (datatype.equals(XMLSchema.INT))
							buffer.append(lv.intValue());
						else if (datatype.equals(XMLSchema.FLOAT))
							buffer.append(lv.floatValue());
						else if (datatype.equals(XMLSchema.DOUBLE))
							buffer.append(lv.doubleValue());
						else if (datatype.equals(XMLSchema.STRING))
							buffer.append(lv.stringValue());
						else
							buffer.append(lv);
					}
					else if (v == null)
						buffer.append("null");
					else
						buffer.append(v);
					
					if (i < bindingNamesSize-1)
						buffer.append(";");
					else
						buffer.append("\n");
				}
				count++;
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return buffer.toString();
	}
	
}
