package gr.iti.gnorasi.rules.geo;

import org.openrdf.model.URI;

public class GeoRuleHead extends GeoRuleTerm {
	double confidence;
	
	public GeoRuleHead() {
		super();
		confidence = 0;
	}
	
	public GeoRuleHead(URI rc, int sl, double conf) {
		super(rc, sl);
		confidence = conf;
	}
	
	public void setConfidence(double conf) {
		confidence = conf;
	}
	
	public double getConfidence() {
		return confidence;
	}
	
	@Override
	public String toString() {
		String r="\n Head=[\n";
		r+="  Class="+regionDepiction+"\n";
		r+="  Confidence="+confidence+"\n";
		r+="  Level="+segmentationLevel+"\n";
		r+=" ]\n";
		
		return r;
	}
}
