package air.wips.inspect.result;

public class AirShooterResult {
	private String key;
	private String sec;
	private String usec;
	private String elapsed;
	public String getKey() {
		return key;
	}
	public void setKey(String key) {
		this.key = key;
	}
	public String getSec() {
		return sec;
	}
	public void setSec(String sec) {
		this.sec = sec;
	}
	public String getUsec() {
		return usec;
	}
	public void setUsec(String usec) {
		this.usec = usec;
	}
	public String getElapsed() {
		return elapsed;
	}
	public void setElapsed(String elapsed) {
		this.elapsed = elapsed;
	}
	@Override
	public String toString() {
		return "AirShooterResult [key=" + key + ", sec=" + sec + ", usec=" + usec + ", elapsed=" + elapsed + "]";
	}
}
