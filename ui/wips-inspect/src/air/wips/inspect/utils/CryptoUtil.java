package air.wips.inspect.utils;

import java.security.MessageDigest;

public class CryptoUtil {

	public static String sha512(final String text) {
		StringBuffer sb = new StringBuffer();
		try {
			MessageDigest md = MessageDigest.getInstance("SHA-256");
			md.update(text.getBytes());
			byte [] hash = md.digest();			
			for(int i = 0 ; i < hash.length ; i++){
				sb.append(toHex(hash[i]));
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return sb.toString();
	}	
	
	public static String toHex(byte data) {
		return Integer.toString((data&0xff) + 0x100, 16).substring(1);
	}
	public static void main(String []args) {
		String out = CryptoUtil.sha512("qwe123");
		
		System.out.println(out);
	}
}
