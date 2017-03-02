<%@page import="sql.inspect.wips.air.Profile"%>
<%@page import="air.wips.inspect.error.GoodBad"%>
<%@page import="air.wips.inspect.servlet.HttpGet"%>
<%@page import="java.util.Enumeration"%>
<%@page import="com.google.gson.Gson"%>
<%@page import="sql.inspect.wips.air.Config"%>
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%
	String name = HttpGet.get(request, "name");
	String id = HttpGet.get(request, "id");
	
	try {
		if (id != null) {
			response.getWriter().print(new Gson().toJson(Profile.getById(id)));
		} else if (name != null) {
			response.getWriter().print(new Gson().toJson(Profile.getByName(name)));
		} else {
			response.getWriter().print(new Gson().toJson(Profile.getAllWithConfig()));	
		}
	} catch (Exception e) {
		e.printStackTrace();
		response.getWriter().print(new Gson().toJson(new GoodBad().bad(e)));
	}
%>