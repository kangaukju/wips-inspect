/*
 * Generated by the Jasper component of Apache Tomcat
 * Version: Apache Tomcat/9.0.0.M15
 * Generated at: 2017-02-25 02:18:25 UTC
 * Note: The last modified time of this file was set to
 *       the last modified time of the source file after
 *       generation to assist with modification tracking.
 */
package org.apache.jsp;

import javax.servlet.*;
import javax.servlet.http.*;
import javax.servlet.jsp.*;
import air.wips.inspect.utils.ArrayUtil;
import java.util.ArrayList;
import java.util.List;
import air.wips.inspect.conf.XmlAirConfTag;
import air.wips.inspect.log.D;
import air.wips.inspect.error.GoodBad;
import air.wips.inspect.conf.XmlAirConf;
import air.wips.inspect.servlet.HttpGet;
import sql.inspect.wips.air.ConfigKey;
import java.util.Enumeration;
import com.google.gson.Gson;
import sql.inspect.wips.air.Config;

public final class set_005fconfig_jsp extends org.apache.jasper.runtime.HttpJspBase
    implements org.apache.jasper.runtime.JspSourceDependent,
                 org.apache.jasper.runtime.JspSourceImports {


public List<XmlAirConfTag> getTag(HttpServletRequest request, final String to) {
	String[] tag_id = HttpGet.vals(request, to+"_tag_id");
	String[] tag_len = HttpGet.vals(request, to+"_tag_len");
	String[] tag_data = HttpGet.vals(request, to+"_tag_data");
	String[] tag_type = HttpGet.vals(request, to+"_tag_type");
	
	List<XmlAirConfTag> list = new ArrayList<>();
	
	if (ArrayUtil.isNull(tag_id) || 
			ArrayUtil.isNull(tag_len) || 
			ArrayUtil.isNull(tag_data) || 
			ArrayUtil.isNull(tag_type)) {
		// empty list
		return list;
	}
	
	if (tag_id.length + tag_len.length - tag_data.length - tag_type.length != 0) {
		// empty list
		return list;
	}
	
	for (int i=0; i<tag_id.length; i++) {
		XmlAirConfTag tag = new XmlAirConfTag();
		tag.setId(tag_id[i]);
		tag.setLen(tag_len[i]);
		tag.setData(tag_data[i]);
		tag.setType(tag_type[i]);
		list.add(tag);
	}	
	return list;
}

  private static final javax.servlet.jsp.JspFactory _jspxFactory =
          javax.servlet.jsp.JspFactory.getDefaultFactory();

  private static java.util.Map<java.lang.String,java.lang.Long> _jspx_dependants;

  private static final java.util.Set<java.lang.String> _jspx_imports_packages;

  private static final java.util.Set<java.lang.String> _jspx_imports_classes;

  static {
    _jspx_imports_packages = new java.util.HashSet<>();
    _jspx_imports_packages.add("javax.servlet");
    _jspx_imports_packages.add("javax.servlet.http");
    _jspx_imports_packages.add("javax.servlet.jsp");
    _jspx_imports_classes = new java.util.HashSet<>();
    _jspx_imports_classes.add("air.wips.inspect.log.D");
    _jspx_imports_classes.add("java.util.List");
    _jspx_imports_classes.add("sql.inspect.wips.air.ConfigKey");
    _jspx_imports_classes.add("java.util.Enumeration");
    _jspx_imports_classes.add("sql.inspect.wips.air.Config");
    _jspx_imports_classes.add("com.google.gson.Gson");
    _jspx_imports_classes.add("air.wips.inspect.servlet.HttpGet");
    _jspx_imports_classes.add("air.wips.inspect.utils.ArrayUtil");
    _jspx_imports_classes.add("air.wips.inspect.conf.XmlAirConfTag");
    _jspx_imports_classes.add("air.wips.inspect.conf.XmlAirConf");
    _jspx_imports_classes.add("air.wips.inspect.error.GoodBad");
    _jspx_imports_classes.add("java.util.ArrayList");
  }

  private volatile javax.el.ExpressionFactory _el_expressionfactory;
  private volatile org.apache.tomcat.InstanceManager _jsp_instancemanager;

  public java.util.Map<java.lang.String,java.lang.Long> getDependants() {
    return _jspx_dependants;
  }

  public java.util.Set<java.lang.String> getPackageImports() {
    return _jspx_imports_packages;
  }

  public java.util.Set<java.lang.String> getClassImports() {
    return _jspx_imports_classes;
  }

  public javax.el.ExpressionFactory _jsp_getExpressionFactory() {
    if (_el_expressionfactory == null) {
      synchronized (this) {
        if (_el_expressionfactory == null) {
          _el_expressionfactory = _jspxFactory.getJspApplicationContext(getServletConfig().getServletContext()).getExpressionFactory();
        }
      }
    }
    return _el_expressionfactory;
  }

  public org.apache.tomcat.InstanceManager _jsp_getInstanceManager() {
    if (_jsp_instancemanager == null) {
      synchronized (this) {
        if (_jsp_instancemanager == null) {
          _jsp_instancemanager = org.apache.jasper.runtime.InstanceManagerFactory.getInstanceManager(getServletConfig());
        }
      }
    }
    return _jsp_instancemanager;
  }

  public void _jspInit() {
  }

  public void _jspDestroy() {
  }

  public void _jspService(final javax.servlet.http.HttpServletRequest request, final javax.servlet.http.HttpServletResponse response)
      throws java.io.IOException, javax.servlet.ServletException {

    final java.lang.String _jspx_method = request.getMethod();
    if (!"GET".equals(_jspx_method) && !"POST".equals(_jspx_method) && !"HEAD".equals(_jspx_method) && !javax.servlet.DispatcherType.ERROR.equals(request.getDispatcherType())) {
      response.sendError(HttpServletResponse.SC_METHOD_NOT_ALLOWED, "JSPs only permit GET POST or HEAD");
      return;
    }

    final javax.servlet.jsp.PageContext pageContext;
    javax.servlet.http.HttpSession session = null;
    final javax.servlet.ServletContext application;
    final javax.servlet.ServletConfig config;
    javax.servlet.jsp.JspWriter out = null;
    final java.lang.Object page = this;
    javax.servlet.jsp.JspWriter _jspx_out = null;
    javax.servlet.jsp.PageContext _jspx_page_context = null;


    try {
      response.setContentType("text/html; charset=UTF-8");
      pageContext = _jspxFactory.getPageContext(this, request, response,
      			null, true, 8192, true);
      _jspx_page_context = pageContext;
      application = pageContext.getServletContext();
      config = pageContext.getServletConfig();
      session = pageContext.getSession();
      out = pageContext.getOut();
      _jspx_out = out;

      out.write("\n");
      out.write("\n");
      out.write("\n");
      out.write("\n");
      out.write("\n");
      out.write("\n");
      out.write("\n");
      out.write("\n");
      out.write("\n");
      out.write("\n");
      out.write("\n");
      out.write("\n");
      out.write("\n");
      out.write('\n');

	//HttpGet.debugRequestParameters(request);

	String change_config_name = HttpGet.get(request, "change_config_name");
	String new_config_name = HttpGet.get(request, "new_config_name");
	String[] enable = HttpGet.gets(request, "capture_enable", "shooter_enable");
	String[] id = HttpGet.gets(request, "capture_id", "shooter_id");
	String[] xml = HttpGet.gets(request, "capture_xml", "shooter_xml");
	String[] desc = HttpGet.gets(request, "capture_desc", "shooter_desc");
//	String[] key = HttpGet.gets(request, "capture_key", "shooter_key");
	// TODO: fixed key - capture=1, shooter=2
	String[] key = { "1", "2" };
	String[] dwell = HttpGet.gets(request, "capture_dwell", "shooter_dwell");
//	String[] delay = HttpGet.gets(request, "capture_delay", "shooter_delay");
	String[] channel = HttpGet.gets(request, "capture_channel", "shooter_channel");
//	String[] type = HttpGet.gets(request, "capture_type", "shooter_type");
//	String[] subtype = HttpGet.gets(request, "capture_subtype", "shooter_subtype");
	String[] frametype = HttpGet.gets(request, "capture_frametype", "shooter_frametype");
	String[] ds = HttpGet.gets(request, "capture_ds", "shooter_ds");
	String[] address = HttpGet.gets(request, "capture_address", "shooter_address");	
	String[] addr1_da = HttpGet.gets(request, "capture_addr1_da", "shooter_addr1_da");
	String[] addr2_da = HttpGet.gets(request, "capture_addr2_da", "shooter_addr2_da");
	String[] addr2_sa = HttpGet.gets(request, "capture_addr2_sa", "shooter_addr2_sa");
	String[] addr3_bssid = HttpGet.gets(request, "capture_addr3_bssid", "shooter_addr3_bssid");
	String[] addr3_da = HttpGet.gets(request, "capture_addr3_da", "shooter_addr3_da");
	String[] addr3_sa = HttpGet.gets(request, "capture_addr3_sa", "shooter_addr3_sa");	
	String[] addr4_da = HttpGet.gets(request, "capture_addr4_da", "shooter_addr4_da");
	String[] addr4_sa = HttpGet.gets(request, "capture_addr4_sa", "shooter_addr4_sa");
	String[] addr4_ta = HttpGet.gets(request, "capture_addr4_ta", "shooter_addr4_ta");
	String[] addr4_ra = HttpGet.gets(request, "capture_addr4_ra", "shooter_addr4_ra");
	String[] any_addr = HttpGet.gets(request, "capture_any_addr", "shooter_any_addr");
	String[] ap = HttpGet.gets(request, "capture_ap", "shooter_ap");
	String[] sta = HttpGet.gets(request, "capture_sta", "shooter_sta");
	List<XmlAirConfTag> capture_taglist = getTag(request, "capture");
	List<XmlAirConfTag> shooter_taglist = getTag(request, "shooter");
	
	XmlAirConf[] ac = new XmlAirConf[2];
	for (int i=0; i<2; i++) {
		ac[i] = new XmlAirConf();
		
		ac[i].setEnable(enable[i]);
		ac[i].setDesc(desc[i]);
		ac[i].setFilename(xml[i]);
		ac[i].setKey(key[i]);
		ac[i].setDwell(dwell[i]);
//		ac[i].setDelay(delay[i]);
		ac[i].setChannel(channel[i]);
//		ac[i].setType(type[i]);
//		ac[i].setSubtype(subtype[i]);
		if (frametype[i] != null) {			
			ac[i].setType(   Byte.parseByte(frametype[i].substring(0,1), 16)+"");
			ac[i].setSubtype(Byte.parseByte(frametype[i].substring(1,2), 16)+"");
		}
		if (ds[i] != null) {
			ac[i].setFromds(ds[i].substring(0, 1));
			ac[i].setTods(  ds[i].substring(1, 2));
		}
		if ("addr1".equals(address[i])) {
			ac[i].setAddrCount("1");
			ac[i].setDa(addr1_da[i]);
		} else if ("addr2".equals(address[i])) {
			ac[i].setAddrCount("2");
			ac[i].setDa(addr2_da[i]);
			ac[i].setSa(addr2_sa[i]);
		} else if ("addr3".equals(address[i])) {
			ac[i].setAddrCount("3");
			ac[i].setBssid(addr3_bssid[i]);
			ac[i].setDa(addr3_da[i]);
			ac[i].setSa(addr3_sa[i]);
		} else if ("addr4".equals(address[i])) {
			ac[i].setAddrCount("4");
			ac[i].setRa(addr4_ra[i]);
			ac[i].setTa(addr4_ta[i]);
			ac[i].setDa(addr4_da[i]);
			ac[i].setSa(addr4_sa[i]);
		} else if ("apsta".equals(address[i])) {
			ac[i].setAddrCount("0");
			ac[i].setAp(ap[i]);
			ac[i].setSta(sta[i]);
		} else if ("any_addr".equals(address[i])) {
			ac[i].setAddrCount("0");
			ac[i].setAnyAddr(any_addr[i]);
		}
	}

	Config c = new Config();
	try {
		if (new_config_name != null) {
			D.log("new config = "+new_config_name);
			String newId = Config.newConfigId();
			// ConfigKey.newKey()
			ac[0].setKey("1");
			ac[1].setKey("2");
			ac[0].setFilename("capture_"+newId);
			ac[1].setFilename("shooter_"+newId);
			
			ac[0].setTaglist(capture_taglist);
			ac[1].setTaglist(shooter_taglist);
			
			try {
				ac[0].writeAirConf();
				ac[1].writeAirConf();
			} catch (Exception e) {
				XmlAirConf.deleteAirConf(newId);
				throw e;
			}
			
			c.setId(String.valueOf(newId));
			c.setName(new_config_name);
			c.setCapturexml(ac[0].getFilename());
			c.setShooterxml(ac[1].getFilename());
			c.add();
		} else {
			// update config
			D.log("update config");
			D.log(ac[0]);
			
			ac[0].setTaglist(capture_taglist);
			ac[1].setTaglist(shooter_taglist);
			
			ac[0].writeAirConf();
			ac[1].writeAirConf();
			
			c.setId(id[0]);
			if (change_config_name != null) {
				c.setName(change_config_name);
			}
			c.setCapturexml(ac[0].getFilename());
			c.setShooterxml(ac[1].getFilename());
			c.update();
		}
		response.getWriter().print(new Gson().toJson(new GoodBad().good()));
	} catch (Exception e) {
		e.printStackTrace();
		response.getWriter().print(new Gson().toJson(new GoodBad().bad(e)));
	}

    } catch (java.lang.Throwable t) {
      if (!(t instanceof javax.servlet.jsp.SkipPageException)){
        out = _jspx_out;
        if (out != null && out.getBufferSize() != 0)
          try {
            if (response.isCommitted()) {
              out.flush();
            } else {
              out.clearBuffer();
            }
          } catch (java.io.IOException e) {}
        if (_jspx_page_context != null) _jspx_page_context.handlePageException(t);
        else throw new ServletException(t);
      }
    } finally {
      _jspxFactory.releasePageContext(_jspx_page_context);
    }
  }
}