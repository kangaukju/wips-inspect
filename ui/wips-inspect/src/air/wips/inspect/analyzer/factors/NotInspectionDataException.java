package air.wips.inspect.analyzer.factors;

public class NotInspectionDataException extends Exception {
	private String inspectionName;
	
	NotInspectionDataException(final String inspectionName) {
		this.inspectionName = inspectionName;
	}

	@Override
	public String getMessage() {
		return String.format("[%s] no data for inspection", inspectionName);
	}

	@Override
	public void printStackTrace() {
		System.err.println(getMessage());
	}
	
}
