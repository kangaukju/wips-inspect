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
	String name = HttpGet.get(request, "name");
	String profileId = HttpGet.get(request, "profile_id");
	List<String> configIdList = HttpGet.valList(request, "config_id");
	
	try {
		// new profile
		if (name != null) {
			Profile profile = new Profile();
			profile.setName(name);
			Profile.addProfile(profile);
		}
		// add config to profile
		else if (profileId != null && configIdList.size() > 0) {
			Profile.addConfig(profileId, configIdList);
		}
		response.getWriter().print(new Gson().toJson(new GoodBad().good()));
	} catch (Exception e) {
		e.printStackTrace();
		response.getWriter().print(new Gson().toJson(new GoodBad().bad(e)));
	}
%>