package gr.iti.gnorasi.rules.fuzzy;

import org.openrdf.model.URI;

public class FuzzyRestriction {

	Operator operator;
	URI property;
	double threshold;
	
	public URI getProperty() {
		return property;
	}
	public void setProperty(URI property) {
		this.property = property;
	}
	public Operator getOperator() {
		return operator;
	}
	public void setOperator(Operator operator) {
		this.operator = operator;
	}
	public double getThreshold() {
		return threshold;
	}
	public void setThreshold(double threshold) {
		this.threshold = threshold;
	}
	
	@Override
	public String toString() {
		String ret="";
		
		ret = "restriction is (" + 
				" if " + property.getLocalName() + 
				" is "+ operator.getSymbol() + " "+ threshold + " )";
		
		return ret;
	}
	
	public enum Operator {
		LE("le"),GE("ge"),EQ("eq"),G("g"),L("l");
		private String op;
		
		private Operator(String s) {
			op=s;
		}
		public String getText() {
			return op;
		}
		
		public String getSymbol() {
			if (getText().equals("le"))
				return "<=";
			else if (getText().equals("ge"))
				return ">=";
			else if (getText().equals("eq"))
				return "=";
			else if (getText().equals("g"))
				return ">";
			else if (getText().equals("l"))
				return "<";
			
			return "";
		}
	}
}
