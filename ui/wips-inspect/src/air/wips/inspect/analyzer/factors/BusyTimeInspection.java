package air.wips.inspect.analyzer.factors;

import java.util.List;

import air.wips.inspect.result.AirCaptureResult;

public class BusyTimeInspection extends InspectorAbstract {

	/* 차단이 되는 최적의 deauth packet 송출 시간 간격  - time(msec) */
	private double threshold;
	private double busyPercent;
	private double busyScore;
	private double busys[];
	private double busyDeltaTimes[]; // 혼잡율 계산을 위한 송신된 패킷의 시간 간격
	
	
	public BusyTimeInspection(double threshold) {
		super(BusyTimeInspection.class.getSimpleName());
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
		
		busyScore = calcBusyTimes(captureResults);
		busyPercent = (busyScore / sumTime) * 100;
		
		return busyPercent;
	}
	
	/**
	 * @brief deauth 잦은시간 계산 (혼잡율)
	 * @param captureResults
	 */
	public double calcBusyTimes(List<AirCaptureResult> captureResults) {
		double elapsedTime;
		double prevTime = 0;
		double deltaTime;
		double busyTime;
		double score = 0;
		int i = 0;
		
		busys = new double[captureResults.size()];
		busyDeltaTimes = new double[captureResults.size()];
		
		for (AirCaptureResult r : captureResults) {
			elapsedTime = Double.valueOf(r.getElapsed()).intValue();
			
			// delta time = 패킷 수신 시간 - 이전 시간
			busyDeltaTimes[i] = deltaTime = elapsedTime - prevTime;
			// 기준시간 간격과 잦은시간 간격 차이
			busyTime = deltaTime - threshold;
			// 빈번이 발생하는 경우에만 계산에 포함
			busys[i] = (busyTime < 0) ? busyTime : 0;
			
			score += busys[i];
			
			prevTime = elapsedTime;
			i++;
		}
		return score;
	}
	
	public double getThreshold() {
		return threshold;
	}

	public double getBusyPercent() {
		return busyPercent;
	}

	public double getBusyScore() {
		return busyScore;
	}

	public double[] getBusys() {
		return busys;
	}

	public double[] getBusyDeltaTimes() {
		return busyDeltaTimes;
	}
}
