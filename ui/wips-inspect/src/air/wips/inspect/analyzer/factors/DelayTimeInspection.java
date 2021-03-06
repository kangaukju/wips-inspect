package air.wips.inspect.analyzer.factors;

import java.util.List;

import air.wips.inspect.result.AirCaptureResult;

public class DelayTimeInspection extends InspectorAbstract {

	/* 차단이 되는 최적의 deauth packet 송출 시간 간격  - time(msec) */
	private double threshold;
	private double delayPercent;
	private double delayScore;
	private double delays[];
	private double delayDeltaTimes[]; // 유실율 계산을 위한 송신된 패킷의 시간 간격
	
	
	public DelayTimeInspection(double threshold) {
		super(DelayTimeInspection.class.getSimpleName());
		this.threshold = threshold;
	}
	
	public double inspect(
			List<AirCaptureResult> captureResults, 
			List<AirCaptureResult> shooterResults) 
					throws NotInspectionDataException 
	{
		if (captureResults.size() == 0) {
			throw new NotInspectionDataException(getName());
		}
		double sumTime = Double.valueOf(
				captureResults.get(captureResults.size()-1).getElapsed());
		
		delayScore = calcDelayTimes(captureResults);
		delayPercent = (delayScore / sumTime) * 100;
		
		return delayPercent;
	}
	
	/**
	 * @brief deauth 지연시간 계산 (유실율)
	 * @param captureResults
	 */
	public double calcDelayTimes(List<AirCaptureResult> captureResults) {
		double elapsedTime;
		double prevTime = 0;
		double deltaTime;
		double delayTime;
		double score = 0;
		int i = 0;
		
		delays = new double[captureResults.size()];
		delayDeltaTimes = new double[captureResults.size()];
		
		for (AirCaptureResult r : captureResults) {
			elapsedTime = Double.valueOf(r.getElapsed()).intValue();
			
			// delta time = 패킷 수신 시간 - 이전 시간
			delayDeltaTimes[i] = deltaTime = (elapsedTime - prevTime);
			// 기준시간 간격과 지연시간 간격 차이
			delayTime = deltaTime - threshold;
			// 지연이 발생하는 경우에만 계산에 포함
			delays[i] = (delayTime > 0) ? delayTime : 0;
			
			score += delays[i];
			
			prevTime = elapsedTime;
			i++;
		}
		return score;
	}

	public double getThreshold() {
		return threshold;
	}

	public double getDelayPercent() {
		return delayPercent;
	}

	public double getDelayScore() {
		return delayScore;
	}

	public double[] getDelays() {
		return delays;
	}

	public double[] getDelayDeltaTimes() {
		return delayDeltaTimes;
	}
}
