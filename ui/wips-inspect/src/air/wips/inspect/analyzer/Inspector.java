package air.wips.inspect.analyzer;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

import air.wips.inspect.analyzer.factors.InspectorAbstract;
import air.wips.inspect.analyzer.factors.NotInspectionDataException;
import air.wips.inspect.log.D;
import air.wips.inspect.result.AirCaptureResult;

public class Inspector {
	private static Inspector $ = null;
	
	private Map<String, List<AirCaptureResult>> configMap = new HashMap<>();
	
	/**
	 * Insepctor singletone
	 * @return
	 */
	public static Inspector getInstance() {
		return getInstance(false);
	}
	
	public static Inspector getInstance(boolean clear) {
		if ($ == null) {
			$ = new Inspector();
		}
		if (clear) {
			$.clear();
		}
		return $;
	}
	
	private Inspector() { }
	
	public void clear() {
		configMap.clear();
	}
	
	/**
	 * 
	 * @param result
	 */
	public void appendAirCaptureResult(AirCaptureResult result) {
		String config = result.getConf();
		List<AirCaptureResult> airCaptureResultList = null;
		
		airCaptureResultList = configMap.get(config);
		if (airCaptureResultList == null) {
			airCaptureResultList = new ArrayList<>();
			configMap.put(config, airCaptureResultList);
		}
		airCaptureResultList.add(result);
	}
	
	public void showAll(List<AirCaptureResult> airCaptureResultList) {
		for (AirCaptureResult result : airCaptureResultList) {
			System.out.println(result);
		}
	}
	
	public void showAll() {
		Set<String> configSet = configMap.keySet();
		Iterator<String> configIt = configSet.iterator();
		String config;
		
		while (configIt.hasNext()) {
			config = configIt.next();
			System.out.println("[config] "+config);
			showAll(configMap.get(config));
		}
	}
	
	public double inspect(String config, InspectorAbstract [] inspections) throws Exception {
		double score = 0;
		
		List<AirCaptureResult> list = configMap.get(config);
		if (list == null) {
			throw new Exception("not found AirCaptureResult of config - "+config);
		}
		
		List<AirCaptureResult> captureResults = AirCaptureResult.extractAirCaptureResult(list);
		List<AirCaptureResult> shooterResults = AirCaptureResult.extractAirShooterResult(list);
		
		
		for (InspectorAbstract inspection : inspections) {
			try {
				score += inspection.inspect(captureResults, shooterResults);
			} catch (NotInspectionDataException e) {
				D.log(e.getMessage());
			}
		}
		
		return (score/inspections.length)*100;
	}
}
