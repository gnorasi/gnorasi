package gr.iti.gnorasi.csvparser;

import gr.iti.gnorasi.owlim.OWLIMRepository;
import gr.iti.gnorasi.utils.CSV2RDF;
import gr.iti.gnorasi.utils.Constants;
import gr.iti.gnorasi.utils.Utilities;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.net.URLDecoder;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Set;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.geotools.geometry.jts.JTS;
import org.geotools.referencing.CRS;
import org.geotools.referencing.operation.DefaultCoordinateOperationFactory;
import org.opengis.referencing.FactoryException;
import org.opengis.referencing.crs.CoordinateReferenceSystem;
import org.opengis.referencing.operation.CoordinateOperation;
import org.opengis.referencing.operation.CoordinateOperationFactory;
import org.opengis.referencing.operation.MathTransform;
import org.openrdf.model.Literal;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;
import org.openrdf.model.Value;
import org.openrdf.model.ValueFactory;
import org.openrdf.model.impl.ValueFactoryImpl;
import org.openrdf.model.vocabulary.RDF;
import org.openrdf.query.BindingSet;
import org.openrdf.query.Query;
import org.openrdf.query.QueryLanguage;
import org.openrdf.query.TupleQuery;
import org.openrdf.query.TupleQueryResult;
import org.openrdf.query.Update;
import org.openrdf.repository.RepositoryConnection;
import org.openrdf.repository.RepositoryException;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import com.vividsolutions.jts.geom.Geometry;
import com.vividsolutions.jts.io.WKTReader;

public class CSV2SPARQL {
	Document mappingsFile;
	
	public CSV2SPARQL() {
		/*try {
			String jarPath = URLDecoder.decode(OWLIMRepository.class.getProtectionDomain().getCodeSource().getLocation().getPath(), "UTF-8");
			String path = (new File(jarPath)).getParentFile().getParentFile().getPath()+System.getProperty("file.separator");
			setMappingsFile(path+"data/mappings.xml");
		}catch (Exception e) {
			e.printStackTrace();
		}*/
	}
	
	public void setMappingsFile(String file) {
		Document doc = null;
		try {	
			InputStream instream = new FileInputStream(file); 
			DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
		    DocumentBuilder db = dbf.newDocumentBuilder();
	    	doc = db.parse(instream);
		}catch (Exception e) {
			e.printStackTrace();
		}
		mappingsFile = doc;
	}
	
	public ArrayList<ArrayList<String> > parseCSVHeader(String line) {
		ArrayList<ArrayList<String> > headers = new ArrayList<ArrayList<String> >();
		System.out.println("header line is: "+line);
		
		String[] parts = line.split(";");
		
		for (int i=0;i<parts.length;i++) {
			headers.add(new ArrayList<String>());
			String headerValue = parts[i];
			String[] headerValueList = headerValue.split("::");
			for (int j=0;j<headerValueList.length;j++) {
				String val = headerValueList[j];
				headers.get(i).add(val);
			}
		}
		
		return headers;
	}
	
	public List<Statement> parseCSVLine(String line, int lineNum, MathTransform transform, WKTReader wktreader, List<ArrayList<String> > headers, ValueFactory f, boolean parseFeatures) throws IllegalStateException {
		String[] parts = line.split(";");
		List<Statement> list = new ArrayList<Statement>(100);
		URI region = null;
		
		if (parts.length != headers.size()) {
			throw new IllegalStateException("Wrong number of arguments at line "+lineNum+". Was "+parts.length+", should be "+headers.size());
		}
		
		String id="";
		Pattern WKTPattern = Pattern.compile("[a-zA-Z]+ \\([\\([0-9\\.\\, ]+\\)[\\,]?]+\\)");

		for (int i=0;i<parts.length;i++) {
			String part = parts[i];
			
			ArrayList<String> header = headers.get(i);
			try {
				if (header.get(0).equalsIgnoreCase("ID")) {
					id = part;
					region = f.createURI(Constants.mainNS+"region_"+id);
					URI hasID = f.createURI(Constants.mainNS+"hasID");
					Literal idLit = f.createLiteral(Integer.valueOf(id).intValue());
					list.add(f.createStatement(region, hasID, idLit));
					list.add(f.createStatement(region, RDF.TYPE, f.createURI(Constants.mainNS+"Region")));
					
				}
				else if (header.get(0).equalsIgnoreCase("WKT")) {
					Matcher m = WKTPattern.matcher(part);
					URI asWKT = f.createURI(Constants.geoNS+Constants.asWKT);
					if (m.find()) {
						Geometry temp = wktreader.read(m.group());
						if (transform != null) {
							String wktTransformed = "";
							if (temp.isValid()) {
								Geometry targetGeometry = JTS.transform( temp, transform);
								wktTransformed = targetGeometry.toText();
								Literal wktLiteral = f.createLiteral(wktTransformed, f.createURI(Constants.geoNS+CSV2RDF.wkt));
								list.add(f.createStatement(region, asWKT, wktLiteral));
							}
							else {
								//throw new IllegalStateException("geometry"+lineNum+" is bad");
							}
						}
						else {
							if (temp.isValid()) {
								Literal wktLiteral = f.createLiteral(m.group(), f.createURI(Constants.geoNS+CSV2RDF.wkt));
								list.add(f.createStatement(region, asWKT, wktLiteral));
							}
							else {
								//throw new IllegalStateException("geometry"+lineNum+" is bad");
							}
						}
					}
					else {
						throw new IllegalStateException("Wrong WKT definition at line "+lineNum);
					}
				}
				else if (header.size() > 2) {
					if (header.get(2).trim().startsWith("Fuzzy")) {
						URI fuzzyProperty = f.createURI(Constants.userNS+header.get(2)+header.get(1));
						Literal value = f.createLiteral(Float.parseFloat(part));
						list.add(f.createStatement(region, fuzzyProperty, value));
					}
				}
				else {
					if (parseFeatures) {
						URI hasFeature = f.createURI(Constants.mainNS+"hasFeature");
						URI featureInstance = f.createURI(Constants.mainNS+"feature_"+id+"_"+i);
						list.add(f.createStatement(region, hasFeature, featureInstance));
						
						NodeList nodelist = mappingsFile.getElementsByTagName(header.get(0));
						NodeList children = nodelist.item(0).getChildNodes();
						
						String featureType = nodelist.item(0).getAttributes().getNamedItem("type").getNodeValue();
						String featureProperty = nodelist.item(0).getAttributes().getNamedItem("property").getNodeValue();
						
						URI featureTypeProp = f.createURI(Constants.mainNS+featureProperty);
						URI featureTypeIns = f.createURI(Constants.mainNS+featureType);
						list.add(f.createStatement(featureInstance, featureTypeProp, featureTypeIns));
	
						for (int j=1;j<header.size();j++) {
							Node n = null;
							for (int k=0;k<children.getLength();k++) {
								if (children.item(k).getNodeName().equals(header.get(j))) {
									n = children.item(k);
									break;
								}
									
							}
	
							String property = n.getAttributes().getNamedItem("property").getNodeValue();
							String propertyType = n.getTextContent();
							
							URI propertyProp = f.createURI(Constants.mainNS+property);
							URI propertyIns = f.createURI(Constants.mainNS+propertyType);
							list.add(f.createStatement(featureInstance, propertyProp, propertyIns));
						}
	
						URI featureHasValue = f.createURI(Constants.mainNS+"featureHasValue");
						Literal value = f.createLiteral(Float.valueOf(part).floatValue());
						list.add(f.createStatement(featureInstance, featureHasValue, value));
					}
				}
			}catch (Exception e) {
				e.printStackTrace();
			}
		}
		
		/*for (int i=0;i<list.size();i++) {
			System.out.println(list.get(i));
		}*/
		
		return list;
	}
	
	public void parseCSV(BufferedReader reader, RepositoryConnection con, String context, int srid, boolean verbose, boolean parseFeatures) {
		try {
			//File file = new File(csvFile);
			//BufferedReader reader = new BufferedReader(new FileReader(file));
			String line;
			
			//parse header line
			line = reader.readLine();
			ArrayList<ArrayList<String> > headers = parseCSVHeader(line);
			
			List<Statement> list = new ArrayList<Statement>();
			
			//parse the rest of the file and add data to the repository
			int lineNumber = 1;
			int batch = 10;
			StringBuffer updateQuery = new StringBuffer(10000);
			MathTransform transform = null;
			WKTReader wktreader = null;
			ValueFactory f = con.getValueFactory();
			
			if (srid != Constants.targetCRS) {
				CoordinateReferenceSystem sourceCRS = CRS.decode("EPSG:"+srid); //2100: GGRS87
				CoordinateReferenceSystem targetCRS = CRS.decode("EPSG:"+Constants.targetCRS); //4326: WGS84
				transform = CRS.findMathTransform(sourceCRS, targetCRS);
				wktreader = new WKTReader();
			}
			
			while ((line = reader.readLine()) != null) {
				lineNumber++;
				
				try {
					List<Statement> lineList = parseCSVLine(line, lineNumber, transform, wktreader, headers, f, parseFeatures);
					
					list.addAll(lineList);
					
					if (lineNumber % batch == 0) {
						StringBuffer sb = new StringBuffer(updateQuery.length()+1000);
						long start = System.currentTimeMillis();
						if (context.equals(""))
							con.add(list);
						else
							con.add(list, f.createURI(context));
						
						con.commit();
						long end = System.currentTimeMillis();
						//System.out.println("Time for "+list.size()+" adds = "+(end-start)+"ms");
						//System.out.println("Number of statements/line = "+lineList.size());
						
						if (verbose)
							System.out.println(sb.toString());
						
						list.clear();
					}
				}catch (IllegalStateException e) {
					System.out.println(e.getMessage());
				}
			}
			if (list.size() > 0) {
				StringBuffer sb = new StringBuffer(updateQuery.length()+1000);
				
				if (context.equals(""))
					con.add(list);
				else
					con.add(list, f.createURI(context));

				if (verbose)
					System.out.println(sb.toString());
				
				con.commit();
				list.clear();
			}
		}catch (RepositoryException e) {
			e.printStackTrace();
		}catch (FactoryException e) {
			System.out.println(e.getMessage());
		}catch (IOException e) {
			System.out.println(e.getMessage());
		}
	}
	
	public String getResults2CSV(RepositoryConnection con, String context) {
		StringBuffer buffer = new StringBuffer(10000);
		buffer.append("ID;CLASS;CONFIDENCE\n");
		try {
			String sparql = "SELECT ?regionID ?classID ?confidence \n";
			if (context != null) {
				if (!context.equals("")) {
					sparql += "FROM <"+context+"> ";
				}
			}
			
			sparql +=	"WHERE {" +
						"?region gno:"+Constants.depicts+" ?depiction . " +
						"?depiction gno:"+Constants.depictsObject+" ?objectClass . " +
						"?depiction gno:"+Constants.withConfidence+" ?confidence . " +
						"?region gno:"+Constants.regionHasID+" ?regionID . " +
						"?objectClass gno:"+Constants.objectClassHasID+" ?classID . \n"+
						"} \n" +
						//"GROUP BY ?regionID ?classID \n"+
						"ORDER BY ?regionID ?confidence \n";
						
			TupleQuery q = con.prepareTupleQuery(QueryLanguage.SPARQL, Constants.queryPrefix+sparql);
			TupleQueryResult regResult = q.evaluate();
			int currentRegion = -1;
			int currentClass = -1;
			float conf = -1;
			while (regResult.hasNext()) {
				BindingSet bindingSet = regResult.next();
				Literal regionID = (Literal)bindingSet.getValue("regionID");
				Literal classID = (Literal)bindingSet.getValue("classID");
				Literal confidence = (Literal)bindingSet.getValue("confidence");
				
				//buffer.append(regionID.intValue()).append(";");
				//buffer.append(classID.intValue()).append(";");
				//buffer.append(confidence.floatValue()).append("\n");
				
				if (regionID.intValue() != currentRegion) {
					if (currentRegion != -1) {
						buffer.append(currentRegion).append(";");
						buffer.append(currentClass).append(";");
						buffer.append(conf).append("\n");
					}
					currentRegion = regionID.intValue();
					currentClass = classID.intValue();
					conf = confidence.floatValue();
				}
				
				if (confidence.floatValue() > conf) {
					conf = confidence.floatValue();
				}
			}
			
		}catch (Exception e) {
			e.printStackTrace();
		}
		
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
				for (int i=0;i<bindingNamesSize-1;i++)
					buffer.append(bindingSet.getValue(bindingNames.get(i))).append(";");
				buffer.append(bindingSet.getValue(bindingNames.get(bindingNamesSize-1))).append("\n");
				
				count++;
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return buffer.toString();
	}
	
}
