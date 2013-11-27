package gr.iti.gnorasi.rules.fuzzy.function;

public class FuzzyFunctionFactory {
	
	public static FuzzyFunction createFuzzyFunction(String name) {
		if (name.equals(LeftShoulder.name))
			return new LeftShoulder();
		else if (name.equals(RightShoulder.name))
			return new RightShoulder();
		else if (name.equals(Trapezoidal.name))
			return new Trapezoidal();
		else if (name.equals(Triangular.name))
			return new Triangular();
		else
			return new LeftShoulder();
	}
}
