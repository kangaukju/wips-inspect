<%@page import="org.apache.commons.codec.binary.Base64"%>
<%@page import="java.io.FileOutputStream"%>
<%@page import="air.wips.inspect.error.GoodBad"%>
<%@page import="com.google.gson.Gson"%>
<%@page import="air.wips.inspect.servlet.HttpGet"%><%@
page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%><%
	
try {
	HttpGet.debugRequestParameterValues(request);

	String [] chartImgs = HttpGet.vals(request, "chart_img");
	String [] configIds = HttpGet.vals(request, "config_id");
	String profileId = HttpGet.get(request, "profile_id");
	
	if (profileId == null) {
		throw new Exception("Unknown profile id");
	}
	if (chartImgs == null) {
		throw new Exception("empty chart images");
	}
	if (configIds == null) {
		throw new Exception("empty config id list");
	}
	if (chartImgs.length != configIds.length) {
		throw new Exception("mismatch chart and config count");
	}
	System.out.println(chartImgs.length);
	for (int i=0; i<chartImgs.length; i++) {
		String dataBase64 = chartImgs[i].replaceAll("data:image/jpeg;base64,", "");
		String filename = "/tmp/"+configIds[i]+".png";
		
		System.out.println(filename);
		
		FileOutputStream fos = new FileOutputStream(filename);
		fos.write(Base64.decodeBase64(dataBase64));
		fos.close();
	}
	response.getWriter().print(new Gson().toJson(new GoodBad().good()));
	
} catch (Exception e) {
	e.printStackTrace();
	response.getWriter().print(new Gson().toJson(new GoodBad().bad(e)));
}
%>