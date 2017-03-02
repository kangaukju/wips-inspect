<%@page import="utils.inspect.wips.air.ListUtil"%>
<%@page import="utils.inspect.wips.air.StringUtil"%>
<%@page import="java.util.List"%>
<%@page import="profile.inspect.wips.air.AirProfiles"%>
<%@page import="com.google.gson.Gson"%>
<%@page import="osdep.inspect.wips.air.Tools"%>
<%@page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<% 
	String profile = StringUtil.nullAble(request.getParameter("profile"));	

	if (profile == null) {
		List list = ListUtil.nullAble(AirProfiles.getAll());		
		if (list != null) {
			response.getWriter().print(new Gson().toJson(list));
		}
	} else {
		AirProfiles p = AirProfiles.get(profile);
		if (p != null) {
			response.getWriter().print(new Gson().toJson(p));
		}
	}
%>