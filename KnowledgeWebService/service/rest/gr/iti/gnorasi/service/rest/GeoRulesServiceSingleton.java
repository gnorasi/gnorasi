package gr.iti.gnorasi.service.rest;

import gr.iti.gnorasi.GeoRulesService;

public enum GeoRulesServiceSingleton {
	instance;
	
	private GeoRulesService grs;
	
	private GeoRulesServiceSingleton() {
		grs = new GeoRulesService();
	}
	
	public GeoRulesService getService() {
		return grs;
	}	
}
