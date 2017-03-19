<%@page import="air.wips.inspect.osdep.WifiDevice"%>
<%@page import="java.util.List"%>
<%@page import="air.wips.inspect.osdep.Tools"%>
<%@page import="air.wips.inspect.utils.ProcessUtil"%>
<%@page import="java.util.Enumeration"%>
<%@page import="java.util.Properties"%>
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Insert title here</title>
</head>
<body>
<%
List<WifiDevice> list = Tools.getWifiInterface();
System.out.println(list.size());
for (WifiDevice w : list) {
	System.out.println(w);
}
%>
</body>
</html>
