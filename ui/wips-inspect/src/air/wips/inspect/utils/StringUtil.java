package air.wips.inspect.utils;

public class StringUtil {

	public static boolean isBlank(String tmpString) {
		if (isNull(tmpString)) {
			return true;
		}
		if ("".equals(tmpString)) {
			return true;
		}

		return false;
	}
	
	public static String replace(String source, String pattern, String replace) {
		if (source != null) {
			final int len = pattern.length();
			StringBuffer sb = new StringBuffer();
			int found = -1;
			int start = 0;

			while ((found = source.indexOf(pattern, start)) != -1) {
				sb.append(source.substring(start, found));
				sb.append(replace);
				start = found + len;
			}

			sb.append(source.substring(start));

			return sb.toString();
		}

		return "";
	}
	
	public static String str2Html(String srcText) {
		if (isBlank(srcText)) {
			return "";
		}

		String strip = "";
		strip = "&";
		srcText = replace(srcText, strip, "&amp;");
		strip = "<";
		srcText = replace(srcText, strip, "&lt;");
		strip = ">";
		srcText = replace(srcText, strip, "&gt;");
		strip = "\n";
		srcText = replace(srcText, strip, "<br>");
		strip = "\"";
		srcText = replace(srcText, strip, "&quot;");
		strip = "'";
		srcText = replace(srcText, strip, "&apos;");
		strip = " ";
		srcText = replace(srcText, strip, "&nbsp;");

		return srcText;
	}
	
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
