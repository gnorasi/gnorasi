package gr.iti.gnorasi.utils;

import java.util.HashMap;


public class CSV2RDF {
	//public static String geo = "http://www.opengis.net/def/geosparql/";
	public static HashMap<String, String> properties = new HashMap<String, String>();
	
	public static String wkt = "wkt";
	
	static {
		properties.put("WKT", Constants.geoNS+"asWKT");
		properties.put("NDVI", Constants.mainNS+"hasNDVI");
		properties.put("STD1", Constants.mainNS+"hasSTD1");
		properties.put("STD2", Constants.mainNS+"hasSTD2");
		properties.put("STD3", Constants.mainNS+"hasSTD3");
		properties.put("STD4", Constants.mainNS+"hasSTD4");
		properties.put("BAND1", Constants.mainNS+"hasBAND1");
		properties.put("BAND2", Constants.mainNS+"hasBAND2");
		properties.put("BAND3", Constants.mainNS+"hasBAND3");
		properties.put("BAND4", Constants.mainNS+"hasBAND4");
		properties.put("AREA", Constants.mainNS+"hasAREA");
	}
}
