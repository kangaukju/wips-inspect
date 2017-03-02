package air.wips.inspect.protocol;

import java.text.SimpleDateFormat;

public class AirProtocol {
	
	public static long STD_OPN  = 0x0001;
	public static long STD_WEP  = 0x0002;
	public static long STD_WPA  = 0x0004;
	public static long STD_WPA2 = 0x0008;
	public static long STD_FIELD = (STD_OPN | STD_WEP | STD_WPA | STD_WPA2);
	
	public static long ENC_WEP    = 0x0010;
	public static long ENC_TKIP   = 0x0020;
	public static long ENC_WRAP   = 0x0040;
	public static long ENC_CCMP   = 0x0080;
	public static long ENC_WEP40  = 0x1000;
	public static long ENC_WEP104 = 0x0100;
	public static long ENC_FIELD = (ENC_WEP | ENC_TKIP | ENC_WRAP | ENC_CCMP | ENC_WEP40 | ENC_WEP104);
	
	public static long AUTH_OPN = 0x0200;
	public static long AUTH_PSK = 0x0400;
	public static long AUTH_MGT = 0x0800;
	public static long AUTH_FIELD = (AUTH_OPN | AUTH_PSK | AUTH_MGT);
	
	public static long PROTO_80211a  = 0x01;
	public static long PROTO_80211b  = 0x02;
	public static long PROTO_80211g  = 0x04;
	public static long PROTO_80211n  = 0x05;
	public static long PROTO_80211ac = 0x10;
	
	
	public static final String getProto(String proto) {
		return getProto(Long.valueOf(proto));
	}
	public static final String getProto(long proto) {
		String media = "";
		
		if ((proto & PROTO_80211a) != 0) {
			if (!"".equals(media)) media += "/";
			media += "a";
		}
		if ((proto & PROTO_80211b) != 0) {
			if (!"".equals(media)) media += "/";
			media += "b";			
		}
		if ((proto & PROTO_80211g) != 0) {
			if (!"".equals(media)) media += "/";
			media += "g";
		}
		if ((proto & PROTO_80211n) != 0) {
			if (!"".equals(media)) media += "/";
			media += "n";
		}
		if ((proto & PROTO_80211ac) != 0) {
			if (!"".equals(media)) media += "/";
			media += "ac";
		}
		return media;
	}
	
	public static final String getAUTH(String security) {
		return getAUTH(Long.valueOf(security));
	}
	
	public static final String getAUTH(long security) {
		String sec = "";
		
		if ((security & ENC_FIELD) == 0) {
			sec = "";
		} else if ((security & STD_WPA2) != 0) {
			sec = "WPA2";
		} else if ((security & STD_WPA) != 0) {
			sec = "WPA";
		} else if ((security & STD_WEP) != 0) {
			sec = "WEP";
		} else if ((security & STD_OPN) != 0) {
			sec = "OPN";
		}
		return sec;
	}
	
	public static final String getENC(String security) {
		return getENC(Long.valueOf(security));
	}
	public static final String getENC(long security) {
		String enc = "";
		
		if ((security & AUTH_FIELD) == 0) {
			enc = "";
		} else if ((security & ENC_CCMP) != 0) {
			enc = "CCMP";
		} else if ((security & ENC_WRAP) != 0) {
			enc = "WRAP";
		} else if ((security & ENC_TKIP) != 0) {
			enc = "TKIP";
		} else if ((security & ENC_WEP104) != 0) {
			enc = "WEP104";
		} else if ((security & ENC_WEP40) != 0) {
			enc = "WEP40";
		} else if ((security & ENC_WEP) != 0) {
			enc = "WEP";
		}
		return enc;
	}
	
	private static final SimpleDateFormat _format_date_hour = new SimpleDateFormat("yyy-MM-DD HH:mm:ss");
	private static final SimpleDateFormat _format_date = new SimpleDateFormat("yyy-MM-DD");
	private static final SimpleDateFormat _format_hour = new SimpleDateFormat("HH:mm:ss");
	public static final String getTime(long time) {
		java.util.Date timestamp = new java.util.Date(time*1000);
		return _format_hour.format(timestamp);
	}
	public static final String getTime(String time) {
		return getTime(Long.valueOf(time));
	}
}
