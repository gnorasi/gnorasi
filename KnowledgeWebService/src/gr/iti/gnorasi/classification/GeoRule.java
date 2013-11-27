package gr.iti.gnorasi.classification;

import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.openrdf.model.URI;


public class GeoRule extends Rule {
	
	GeoFeature feature;
	URI targetClass;
	
	public enum GeoFeature {
		CONTAINS("{1}geof:sfContains{2}"),
		IS_CONTAINED_BY("{2}geof:sfContains{1}"),
		CONTAINS_PROPERLY("{1}ext:containsProperly{2}"),
		IS_CONTAINED_PROPERLY_BY("{2}ext:containsProperly{1}"),
		COVERS("{1}ext:covers{2}"),
		IS_COVERED_BY("{2}ext:covers{1}"),
		CROSSES("{1}geof:sfCrosses{2}"),
		DISJOINT("{1}geof:sfDisjoint{2}"),
		EQUALS("{1}geof:sfEquals{2}"),
		INTERSECTS("{1}geof:sfIntersects{2}"),
		OVERLAPS("{1}geof:sfOverlaps{2}"),
		IS_OVERLAPED_BY("{2}geof:sfOverlaps{1}"),
		TOUCHES("{1}geof:sfTouches{2}"),
		WITHIN("{1}geof:sfWithin{2}"),
		IS_WITHIN("{2}geof:sfWithin{1}"),
		DISTANCE("{1}geof:sfDistance{2}");
		
		private String relation;
		
		private GeoFeature(String g) {
			this.relation = g;
		}
		
		public String getRelation() {
			return relation;
		}
	};
	
	private static final Map<String, GeoFeature> map = new HashMap<String, GeoFeature>();
	static {
		map.put("contains", GeoFeature.CONTAINS);
		map.put("isContainedBy", GeoFeature.IS_CONTAINED_BY);
		map.put("containsProperly", GeoFeature.CONTAINS_PROPERLY);
		map.put("isContainedProperlyBy", GeoFeature.IS_CONTAINED_PROPERLY_BY);
		map.put("covers", GeoFeature.COVERS);
		map.put("isCoveredBy", GeoFeature.IS_COVERED_BY);
		map.put("crosses", GeoFeature.CROSSES);
		map.put("disjoint", GeoFeature.DISJOINT);
		map.put("equals", GeoFeature.EQUALS);
		map.put("intersects", GeoFeature.INTERSECTS);
		map.put("overlaps", GeoFeature.OVERLAPS);
		map.put("isOverlapedBy", GeoFeature.IS_OVERLAPED_BY);
		map.put("touches", GeoFeature.TOUCHES);
		map.put("within", GeoFeature.WITHIN);
		map.put("isWithin", GeoFeature.IS_WITHIN);
		map.put("distance", GeoFeature.DISTANCE);
	}
	
	public static GeoFeature createGeoFeature(String input) {
		return map.get(input);
	}

	public GeoFeature getProperty() {
		return feature;
	}

	public void setProperty(GeoFeature property) {
		this.feature = property;
	}

	public URI getTargetClass() {
		return targetClass;
	}

	public void setTargetClass(URI targetClass) {
		this.targetClass = targetClass;
	}
	
	public String geoFunction(String arg1, String arg2) {
		String ret = "";
		
		Pattern pattern = Pattern.compile("(\\{\\d+\\})([a-zA-Z:]+)(\\{\\d+\\})");
		Matcher m = pattern.matcher(feature.getRelation());
		
		if (m.find()) {
			if (m.group(1).equals("{1}")) {
				ret = m.group(2)+"("+arg1+", "+arg2+")";
			}
			else if (m.group(1).equals("{2}")) {
				ret = m.group(2)+"("+arg2+", "+arg1+")";
			}
		}
		
		return ret;
	}
	
	@Override
	public String toString() {
		String ret = "";
		
		ret = this.getClass().getSimpleName()+" id="+getId()+" [\n";
		ret += "  spatial relation = "+getProperty()+"\n";
		ret += "  target class = "+getTargetClass()+"\n]\n";
		
		return ret;
	}
}
