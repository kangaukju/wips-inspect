package air.wips.inspect.osdep;

import java.util.List;

public class WifiDevice {
	private String phy;
	private String ifname;
	private String driver;
	private String chipset;
	private boolean monitor;
	
	public static boolean contains(List<WifiDevice> list, String ifname) {
		for (WifiDevice dev : list) {
			if (dev.ifname.equals(ifname)) {
				return true;
			}
		}
		return false;
	}
	
	public String getPhy() {
		return phy;
	}
	public void setPhy(String phy) {
		this.phy = phy;
	}
	public String getIfname() {
		return ifname;
	}
	public void setIfname(String ifname) {
		this.ifname = ifname;
	}
	public String getDriver() {
		return driver;
	}
	public void setDriver(String driver) {
		this.driver = driver;
	}
	public String getChipset() {
		return chipset;
	}
	public void setChipset(String chipset) {
		this.chipset = chipset;
	}
	public boolean isMonitor() {
		return monitor;
	}
	public void setMonitor(boolean monitor) {
		this.monitor = monitor;
	}
	@Override
	public String toString() {
		return "WifiDevice [phy=" + phy + ", ifname=" + ifname + ", driver=" + driver + ", chipset=" + chipset
				+ ", monitor=" + monitor + "]";
	}
}
