package air.wips.inspect.result;

public class AirCurrentST {
	private String stamac;
	private String bssid;
	private String channel;
	private String power;
	private String rate;
	private String pktCount;
	private String dataBytes;
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
	public String getChannel() {
		return channel;
	}
	public void setChannel(String channel) {
		this.channel = channel;
	}
	public String getPower() {
		return power;
	}
	public void setPower(String power) {
		this.power = power;
	}
	public String getRate() {
		return rate;
	}
	public void setRate(String rate) {
		this.rate = rate;
	}
	public String getPktCount() {
		return pktCount;
	}
	public void setPktCount(String pktCount) {
		this.pktCount = pktCount;
	}
	public String getDataBytes() {
		return dataBytes;
	}
	public void setDataBytes(String dataBytes) {
		this.dataBytes = dataBytes;
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
		return "AirCurrentST [stamac=" + stamac + ", bssid=" + bssid + ", channel=" + channel + ", power=" + power
				+ ", rate=" + rate + ", pktCount=" + pktCount + ", dataBytes=" + dataBytes + ", tinit=" + tinit
				+ ", tlast=" + tlast + "]";
	}
}
