/*
 * Generated by the Jasper component of Apache Tomcat
 * Version: Apache Tomcat/9.0.0.M15
 * Generated at: 2017-03-02 07:23:04 UTC
 * Note: The last modified time of this file was set to
 *       the last modified time of the source file after
 *       generation to assist with modification tracking.
 */
package org.apache.jsp.M;

import javax.servlet.*;
import javax.servlet.http.*;
import javax.servlet.jsp.*;
import air.wips.inspect.utils.StringUtil;
import air.wips.inspect.error.GoodBad;
import com.google.gson.Gson;
import sql.inspect.wips.air.Config;
import java.util.ArrayList;
import java.util.List;
import air.wips.inspect.conf.XmlAirConfTag;
import java.util.Enumeration;
import air.wips.inspect.conf.XmlAirConf;
import air.wips.inspect.servlet.HttpGet;

public final class add_005fconfig_005fr_jsp extends org.apache.jasper.runtime.HttpJspBase
    implements org.apache.jasper.runtime.JspSourceDependent,
                 org.apache.jasper.runtime.JspSourceImports {

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
    _jspx_imports_classes.add("java.util.List");
    _jspx_imports_classes.add("sql.inspect.wips.air.Config");
    _jspx_imports_classes.add("java.util.Enumeration");
    _jspx_imports_classes.add("com.google.gson.Gson");
    _jspx_imports_classes.add("air.wips.inspect.servlet.HttpGet");
    _jspx_imports_classes.add("air.wips.inspect.utils.StringUtil");
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

      out.write('\n');



	int tagOffset = 0;
	List<XmlAirConf> shooterXmlAirConfList = new ArrayList<>();
	List<XmlAirConf> captureXmlAirConfList = new ArrayList<>();
	String newConfigId = Config.newConfigId();
	String shooterXmlFilename = null;
	String captureXmlFilename = null;
	
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
				ac.setShooterFilename(newConfigId);
				shooterXmlFilename = ac.getFilename();
			} else {
				captureXmlAirConfList.add(ac);
				ac.setCaptureFilename(newConfigId);
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
		c.setId(newConfigId);
		c.setName(HttpGet.get(request, "config_name"));
		System.out.println(c.getName());
		if (shooterXmlFilename != null)
			c.setShooterxml(shooterXmlFilename);
		if (captureXmlFilename != null)
			c.setCapturexml(captureXmlFilename);
		c.add();
		response.getWriter().print(new Gson().toJson(new GoodBad().good()));
	} catch (Exception e) {
		e.printStackTrace();
		XmlAirConf.deleteAirConf(newConfigId);	
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
