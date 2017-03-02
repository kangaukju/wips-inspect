package air.wips.inspect.conf;

public class XmlAirConfEnv {
	private static final String CONFIG_HOME = System.getProperty("catalina.home") + "/airconf";
	
	public static final String path() {
		return CONFIG_HOME;
	}
	public static final String path(final String filename) {
		return CONFIG_HOME+"/"+filename;
	}
}
