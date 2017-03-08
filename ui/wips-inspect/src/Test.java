import java.io.ByteArrayInputStream;
import java.io.FileOutputStream;
import java.io.FileWriter;

import air.wips.inspect.utils.ProcessUtil;

public class Test {

	public static void main(String[] args) throws Exception {
		
		
		
		String pid = "15899";
		
		String file = String.format("/proc/%s/comm", pid);

		Process process = ProcessUtil.processRun("cat", new String[] { file }, null, true, 1);
		
		System.out.println(ProcessUtil.getProcessInput(process));
		
		
		byte [] a;
		
		FileOutputStream fos =
	}

}
