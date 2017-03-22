<%@page import="sql.inspect.wips.air.History"%><%@
page import="air.wips.inspect.error.GoodBad"%><%@
page import="air.wips.inspect.servlet.HttpGet"%><%@
page import="com.google.gson.Gson"%><%@
page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%><%

	String profileId = HttpGet.get(request, "profile_id");
	String timestamp = HttpGet.get(request, "timestamp");
	boolean detail = HttpGet.getBoolean(request, "detail", false);
	
	//HttpGet.debugRequestParameters(request);
	
	try {
		if (profileId != null && timestamp != null) {
			response.getWriter().print(new Gson().toJson(History.get(profileId, timestamp, detail)));
		} else if (profileId != null) {
			response.getWriter().print(new Gson().toJson(History.get(profileId, detail)));
		} else {
			response.getWriter().print(new Gson().toJson(History.getAll(detail)));
		}
	} catch (Exception e) {
		e.printStackTrace();
		response.getWriter().print(new Gson().toJson(new GoodBad().bad(e)));
	}
%>