<%@page import="air.wips.inspect.log.D"%>
<%@page import="java.util.List"%>
<%@page import="sql.inspect.wips.air.Profile"%>
<%@page import="air.wips.inspect.error.GoodBad"%>
<%@page import="air.wips.inspect.servlet.HttpGet"%>
<%@page import="java.util.Enumeration"%>
<%@page import="com.google.gson.Gson"%>
<%@page import="sql.inspect.wips.air.Config"%>
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%
	String profileId = HttpGet.get(request, "profile_id");
	String configId = HttpGet.get(request, "config_id");
	
	try {
		if (configId == null) {
			Profile.del(profileId);
		}
		else if (profileId != null && configId != null) {
			Profile.removeConfig(profileId, configId);
		}
		response.getWriter().print(new Gson().toJson(new GoodBad().good()));
	} catch (Exception e) {
		e.printStackTrace();
		response.getWriter().print(new Gson().toJson(new GoodBad().bad(e)));
	}
%>