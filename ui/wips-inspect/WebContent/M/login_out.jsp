<%@page import="air.wips.inspect.log.D"%><%@
page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%><%
	D.log("logout");
	if (session.getAttribute("admin") != null) {
		session.removeAttribute("admin");
	}
	session.invalidate();
%>