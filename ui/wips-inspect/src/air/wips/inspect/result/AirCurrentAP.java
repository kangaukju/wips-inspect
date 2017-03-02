package air.wips.inspect.result;

public class AirCurrentAP {
	private String bssid;
	private String channel;
	private String ssid;
	private String proto;
	private String power;
	private String auth;
	private String enc;
	private String ip;
	private String tinit;
	private String tlast;
		
	public String getBssid() {
		return bssid;
	}

	public void setBssid(String bssid) {
		this.bssid = bssid;
	}

	public String getChannel() {
		return channel;
	}

	public void setChannel(String channel) {
		this.channel = channel;
	}

	public String getSsid() {
		return ssid;
	}

	public void setSsid(String ssid) {
		this.ssid = ssid;
	}

	public String getProto() {
		return proto;
	}

	public void setProto(String proto) {
		this.proto = proto;
	}

	public String getPower() {
		return power;
	}

	public void setPower(String power) {
		this.power = power;
	}

	public String getAuth() {
		return auth;
	}

	public void setAuth(String auth) {
		this.auth = auth;
	}

	public String getEnc() {
		return enc;
	}

	public void setEnc(String enc) {
		this.enc = enc;
	}

	public String getIp() {
		return ip;
	}

	public void setIp(String ip) {
		this.ip = ip;
	}

	public String getTinit() {
		return tinit;
	}

	public void setTinit(String tinit) {
		this.tinit = tinit;
	}

	public String getTlast() {
		return tlast;
	}

	public void setTlast(String tlast) {
		this.tlast = tlast;
	}

	@Override
	public String toString() {
		return "AirCurrent [bssid=" + bssid + ", channel=" + channel + ", ssid=" + ssid + ", proto=" + proto
				+ ", power=" + power + ", auth=" + auth + ", enc=" + enc + ", ip=" + ip + ", tinit=" + tinit
				+ ", tlast=" + tlast + "]";
	}
	
}