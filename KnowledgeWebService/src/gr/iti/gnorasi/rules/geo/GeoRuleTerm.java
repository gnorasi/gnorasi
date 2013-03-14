package gr.iti.gnorasi.rules.geo;

import org.openrdf.model.URI;

public class GeoRuleTerm {
	URI regionDepiction;
	int segmentationLevel = 1;
	TargetImage tImage;
	
	public GeoRuleTerm() {
		regionDepiction = null;
		tImage = null;
	}
	
	public GeoRuleTerm(URI rc, int sl) {
		regionDepiction = rc;
		tImage = null;
		if (sl < 0)
			throw new IllegalStateException("Segmentation level is "+sl+". Should be >0");
		else
			segmentationLevel = sl;
	}
	
	public GeoRuleTerm(URI rc, int sl, TargetImage ti) {
		regionDepiction = rc;
		tImage = ti;
		if (sl < 0)
			throw new IllegalStateException("Segmentation level is "+sl+". Should be >0");
		else
			segmentationLevel = sl;
	}
	
	public TargetImage gettImage() {
		return tImage;
	}

	public void settImage(TargetImage tImage) {
		this.tImage = tImage;
	}
	
	public void setRegionDepiction(URI rc) {
		regionDepiction = rc;
	}
	public void setSegmentationLevel(int l) {
		if (l < 0)
			throw new IllegalStateException("Segmentation level is "+l+". Should be >0");
		else
			segmentationLevel = l;
	}

	public URI getRegionDepiction() {
		return regionDepiction;
	}
	public int getSegmentationLevel() {
		return segmentationLevel;
	}
	
	
	public enum TargetImage {
		PASTIMAGE("PASTIMAGE"), 
		CURRENTIMAGE("CURRENTIMAGE"), 
		DEM("DEM");
		String ontoInstance;
		
		TargetImage(String oi) {
			ontoInstance = oi;
		}
		
		public String getOntoInstance() {
			return ontoInstance;
		}
		
		public static TargetImage fromString(String val) {
			if (val != null) {
				for (TargetImage t : TargetImage.values()) {
					if (val.equalsIgnoreCase(t.getOntoInstance()))
						return t;
				}
			}
			return null;
		}
	}
}
