package air.wips.inspect.analyzer.factors;

import java.util.List;

import air.wips.inspect.result.AirCaptureResult;

public abstract class InspectorAbstract {
	protected String name;
	
	public InspectorAbstract(final String name) {
		this.name = name;
	}
	
	abstract public double inspect(
			final List<AirCaptureResult> captureResults, 
			final List<AirCaptureResult> shooterResults) throws NotInspectionDataException;
	
	public String getName() {
		return name;
	}
}
