package air.wips.inspect.result;

public class AirCurrentST {
	private String stamac;
	private String bssid;
	private String ch;
	private String pwr;
	private String rate;
	private String pkt;
	private String bytes;
	private String tinit;
	private String tlast;
	
	public String getStamac() {
		return stamac;
	}
	public void setStamac(String stamac) {
		this.stamac = stamac;
	}
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
	public String getPwr() {
		return pwr;
	}
	public void setPwr(String pwr) {
		this.pwr = pwr;
	}
	public String getRate() {
		return rate;
	}
	public void setRate(String rate) {
		this.rate = rate;
	}
	public String getPkt() {
		return pkt;
	}
	public void setPkt(String pkt) {
		this.pkt = pkt;
	}
	public String getBytes() {
		return bytes;
	}
	public void setBytes(String bytes) {
		this.bytes = bytes;
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
		return "AirCurrentST [stamac=" + stamac + ", bssid=" + bssid + ", ch=" + ch + ", pwr=" + pwr + ", rate=" + rate
				+ ", pkt=" + pkt + ", bytes=" + bytes + ", tinit=" + tinit + ", tlast=" + tlast + "]";
	}	
}
