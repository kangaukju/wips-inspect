<%@page import="air.wips.inspect.error.GoodBad"%><%@
page import="air.wips.inspect.servlet.HttpGet"%><%@
page import="java.util.Enumeration"%><%@
page import="com.google.gson.Gson"%><%@
page import="sql.inspect.wips.air.Config"%><%@
page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%><%
	boolean selected = HttpGet.getBoolean(request, "selected", false);
	String profileId = HttpGet.get(request, "profile_id");
	boolean detail = HttpGet.getBoolean(request, "detail", false);
	
	try {
		if (profileId == null) {
			response.getWriter().print(new Gson().toJson(Config.getAll(detail)));
		} else {
			response.getWriter().print(new Gson().toJson(Config.getSelectedProfile(profileId, selected, detail)));	
		}
	} catch (Exception e) {
		e.printStackTrace();
		response.getWriter().print(new Gson().toJson(new GoodBad().bad(e)));
	}
%>