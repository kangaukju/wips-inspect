package air.wips.inspect.log;

import java.util.List;

public class D {

	private static boolean debug = true;
	
	public static void log(Object obj) {
		if (debug) {
			System.out.println(obj);
		}
	}
	
	public static void log(String msg, List<String> msgList) {		
		if (debug) {
			String str = msg;
			if (msgList == null) {
				System.out.println(str + " null");
				return;
			}
			for (String s : msgList) {
				str += " "+s;
			}
			System.out.println(str);
		}
	}
	
	public static void log(String msg, String[] msgList) {		
		if (debug) {
			String str = msg;
			if (msgList == null) {
				System.out.println(str + " null");
				return;
			}
			for (String s : msgList) {
				str += " "+s;
			}
			System.out.println(str);
		}
	}
		
	public static void log(Class clazz, String msg) {
		if (debug) {
			System.out.println("["+clazz.getSimpleName()+"] "+msg);
		}
	}
}
