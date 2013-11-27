package gr.iti.gnorasi.classification;

import java.util.ArrayList;
import java.util.List;

import org.openrdf.model.URI;

public class ClassificationRules {

	//List<AttributeRule> attributeRules;
	//List<ShapeRule> shapeRules;
	List<FuzzyRule> fuzzyRules;
	List<GeoRule> geoRules;
	RuleOperator op;
	URI head;
	
	public ClassificationRules() {
		//attributeRules = new ArrayList<AttributeRule>();
		//shapeRules = new ArrayList<ShapeRule>();
		fuzzyRules = new ArrayList<FuzzyRule>();
		geoRules = new ArrayList<GeoRule>();
		head = null;
	}
	
/*	public List<AttributeRule> getAttributeRules() {
		return attributeRules;
	}

	public void setAttributeRules(List<AttributeRule> attributeRules) {
		this.attributeRules = attributeRules;
	}
*/
	public List<FuzzyRule> getFuzzyRules() {
		return fuzzyRules;
	}

	public void setFuzzyRules(List<FuzzyRule> fuzzyRules) {
		this.fuzzyRules = fuzzyRules;
	}
	
/*	public List<ShapeRule> getShapeRules() {
		return shapeRules;
	}
	
	public void setShapeRules(List<ShapeRule> shapeRules) {
		this.shapeRules = shapeRules;
	}
*/	
	public List<GeoRule> getGeoRules() {
		return geoRules;
	}

	public void setGeoRules(List<GeoRule> geoRules) {
		this.geoRules = geoRules;
	}

	public RuleOperator getOp() {
		return op;
	}

	public void setOp(RuleOperator op) {
		this.op = op;
	}

	public URI getHead() {
		return head;
	}

	public void setHead(URI head) {
		this.head = head;
	}

	public enum RuleOperator {
		MIN("min"), MAX("max");
		
		private String operator;
		
		private RuleOperator(String o) {
			this.operator = o;
		}
		
		public String getOperator() {
			return operator;
		}
		
		public static RuleOperator fromString(String val) {
			if (val != null) {
				for (RuleOperator t : RuleOperator.values()) {
					if (val.equalsIgnoreCase(t.getOperator()))
						return t;
				}
			}
			return null;
		}
		
	};
	
	@Override
	public String toString() {
		String ret = "\n";
		
		ret = "head = "+getHead()+", operator = "+getOp()+"\n";
		for (FuzzyRule rule : fuzzyRules) {
			ret += rule;
		}
		for (GeoRule rule : geoRules) {
			ret += rule;
		}
		
		ret += "--------------------------\n";
		
		return ret;
	}
	
}
