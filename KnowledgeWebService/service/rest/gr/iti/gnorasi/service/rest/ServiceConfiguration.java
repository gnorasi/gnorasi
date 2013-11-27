package gr.iti.gnorasi.service.rest;

import java.io.FileInputStream;
import java.io.IOException;
import java.util.Properties;

public class ServiceConfiguration {
	private static Properties properties = new Properties();
	
	public static void readConfigurationFile(String file) {
		try {
			properties.load(new FileInputStream(file));
    	}catch (IOException ex) {
    		//ex.printStackTrace();
    		System.err.println("Could not find configuration file \"conf/service.properties\". Program will exit");
    		System.exit(0);
        }
	}
	
	public static String getProperty(String key) {
		if (properties != null)
			return properties.getProperty(key);
		
		return null;
	}
}
