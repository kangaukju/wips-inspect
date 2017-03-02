<%@page import="air.wips.inspect.error.GoodBad"%>
<%@page import="air.wips.inspect.osdep.Tools"%>
<%@page import="com.google.gson.Gson"%>
<%@page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%
	Tools.changeMonitorAll(true);
	try {
		response.getWriter().print(new Gson().toJson(Tools.getWifiInterface()));
	} catch (Exception e) {
		e.printStackTrace();
		response.getWriter().print(new Gson().toJson(new GoodBad().bad(e)));
	}
%>