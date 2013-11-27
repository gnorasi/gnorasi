package gr.iti.gnorasi.rules.fuzzy.function;

public class Trapezoidal extends FuzzyFunction {
	
	public static final String name = "trapezoidal";
	public static final int numberOfParameters = 4;

	public Trapezoidal() {
		super(numberOfParameters);
	}
	
	@Override
	public double calculate(double val) {
		if (isReady()) {
			double trpz = Math.min((val - this.getParameters()[0])/(this.getParameters()[1] - this.getParameters()[0]), 1.0);
	        trpz = Math.min (trpz, (this.getParameters()[3] - val)/(this.getParameters()[3] - this.getParameters()[2]));
	        trpz = Math.max (trpz, 0.0);
	        
	        return trpz;
		}
		
		return -1;
	}
	
	@Override
	public boolean isReady() {
		if (this.getParameters()[0] != this.getParameters()[1] && 
			this.getParameters()[1]!=this.getParameters()[2] && 
			this.getParameters()[2]!=this.getParameters()[3]) {
			
			return true;
		}
		
		return false;
	}

}
