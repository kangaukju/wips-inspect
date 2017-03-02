<%@page import="air.wips.inspect.servlet.HttpGet"%>
<%@page import="air.wips.inspect.error.GoodBad"%>
<%@page import="air.wips.inspect.conf.XmlAirConf"%>
<%@page import="air.wips.inspect.utils.StringUtil"%>
<%@page import="com.google.gson.Gson"%>
<%@page import="sql.inspect.wips.air.Config"%>
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%
	String shooterId = HttpGet.get(request, "shooter_id");
	String captureId = HttpGet.get(request, "capture_id");
	String filename = HttpGet.get(request, "filename");
	XmlAirConf ac;
	try {
		if (shooterId != null) {
			Config c = Config.getById(shooterId);
			if (c != null) {
				ac = XmlAirConf.loadAirConf(c.getShooterxml());
				if (ac != null) {
					response.getWriter().print(new Gson().toJson(ac));
				}
			}
		}
		else if (captureId != null) {
			Config c = Config.getById(captureId);
			if (c != null) {
				ac = XmlAirConf.loadAirConf(c.getCapturexml());
				if (ac != null) {
					response.getWriter().print(new Gson().toJson(ac));
				}
			}
		}
		else if (filename != null) {
			ac = XmlAirConf.loadAirConf(filename);
			response.getWriter().print(new Gson().toJson(ac));
		}
	} catch (Exception e) {
		e.printStackTrace();
		response.getWriter().print(new Gson().toJson(new GoodBad().bad(e)));
	}
%>