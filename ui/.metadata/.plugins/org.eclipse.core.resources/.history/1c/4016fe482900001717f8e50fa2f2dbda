<%@page import="java.util.Arrays"%><%@
page import="air.wips.inspect.error.GoodBad"%><%@
page import="com.google.gson.Gson"%><%@
page import="sql.inspect.wips.air.Profile"%><%@
page import="air.wips.inspect.servlet.HttpGet"%><%@
page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%

	String profileId = Profile.newProfileId();
	boolean edit = false;

	if (HttpGet.get(request, "profile_id") != null) {
		profileId = HttpGet.get(request, "profile_id");
		edit = true;
	}
	
	try {
		//HttpGet.debugRequestParameterValues(request);
		String profile_name = HttpGet.get(request, "profile_name");
		String [] config_id = HttpGet.vals(request, "config_id");
		
		Profile profile = new Profile();
		profile.setId(profileId);
		profile.setName(profile_name);
		
		if (edit) {
			profile.update();
			Profile.removeAllConfig(profileId);
			Profile.addConfig(profileId, Arrays.asList(config_id));
		} else {
			profile.addProfile();
			Profile.addConfig(profileId, Arrays.asList(config_id));
		}
		response.getWriter().print(new Gson().toJson(new GoodBad().good()));
	}
	catch (Exception e) {
		response.getWriter().print(new Gson().toJson(new GoodBad().bad(e)));
	}
%>