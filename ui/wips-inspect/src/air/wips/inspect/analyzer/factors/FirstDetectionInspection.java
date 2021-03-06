package air.wips.inspect.analyzer.factors;

import java.util.List;

import air.wips.inspect.result.AirCaptureResult;

public class FirstDetectionInspection extends InspectorAbstract {
	private double threshold;
	private double firstDectectPercent;
	
	
	/**

__________|____|____|____|____|____|____|__________
         0              t
            f1
                               f2

	 */
	

	
	public FirstDetectionInspection(double threshold) {
		super(FirstDetectionInspection.class.getSimpleName());
		this.threshold = threshold;
	}

	public double inspect(
			List<AirCaptureResult> captureResults, 
			List<AirCaptureResult> shooterResults) 
					throws NotInspectionDataException 
	{
		if (shooterResults.size() == 0) {
			throw new NotInspectionDataException(getName());
		}
		
		double elapsedTime = Double.valueOf(shooterResults.get(0).getElapsed());
		
		firstDectectPercent = 0;
		
		/* perfect */
		if (elapsedTime <= threshold) {
			firstDectectPercent = 100;
		}
		
		return firstDectectPercent;
	}

	public double getThreshold() {
		return threshold;
	}

	public double getFirstDectectPercent() {
		return firstDectectPercent;
	}
}
