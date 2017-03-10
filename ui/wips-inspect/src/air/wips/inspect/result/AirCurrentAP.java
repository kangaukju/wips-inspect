package air.wips.inspect.result;

public class AirCurrentAP {
	private String bssid;
	private String ch;
	private String ssid;
	private String proto;
	private String pwr;
	private String auth;
	private String enc;
	private String tinit;
	private String tlast;
	
	public String getBssid() {
		return bssid;
	}
	public void setBssid(String bssid) {
		this.bssid = bssid;
	}
	public String getCh() {
		return ch;
	}
	public void setCh(String ch) {
		this.ch = ch;
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
	public String getPwr() {
		return pwr;
	}
	public void setPwr(String pwr) {
		this.pwr = pwr;
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
		return "AirCurrentAP [bssid=" + bssid + ", ch=" + ch + ", ssid=" + ssid + ", proto=" + proto + ", pwr=" + pwr
				+ ", auth=" + auth + ", enc=" + enc + ", tinit=" + tinit + ", tlast=" + tlast + "]";
	}
}
