package air.wips.inspect.history;

import java.io.File;

public class InspectHistory {
	private static final String HISTORY_HOME = System.getProperty("catalina.home") + "/history";
	
	static {
		File historyDirectory = new File(HISTORY_HOME);
		if (!historyDirectory.exists()) {
			historyDirectory.mkdirs();
		}
	}
	
	public static final String path() {
		return HISTORY_HOME;
	}
	public static final String path(String directory) {
		return HISTORY_HOME+"/"+directory;
	}
}