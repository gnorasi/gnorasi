package gr.iti.gnorasi.rules.fuzzy.function;

public class RightShoulder extends FuzzyFunction {
	
	public static final String name = "rightShoulder";
	public static final int numberOfParameters = 2;
	
	public RightShoulder() {
		super(numberOfParameters);
	}
	
	@Override
	public double calculate(double val) {
		if (isReady()) {
			if (val < this.getParameters()[0])
				return 0.0;
			else if (val > this.getParameters()[1])
				return 1.0;
			else {
				return ((val - this.getParameters()[0])/(this.getParameters()[1] - this.getParameters()[0]));
			}
		}
		
		return -1;
	}
	
	@Override
	public boolean isReady() {
		if (this.getParameters()[0] != this.getParameters()[1])
			return true;
		
		return false;
	}

}
