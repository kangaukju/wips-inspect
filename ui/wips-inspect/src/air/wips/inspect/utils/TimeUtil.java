package air.wips.inspect.utils;

import java.text.SimpleDateFormat;

public class TimeUtil {
	public static String getTime(final String format, long msec) {
		return new SimpleDateFormat(format).format(new java.util.Date(msec));
	}
}
