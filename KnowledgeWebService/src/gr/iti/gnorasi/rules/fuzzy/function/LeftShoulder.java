package gr.iti.gnorasi.rules.fuzzy.function;

public class LeftShoulder extends FuzzyFunction {
	
	public static final String name = "leftShoulder";
	public static final int numberOfParameters = 2;
	
	public LeftShoulder() {
		super(numberOfParameters);
	}
	
	@Override
	public double calculate(double val) {
		if (isReady()) {
			if (val < this.getParameters()[0])
				return 1.0;
			else if (val > this.getParameters()[1])
				return 0.0;
			else {
				return ((this.getParameters()[1] - val)/(this.getParameters()[1] - this.getParameters()[0]));
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

/*	public static void main(String[] args) {
		FuzzyFunction f = FuzzyFunctionFactory.createFuzzyFunction(FuzzyFunctions.RightShoulder);
		double[] params = new double[2]; params[0] = 100; params[1] = 300;
		f.setParameters(params);
		System.out.println("Fuzzy value = "+f.calculate(150));
		System.out.println("params number = "+f.parameters.length);
	}
*/	
}
