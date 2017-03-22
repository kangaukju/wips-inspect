package air.wips.inspect.result;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import air.wips.inspect.protocol.AirProtocol;
import air.wips.inspect.servlet.AirResultParse;
import air.wips.inspect.utils.StringUtil;

public class AirCaptureResult {
	private String profile;
	private String conf;
	private String key;
	private String xid;
	private String sec;
	private String usec;
	private String elapsed;
	private String pwr;
	private String type;
	private String subtype;
	private String addr1;
	private String addr2;
	private String addr3;
	private String addr4;
	private String ds;
	private String seq;
	private String deauthcode;
	private String frameTypeName;
		
	public static final String AIR_CAPTURE_KEY = "1";
	public static final String AIR_SHOOTER_KEY = "2";
	
	public static AirCaptureResult valueOf(String resultXml) throws Exception {
		AirCaptureResult rs = new AirCaptureResult();
		ByteArrayInputStream input = null;
		try {
			input = new ByteArrayInputStream(resultXml.getBytes());
			Document doc = DocumentBuilderFactory
					.newInstance()
					.newDocumentBuilder()
					.parse(input);
			
			NodeList resultnode = doc.getElementsByTagName("result");
			if (resultnode.getLength() == 1) {
				Node node = resultnode.item(0);
				NamedNodeMap attrs = node.getAttributes();
				
				rs.setProfile(AirResultParse.getNodeValue(attrs, "profile", ""));
				rs.setConf   (AirResultParse.getNodeValue(attrs, "conf", ""));
				rs.setKey    (AirResultParse.getNodeValue(attrs, "key", ""));
				rs.setXid    (AirResultParse.getNodeValue(attrs, "xid", ""));
				rs.setSec    (AirResultParse.getNodeValue(attrs, "sec", ""));
				rs.setUsec   (AirResultParse.getNodeValue(attrs, "usec", ""));
				rs.setElapsed(AirResultParse.getNodeValue(attrs, "elapsed", ""));
				rs.setPwr    (AirResultParse.getNodeValue(attrs, "pwr", ""));
				rs.setType   (AirResultParse.getNodeValue(attrs, "type", ""));
				rs.setSubtype(AirResultParse.getNodeValue(attrs, "subtype", ""));
				rs.setAddr1  (AirResultParse.getNodeValue(attrs, "addr1", ""));
				rs.setAddr2  (AirResultParse.getNodeValue(attrs, "addr2", ""));
				rs.setAddr3  (AirResultParse.getNodeValue(attrs, "addr3", ""));
				rs.setAddr4  (AirResultParse.getNodeValue(attrs, "addr4", ""));
				rs.setDs     (AirResultParse.getNodeValue(attrs, "ds", ""));
				rs.setSeq    (AirResultParse.getNodeValue(attrs, "seq", ""));
				rs.setFrameTypeName(AirProtocol.toFrameTypeName(rs.getType(), rs.getSubtype()));
				
				if (StringUtil.isNull(rs.getProfile())) {
					throw new Exception("empty profile of aircapture result");
				}
				if (StringUtil.isNull(rs.getConf())) {
					throw new Exception("empty config of aircapture result");
				}
				if (StringUtil.isNull(rs.getKey())) {
					throw new Exception("empty key of aircapture result");
				}
				if (StringUtil.isNull(rs.getXid())) {
					throw new Exception("empty xid of aircapture result");
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			if (input != null) try { input.close(); } catch (IOException e) {}
		}
		return rs;
	}
	
	public static List<AirCaptureResult> extractAirShooterResult(List<AirCaptureResult> list) {
		return extractAirResult(list, AIR_SHOOTER_KEY);
	}
	
	public static List<AirCaptureResult> extractAirCaptureResult(List<AirCaptureResult> list) {
		return extractAirResult(list, AIR_CAPTURE_KEY);
	}
	
	public static List<AirCaptureResult> extractAirResult(List<AirCaptureResult> list, String key) {
		List<AirCaptureResult> results = new ArrayList<>();
		for (AirCaptureResult r : list) {
			if (key.equals(r.getKey())) {
				results.add(r);
			}
		}
		return results;
	}
	
	public String getProfile() {
		return profile;
	}
	public void setProfile(String profile) {
		this.profile = profile;
	}
	public String getConf() {
		return conf;
	}
	public void setConf(String conf) {
		this.conf = conf;
	}
	public String getPwr() {
		return pwr;
	}
	public void setPwr(String pwr) {
		this.pwr = pwr;
	}
	public String getKey() {
		return key;
	}
	public void setKey(String key) {
		this.key = key;
	}
	public String getXid() {
		return xid;
	}
	public void setXid(String xid) {
		this.xid = xid;
	}
	public String getSec() {
		return sec;
	}
	public void setSec(String sec) {
		this.sec = sec;
	}
	public String getUsec() {
		return usec;
	}
	public void setUsec(String usec) {
		this.usec = usec;
	}
	public String getElapsed() {
		return elapsed;
	}
	public void setElapsed(String elapsed) {
		this.elapsed = elapsed;
	}
	public String getType() {
		return type;
	}
	public void setType(String type) {
		this.type = type;
	}
	public String getSubtype() {
		return subtype;
	}
	public void setSubtype(String subtype) {
		this.subtype = subtype;
	}
	public String getAddr1() {
		return addr1;
	}
	public void setAddr1(String addr1) {
		this.addr1 = addr1;
	}
	public String getAddr2() {
		return addr2;
	}
	public void setAddr2(String addr2) {
		this.addr2 = addr2;
	}
	public String getAddr3() {
		return addr3;
	}
	public void setAddr3(String addr3) {
		this.addr3 = addr3;
	}
	public String getAddr4() {
		return addr4;
	}
	public void setAddr4(String addr4) {
		this.addr4 = addr4;
	}
	public String getDs() {
		return ds;
	}
	public void setDs(String ds) {
		this.ds = ds;
	}
	public String getSeq() {
		return seq;
	}
	public void setSeq(String seq) {
		this.seq = seq;
	}
	public String getDeauthcode() {
		return deauthcode;
	}
	public void setDeauthcode(String deauthcode) {
		this.deauthcode = deauthcode;
	}
	public String getFrameTypeName() {
		return frameTypeName;
	}
	public void setFrameTypeName(String frameTypeName) {
		this.frameTypeName = frameTypeName;
	}
	@Override
	public String toString() {
		return "AirCaptureResult [profile=" + profile + ", conf=" + conf + ", key=" + key + ", xid=" + xid + ", sec="
				+ sec + ", usec=" + usec + ", elapsed=" + elapsed + ", pwr=" + pwr + ", type=" + type + ", subtype="
				+ subtype + ", addr1=" + addr1 + ", addr2=" + addr2 + ", addr3=" + addr3 + ", addr4=" + addr4 + ", ds="
				+ ds + ", seq=" + seq + ", deauthcode=" + deauthcode + ", frameTypeName=" + frameTypeName + "]";
	}
}
