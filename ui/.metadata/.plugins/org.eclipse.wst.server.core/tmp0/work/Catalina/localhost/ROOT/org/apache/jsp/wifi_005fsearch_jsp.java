/*
 * Generated by the Jasper component of Apache Tomcat
 * Version: Apache Tomcat/9.0.0.M15
 * Generated at: 2017-03-22 00:45:57 UTC
 * Note: The last modified time of this file was set to
 *       the last modified time of the source file after
 *       generation to assist with modification tracking.
 */
package org.apache.jsp;

import javax.servlet.*;
import javax.servlet.http.*;
import javax.servlet.jsp.*;
import air.wips.inspect.log.D;
import java.util.Date;
import air.wips.inspect.Login.Admin;
import air.wips.inspect.servlet.HttpGet;

public final class wifi_005fsearch_jsp extends org.apache.jasper.runtime.HttpJspBase
    implements org.apache.jasper.runtime.JspSourceDependent,
                 org.apache.jasper.runtime.JspSourceImports {

  private static final javax.servlet.jsp.JspFactory _jspxFactory =
          javax.servlet.jsp.JspFactory.getDefaultFactory();

  private static java.util.Map<java.lang.String,java.lang.Long> _jspx_dependants;

  static {
    _jspx_dependants = new java.util.HashMap<java.lang.String,java.lang.Long>(2);
    _jspx_dependants.put("/footer.jsp", Long.valueOf(1489967953000L));
    _jspx_dependants.put("/header.jsp", Long.valueOf(1490143542000L));
  }

  private static final java.util.Set<java.lang.String> _jspx_imports_packages;

  private static final java.util.Set<java.lang.String> _jspx_imports_classes;

  static {
    _jspx_imports_packages = new java.util.HashSet<>();
    _jspx_imports_packages.add("javax.servlet");
    _jspx_imports_packages.add("javax.servlet.http");
    _jspx_imports_packages.add("javax.servlet.jsp");
    _jspx_imports_classes = new java.util.HashSet<>();
    _jspx_imports_classes.add("air.wips.inspect.log.D");
    _jspx_imports_classes.add("air.wips.inspect.Login.Admin");
    _jspx_imports_classes.add("java.util.Date");
    _jspx_imports_classes.add("air.wips.inspect.servlet.HttpGet");
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
      out.write("<!DOCTYPE html>\n");
      out.write("<html>\n");
      out.write("<head>\n");
      out.write("<meta charset=\"UTF-8\">\n");
      out.write("\n");
      out.write("\n");
      out.write("\n");
      out.write("\n");
      out.write("\n");
      out.write("\n");
      out.write("<head>\n");
      out.write("\t<meta charset=\"UTF-8\">\n");
      out.write("\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n");
      out.write("\t<title>Wips-Inspect</title>\n");
      out.write("\t<!-- jquery -->\n");
      out.write("\t<script type=\"text/javascript\" src=\"/js/jquery/1.11.1/jquery-1.11.1.min.js\"></script>\n");
      out.write("\t<link rel=\"stylesheet\" href=\"/js/jquery/ui/jquery-ui-1.12.1/jquery-ui.css\">\n");
      out.write("\t<script type=\"text/javascript\" src=\"/js/jquery/ui/jquery-ui-1.12.1/jquery-ui.js\"></script>\n");
      out.write("\t<!-- json2html -->\t\n");
      out.write("\t<script type=\"text/javascript\" src=\"/js/jquery.json2html.js\"></script>\n");
      out.write("\t<script type=\"text/javascript\" src=\"/js/json2html.js\"></script>\t\t\n");
      out.write("\t<!-- https://cdnjs.com/libraries/c3  -->\n");
      out.write("\t<link rel=\"stylesheet\" href=\"/js/c3/0.4.11/c3.min.css\"/>\t\n");
      out.write("\t<script type=\"text/javascript\" src=\"/js/d3/d3.v3.min.js\"></script>\n");
      out.write("\t<script type=\"text/javascript\" src=\"/js/c3/0.4.11/c3.min.js\"></script>\n");
      out.write("\t<!-- icheck -->\n");
      out.write("\t<script type=\"text/javascript\" src=\"/js/icheck/icheck.min.js\"></script>\n");
      out.write("\t<link rel=\"stylesheet\" href=\"/js/icheck/skins/all.css\">\n");
      out.write("\t<!-- tablesorter -->\n");
      out.write("\t<link rel=\"stylesheet\" href=\"/js/tablesorter-master/css/theme.default.css\">\n");
      out.write("\t<script type=\"text/javascript\" src=\"/js/tablesorter-master/js/jquery.tablesorter.js\"></script>\n");
      out.write("\t<script type=\"text/javascript\" src=\"/js/tablesorter-master/js/jquery.tablesorter.widgets.js\"></script>\n");
      out.write("\t<!-- loading -->\n");
      out.write("\t<script src=\"/js/loading/jquery.loading.min.js\"></script>\n");
      out.write("\t<link rel=\"stylesheet\" href=\"/js/loading/jquery.loading.min.css\">\n");
      out.write("\t<!-- animatedModal -->\n");
      out.write("\t<script src=\"/js/animatedModal/animatedModal.js\"></script>\n");
      out.write("\t<link rel=\"stylesheet\" href=\"/js/animatedModal/normalize.min.css\">\n");
      out.write("\t<link rel=\"stylesheet\" href=\"/js/animatedModal/animate.min.css\">\n");
      out.write("\t<!-- html2canvas -->\n");
      out.write("\t<script src=\"/js/html2canvas/html2canvas.js\"></script>\n");
      out.write("\t<!-- my js -->\n");
      out.write("\t<script src=\"/js/multiCharts.js\"></script>\n");
      out.write("\t<script src=\"/js/get_set.js\"></script>\n");
      out.write("\t<script src=\"/js/err.js\"></script>\n");
      out.write("\t<script src=\"/js/wifi_protocol.js\"></script>\t\n");
      out.write("\t<script src=\"/js/inspectChart.js\"></script>\n");
      out.write("\t");
      out.write("\n");
      out.write("\t<style type=\"text/css\">\n");
      out.write("\t* {\t\t\n");
      out.write("\t\tfont: 12px Arial, Sans-serif;\n");
      out.write("\t\tfont-weight: bold;\n");
      out.write("\t}\n");
      out.write("\tbody {\n");
      out.write("\t\tbackground: url(/img/login/bg.png) repeat;\n");
      out.write("\t\tfont-family: \"HelveticaNeue-Light\", \"Helvetica Neue Light\", \"Helvetica Neue\", Helvetica, Arial, \"Lucida Grande\", sans-serif;\n");
      out.write("\t\tfont-weight:300;\n");
      out.write("\t\ttext-align: left;\n");
      out.write("\t\ttext-decoration: none;\n");
      out.write("\t}\n");
      out.write("\ttable.tablesorter thead tr th {\n");
      out.write("\t\tbackground: #407bbf;\n");
      out.write("\t\tcolor: #fff;\n");
      out.write("\t}\n");
      out.write("\ttable.tablesorter_r thead tr th {\n");
      out.write("\t\tbackground: #ffa31a;\n");
      out.write("\t\tcolor: #fff;\n");
      out.write("\t}\n");
      out.write("\ttable.tablesorter thead tr, table.tablesorter tbody tr {\n");
      out.write("\t\tcursor: pointer;\n");
      out.write("\t\theight: 15px;\n");
      out.write("\t}\n");
      out.write("\ttable.tablesorter tbody tr.selected td {\n");
      out.write("   \tbackground: none repeat scroll 0 0 #ffa31a;\n");
      out.write("   \tcolor: #fff;\n");
      out.write("\t}\n");
      out.write("\ttable.tablesorter_r tbody tr.selected td {\n");
      out.write("   \tbackground: none repeat scroll 0 0 #407bbf;\n");
      out.write("   \tcolor: #fff;\n");
      out.write("\t}\n");
      out.write("\timg {\n");
      out.write("\t\tvertical-align: middle;\n");
      out.write("\t}\n");
      out.write("\tinput {\n");
      out.write("\t\tvertical-align: middle;\n");
      out.write("\t}\n");
      out.write("\tinput[type=text], input[type=password], select {\n");
      out.write("\t    width: 90%;\n");
      out.write("\t    padding: 3px 5px;\n");
      out.write("\t    display: inline-block;\n");
      out.write("\t    border: 1px solid #ccc;\n");
      out.write("\t    border-radius: 4px;\n");
      out.write("\t    box-sizing: border-box;\n");
      out.write("\t    background: #fff;\n");
      out.write("\t}\n");
      out.write("\t.login_img {\n");
      out.write("\t\twidth: 60px;\n");
      out.write("\t}\n");
      out.write("\t.head_img {\n");
      out.write("\t\twidth: 20px;\n");
      out.write("\t}\n");
      out.write("\t.action_img {\n");
      out.write("\t\tcursor: pointer;\n");
      out.write("\t\twidth: 30px;\n");
      out.write("\t}\n");
      out.write("\t.table_img {\n");
      out.write("\t\tpadding: 0px;\n");
      out.write("\t\tmargin: 0px;\n");
      out.write("\t\tcursor: pointer;\n");
      out.write("\t\twidth: 20px;\n");
      out.write("\t}\n");
      out.write("\t.index_background {\n");
      out.write("\t\tbackground-color: #407bbf;\n");
      out.write("\t}\t\n");
      out.write("\t.main_fieldset {\n");
      out.write("\t\tbackground: #ddd;\n");
      out.write("\t\tborder: 2px solid #407bbf;\n");
      out.write("\t\tborder-radius: 5px;\n");
      out.write("\t\tmargin-top: 10px;\n");
      out.write("\t\tmargin-bottom: 10px;\n");
      out.write("\t\tpadding: 10px;\n");
      out.write("\t}\n");
      out.write("\t.main_fieldset > legend {\n");
      out.write("\t\tbackground: #407bbf;\n");
      out.write("\t\tcolor: #fff;\n");
      out.write("\t\tpadding: 5px 10px ;\n");
      out.write("\t\tfont-size: 14px;\n");
      out.write("\t\tborder-radius: 5px;\n");
      out.write("\t\tbox-shadow: 0 0 0 5px #ddd;\n");
      out.write("\t\tmargin-left: 20px;\n");
      out.write("\t}\n");
      out.write("\t.main_fieldset > legend > span {\n");
      out.write("\t\tfont-size: 14px;\n");
      out.write("\t}\n");
      out.write("\t.sub_fieldset {\n");
      out.write("\t\tbackground: #ddd;\n");
      out.write("\t\tborder: 2px solid #666C73;\n");
      out.write("\t\tborder-radius: 5px;\n");
      out.write("\t\tmargin-top: 10px;\n");
      out.write("\t\tmargin-bottom: 10px;\n");
      out.write("\t\tpadding: 10px;\n");
      out.write("\t}\n");
      out.write("\t.sub_fieldset > legend {\n");
      out.write("\t\tbackground: #666C73;\n");
      out.write("\t\tcolor: #fff;\n");
      out.write("\t\tpadding: 5px 10px ;\n");
      out.write("\t\tfont-size: 15px;\n");
      out.write("\t\tborder-radius: 5px;\n");
      out.write("\t\tbox-shadow: 0 0 0 5px #ddd;\n");
      out.write("\t\tmargin-left: 20px;\n");
      out.write("\t}\n");
      out.write("\t.sub_fieldset > legend > span {\n");
      out.write("\t\tfont-size: 15px;\n");
      out.write("\t}\n");
      out.write("\t.main_fieldset_r {\n");
      out.write("\t\tbackground: #ddd;\n");
      out.write("\t\tborder: 2px solid #ffa31a;\n");
      out.write("\t\tborder-radius: 5px;\n");
      out.write("\t\tmargin-top: 10px;\n");
      out.write("\t\tmargin-bottom: 10px;\n");
      out.write("\t\tpadding: 10px;\n");
      out.write("\t}\n");
      out.write("\t.main_fieldset_r > legend {\n");
      out.write("\t\tbackground: #ffa31a;\n");
      out.write("    color: #fff;\n");
      out.write("    padding: 5px 10px ;\n");
      out.write("    font-size: 14px;\n");
      out.write("    border-radius: 5px;\n");
      out.write("    box-shadow: 0 0 0 5px #ddd;\n");
      out.write("    margin-left: 20px;\n");
      out.write("\t}\n");
      out.write("\t.main_fieldset_r > legend > span {\n");
      out.write("\t\tfont-size: 14px;\n");
      out.write("\t}\n");
      out.write("\t.sub_fieldset_r {\n");
      out.write("\t\tbackground: #ddd;\n");
      out.write("\t\tborder: 2px solid #666C73;\n");
      out.write("\t\tborder-radius: 5px;\n");
      out.write("\t\tmargin-top: 10px;\n");
      out.write("\t\tmargin-bottom: 10px;\n");
      out.write("\t\tpadding: 10px;\n");
      out.write("\t}\n");
      out.write("\t.sub_fieldset_r > legend {\n");
      out.write("\t\tbackground: #666C73;\n");
      out.write("\t\tcolor: #fff;\n");
      out.write("\t\tpadding: 5px 10px ;\n");
      out.write("\t\tfont-size: 15px;\n");
      out.write("\t\tborder-radius: 5px;\n");
      out.write("\t\tbox-shadow: 0 0 0 5px #ddd;\n");
      out.write("\t\tmargin-left: 20px;\n");
      out.write("\t}\n");
      out.write("\t.sub_fieldset_r > legend > span {\n");
      out.write("\t\tfont-size: 15px;\n");
      out.write("\t}\n");
      out.write("\t.title_outer {\n");
      out.write("\t\tcursor: pointer;\n");
      out.write("\t\tvertical-align: middle;\n");
      out.write("\t\ttext-align: center;\n");
      out.write("\t\tpadding: 5px;\n");
      out.write("\t}\n");
      out.write("\t.title_img {\t\t\n");
      out.write("\t\twidth: 30px;\n");
      out.write("\t}\n");
      out.write("\t.hidden {\n");
      out.write("\t\tdisplay: none;\n");
      out.write("\t}\n");
      out.write("\t.where_link_img {\n");
      out.write("\t\twidth: 20px;\n");
      out.write("\t}\n");
      out.write("\t.head_link_img {\n");
      out.write("\t\twidth: 25px;\n");
      out.write("\t\tcursor: pointer;\n");
      out.write("\t}\n");
      out.write("\t.icon_img {\n");
      out.write("\t\twidth: 25px;\n");
      out.write("\t\theight: 25px;\n");
      out.write("\t\tvertical-align: middle;\n");
      out.write("\t\tcursor: pointer;\n");
      out.write("\t}\n");
      out.write("\t.pop_error {\n");
      out.write("\t\tfont: 12px Arial, Sans-serif;\n");
      out.write("\t\tfont-weight: bold;\n");
      out.write("\t\tpadding: 20px;\n");
      out.write("\t\tbackground-color: #f44336; /* Red */\n");
      out.write("\t\tcolor: white;\n");
      out.write("\t\tmargin-bottom: 15px;\n");
      out.write("\t}\n");
      out.write("\t.pop_success {\n");
      out.write("\t\tfont: 12px Arial, Sans-serif;\n");
      out.write("\t\tfont-weight: bold;\n");
      out.write("\t\tpadding: 20px;\n");
      out.write("\t\tbackground-color: #4CAF50; /* Green */\n");
      out.write("\t\tcolor: white;\n");
      out.write("\t\tmargin-bottom: 15px;\n");
      out.write("\t}\n");
      out.write("\t.pop_info {\n");
      out.write("\t\tfont: 12px Arial, Sans-serif;\n");
      out.write("\t\tfont-weight: bold;\n");
      out.write("\t\tpadding: 20px;\n");
      out.write("\t\tbackground-color: #2196F3; /* Blue */\n");
      out.write("\t\tcolor: white;\n");
      out.write("\t\tmargin-bottom: 15px;\n");
      out.write("\t}\n");
      out.write("\t.pop_warning {\n");
      out.write("\t\tfont: 12px Arial, Sans-serif;\n");
      out.write("\t\tfont-weight: bold;\n");
      out.write("\t\tpadding: 20px;\n");
      out.write("\t\tbackground-color: #ff9800; /* Yellow */\n");
      out.write("\t\tcolor: white;\n");
      out.write("\t\tmargin-bottom: 15px;\n");
      out.write("\t}\n");
      out.write("\t</style>\n");
      out.write("\t<link rel=\"shortcut icon\" href=\"/img/wifi.ico\">\n");
      out.write("</head>\n");
      out.write("<body>\n");
      out.write("<table style=\"width: 100%;\">\n");
      out.write("\t<tr>\n");
      out.write("\t\t<td class=\"title_outer\">\n");
      out.write("\t\t\t<div align=\"left\">\n");
      out.write("\t\t\t\t<span class=\"title\">\n");
      out.write("\t\t\t\t\t<img class=\"title_img\" src=\"/img/wifi.svg\">&nbsp;&nbsp;<b>WIPS INSPECTOR</b>\n");
      out.write("\t\t\t\t</span>\t\t\t\t\n");
      out.write("\t\t\t\t<span id=\"where\"></span>\n");
      out.write("\t\t\t</div>\n");
      out.write("\t\t</td>\n");
      out.write("\t\t<td align=\"right\">\n");
      out.write("\t\t\t<img class=\"head_link_img home_link\" id=\"users\"        src=\"/img/users.png\">\n");
      out.write("\t\t\t<img class=\"head_link_img home_link\" id=\"inspect\"      src=\"/img/inspect.png\">\n");
      out.write("\t\t\t<img class=\"head_link_img home_link\" id=\"profile_list\" src=\"/img/profiles.png\">\n");
      out.write("\t\t\t<img class=\"head_link_img home_link\" id=\"config_list\"  src=\"/img/configs.png\">\n");
      out.write("\t\t\t<img class=\"head_link_img home_link\" id=\"history_list\" src=\"/img/database.png\">\n");
      out.write("\t\t\t<img class=\"head_link_img home_link\" id=\"wifi_search\"  src=\"/img/wifi_search.png\">\n");
      out.write("\t\t\t<img class=\"head_link_img home_link\" id=\"setting\"      src=\"/img/setting.png\">\n");
      out.write("\t\t</td>\n");
      out.write("\t</tr>\n");
      out.write("</table>\n");
      out.write("\n");
      out.write("<div id=\"custom-overlay\" style=\"text-align: center; vertical-align: middle;\">\n");
      out.write("\t<img src=\"/img/loading.svg\" style=\"width: 70px; height: 70px;\">\n");
      out.write("</div>\n");
      out.write("\n");

	Admin admin = null;
	String curPage = HttpGet.currentPage(request);
	if (!"index.jsp".equals(curPage) && !"".equals(curPage)) {
		if ((session == null) || (session.getAttribute("admin") == null)) {
			response.sendRedirect("index.jsp");
			return;
		}
		admin = (Admin)session.getAttribute("admin");
		long now = new Date().getTime();
		if ((now - admin.getSessionTimestamp()) > admin.getSessionTimeout()) {
			D.log("session timeout");
			response.sendRedirect("index.jsp");
			return;
		}
		admin.setSessionTimestamp(now);
	}

      out.write('\n');
      out.write('\n');
 boolean debug = HttpGet.sessionBoolean(request, "debug", true); 
      out.write("\n");
      out.write("<script type=\"text/javascript\">\n");
      out.write("var debug = ");
      out.print( debug );
      out.write(";\n");
      out.write("function CtrlAlt() {\n");
      out.write("\tif (event.keyCode == 17) { return false; }\n");
      out.write("\tif (event.keyCode == 18) { return false; }\n");
      out.write("\tif (event.keyCode == 91) { event.keyCode = 505; }\n");
      out.write("\tif (event.keyCode > 112 && event.keyCode < 123){ event.keyCode = 505; }\n");
      out.write("\tif (event.keyCode == 505) { return false; }\n");
      out.write("\tif(event.keyCode === 37 || event.keyCode === 38 || event.keyCode === 39 || event.keyCode === 40) {\n");
      out.write("\t\treturn false;\n");
      out.write("\t}\n");
      out.write("}\n");
      out.write("if (!debug) {\n");
      out.write("\tdocument.onkeydown = CtrlAlt;\n");
      out.write("\tdocument.onmousedown = CtrlAlt;\n");
      out.write("\t\n");
      out.write("\t$(window.document).ready(function(){\n");
      out.write("\t\t$(window.document).on(\"contextmenu\", function(event){return false;});\t//우클릭방지\n");
      out.write("\t\t$(window.document).on(\"selectstart\", function(event){return false;});\t//더블클릭을 통한 선택방지\n");
      out.write("\t\t$(window.document).on(\"dragstart\"\t, function(event){return false;});\t//드래그\n");
      out.write("\t});\n");
      out.write("\t\n");
      out.write("\t$(document).keydown(function (e) {\n");
      out.write("\t\t// F5, ctrl + F5, ctrl + r 새로고침 막기\n");
      out.write("\t\tvar allowPageList   = new Array('');\n");
      out.write("\t\tvar bBlockF5Key     = true;\n");
      out.write("\t\tfor (number in allowPageList) {\n");
      out.write("\t\t\tvar regExp = new RegExp('^' + allowPageList[number] + '.*', 'i');\n");
      out.write("\t\t\tif (regExp.test(document.location.pathname)) {\n");
      out.write("\t\t\t\tbBlockF5Key = false;\n");
      out.write("\t\t\t\tbreak;\n");
      out.write("\t\t\t}\n");
      out.write("\t\t}\n");
      out.write("\t     \n");
      out.write("\t\tif (bBlockF5Key) {\n");
      out.write("\t\t\tif (e.which === 116) {\n");
      out.write("\t\t\t\tif (typeof event == \"object\") {\n");
      out.write("\t\t\t\t\tevent.keyCode = 0;\n");
      out.write("\t\t\t\t}\n");
      out.write("\t\t\t\treturn false;\n");
      out.write("\t\t\t} else if (e.which === 82 && e.ctrlKey) {\n");
      out.write("\t\t\t\treturn false;\n");
      out.write("\t\t\t}\n");
      out.write("\t\t}\n");
      out.write("\t});\n");
      out.write("}\n");
      out.write("\n");
      out.write("$(\"body\").loading({\n");
      out.write("\ttheme: 'dark',\n");
      out.write("\toverlay: $(\"#custom-overlay\"),\n");
      out.write("\tonStart: function(loading) {\n");
      out.write("\t\tloading.overlay.fadeIn(10);\n");
      out.write("\t},\n");
      out.write("\tonStop: function(loading) {\n");
      out.write("\t\tloading.overlay.fadeOut(200);\n");
      out.write("\t}\n");
      out.write("});\n");
      out.write("\n");
      out.write("function load_ok() {\n");
      out.write("\t$(\"body\").loading('stop');\n");
      out.write("}\n");
      out.write("\n");
      out.write("/*\n");
      out.write("$(window).on('unload ',function() {\n");
      out.write("\tjQuery.ajax({\n");
      out.write("\t\turl: \"login_out.jsp\",\n");
      out.write("\t\ttype: \"POST\",\n");
      out.write("\t\tcache: false,\n");
      out.write("\t\tasync: false,\n");
      out.write("\t\tdataType: \"json\",\n");
      out.write("\t\tsuccess: function(result) {\n");
      out.write("\t\t\tgogo(\"/M\");\n");
      out.write("\t\t},\n");
      out.write("\t\terror: function(e) {\n");
      out.write("\t\t\tgogo(\"/M\");\n");
      out.write("\t\t}\n");
      out.write("\t});\n");
      out.write("});\n");
      out.write("*/\n");

String currentID 
	= request.getServletPath().substring(
			request.getServletPath().indexOf("/")+1, 
			request.getServletPath().lastIndexOf(".jsp"));

      out.write("\n");
      out.write("\n");
      out.write("$(function() {\n");
      out.write("\t$(\".title\").click(function() {\n");
      out.write("\t\tgogo(\"main.jsp\");\n");
      out.write("\t});\n");
      out.write("\t$(\".home_link\").click(function() {\t\t\n");
      out.write("\t\tgogo($(this).attr(\"id\")+\".jsp\");\t\t\n");
      out.write("\t});\n");
      out.write("\t\n");
      out.write("\t$(\"#where\").empty();\n");
      out.write("\tvar $currentID = $(\"#\"+\"");
      out.print(currentID);
      out.write("\");\n");
      out.write("\tif (_defined_($currentID)) {\n");
      out.write("\t\tif (_defined_($currentID.attr(\"src\"))) {\n");
      out.write("\t\t\t$(\"<span>\")\n");
      out.write("\t\t\t\t.html(\"&nbsp;&nbsp;>&nbsp;&nbsp;\")\n");
      out.write("\t\t\t\t.appendTo(\"#where\");\n");
      out.write("\t\t\t$(\"<img>\")\n");
      out.write("\t\t\t\t.attr(\"src\", $currentID.attr(\"src\"))\n");
      out.write("\t\t\t\t.addClass(\"where_link_img\")\t\t\t\t\n");
      out.write("\t\t\t\t.appendTo(\"#where\");\n");
      out.write("\t\t\tvar naming = $currentID.attr(\"id\").toUpperCase().replace(\"_\", \" \");\n");
      out.write("\t\t\t$(\"<span>\")\n");
      out.write("\t\t\t\t.html(\"&nbsp;\"+naming)\n");
      out.write("\t\t\t\t.appendTo(\"#where\");\n");
      out.write("\t\t}\n");
      out.write("\t}\n");
      out.write("});\n");
      out.write("\n");
      out.write("function _defined_(v) {\n");
      out.write("\treturn (typeof v != \"undefined\");\n");
      out.write("}\n");
      out.write("\n");
      out.write("function pop(msg, config) {\n");
      out.write("\tvar c = {};\n");
      out.write("\t\n");
      out.write("\tif (_defined_(config)) {\n");
      out.write("\t\tc = config;\n");
      out.write("\t}\n");
      out.write("\t\n");
      out.write("\tif (1) {\n");
      out.write("\t\tvar css = \"pop_error\";\n");
      out.write("\t\tif (_defined_(c.type)) {\n");
      out.write("\t\t\tif (c.type == \"info\") {\n");
      out.write("\t\t\t\tcss = \"pop_info\";\n");
      out.write("\t\t\t} else if (c.type == \"success\") {\n");
      out.write("\t\t\t\tcss = \"pop_success\";\n");
      out.write("\t\t\t} else if (c.type == \"warning\") {\n");
      out.write("\t\t\t\tcss = \"pop_warning\";\n");
      out.write("\t\t\t}\n");
      out.write("\t\t}\n");
      out.write("\t\tvar box_msg = \"<div class='\"+css+\"'>\"+msg+\"</div>\";\n");
      out.write("\t\t\n");
      out.write("\t\t$(\"body\").loading({\n");
      out.write("\t\t\ttheme: 'dark',\n");
      out.write("\t\t\tmessage: box_msg,\n");
      out.write("\t\t\tonStart: function(loading) {\n");
      out.write("\t\t\t\tloading.overlay.slideDown(350);\n");
      out.write("\t\t\t},\n");
      out.write("\t\t\tonClick: function(loading) {\n");
      out.write("\t\t\t\tloading.overlay.slideUp(250);\n");
      out.write("\t\t\t\tif (_defined_(c.focus)) {\n");
      out.write("\t\t\t\t\tc.focus.focus();\n");
      out.write("\t\t\t\t}\n");
      out.write("\t\t\t\tif (_defined_(c.page)) {\n");
      out.write("\t\t\t\t\tgogo(c.page);\n");
      out.write("\t\t\t\t}\n");
      out.write("\t\t\t}\n");
      out.write("\t\t});\n");
      out.write("\t\t$(\"body\").loading();\n");
      out.write("\t} else {\t\t\n");
      out.write("\t\talert(msg);\n");
      out.write("\t\tif (typeof $focus != \"undefined\") {\n");
      out.write("\t\t\t$focus.focus();\n");
      out.write("\t\t}\n");
      out.write("\t}\n");
      out.write("}\n");
      out.write("\n");
      out.write("function gogo(page) {\n");
      out.write("\t$(\"body\").animate({\n");
      out.write("\t\t\"opacity\": \"0\",\n");
      out.write("\t\t\"top\": \"10px\"\n");
      out.write("\t},400, function () {\n");
      out.write("\t\tlocation.href = page;\n");
      out.write("\t});\n");
      out.write("}\n");
      out.write("\n");
      out.write("function pN($this, n) {\n");
      out.write("\tvar p = $this;\n");
      out.write("\tfor (var i=0; i<n; i++) {\n");
      out.write("\t\tp = p.parent();\n");
      out.write("\t}\n");
      out.write("\treturn p;\n");
      out.write("}\n");
      out.write("</script>\n");
      out.write("</body>");
      out.write("\n");
      out.write("<style type=\"text/css\">\n");
      out.write("\n");
      out.write("</style>\n");
      out.write("</head>\n");
      out.write("<body>\n");
      out.write("\t<fieldset class=\"main_fieldset\">\n");
      out.write("\t\t<legend>\n");
      out.write("\t\t\t<img class=\"head_img\" src=\"/img/wifi_search.png\">\n");
      out.write("\t\t\t<span>Air Current</span>\n");
      out.write("\t\t</legend>\n");
      out.write("\t\t\n");
      out.write("\t\t<div>\n");
      out.write("\t\t\t<img class=\"action_img\" src=\"/img/refresh-outline.svg\">\n");
      out.write("\t\t\t<select id=\"wifi_search_timer\" style=\"width: 100px;\">\n");
      out.write("\t\t\t\t<option value=\"5\">5</option>\n");
      out.write("\t\t\t\t<option value=\"10\">10</option>\n");
      out.write("\t\t\t\t<option value=\"20\">20</option>\n");
      out.write("\t\t\t\t<option value=\"30\">30</option>\n");
      out.write("\t\t\t\t<option value=\"40\">40</option>\n");
      out.write("\t\t\t\t<option value=\"50\">50</option>\n");
      out.write("\t\t\t\t<option value=\"60\">60</option>\n");
      out.write("\t\t\t</select>\n");
      out.write("\t\t\t\n");
      out.write("\t\t\t<img class=\"table_img\" src=\"/img/apps.svg\" style=\"padding-left: 10px;\">\n");
      out.write("\t\t\t<select id=\"channel\" style=\"width: 120px;\">\n");
      out.write("\t\t\t</select>\n");
      out.write("\t\t\t<img class=\"action_img\" src=\"/img/play4.svg\" id=\"wifi_search_img\">\n");
      out.write("\t\t</div>\n");
      out.write("\t\t\n");
      out.write("\t\t<fieldset class=\"sub_fieldset\">\n");
      out.write("\t\t\t<legend>AP List</legend>\n");
      out.write("\t\t\t<table id=\"aplist_table\" class=\"wifi_search_table\">\n");
      out.write("\t\t\t\t<thead>\n");
      out.write("\t\t\t\t\t<tr>\n");
      out.write("\t\t\t\t\t\t<td>bssid</td>\n");
      out.write("\t\t\t\t\t\t<td>ch</td>\n");
      out.write("\t\t\t\t\t\t<td>ssid</td>\n");
      out.write("\t\t\t\t\t\t<td>proto</td>\n");
      out.write("\t\t\t\t\t\t<td>pwr</td>\n");
      out.write("\t\t\t\t\t\t<td>auth</td>\n");
      out.write("\t\t\t\t\t\t<td>enc</td>\n");
      out.write("\t\t\t\t\t\t<td>tinit</td>\n");
      out.write("\t\t\t\t\t\t<td>tlast</td>\n");
      out.write("\t\t\t\t\t</tr>\n");
      out.write("\t\t\t\t</thead>\n");
      out.write("\t\t\t\t<tbody>\n");
      out.write("\t\t\t\t</tbody>\n");
      out.write("\t\t\t</table>\n");
      out.write("\t\t</fieldset>\n");
      out.write("\t\t\n");
      out.write("\t\t<fieldset class=\"sub_fieldset\">\n");
      out.write("\t\t\t<legend>STA List</legend>\n");
      out.write("\t\t\t<table id=\"stlist_table\" class=\"wifi_search_table\">\n");
      out.write("\t\t\t\t<thead>\n");
      out.write("\t\t\t\t\t<tr>\n");
      out.write("\t\t\t\t\t\t<td>stamac</td>\n");
      out.write("\t\t\t\t\t\t<td>bssid</td>\n");
      out.write("\t\t\t\t\t\t<td>ch</td>\n");
      out.write("\t\t\t\t\t\t<td>pwr</td>\n");
      out.write("\t\t\t\t\t\t<td>rate</td>\n");
      out.write("\t\t\t\t\t\t<td>pkt</td>\n");
      out.write("\t\t\t\t\t\t<td>bytes</td>\n");
      out.write("\t\t\t\t\t\t<td>tinit</td>\n");
      out.write("\t\t\t\t\t\t<td>tlast</td>\n");
      out.write("\t\t\t\t\t</tr>\n");
      out.write("\t\t\t\t</thead>\n");
      out.write("\t\t\t\t<tbody>\n");
      out.write("\t\t\t\t</tbody>\n");
      out.write("\t\t\t</table>\n");
      out.write("\t\t</fieldset>\n");
      out.write("\t\t\n");
      out.write("\t</fieldset>\n");
      out.write("</body>\n");
      out.write("\n");
      out.write("<script type=\"text/javascript\">\n");
      out.write("var ap_list_table_xlat = {\n");
      out.write("\ttag: \"tr\",\n");
      out.write("\tchildren: [\t\t\n");
      out.write("\t\t{tag:\"td\", html:\"$\\{bssid\\}\"},\n");
      out.write("\t\t{tag:\"td\", html:\"$\\{ch\\}\"},\n");
      out.write("\t\t{tag:\"td\", html:\"$\\{ssid\\}\"},\n");
      out.write("\t\t{tag:\"td\", html:\"$\\{proto\\}\"},\n");
      out.write("\t\t{tag:\"td\", html:\"$\\{pwr\\}\"},\n");
      out.write("\t\t{tag:\"td\", html:\"$\\{auth\\}\"},\n");
      out.write("\t\t{tag:\"td\", html:\"$\\{enc\\}\"},\n");
      out.write("\t\t{tag:\"td\", html:\"$\\{tinit\\}\"},\n");
      out.write("\t\t{tag:\"td\", html:\"$\\{tlast\\}\"},\n");
      out.write("\t]\n");
      out.write("};\n");
      out.write("\n");
      out.write("var st_list_table_xlat = {\n");
      out.write("\ttag: \"tr\",\n");
      out.write("\tchildren: [\t\t\n");
      out.write("\t\t{tag:\"td\", html:\"$\\{stamac\\}\"},\n");
      out.write("\t\t{tag:\"td\", html:\"$\\{bssid\\}\"},\n");
      out.write("\t\t{tag:\"td\", html:\"$\\{ch\\}\"},\n");
      out.write("\t\t{tag:\"td\", html:\"$\\{pwr\\}\"},\n");
      out.write("\t\t{tag:\"td\", html:\"$\\{rate\\}\"},\n");
      out.write("\t\t{tag:\"td\", html:\"$\\{pkt\\}\"},\n");
      out.write("\t\t{tag:\"td\", html:\"$\\{bytes\\}\"},\n");
      out.write("\t\t{tag:\"td\", html:\"$\\{tinit\\}\"},\n");
      out.write("\t\t{tag:\"td\", html:\"$\\{tlast\\}\"},\n");
      out.write("\t]\n");
      out.write("};\n");
      out.write("\t\n");
      out.write("$(document).ready(function() {\n");
      out.write("\t\n");
      out.write("\tload_ok();\n");
      out.write("\t\n");
      out.write("\tvar $channel = $(\"#channel\");\n");
      out.write("\tvar all_channel = [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,36,37,38,39,40,41,42,43,44,45,46,47,48,52,53,54,55,56,57,58,59,60,64,100,104,108,112,116,120,124,128,132,136,140,149,150,152,153,157,161,165,160];\n");
      out.write("\t$.each(all_channel, function(i, v) {\n");
      out.write("\t\tvar name = v;\n");
      out.write("\t\tif (v == 0) {\n");
      out.write("\t\t\tname = \"All channel\";\n");
      out.write("\t\t}\n");
      out.write("\t\t$(\"<option></option>\").attr({\n");
      out.write("\t\t\tvalue: v,\n");
      out.write("\t\t}).html(name).appendTo($channel);\n");
      out.write("\t});\n");
      out.write("\t\n");
      out.write("\t$(\"#aplist_table\").tablesorter();\n");
      out.write("\t$(\"#stlist_table\").tablesorter();\n");
      out.write("\t\n");
      out.write("\tvar ws;\n");
      out.write("\tvar host = '");
      out.print(request.getHeader("host"));
      out.write("';\n");
      out.write("\t$(\"#wifi_search_img\").click(function() {\t\t\n");
      out.write("\t\tvar $img = $(this);\n");
      out.write("\t\tws_raise_error = false;\n");
      out.write("\t\tvar running = ($img.attr(\"src\") == \"/img/play4.svg\") ? false : true;\n");
      out.write("\t\tif (running) {\n");
      out.write("\t\t\tws.close();\n");
      out.write("\t\t\t$img.attr(\"src\", \"/img/play4.svg\");\n");
      out.write("\t\t} else {\n");
      out.write("\t\t\tws = new WebSocket(\"ws://\"+host+\"/wifi_search\");\n");
      out.write("\t\t\t/* onopen */\n");
      out.write("\t\t\tws.onopen = function(msg) {\n");
      out.write("\t\t\t\t$(\".wifi_search_table > tbody\").empty();\n");
      out.write("\t\t\t\t\n");
      out.write("\t\t\t\tws.send(JSON.stringify({\n");
      out.write("\t\t\t\t\tstate: \"start\",\n");
      out.write("\t\t\t\t\treportInterval: $(\"#wifi_search_timer\").val(),\n");
      out.write("\t\t\t\t\tchannel: $(\"#channel\").val(),\n");
      out.write("\t\t\t\t}));\n");
      out.write("\t\t\t\t$img.attr(\"src\", \"/img/stop4.svg\");\n");
      out.write("\t\t\t}\n");
      out.write("\t\t\t/* onmessage */\n");
      out.write("\t\t\tws.onmessage = function(msg) {\n");
      out.write("\t\t\t\tif (_isnull(msg.data)) {\n");
      out.write("\t\t\t\t\treturn;\n");
      out.write("\t\t\t\t}\n");
      out.write("\t\t\t\tvar result = JSON.parse(msg.data);\n");
      out.write("\t\t\t\tif (result.good == false) {\n");
      out.write("\t\t\t\t\tpop(\"Error running wifi search: \"+result.cause);\n");
      out.write("\t\t\t\t\t$img.attr(\"src\", \"/img/play4.svg\");\n");
      out.write("\t\t\t\t\tws_raise_error = true;\n");
      out.write("\t\t\t\t\tws.close();\n");
      out.write("\t\t\t\t\treturn;\n");
      out.write("\t\t\t\t}\n");
      out.write("\t\t\t\tif (result.good == true) {\n");
      out.write("\t\t\t\t\treturn;\n");
      out.write("\t\t\t\t}\n");
      out.write("\t\t\t\t\n");
      out.write("\t\t\t\tif (result.length != 2) {\n");
      out.write("\t\t\t\t\treturn;\n");
      out.write("\t\t\t\t}\n");
      out.write("\t\t\t\t\n");
      out.write("\t\t\t\t$(\".wifi_search_table > tbody\").empty();\n");
      out.write("\t\t\t\t\n");
      out.write("\t\t\t\tvar aplist = result[0];\n");
      out.write("\t\t\t\tvar stlist = result[1];\n");
      out.write("\t\t\t\t\n");
      out.write("\t\t\t\t$(\"#aplist_table > tbody\").html(json2html.transform(aplist, ap_list_table_xlat));\n");
      out.write("\t\t\t\t$(\"#stlist_table > tbody\").html(json2html.transform(stlist, st_list_table_xlat));\t\t\t\t\n");
      out.write("\t\t\t\t\n");
      out.write("\t\t\t\t$(\"#aplist_table\").tablesorter();\n");
      out.write("\t\t\t\t$(\"#stlist_table\").tablesorter();\n");
      out.write("\t\t\t\t\t\t\n");
      out.write("\t\t\t}\n");
      out.write("\t\t\t/* onerror */\n");
      out.write("\t\t\tws.onerror = function(msg) {\n");
      out.write("\t\t\t\t$img.attr(\"src\", \"/img/play4.svg\");\n");
      out.write("\t\t\t\tpop(\"Error running wifi search: \"+msg);\n");
      out.write("\t\t\t\tws_raise_error = true;\n");
      out.write("\t\t\t}\n");
      out.write("\t\t\t/* onerror */\n");
      out.write("\t\t\tws.onclose = function() {\n");
      out.write("\t\t\t\t$img.attr(\"src\", \"/img/play4.svg\");\n");
      out.write("\t\t\t\tif (ws_raise_error == false) {\n");
      out.write("\t\t\t\t\tpop(\"Finished wifi search\", {type:\"success\"});\n");
      out.write("\t\t\t\t}\n");
      out.write("\t\t\t}\n");
      out.write("\t\t}\n");
      out.write("\t});\n");
      out.write("});\n");
      out.write("</script>\n");
      out.write("\n");
      out.write("<script type=\"text/javascript\">\n");
      out.write("$(function() {\n");
      out.write("\t$(\".link\").click(function() {\n");
      out.write("\t\tvar param = \"\";\n");
      out.write("\t\tvar call = $(this).attr(\"call\");\n");
      out.write("\t\tif (call != undefined) {\n");
      out.write("\t\t\tparam = eval(call+\"()\")\n");
      out.write("\t\t\tparam = \"?\"+eval(call+\"()\");\n");
      out.write("\t\t}\n");
      out.write("\t\tvar page = $(this).attr(\"page\");\n");
      out.write("\t\tif (page != undefined) {\n");
      out.write("\t\t\t$(\"body\").animate({\n");
      out.write("\t\t\t\t\"opacity\": \"0\",\n");
      out.write("\t\t\t\t\"top\": \"10px\"\n");
      out.write("\t\t\t},400, function () {\n");
      out.write("\t\t\t\tlocation.href = page+param;\n");
      out.write("\t\t\t});\n");
      out.write("\t\t}\n");
      out.write("\t});\n");
      out.write("});\n");
      out.write("\n");
      out.write("</script>");
      out.write("\n");
      out.write("</html>");
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
