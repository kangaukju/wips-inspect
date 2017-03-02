package air.wips.inspect.result;

import java.util.ArrayList;
import java.util.List;

public class AirCaptureResult {
	private String profile;
	private String conf;
	private String key;
	private String xid;
	private String sec;
	private String usec;
	private String elapsed;
	private String pwr;
	private String type;
	private String subtype;
	private String addr1;
	private String addr2;
	private String addr3;
	private String addr4;
	private String ds;
	private String seq;
	private String deauthcode;
	
		
	public static final String AIR_CAPTURE_KEY = "1";
	public static final String AIR_SHOOTER_KEY = "2";
	
	public static List<AirCaptureResult> extractAirShooterResult(List<AirCaptureResult> list) {
		return extractAirResult(list, AIR_SHOOTER_KEY);
	}
	
	public static List<AirCaptureResult> extractAirCaptureResult(List<AirCaptureResult> list) {
		return extractAirResult(list, AIR_CAPTURE_KEY);
	}
	
	public static List<AirCaptureResult> extractAirResult(List<AirCaptureResult> list, String key) {
		List<AirCaptureResult> results = new ArrayList<>();
		for (AirCaptureResult r : list) {
			if (key.equals(r.getKey())) {
				results.add(r);
			}
		}
		return results;
	}
	
	public String getProfile() {
		return profile;
	}
	public void setProfile(String profile) {
		this.profile = profile;
	}
	public String getConf() {
		return conf;
	}
	public void setConf(String conf) {
		this.conf = conf;
	}
	public String getPwr() {
		return pwr;
	}
	public void setPwr(String pwr) {
		this.pwr = pwr;
	}
	public String getKey() {
		return key;
	}
	public void setKey(String key) {
		this.key = key;
	}
	public String getXid() {
		return xid;
	}
	public void setXid(String xid) {
		this.xid = xid;
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
	public String getType() {
		return type;
	}
	public void setType(String type) {
		this.type = type;
	}
	public String getSubtype() {
		return subtype;
	}
	public void setSubtype(String subtype) {
		this.subtype = subtype;
	}
	public String getAddr1() {
		return addr1;
	}
	public void setAddr1(String addr1) {
		this.addr1 = addr1;
	}
	public String getAddr2() {
		return addr2;
	}
	public void setAddr2(String addr2) {
		this.addr2 = addr2;
	}
	public String getAddr3() {
		return addr3;
	}
	public void setAddr3(String addr3) {
		this.addr3 = addr3;
	}
	public String getAddr4() {
		return addr4;
	}
	public void setAddr4(String addr4) {
		this.addr4 = addr4;
	}
	public String getDs() {
		return ds;
	}
	public void setDs(String ds) {
		this.ds = ds;
	}
	public String getSeq() {
		return seq;
	}
	public void setSeq(String seq) {
		this.seq = seq;
	}
	public String getDeauthcode() {
		return deauthcode;
	}
	public void setDeauthcode(String deauthcode) {
		this.deauthcode = deauthcode;
	}
	@Override
	public String toString() {
		return "AirCaptureResult [profile=" + profile + ", conf=" + conf + ", key=" + key + ", xid=" + xid + ", sec="
				+ sec + ", usec=" + usec + ", elapsed=" + elapsed + ", pwr=" + pwr + ", type=" + type + ", subtype="
				+ subtype + ", addr1=" + addr1 + ", addr2=" + addr2 + ", addr3=" + addr3 + ", addr4=" + addr4 + ", ds="
				+ ds + ", seq=" + seq + ", deauthcode=" + deauthcode + "]";
	}
}
