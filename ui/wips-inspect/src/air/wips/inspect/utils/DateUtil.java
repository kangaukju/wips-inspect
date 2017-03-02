package air.wips.inspect.utils;

import java.text.SimpleDateFormat;
import java.util.Date;

public class DateUtil {	
	private static final SimpleDateFormat 
	_format_yyyyMMDDHHmmss = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
	
	private static final SimpleDateFormat 
	_format_HHmmss = new SimpleDateFormat("HH:mm:ss");

	public static final String yyyyMMDDHHmmss() {
		return _format_yyyyMMDDHHmmss.format(new Date());
	}
	
	public static final String HHmmss() {
		return _format_HHmmss.format(new Date());
	}
	
	public static final long timelong() {
		return new Date().getTime();
	}
	
	public static void main(String [] args) {
		System.out.println(yyyyMMDDHHmmss());
	}
}
