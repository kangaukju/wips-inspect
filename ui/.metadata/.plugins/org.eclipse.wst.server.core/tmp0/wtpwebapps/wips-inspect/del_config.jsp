<%@page import="air.wips.inspect.error.GoodBad"%>
<%@page import="air.wips.inspect.servlet.HttpGet"%>
<%@page import="java.util.Enumeration"%>
<%@page import="com.google.gson.Gson"%>
<%@page import="sql.inspect.wips.air.Config"%>
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%
	String[] ids = HttpGet.gets(request, "capture_id", "shooter_id");
	String[] configId = HttpGet.gets(request, "config_id");

	try {
		for (String id : ids) {
			if (id != null) {
				Config.del(id);
				break;
			}
		}
		for (String id : configId) {
			if (id != null) {
				Config.del(id);
				break;
			}
		}
		response.getWriter().print(new Gson().toJson(new GoodBad().good()));
	} catch (Exception e) {
		e.printStackTrace();
		response.getWriter().print(new Gson().toJson(new GoodBad().bad(e)));
	}
%>