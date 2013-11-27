package gr.iti.gnorasi.rules.fuzzy.function;

public class Triangular extends FuzzyFunction {

	public static final String name = "triangular";
	public static final int numberOfParameters = 3;

	public Triangular() {
		super(numberOfParameters);
	}
	
	@Override
	public double calculate(double val) {
		if (isReady()) {
			double c = 0.5 * ( this.getParameters()[0] + this.getParameters()[1]);
		      if ((val < this.getParameters()[0]) || (val > this.getParameters()[1]))
			return 0;
		      else if ((val >= this.getParameters()[0]) && (val <= c ))
			return 2 * (val - this.getParameters()[0]) / (this.getParameters()[1] - this.getParameters()[0]);
		      else if ((val <= this.getParameters()[1]) && (val > c))
			return 2 * (this.getParameters()[1] - val) / (this.getParameters()[1] - this.getParameters()[0]);
		}
		
		return -1;
	}
	
	@Override
	public boolean isReady() {
		if (this.getParameters()[0] != this.getParameters()[1] && 
			this.getParameters()[1]!= this.getParameters()[2]) {
			
			return true;
		}
		
		return false;
	}
}
