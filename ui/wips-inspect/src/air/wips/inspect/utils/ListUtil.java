package air.wips.inspect.utils;

import java.util.List;

public class ListUtil {
	
	public static <E> boolean isNull(List<E> list) {
		if (list == null || list.size() == 0) {
			return true;
		}
		return false;
	}
	
	public static <E> List<E> Null(List<E> list) {
		if (isNull(list)) {
			return null;
		}
		return list;
	}
}
