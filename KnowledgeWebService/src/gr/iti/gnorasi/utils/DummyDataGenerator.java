package gr.iti.gnorasi.utils;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;


public class DummyDataGenerator {

	/**
	 * @param args
	 */
	
	public String parseCSVLine(String line, int lineNum, String[] headers) throws IllegalStateException {
		String[] parts = line.split(";");
		
		Pattern WKTPattern = Pattern.compile("[a-zA-Z]+ \\([\\([0-9\\.\\, ]+\\)[\\,]?]+\\)");
		StringBuffer updateString = new StringBuffer(line.length()*60);
		for (int i=0;i<parts.length;i++) {
			String part = parts[i];
			
			String header = headers[i];
			try {
				if (header.equalsIgnoreCase("WKT")) {
					Matcher m = WKTPattern.matcher(part);
					if (m.find()) {
						updateString.append("\"").append(m.group()).append("\"");
					}
				}
			}catch (Exception e) {
				e.printStackTrace();
			}
		}
		
		return updateString.toString();
	}

	
	public String readWriteCSVFiles(String WKTfilename, String Featurefilename) {
		try {
			//read the WKT shape file which has the most entries
			BufferedReader WKTreader = new BufferedReader(new FileReader(new File(WKTfilename)));
			String line;
			//parse header line
			String headerline = WKTreader.readLine();
			String[] polygonHeaders = headerline.split(",");
					
			//parse the rest of the file and add data to the repository
			int lineNumber = 1;
			List<String> WKTList = new ArrayList<String>(12000);
			
			while ((line = WKTreader.readLine()) != null) {
				lineNumber++;
				try {
					WKTList.add(parseCSVLine(line, lineNumber, polygonHeaders));
				}catch (Exception e) {
					e.printStackTrace();
				}
			}
			
			BufferedReader featureReader = new BufferedReader(new FileReader(new File(Featurefilename)));
			//parse header line
			String originalHeaders = featureReader.readLine();
			headerline = originalHeaders.concat(";WKT");
			
			int counter = 1;
			List<String> dummyList = new ArrayList<String>(WKTList.size());
			for (String wkt : WKTList) {
				String featureLine = featureReader.readLine();
				if (featureLine != null) {
					String newFeatureLine = featureLine.concat(";").concat(wkt);
					newFeatureLine = newFeatureLine.replaceFirst("^[\\d]+", String.valueOf(counter));
					dummyList.add(newFeatureLine);
					counter++;
				}
				else {
					featureReader = new BufferedReader(new FileReader(new File(Featurefilename)));
					//read the header line and discard it
					featureReader.readLine();
				}
			}
			featureReader.close();
			
			//write the new file
			BufferedWriter bf = new BufferedWriter(new FileWriter(new File("data/feature+wkt_Data.csv")));
			bf.write(headerline);
			bf.write("\n");
			for (int i=0;i<dummyList.size();i++) {
				bf.write(dummyList.get(i));
				bf.write("\n");
			}
			bf.close();
			
		}catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}
	
}
