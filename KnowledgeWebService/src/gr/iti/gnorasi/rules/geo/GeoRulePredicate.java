package gr.iti.gnorasi.rules.geo;

import gr.iti.gnorasi.utils.Constants;
import gr.iti.gnorasi.utils.Utilities;

import org.openrdf.model.URI;

public class GeoRulePredicate extends GeoRuleTerm {
	String simpleFeature = null;
	int id = -1;
	
	public GeoRulePredicate() {
		super();
		simpleFeature = null;
		id=-1;
	}
	
	public GeoRulePredicate(String sf, URI rc, int id, int sl) throws IllegalStateException {
		super(rc, sl);
		if (Utilities.contains(Constants.simpleFeatures, sf)) {
			simpleFeature = sf;
			this.id = id;
		}
		else
			throw new IllegalStateException("Property '"+ sf + "' not recognized");
	}
	
	
	
	public void setSF(String sf) throws IllegalStateException {
		if (Utilities.contains(Constants.simpleFeatures, sf))
			simpleFeature = sf;
		else {
			simpleFeature = null;
			throw new IllegalStateException("Property '"+ sf + "' not recognized");
		}
				
	}
	
	public void setID(int id) {
		this.id = id;
	}
	
	public String getSimpleFeature() {
		return simpleFeature;
	}
	
	public int getID() {
		return id;
	}
	
	@Override
	public String toString() {
		
		String r="\n Predicate=[\n";
		r+="  Class="+regionDepiction+"\n";
		r+="  SF="+simpleFeature+"\n";
		r+="  Level="+segmentationLevel+"\n";
		r+="  ID="+id+"\n";
		r+=" ]\n";
		
		return r;
	}
}
