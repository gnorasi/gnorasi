package gr.iti.gnorasi.classification;

import org.openrdf.model.URI;

public class FuzzyRule extends Rule {
	URI fuzzyProperty;
	String ruleType = "";

	public URI getFuzzyProperty() {
		return fuzzyProperty;
	}

	public void setFuzzyProperty(URI fuzzyProperty) {
		this.fuzzyProperty = fuzzyProperty;
	}
	
	public String getRuleType() {
		return ruleType;
	}
	
	@Override
	public String toString() {
		String ret = "";
		
		//ret = this.getClass().getSimpleName()+" id="+getId()+" [\n";
		ret = getRuleType()+" id="+getId()+" [\n";
		ret += "  property="+getFuzzyProperty()+"\n]\n";
		
		return ret;
	}

}
