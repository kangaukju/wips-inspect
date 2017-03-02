package air.wips.inspect.utils;

public class ArrayUtil {

	public static <E> boolean isNull(E[] array) {
		if (array == null || array.length == 0) {
			return true;
		}
		return false;
	}
	
	public static <E> E[] Null(E[] array) {
		if (isNull(array)) {
			return null;
		}
		return array;
	}
}
