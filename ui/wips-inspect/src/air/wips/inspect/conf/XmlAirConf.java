package air.wips.inspect.conf;

import java.io.File;
import java.io.FileOutputStream;
import java.util.ArrayList;
import java.util.List;

import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import air.wips.inspect.log.D;
import air.wips.inspect.utils.FileUtil;
import air.wips.inspect.utils.ListUtil;
import air.wips.inspect.utils.StringUtil;
import air.wips.inspect.utils.Validation;

public class XmlAirConf {
	private String enable;
	private String filename;
	private String seq;
	private String channel;
	private String dwell;
	private String desc;
	private String magicKey;
	private String key;
	private String version;
	private String type;
	private String subtype;
	private String tods;
	private String fromds;
	private String protect;
	private String pwrSleep;
	private String duration;
	private String deauthReason;
	private String dataSize;
	private String dataSign;
	private String count;
	private String addrCount;
	private String bssid;
	private String da;
	private String sa;
	private String ra;
	private String ta;
	private String anyAddr;
	private String ap;
	private String sta;
	private String delay;
	private List<XmlAirConfTag> taglist;
	
	static class AttrPair {
		String name;
		String value;
		public AttrPair(String name, String value) {
			this.name = name;
			this.value = value;
		}
	}

	private static Element appendEl(Document doc, Element parent, String elname) {
		Element e = doc.createElement(elname);
		parent.appendChild(e);
		return e;
	}
	
	private static Element appendElTxt(Document doc, Element parent, String elname, String txt) {
		if (StringUtil.isNull(txt)) {
			return null;
		}
		Element e = doc.createElement(elname);
		e.appendChild(doc.createTextNode(txt));
		parent.appendChild(e);
		return e;
	}

	private static Element appendElTxtAttrs(Document doc, Element parent, String elname, AttrPair[] attrPairs) {
		for (AttrPair pair : attrPairs) {
			if (StringUtil.isNull(pair.name)) {
				return null;
			}
			if (StringUtil.isNull(pair.value)) {
				return null;
			}
		}		
		Element e = doc.createElement(elname);
		for (AttrPair pair : attrPairs) {			
			e.setAttribute(pair.name, pair.value);
		}
		parent.appendChild(e);
		return e;
	}
	
	static public void deleteAirConf(String id) throws Exception {
		File path = new File(XmlAirConfEnv.path());
		if (!path.exists())
			return;
				
		for (File sub: path.listFiles()) {
			if (sub.isFile()) {
				if (sub.getName().matches(".*"+id+".*")) {
					sub.delete();
				}
			}
		}
	}

	public void writeAirConf() throws Exception {
		List<XmlAirConf> one = new ArrayList<>();
		one.add(this);
		intenalWriteAirConf(one);
	}
	
	static public void writeAirConf(XmlAirConf ac) throws Exception {
		List<XmlAirConf> one = new ArrayList<>();
		one.add(ac);
		intenalWriteAirConf(one);		
	}
	
	static public void writeAirConf(List<XmlAirConf> aclist) throws Exception {
		intenalWriteAirConf(aclist);
	}
	
	synchronized static private void intenalWriteAirConf(List<XmlAirConf> aclist) throws Exception {
		if (aclist == null || aclist.size() == 0) {
			return;
		}
		
		Document doc = DocumentBuilderFactory.
				newInstance().
				newDocumentBuilder().
				newDocument();
		
		Element list = doc.createElement("list");
		doc.appendChild(list);
		
		// make airconf xml element
		for (XmlAirConf ac : aclist) {
			list.appendChild(newElementAirConf(doc, ac));
		}

		Transformer transformer = TransformerFactory.newInstance().newTransformer();
		transformer.setOutputProperty(OutputKeys.ENCODING, "UTF-8");
		transformer.setOutputProperty(OutputKeys.INDENT, "yes");
		transformer.setOutputProperty("{http://xml.apache.org/xslt}indent-amount", "2");

		String filename = XmlAirConfEnv.path(aclist.get(0).filename);
		DOMSource source = new DOMSource(doc);
		StreamResult result = new StreamResult(new FileOutputStream(new File(filename)));
		transformer.transform(source, result);		
		D.log("write airconf xml to "+filename);

		/*
		 * DOMSource source = new DOMSource(doc); StreamResult result = new
		 * StreamResult(new File(filename)); System.out.println(source);
		 * System.out.println(result);
		 * TransformerFactory.newInstance().newTransformer().transform(source,
		 * result);
		 */
	}
	
	synchronized static private Element newElementAirConf(Document doc, XmlAirConf ac) throws Exception {
		Element airconf = doc.createElement("airconf");
		
		appendElTxt(doc, airconf, "enable", ac.enable);
		appendElTxt(doc, airconf, "desc", ac.desc);
		appendElTxt(doc, airconf, "type", ac.type);
		appendElTxt(doc, airconf, "subtype", ac.subtype);
		appendElTxtAttrs(doc, airconf, "ds", 
				new AttrPair[] { 
						new AttrPair("tods", ac.tods), 
						new AttrPair("fromds", ac.fromds),
				}
		);
		appendElTxt(doc, airconf, "key", ac.key);
		appendElTxt(doc, airconf, "dwell", ac.dwell);
		appendElTxt(doc, airconf, "delay", ac.delay);
		appendElTxt(doc, airconf, "channel", ac.channel);
		
		if (StringUtil.isNull(ac.addrCount) || "0".equals(ac.addrCount)) {
			if (!StringUtil.isNull(ac.anyAddr)) {
				if (!Validation.macaddress(ac.anyAddr)) {
					throw new Exception("malformed <any_addr>"+ac.anyAddr+"</any_addr>");	
				}
				appendElTxt(doc, airconf, "any_addr", ac.anyAddr);
			}
			else {
				if (!StringUtil.isNull(ac.ap)) {
					if (!Validation.macaddress(ac.ap)) {
						throw new Exception("malformed <ap>"+ac.ap+"</ap>");	
					}
					appendElTxt(doc, airconf, "ap", ac.ap);
				}
				if (!StringUtil.isNull(ac.sta)) {
					if (!Validation.macaddress(ac.sta)) {
						throw new Exception("malformed <sta>"+ac.sta+"</sta>");	
					}
					appendElTxt(doc, airconf, "sta", ac.sta);
				}
			}
		} else {
			Element addr = appendElTxtAttrs(doc, airconf, "addr", 
					new AttrPair[] { new AttrPair("count", ac.getAddrCount()), }
			);
			
			int addrCount = StringUtil.Int(ac.addrCount);
			switch (addrCount) {
			case 1: {
				if (!Validation.macaddress(ac.da)) {
					throw new Exception("malformed <addr count='"+addrCount+"'> <da>"+ac.da+"</da>");	
				}
				Element e = doc.createElement("da");
				e.setTextContent(ac.da);
				addr.appendChild(e);
				break;
			}
			case 2: {
				if (!Validation.macaddress(ac.da)) {
					throw new Exception("malformed <addr count='"+addrCount+"'> <da>"+ac.da+"</da>");	
				}
				if (!Validation.macaddress(ac.sa)) {
					throw new Exception("malformed <addr count='"+addrCount+"'> <sa>"+ac.sa+"</sa>");	
				}
				Element [] es = {
						doc.createElement("da"),
						doc.createElement("sa")};				
				es[0].setTextContent(ac.da);
				es[1].setTextContent(ac.sa);
				for (Element e : es) {
					addr.appendChild(e);
				}
				break;
			}
			case 3: {
				if (!Validation.macaddress(ac.bssid)) {
					throw new Exception("malformed <addr count='"+addrCount+"'> <bssid>"+ac.bssid+"</bssid>");	
				}
				if (!Validation.macaddress(ac.da)) {
					throw new Exception("malformed <addr count='"+addrCount+"'> <da>"+ac.da+"</da>");	
				}
				if (!Validation.macaddress(ac.sa)) {
					throw new Exception("malformed <addr count='"+addrCount+"'> <sa>"+ac.sa+"</sa>");	
				}
				Element [] es = {
						doc.createElement("bssid"),
						doc.createElement("da"),
						doc.createElement("sa")};
				es[0].setTextContent(ac.bssid);
				es[1].setTextContent(ac.da);
				es[2].setTextContent(ac.sa);
				for (Element e : es) {
					addr.appendChild(e);
				}
				break;
			}
			case 4: {
				if (!Validation.macaddress(ac.ra)) {
					throw new Exception("malformed <addr count='"+addrCount+"'> <ra>"+ac.ra+"</ra>");	
				}
				if (!Validation.macaddress(ac.ta)) {
					throw new Exception("malformed <addr count='"+addrCount+"'> <ta>"+ac.ta+"</ta>");	
				}
				if (!Validation.macaddress(ac.da)) {
					throw new Exception("malformed <addr count='"+addrCount+"'> <da>"+ac.da+"</da>");	
				}
				if (!Validation.macaddress(ac.sa)) {
					throw new Exception("malformed <addr count='"+addrCount+"'> <sa>"+ac.sa+"</sa>");	
				}
				Element [] es = {
						doc.createElement("ra"),
						doc.createElement("ta"),
						doc.createElement("da"),
						doc.createElement("sa")};
				es[0].setTextContent(ac.ra);
				es[1].setTextContent(ac.ta);
				es[2].setTextContent(ac.da);
				es[3].setTextContent(ac.sa);
				for (Element e : es) {
					addr.appendChild(e);
				}
				break;
			}
			default: 
				throw new Exception("Not support addr count="+ac.addrCount);
			}
		}
		
		if (!ListUtil.isNull(ac.getTaglist())) {
			Element taglist = appendEl(doc, airconf, "taglist");
			
			for (XmlAirConfTag tag : ac.getTaglist()) {
				appendElTxtAttrs(doc, taglist, "tag", 
						new AttrPair[] {
								new AttrPair("id", tag.getId()),
								new AttrPair("len", tag.getLen()),
								new AttrPair("type", tag.getType()),
								new AttrPair("data", tag.getData()),
								}
				);
			}
		}
		return airconf;
	}
		
	private String getNodeValue(Node parent, String tagName) {
		NodeList nodeList = parent.getChildNodes();
		for (int i=0; i<nodeList.getLength(); i++) {
			if (nodeList.item(i).getNodeType() == Node.ELEMENT_NODE) {
				Element e = (Element)nodeList.item(i);
				if (e.getTagName().equals(tagName)) {
					return e.getTextContent();
				}
			}
		}
		return null;
	}
	
	private void setAirConfAddr(int addrCount, Node nodeAddr) throws Exception {
		switch (addrCount) {
		case 0:
			break;
		case 1:
			da = getNodeValue(nodeAddr, "da");
			if (!Validation.macaddress(da)) {
				throw new Exception("malformed <addr count='"+addrCount+"'> <da>"+da+"</da>");
			}
			break;
		case 2:
			da = getNodeValue(nodeAddr, "da");
			sa = getNodeValue(nodeAddr, "sa");
			if (!Validation.macaddress(da)) {
				throw new Exception("malformed <addr count='"+addrCount+"'> <da>"+da+"</da>");
			}
			if (!Validation.macaddress(sa)) {
				throw new Exception("malformed <addr count='"+addrCount+"'> <sa>"+sa+"</sa>");
			}
			break;
		case 3:
			bssid = getNodeValue(nodeAddr, "bssid");
			da = getNodeValue(nodeAddr, "da");
			sa = getNodeValue(nodeAddr, "sa");
			if (!Validation.macaddress(bssid)) {
				throw new Exception("malformed <addr count='"+addrCount+"'> <bssid>"+bssid+"</bssid>");
			}
			if (!Validation.macaddress(da)) {
				throw new Exception("malformed <addr count='"+addrCount+"'> <da>"+da+"</da>");
			}
			if (!Validation.macaddress(sa)) {
				throw new Exception("malformed <addr count='"+addrCount+"'> <sa>"+sa+"</sa>");
			}
			break;
		case 4:
			da = getNodeValue(nodeAddr, "da");
			sa = getNodeValue(nodeAddr, "sa");
			ra = getNodeValue(nodeAddr, "ra");
			ta = getNodeValue(nodeAddr, "ta");
			if (!Validation.macaddress(da)) {
				throw new Exception("malformed <addr count='"+addrCount+"'> <da>"+da+"</da>");
			}
			if (!Validation.macaddress(sa)) {
				throw new Exception("malformed <addr count='"+addrCount+"'> <sa>"+sa+"</sa>");
			}
			if (!Validation.macaddress(da)) {
				throw new Exception("malformed <addr count='"+addrCount+"'> <ra>"+ra+"</ra>");
			}
			if (!Validation.macaddress(sa)) {
				throw new Exception("malformed <addr count='"+addrCount+"'> <ta>"+ta+"</ta>");
			}
			break;
		default:
			throw new Exception("malformed <addr count='"+addrCount+"'>");
		}
	}
	
	public static List<XmlAirConfTag> loadAirConfTag(Node node) throws Exception {
		List<XmlAirConfTag> tagList = new ArrayList<>();
		NodeList nodelist = node.getChildNodes();
		for (int i=0; i<nodelist.getLength(); i++) {
			if (nodelist.item(i).getNodeType() == Node.ELEMENT_NODE) {
				Element e = (Element)nodelist.item(i);
				
				XmlAirConfTag tag = new XmlAirConfTag();				
				tag.setId(e.getAttribute("id"));
				tag.setLen(e.getAttribute("len"));
				tag.setType(e.getAttribute("type"));
				tag.setData(e.getAttribute("data"));
				
				if (StringUtil.isNull(tag.getId())) {
					throw new Exception("malformed <tag id=''/>");	
				}
				if (StringUtil.isNull(tag.getLen())) {
					throw new Exception("malformed <tag len=''/>");
				}
				if (StringUtil.isNull(tag.getData())) {
					throw new Exception("malformed <tag data=''/>");
				}
				if (StringUtil.isNull(tag.getType())) {
					tag.setType(XmlAirConfTag.TYPE_STRING);
				} else if (XmlAirConfTag.TYPE_HEX.equals(tag.getType())) {
					tag.setType(XmlAirConfTag.TYPE_HEX);
				} else if (XmlAirConfTag.TYPE_STRING.equals(tag.getType())) {
					tag.setType(XmlAirConfTag.TYPE_STRING);
				} else {
					throw new Exception("malformed <tag type='"+tag.getType()+"'/>");
				}
				tagList.add(tag);
			}
		}
		return tagList;
	}
	
	public static XmlAirConf loadAirConf(String filename, Node node) throws Exception {
		XmlAirConf airConf = new XmlAirConf();
		NodeList nodelist = node.getChildNodes();
		
		airConf.filename = filename;
		for (int i=0; i<nodelist.getLength(); i++) {
			if (nodelist.item(i).getNodeType() == Node.ELEMENT_NODE) {
				Element e = (Element)nodelist.item(i);
				String t = e.getTagName();
				String v = e.getTextContent();
				
				if ("enable".equals(t)) {
					airConf.enable = v;
				} else if ("seq".equals(t)) {
					airConf.seq = v;
				} else if ("channel".equals(t)) {
					airConf.channel = v;
				} else if ("magic_key".equals(t)) {
					airConf.magicKey = v;
				} else if ("dwell".equals(t)) {
					airConf.dwell = v;
				} else if ("key".equals(t)) {
					airConf.key = v;
				} else if ("desc".equals(t)) {
					airConf.desc = v;
				} else if ("version".equals(t)) {
					airConf.version = v;
				} else if ("type".equals(t)) {
					airConf.type = v;
				} else if ("subtype".equals(t)) {
					airConf.subtype = v;
				} else if ("ds".equals(t)) {
					airConf.tods = e.getAttribute("tods");
					airConf.fromds = e.getAttribute("fromds");
					
					if (!"0".equals(airConf.tods) && 
						!"1".equals(airConf.tods) && 
						!"0".equals(airConf.fromds) && 
						!"1".equals(airConf.fromds)) {
						throw new Exception(
							"malformed <ds tods='"+airConf.tods+"' fromds='"+airConf.fromds+"'");
					}					
				} else if ("addr".equals(t)) {
					airConf.addrCount = e.getAttribute("count");
					airConf.setAirConfAddr(StringUtil.Int(airConf.addrCount), e);
				} else if ("protect".equals(t)) {
					airConf.protect = v;
				} else if ("pwr_sleep".equals(t)) {
					airConf.pwrSleep = v;
				} else if ("duration".equals(t)) {
					airConf.duration = v;
				} else if ("delay".equals(t)) {
					airConf.delay = v;
				} else if ("deauth_reason".equals(t)) {
					airConf.deauthReason = v;
				} else if ("data_size".equals(t)) {
					airConf.dataSize = v;
				} else if ("data_sign".equals(t)) {
					airConf.dataSign = v;
				} else if ("any_addr".equals(t)) {
					airConf.anyAddr = v;
					if (!Validation.macaddress(airConf.anyAddr)) {
						throw new Exception(filename+": malformed <any_addr>"+airConf.anyAddr+"</any_addr>");
					}
				} else if ("ap".equals(t)) {
					airConf.ap = v;
					if (!Validation.macaddress(airConf.ap)) {
						throw new Exception(filename+": malformed <ap>"+airConf.ap+"</ap>");
					}
				} else if ("sta".equals(t)) {
					airConf.sta = v;
					if (!Validation.macaddress(airConf.sta)) {
						throw new Exception(filename+": malformed <sta>"+airConf.sta+"</sta>");
					}
				} else if ("taglist".equals(t)) {
					try {
						if (airConf.taglist == null) {
							airConf.taglist = loadAirConfTag(e);
						} else {
							airConf.taglist.addAll(loadAirConfTag(e));
						}
					} catch (Exception ex) {
						throw new Exception(filename+": "+ex.getMessage());
					}
				}
			}
		}
		if (StringUtil.isNull(airConf.enable)) {
			airConf.enable = "1";
		}
		if (StringUtil.isNull(airConf.key)) {
			throw new Exception(filename+": malformed <key> tag not found");
		}
		return airConf;
	}
	
	public static XmlAirConf loadAirConf(String filename) throws Exception {
		List<XmlAirConf> list = loadAirConfList(filename);
		if (ListUtil.isNull(list)) {
			return null;
		}
		return list.get(0);
	}
	
	synchronized public static List<XmlAirConf> loadAirConfList(String filename) throws Exception {
		return loadAirConfList(XmlAirConfEnv.path(), filename);
	}
	
	synchronized public static List<XmlAirConf> loadAirConfList(String directory, String filename) throws Exception {
		List<XmlAirConf> airConfList = new ArrayList<>();
		String path;
		try {
			if (StringUtil.isNull(filename)) {
				return null;
			}
			//path = XmlAirConfEnv.path(filename);
			path = directory + "/" + filename;
			File airConfFile = new File(path);
			if (!airConfFile.exists()) {
				throw new Exception("airconf file not found - "+path);
			}
			Document doc = DocumentBuilderFactory.
					newInstance().
					newDocumentBuilder().
					parse(airConfFile);
			NodeList listNode = doc.getElementsByTagName("list");
			if (listNode.getLength() != 1) {
				throw new Exception("malformed airconf file <list> - "+filename);
			}
			
			NodeList airConfNodeList = listNode.item(0).getChildNodes();
			Node airConfNode;
			for (int i=0; i<airConfNodeList.getLength(); i++) {
				airConfNode = airConfNodeList.item(i);
				if (airConfNode.getNodeType() == Node.ELEMENT_NODE) {
					XmlAirConf conf = loadAirConf(filename, airConfNode);
					airConfList.add(conf);
				}
			}
		} catch (Exception e) {
			throw e;
		}
		return airConfList;
	}
	
	public static String getXML(final String filename) {
		return FileUtil.getContent(XmlAirConfEnv.path(filename));
	}
	
	public static String captureFilename(String configId) {
		return "capture_"+configId;
	}
	
	public static String shooterFilename(String configId) {
		return "shooter_"+configId;
	}
	
	public void setCaptureFilename(String configId) {
		this.filename = "capture_"+configId;
	}
	
	public void setShooterFilename(String configId) {
		this.filename = "shooter_"+configId;
	}

	@Override
	public String toString() {
		return "XmlAirConf [enable=" + enable + ", filename=" + filename + ", seq=" + seq + ", channel=" + channel
				+ ", dwell=" + dwell + ", desc=" + desc + ", magicKey=" + magicKey + ", key=" + key + ", version="
				+ version + ", type=" + type + ", subtype=" + subtype + ", tods=" + tods + ", fromds=" + fromds
				+ ", protect=" + protect + ", pwrSleep=" + pwrSleep + ", duration=" + duration + ", deauthReason="
				+ deauthReason + ", dataSize=" + dataSize + ", dataSign=" + dataSign + ", count=" + count
				+ ", addrCount=" + addrCount + ", bssid=" + bssid + ", da=" + da + ", sa=" + sa + ", ra=" + ra + ", ta="
				+ ta + ", anyAddr=" + anyAddr + ", ap=" + ap + ", sta=" + sta + ", delay=" + delay + ", taglist="
				+ taglist + "]";
	}

	public String getFilename() {
		return filename;
	}

	public void setFilename(String filename) {
		this.filename = filename;
	}

	public String getSeq() {
		return seq;
	}

	public void setSeq(String seq) {
		this.seq = seq;
	}

	public String getChannel() {
		return channel;
	}

	public void setChannel(String channel) {
		this.channel = channel;
	}

	public String getDwell() {
		return dwell;
	}

	public void setDwell(String dwell) {
		this.dwell = dwell;
	}

	public String getDesc() {
		return desc;
	}

	public void setDesc(String desc) {
		this.desc = desc;
	}

	public String getMagicKey() {
		return magicKey;
	}

	public void setMagicKey(String magicKey) {
		this.magicKey = magicKey;
	}

	public String getKey() {
		return key;
	}

	public void setKey(String key) {
		this.key = key;
	}

	public String getVersion() {
		return version;
	}

	public void setVersion(String version) {
		this.version = version;
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

	public String getTods() {
		return tods;
	}

	public void setTods(String tods) {
		this.tods = tods;
	}

	public String getFromds() {
		return fromds;
	}

	public void setFromds(String fromds) {
		this.fromds = fromds;
	}

	public String getProtect() {
		return protect;
	}

	public void setProtect(String protect) {
		this.protect = protect;
	}

	public String getPwrSleep() {
		return pwrSleep;
	}

	public void setPwrSleep(String pwrSleep) {
		this.pwrSleep = pwrSleep;
	}

	public String getDuration() {
		return duration;
	}

	public void setDuration(String duration) {
		this.duration = duration;
	}

	public String getDeauthReason() {
		return deauthReason;
	}

	public void setDeauthReason(String deauthReason) {
		this.deauthReason = deauthReason;
	}

	public String getDataSize() {
		return dataSize;
	}

	public void setDataSize(String dataSize) {
		this.dataSize = dataSize;
	}

	public String getDataSign() {
		return dataSign;
	}

	public void setDataSign(String dataSign) {
		this.dataSign = dataSign;
	}

	public String getCount() {
		return count;
	}

	public void setCount(String count) {
		this.count = count;
	}

	public String getAddrCount() {
		return addrCount;
	}

	public void setAddrCount(String addrCount) {
		this.addrCount = addrCount;
	}

	public String getBssid() {
		return bssid;
	}

	public void setBssid(String bssid) {
		this.bssid = bssid;
	}

	public String getDa() {
		return da;
	}

	public void setDa(String da) {
		this.da = da;
	}

	public String getSa() {
		return sa;
	}

	public void setSa(String sa) {
		this.sa = sa;
	}

	public String getRa() {
		return ra;
	}

	public void setRa(String ra) {
		this.ra = ra;
	}

	public String getTa() {
		return ta;
	}

	public void setTa(String ta) {
		this.ta = ta;
	}

	public String getAp() {
		return ap;
	}

	public void setAp(String ap) {
		this.ap = ap;
	}

	public String getSta() {
		return sta;
	}

	public void setSta(String sta) {
		this.sta = sta;
	}

	public String getAnyAddr() {
		return anyAddr;
	}

	public void setAnyAddr(String anyAddr) {
		this.anyAddr = anyAddr;
	}

	public String getDelay() {
		return delay;
	}

	public void setDelay(String delay) {
		this.delay = delay;
	}

	public List<XmlAirConfTag> getTaglist() {
		return taglist;
	}

	public void setTaglist(List<XmlAirConfTag> taglist) {
		this.taglist = taglist;
	}

	public String getEnable() {
		return enable;
	}

	public void setEnable(String enable) {
		this.enable = enable;
	}

	public static void main(String[] args) throws Exception {
		/*
		List<AirConf> list = AirConf.loadAirConf("/home/kinow/git/wips-inspect/src/send_conf/beacon_test/bs.xml");
		for (AirConf a : list) {
			System.out.println(a);
		}
		*/
		//String json = "{'seq':'1', 'channel':'2', 'da':'00:11:22:33:44:55', 'taglist':[ {id:0, len:4, data:'qwe123'} ]  }";
		//AirConf a = new AirConf(json);
		
		XmlAirConf a = new XmlAirConf();
		a.fromds = "0";
		a.tods = "1";
		/*
		a.addr_count = "3";
		a.da = "11:22:33:44:55:66";
		a.sa = "33:44:55:66:77:88";
		a.bssid = "22:33:44:55:66:77";
		*/
		a.ap = "11:22:33:44:55:66";
		a.filename = "kaka.conf";
		
		a.writeAirConf();
		
	}
	
	
	
}