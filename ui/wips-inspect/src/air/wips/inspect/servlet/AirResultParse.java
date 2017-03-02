package air.wips.inspect.servlet;

import java.util.ArrayList;
import java.util.List;

import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import air.wips.inspect.protocol.AirProtocol;
import air.wips.inspect.result.AirCurrentAP;
import air.wips.inspect.result.AirCurrentST;

public class AirResultParse {
	
	static public String getNodeValue(NamedNodeMap attrs, final String name, final String def) {
		Node node = attrs.getNamedItem(name);
		if (node == null) {
			return def;
		}
		return node.getNodeValue();
	}
	
	static public List<AirCurrentST> getAircurrentST(Document doc) {
		AirCurrentST st;
		List<AirCurrentST> stlist = new ArrayList<>();
		
		NodeList apnode = doc.getElementsByTagName("sta");
		for (int i=0; i<apnode.getLength(); i++) {
			Node node = apnode.item(i);
			NamedNodeMap attrs = node.getAttributes();
			
			st = new AirCurrentST();
			st.setStamac(getNodeValue(attrs, "stamac", ""));
			st.setBssid(getNodeValue(attrs, "bssid", ""));
			st.setChannel(getNodeValue(attrs, "ch", ""));
			st.setPower(getNodeValue(attrs, "pwr", ""));
			st.setRate(getNodeValue(attrs, "rate", ""));
			st.setPktCount(getNodeValue(attrs, "pkt", ""));
			st.setDataBytes(getNodeValue(attrs, "bytes", ""));			
			String tinit = getNodeValue(attrs, "tinit", null);
			if (tinit != null) {
				st.setTinit(AirProtocol.getTime(tinit));
			}
			String tlast = getNodeValue(attrs, "tlast", null);
			if (tlast != null) {
				st.setTlast(AirProtocol.getTime(tlast));
			}
			stlist.add(st);
		}
		return stlist;
	}

	static public List<AirCurrentAP> getAircurrentAP(Document doc) {
		AirCurrentAP ap;
		List<AirCurrentAP> aplist = new ArrayList<>();
		
		NodeList apnode = doc.getElementsByTagName("ap");
		for (int i=0; i<apnode.getLength(); i++) {
			Node node = apnode.item(i);
			NamedNodeMap attrs = node.getAttributes();
			
			ap = new AirCurrentAP();
			ap.setBssid(getNodeValue(attrs, "bssid", ""));
			ap.setChannel(getNodeValue(attrs, "ch", ""));
			if ("-1".equals(ap.getChannel())) {
				ap.setChannel("");
			}
			ap.setSsid(getNodeValue(attrs, "ssid", ""));
			String proto = getNodeValue(attrs, "proto", null);
			if (proto != null) {
				ap.setProto(AirProtocol.getProto(proto));	
			}
			ap.setPower(getNodeValue(attrs, "pwr", ""));
			if ("-1".equals(ap.getPower())) {
				ap.setPower("");
			}
			String security = getNodeValue(attrs, "sec", null);
			if (security != null) {
				ap.setAuth(AirProtocol.getAUTH(security));
				ap.setEnc(AirProtocol.getENC(security));	
			}
			ap.setIp(getNodeValue(attrs, "ip", ""));
			String tinit = getNodeValue(attrs, "tinit", null);
			if (tinit != null) {
				ap.setTinit(AirProtocol.getTime(tinit));
			}
			String tlast = getNodeValue(attrs, "tlast", null);
			if (tlast != null) {
				ap.setTlast(AirProtocol.getTime(tlast));
			}
			aplist.add(ap);
		}
		return aplist;
	}
}