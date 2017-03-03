package air.wips.inspect.servlet;

import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;

import javax.servlet.http.HttpServletRequest;

import air.wips.inspect.utils.StringUtil;

public class HttpGet {
	
	public static List<String> valList(HttpServletRequest request, final String name) {
		List<String> list = new ArrayList<>();
		String[] vals = request.getParameterValues(name);
		if (vals == null) {
			return list;
		}
		for (String p : vals) {
			if (!StringUtil.isNull(p)) {
				list.add(p);
			}
		}
		return list;
	}
	
	public static String[] vals(HttpServletRequest request, final String name) {
		String[] vals = request.getParameterValues(name);
		if (vals == null) {
			return null;
		}
		for (int i=0; i<vals.length; i++) {
			vals[i] = StringUtil.Null(vals[i]);
		}
		return vals;
	}
	
	public static String get(HttpServletRequest request, final String name) {
		return StringUtil.Null(request.getParameter(name));
	}
	
	public static String get(HttpServletRequest request, final String name, String def) {
		String v = request.getParameter(name);
		if (StringUtil.isNull(v)) {
			return def;
		}
		return v;
	}
	
	public static boolean getBoolean(HttpServletRequest request, final String name, boolean def) {
		String value = request.getParameter(name);
		if (StringUtil.isNull(value)) {
			return def;
		}
		if ("true".equalsIgnoreCase(value) || "yes".equalsIgnoreCase(value)) {
			return true;
		}
		else if ("false".equalsIgnoreCase(value) || "no".equalsIgnoreCase(value)) {
			return false;
		} else {
			return def;
		}
	}

	public static String[] gets(HttpServletRequest request, String... names) {
		int i=0;
		String [] results = new String[names.length];
		for (String n : names) {
			results[i] = StringUtil.Null(request.getParameter(n));
//			System.out.println(n+"="+results[i]);
			i++;
		}
		return results;
	}
	
	public static String[] getsWithPrefix(HttpServletRequest request, String name, String... prefixs) {
		int i=0;
		String [] results = new String[prefixs.length];
		for (String p : prefixs) {
			results[i++] = StringUtil.Null(request.getParameter(p+"_"+name));
		}
		return results;
	}
	
	public static void debugRequestParameterValues(HttpServletRequest request) {
		Enumeration<String> e = request.getParameterNames();
		String key;
		while (e.hasMoreElements()) {
			key = e.nextElement();
			String [] vals = request.getParameterValues(key);
			if (vals != null) {
				int i = 0;
				for (String val : vals) {
					System.out.println("{"+i+"} ["+key+"]=["+val+"]");
					i++;
				}
			}
		}
	}
	
	public static void debugRequestParameters(HttpServletRequest request) {
		Enumeration<String> e = request.getParameterNames();
		String key, val;
		while (e.hasMoreElements()) {
			key = e.nextElement();
			val = request.getParameter(key);
			System.out.println("["+key+"]=["+val+"]");
		}
	}
}
