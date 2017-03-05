package air.wips.inspect.utils;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;

public class FileUtil {

	
	public static String getContent(final String filename) {
		File f = new File(filename);
		if (!f.exists()) {
			return "";
		}
		String line = null;
		BufferedReader br = null;
		StringBuffer sb = new StringBuffer();
		try {
			br = new BufferedReader(new FileReader(f));
			while ((line = br.readLine()) != null) {
				sb.append(line);
				sb.append("\n");
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				if (br != null) { br.close(); }
			} catch (Exception e) {}
		}
		return sb.toString();
	}
}
