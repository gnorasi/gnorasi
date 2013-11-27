package gr.iti.gnorasi.rules.fuzzy.function;

public abstract class FuzzyFunction {

	public double[] parameters;
		
	public FuzzyFunction(int paramsNum) {
		parameters = new double[paramsNum];
		for (int i=0;i<paramsNum;i++) 
			parameters[i] = 0;
	}
	
	public abstract double calculate(double val);
	
	public void setParameters(double params[]) {
		if (parameters.length != params.length)
			return;
	
		for (int i=0;i<params.length;i++)
			parameters[i] = params[i];
	}
	
	public double[] getParameters() {
		return parameters;
	}
		
	public abstract boolean isReady();

}
