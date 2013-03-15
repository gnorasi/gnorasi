package gr.iti.gnorasi.utils;

import org.openrdf.model.URI;
import org.openrdf.model.ValueFactory;
import org.openrdf.model.impl.ValueFactoryImpl;

import com.useekm.geosparql.Contains;
import com.useekm.geosparql.ContainsProperly;
import com.useekm.geosparql.CoveredBy;
import com.useekm.geosparql.Covers;
import com.useekm.geosparql.Crosses;
import com.useekm.geosparql.Disjoint;
import com.useekm.geosparql.Equals;
import com.useekm.geosparql.Intersects;
import com.useekm.geosparql.Overlaps;
import com.useekm.geosparql.Touches;
import com.useekm.geosparql.Within;

public class Constants {
	static ValueFactory f;
	static {
		f = new ValueFactoryImpl();
	}
	public static String POSTGISINDEX = "postgisindex";
	public static String TEXTINDEX = "textindex";
	public static String mainNS = "http://www.gnorasi.gr/ontology#";
	public static String userNS = "http://www.gnorasi.gr/ontology#";//userNS will change when user ontology is uploaded
	public static String geoNS = "http://rdf.opensahara.com/type/geo/";
	public static String searchNS = "http://rdf.opensahara.com/search#";
	
	public static String depictsObject = "depictsObject";
	public static URI depictsObjectProperty = f.createURI(mainNS+depictsObject);
	public static String depicts = "depicts";
	public static URI depictsProperty = f.createURI(mainNS+depicts);
	public static String withConfidence = "withConfidence";
	public static URI withConfidenceProperty = f.createURI(mainNS+withConfidence);
	public static String asWKT = "asWKT";
	public static String segmentationLevel = "isFromSegmentationLevel";
	public static String regionHasID = "hasID";
	public static URI regionHasIDProperty = f.createURI(mainNS+regionHasID);
	public static String objectClassHasID = "hasObjectClassID";
	
	public static int targetCRS = 4326; //WGS84 	
	
	public static String GGRS = "http://www.opengis.net/def/crs/EPSG/0/2100";
	
	public static String queryPrefix = "PREFIX gno: <"+mainNS+">\n" +
									   "PREFIX search: <"+searchNS+">\n" +
									   "PREFIX geo: <"+geoNS+">\n" +
									   "PREFIX rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#>\n" +
									   "PREFIX rdfs: <http://www.w3.org/2000/01/rdf-schema#>\n" + 
									   "PREFIX xsd: <http://www.w3.org/2001/XMLSchema#>\n";
	
	public static String[] simpleFeatures = {Within.NAME, 
											 Intersects.NAME, 
											 Overlaps.NAME, 
											 Crosses.NAME, 
											 CoveredBy.NAME, 
											 Covers.NAME, 
											 Contains.NAME, 
											 ContainsProperly.NAME,  
											 Equals.NAME, 
											 Disjoint.NAME, 
											 Touches.NAME};
	
}
