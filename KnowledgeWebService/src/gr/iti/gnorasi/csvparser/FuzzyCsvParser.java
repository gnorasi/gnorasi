package gr.iti.gnorasi.csvparser;

import gr.iti.gnorasi.utils.Constants;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.StringReader;
import java.util.ArrayList;

import org.apache.log4j.Level;
import org.apache.log4j.Logger;
import org.geotools.geometry.jts.JTS;
import org.geotools.referencing.CRS;
import org.opengis.referencing.crs.CoordinateReferenceSystem;
import org.opengis.referencing.operation.MathTransform;
import org.openrdf.model.Literal;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;
import org.openrdf.model.ValueFactory;
import org.openrdf.model.vocabulary.RDF;
import org.openrdf.repository.RepositoryConnection;
import org.openrdf.repository.RepositoryException;

import com.vividsolutions.jts.geom.Geometry;
import com.vividsolutions.jts.io.WKTReader;

public class FuzzyCsvParser {

	private static Logger logger = Logger.getLogger(FuzzyCsvParser.class);
	static {
		logger.setLevel(Level.INFO);
	}
	BufferedReader reader;
	boolean readerIsRead;
	
	static String SEPARATOR = ";";
	static String ID = "id";
	static String LEVEL = "level";
	static String WKT = "WKT";
	
	
	public FuzzyCsvParser() {
		reader = null;
		readerIsRead = false;
	}
	
	public FuzzyCsvParser(String file) {
		try {
			reader = new BufferedReader(new FileReader(file));			
		}catch (FileNotFoundException e) {
			reader = new BufferedReader(new StringReader(file));
		}
		readerIsRead = false;
	}
	
	private ArrayList<String> parseCsvHeader() {
		ArrayList<String> headers = new ArrayList<String>();
		try {
			String line = reader.readLine();
			readerIsRead = true;
			
			String[] parts = line.split(SEPARATOR);
			
			for (int i=0;i<parts.length;i++) {
				headers.add(parts[i]);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		
		return headers;
	}
	
	public void parseLine(String line, ArrayList<String> headers, RepositoryConnection con) throws RepositoryException {
		if (headers == null)
			return;
		
		int idIndex=0, levelIndex=1, wktIndex=2;
		String id="", level="", wkt="", wktTransformed="";
		String[] parts = line.split(SEPARATOR);
		ValueFactory f = con.getValueFactory();
		
		if (headers.get(0).equals(ID)) {
			id = parts[0]; 
		}
		else {
			for (int i=1;i<headers.size();i++) {
				if (headers.get(i).equals(ID)) {
					idIndex=i;
					id = parts[i];
					break;
				}
			}
		}
		
		if (headers.get(1).equals(LEVEL)) {
			level = parts[1];
		}
		else {
			for (int i=0;i<headers.size();i++) {
				if (headers.get(i).equals(LEVEL)) {
					levelIndex=i;
					level = parts[i];
					break;
				}
			}
			if (level.equals(""))
				levelIndex = -1;
		}
		
		if (headers.get(2).equals(WKT)) {
			wkt = parts[2];
		}
		else {
			for (int i=0;i<headers.size();i++) {
				if (headers.get(i).equals(WKT)) {
					wktIndex = i;
					wkt = parts[i];
					break;
				}
			}
			if (wkt.equals(""))
				wktIndex = -1;
			else { //perform coordinate system conversions
				try {
					wkt = wkt.replace("\"", "");
					CoordinateReferenceSystem sourceCRS = CRS.decode("EPSG:2100"); //2100: GGRS87
					CoordinateReferenceSystem targetCRS = CRS.decode("EPSG:"+Constants.targetCRS); //4326: WGS84
					MathTransform transform = CRS.findMathTransform(sourceCRS, targetCRS);
					WKTReader wktreader = new WKTReader();
					Geometry temp = wktreader.read(wkt);
					if (transform == null)
						System.out.println("transform is null");
					else if (temp == null)
						System.out.println("geometry is null");
					else {
						Geometry targetGeometry = JTS.transform(temp, transform);
						wktTransformed = targetGeometry.toText();
					}
					
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}
		
		URI segmentationLevelInstance = f.createURI(Constants.userNS+"segmentationLevel_"+level);
		Literal idLiteral = f.createLiteral(Integer.valueOf(id).intValue());
				
		URI region = f.createURI(Constants.userNS+"region_"+id+"_"+level);
		
		Statement regionTypeSt = f.createStatement(region, RDF.TYPE, f.createURI(Constants.mainNS+"Region"));
		con.add(regionTypeSt, Constants.userNSURI);
		logger.debug(regionTypeSt);
		//con.add(region, RDF.TYPE, f.createURI(Constants.mainNS+"Region"));
		
		Statement segmentationLevelSt = f.createStatement(region, Constants.belongsToSegmentationLevelProperty, segmentationLevelInstance);
		con.add(segmentationLevelSt, Constants.userNSURI);
		logger.debug(segmentationLevelSt);
		//con.add(region, Constants.belongsToSegmentationLevelProperty, segmentationLevelInstance);
		
		Statement idSt = f.createStatement(region, Constants.regionHasIDProperty, idLiteral);
		con.add(idSt, Constants.userNSURI);
		logger.debug(idSt);
		//con.add(region, Constants.regionHasIDProperty, idLiteral);
		
		if (!wktTransformed.equals("")) {
			Literal wktLit = f.createLiteral(wktTransformed, Constants.wktLiteralDatatype);
			Statement wktStatement = f.createStatement(region, Constants.asWKTProperty, wktLit);
			con.add(wktStatement, Constants.userNSURI);
		}
		
		
		for (int i=0;i<headers.size();i++) {
			try {
				if (i!=idIndex && i!=levelIndex && i!=wktIndex) {
					URI fuzzyProperty = f.createURI(Constants.userNS+headers.get(i));
					Literal membershipValue = f.createLiteral(Float.valueOf(parts[i]).floatValue());
					
					Statement membershipSt = f.createStatement(region, fuzzyProperty, membershipValue);
					con.add(membershipSt, Constants.userNSURI);
					logger.debug(membershipSt);
					//con.add(region, fuzzyProperty, membershipValue);
				}
			}catch (NumberFormatException e) {
				logger.error("Line: "+line+"\n"+parts[i] + " is not a number");
			}
		}
	}
	
	public void parse(RepositoryConnection con) {
		if (reader != null) {
			ArrayList<String> headers = null;
			if (!readerIsRead) {
				headers = parseCsvHeader();
			}
			String line;
			try {
				while ((line = reader.readLine()) != null) {
					parseLine(line, headers, con);
					con.commit();
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}
}
