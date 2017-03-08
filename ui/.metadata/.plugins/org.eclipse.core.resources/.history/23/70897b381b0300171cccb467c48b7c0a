<%@page import="air.wips.inspect.servlet.HttpGet"%><%@
page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%><%

	System.out.println("save inspect");
	HttpGet.debugRequestParameterValues(request);
	String [] chartImgs = request.getParameterValues("chart_img");
	
	if (chartImgs == null) {
		System.out.println("null");
	} else {
		for (String chartImg : chartImgs) {
			System.out.println(chartImg);
		}
	}

%>