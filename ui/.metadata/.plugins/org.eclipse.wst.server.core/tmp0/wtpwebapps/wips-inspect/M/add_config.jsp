<%@page import="air.wips.inspect.utils.StringUtil"%><%@
page import="air.wips.inspect.error.GoodBad"%><%@
page import="com.google.gson.Gson"%><%@
page import="sql.inspect.wips.air.Config"%><%@
page import="java.util.ArrayList"%><%@
page import="java.util.List"%><%@
page import="air.wips.inspect.conf.XmlAirConfTag"%><%@
page import="java.util.Enumeration"%><%@
page import="air.wips.inspect.conf.XmlAirConf"%><%@
page import="air.wips.inspect.servlet.HttpGet"%><%@
page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%><%


	int tagOffset = 0;
	List<XmlAirConf> shooterXmlAirConfList = new ArrayList<>();
	List<XmlAirConf> captureXmlAirConfList = new ArrayList<>();
	String configId = Config.newConfigId();
	String shooterXmlFilename = null;
	String captureXmlFilename = null;
	boolean edit = false;
	
	if (HttpGet.get(request, "config_id") != null) {
		configId = HttpGet.get(request, "config_id");
		edit = true;
	}
	
	try {
		HttpGet.debugRequestParameterValues(request);
		String [] target      = HttpGet.vals(request, "target");
		String [] tags        = HttpGet.vals(request, "tags");
		String [] enable      = HttpGet.vals(request, "enable");
		String [] desc        = HttpGet.vals(request, "desc");
		String [] dwell       = HttpGet.vals(request, "dwell");
		String [] channel     = HttpGet.vals(request, "channel");
		String [] frame       = HttpGet.vals(request, "frame");
		String [] ds          = HttpGet.vals(request, "ds");
		String [] address     = HttpGet.vals(request, "address");
		String [] addr1_da    = HttpGet.vals(request, "addr1_da");
		String [] addr2_da    = HttpGet.vals(request, "addr2_da");	
		String [] addr2_sa    = HttpGet.vals(request, "addr2_sa");
		String [] addr3_bssid = HttpGet.vals(request, "addr3_bssid");
		String [] addr3_da    = HttpGet.vals(request, "addr3_da");
		String [] addr3_sa    = HttpGet.vals(request, "addr3_sa");
		String [] addr4_da    = HttpGet.vals(request, "addr4_da");
		String [] addr4_sa    = HttpGet.vals(request, "addr4_sa");
		String [] addr4_ta    = HttpGet.vals(request, "addr4_ta");
		String [] addr4_ra    = HttpGet.vals(request, "addr4_ra");
		String [] ap          = HttpGet.vals(request, "ap");
		String [] sta         = HttpGet.vals(request, "sta");
		String [] any_addr    = HttpGet.vals(request, "any_addr");
		String [] tag_id      = HttpGet.vals(request, "tag_id");
		String [] tag_value   = HttpGet.vals(request, "tag_value");
		String [] tag_type    = HttpGet.vals(request, "tag_type");
		String [] tag_length  = HttpGet.vals(request, "tag_length");
		
		for (int i=0; i<target.length; i++) {
			XmlAirConf ac = new XmlAirConf();
			
			// set tag
			int tagCount = Integer.parseInt(tags[i]);
			if (tagCount > 0) {
				List<XmlAirConfTag> tagList = new ArrayList<>();
				for (int t=0; t<tagCount; t++) {		
					XmlAirConfTag tag = new XmlAirConfTag();
					int index = tagOffset + t;
					tag.setId(tag_id[index]);
					tag.setLen(tag_length[index]);
					tag.setData(tag_value[index]);
					tag.setType(tag_type[index]);
					tagList.add(tag);
				}
				ac.setTaglist(tagList);
				tagOffset += tagCount;
			}		
			
			// set info
			ac.setKey(i+"");
			if (!StringUtil.isNull(enable[i]))
				ac.setEnable(enable[i]);
			if (!StringUtil.isNull(desc[i]))
				ac.setDesc(desc[i]);
			if (!StringUtil.isNull(dwell[i]))
				ac.setDwell(dwell[i]);
			if (!StringUtil.isNull(channel[i]))
				ac.setChannel(channel[i]);
			if (!StringUtil.isNull(frame[i])) {
				ac.setType   (Byte.parseByte(frame[i].substring(0,1), 16)+"");
				ac.setSubtype(Byte.parseByte(frame[i].substring(1,2), 16)+"");
			}
			if (!StringUtil.isNull(ds[i])) {
				ac.setFromds(ds[i].substring(0, 1));
				ac.setTods  (ds[i].substring(1, 2));
			}
			if ("addr1".equals(address[i])) {
				ac.setAddrCount("1");
				ac.setDa(addr1_da[i]);
			} else if ("addr2".equals(address[i])) {
				ac.setAddrCount("2");
				ac.setDa(addr2_da[i]);
				ac.setSa(addr2_sa[i]);
			} else if ("addr3".equals(address[i])) {
				ac.setAddrCount("3");
				ac.setBssid(addr3_bssid[i]);
				ac.setDa(addr3_da[i]);
				ac.setSa(addr3_sa[i]);
			} else if ("addr4".equals(address[i])) {
				ac.setAddrCount("4");
				ac.setRa(addr4_ra[i]);
				ac.setTa(addr4_ta[i]);
				ac.setDa(addr4_da[i]);
				ac.setSa(addr4_sa[i]);
			} else if ("apsta".equals(address[i])) {
				ac.setAddrCount("0");
				ac.setAp(ap[i]);
				ac.setSta(sta[i]);
			} else if ("any_addr".equals(address[i])) {
				ac.setAddrCount("0");
				ac.setAnyAddr(any_addr[i]);
			}
			
			// shooter or capture
			if ("shooter".equals(target[i])) {
				shooterXmlAirConfList.add(ac);
				ac.setShooterFilename(configId);
				shooterXmlFilename = ac.getFilename();
			} else {
				captureXmlAirConfList.add(ac);
				ac.setCaptureFilename(configId);
				captureXmlFilename = ac.getFilename();
			}
		}
	}
	catch (Exception e) {
		response.getWriter().print(new Gson().toJson(new GoodBad().bad(e)));
	}
	
	try {
		if (shooterXmlFilename != null)
			XmlAirConf.writeAirConf(shooterXmlAirConfList);
		if (captureXmlFilename != null)
			XmlAirConf.writeAirConf(captureXmlAirConfList);
		
		Config c = new Config();
		c.setId(configId);
		c.setName(HttpGet.get(request, "config_name"));
		System.out.println(c.getName());
		if (shooterXmlFilename != null)
			c.setShooterxml(shooterXmlFilename);
		if (captureXmlFilename != null)
			c.setCapturexml(captureXmlFilename);
		
		if (edit) c.update();
		else      c.add();
		
		response.getWriter().print(new Gson().toJson(new GoodBad().good()));
	} catch (Exception e) {
		e.printStackTrace();
		XmlAirConf.deleteAirConf(configId);	
		response.getWriter().print(new Gson().toJson(new GoodBad().bad(e)));
	}
%>