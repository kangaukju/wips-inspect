<%@page import="air.wips.inspect.log.D"%><%@
page import="air.wips.inspect.error.GoodBad"%><%@
page import="com.google.gson.Gson"%><%@
page import="air.wips.inspect.Login.Admin"%><%@
page import="air.wips.inspect.servlet.HttpGet"%><%@
page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%><%

	String username = HttpGet.get(request, "username");
	String password = HttpGet.get(request, "password");
	
	HttpGet.debugRequestParameters(request);
	
	try {
		session.setAttribute(
				"admin",
				Admin.check(username, password));
		response.getWriter().print(new Gson().toJson(new GoodBad().good()));
		D.log("Login OK");
	} catch (Exception e) {
		response.getWriter().print(new Gson().toJson(new GoodBad().bad(e)));
	}
%>