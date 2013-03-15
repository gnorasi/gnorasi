package gr.iti.gnorasi.rules.fuzzy;

import gr.iti.gnorasi.utils.CSV2RDF;
import gr.iti.gnorasi.utils.Constants;

import java.io.BufferedReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.geotools.geometry.jts.JTS;
import org.geotools.referencing.CRS;
import org.opengis.referencing.FactoryException;
import org.opengis.referencing.crs.CoordinateReferenceSystem;
import org.opengis.referencing.operation.MathTransform;
import org.openrdf.model.Literal;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;
import org.openrdf.model.ValueFactory;
import org.openrdf.model.vocabulary.RDF;
import org.openrdf.repository.RepositoryConnection;
import org.openrdf.repository.RepositoryException;
import org.openrdf.repository.RepositoryResult;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import com.vividsolutions.jts.geom.Geometry;
import com.vividsolutions.jts.io.WKTReader;

public class FuzzyCSV2SPARQL {
	
	public URI[] parseCSVHeader(String headerLine, ValueFactory f) {
		String[] parts = headerLine.split(";");
		URI[] ret = new URI[parts.length-1];
		
		if (parts.length == 1)
			return ret;
			
		for (int i=1;i<parts.length;i++) {
			ret[i-1] = f.createURI(Constants.userNS+parts[i].trim());
		}
		
		return ret;
	}
	
	public void parseCSV(BufferedReader reader, RepositoryConnection con, String context, boolean verbose) {
		try {
			ValueFactory f = con.getValueFactory();
			//read header line
			String  line = reader.readLine();
			URI[] fuzzyProperties = parseCSVHeader(line, f);
			int lineNumber = 1;
			int batch = 20;
			
			URI region = null;
			
			URI URIcontext = null;
			if (!context.equals(""))
				URIcontext = f.createURI(context);
			
			String currentId = "";
			
			while ((line = reader.readLine()) != null) {
				lineNumber++;
				try {
					String[] parts = line.split(";");
										
					if (parts.length != fuzzyProperties.length+1)
						continue;
					
					if (!currentId.equals(parts[0])) {
						int id = Integer.parseInt(parts[0]);
						RepositoryResult<Statement> res = con.getStatements(null, Constants.regionHasIDProperty, f.createLiteral(id), false);
						if (res.hasNext()) {
							region = (URI)res.next().getSubject();
							currentId = parts[0];
						}
					}
					
					if (region != null) {
						for (int i=0;i<fuzzyProperties.length;i++) {
							Literal value = f.createLiteral(Float.parseFloat(parts[i+1]));
							if (URIcontext == null)
								con.add(region, fuzzyProperties[i], value);
							else {
								con.add(region, fuzzyProperties[i], value, URIcontext);
							}
							if (verbose)
								System.out.println(region+" "+fuzzyProperties[i]+" "+value);
						}
					}
					
					if (lineNumber % batch == 0) {
						con.commit();
					}
				}catch (IllegalStateException e) {
					System.out.println(e.getMessage());
				}
				
			}
			//commit any uncommited statements
			con.commit();
		}catch (Exception e) {
			e.printStackTrace();
		}
	}
}
