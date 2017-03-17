package air.wips.inspect.conf;

import java.io.File;

public class XmlAirConfEnv {
	private static final String CONFIG_HOME = System.getProperty("catalina.home") + "/airconf";
	
	static {
		File configDirectory = new File(CONFIG_HOME);
		if (!configDirectory.exists()) {
			configDirectory.mkdirs();
		}
	}
	
	public static final String path() {
		return CONFIG_HOME;
	}
	public static final String path(final String filename) {
		return CONFIG_HOME+"/"+filename;
	}
}
