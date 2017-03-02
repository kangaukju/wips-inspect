package air.wips.inspect.utils;

import java.util.regex.Pattern;

public class Validation {
	private static Pattern _MAC_PATTERN = Pattern.compile("^([0-9A-Fa-f]{2}[\\.:-]){5}([0-9A-Fa-f]{2})$");
	public static boolean macaddress(String mac) {
		return _MAC_PATTERN.matcher(mac).matches();
	}
}
