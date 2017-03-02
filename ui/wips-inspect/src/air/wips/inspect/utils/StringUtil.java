package air.wips.inspect.utils;

public class StringUtil {

	public static boolean isNull(String string) {
		if (string == null || "".equals(string)) {
			return true;
		}
		return false;
	}
	
	public static String Null(String string) {
		if (isNull(string)) {
			return null;
		}
		return string;
	}
	
	public static int Int(String string) {
		String i = Null(string);
		if (i == null) {
			return 0;
		}
		return Integer.parseInt(i);
	}
	
	public static String def(String value, String def) {
		if (isNull(value)) {
			return def;
		}
		return value;
	}
	
	public static String spread(String [] arr, String delim) {
		StringBuffer sb = new StringBuffer();
		for (int i=0; i<arr.length; i++) {
			sb.append(arr[i] + delim);
		}
		return sb.toString();
	}
}
