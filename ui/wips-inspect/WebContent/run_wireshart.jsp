<%@page import="com.google.gson.Gson"%><%@
page import="air.wips.inspect.error.GoodBad"%><%@
page import="air.wips.inspect.osdep.Tools"%><%@
page import="air.wips.inspect.servlet.HttpGet"%><%@
page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%><%

try {
	String filepath = HttpGet.get(request, "filepath");
	Tools.runWireshark(filepath);
	response.getWriter().print(new Gson().toJson(new GoodBad().good()));
} catch (Exception e) {
	e.printStackTrace();
	response.getWriter().print(new Gson().toJson(new GoodBad().bad(e)));
}
%>