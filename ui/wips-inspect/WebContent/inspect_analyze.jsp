<%@page import="air.wips.inspect.analyzer.factors.InspectorAbstract"%>
<%@page import="air.wips.inspect.analyzer.factors.FirstDetectionInspection"%>
<%@page import="air.wips.inspect.analyzer.factors.DelayTimeInspection"%>
<%@page import="air.wips.inspect.analyzer.factors.BusyTimeInspection"%>
<%@page import="air.wips.inspect.utils.StringUtil"%>
<%@page import="air.wips.inspect.error.GoodBad"%>
<%@page import="com.google.gson.Gson"%>
<%@page import="air.wips.inspect.analyzer.Inspector"%>
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%><%

	String configId = request.getParameter("config_id");

	try {
		if (StringUtil.isNull(configId)) {
			throw new Exception("missing config_id");
		}
		
		InspectorAbstract inspections[] = new InspectorAbstract[] {
				new BusyTimeInspection(100),
				new DelayTimeInspection(100),
				new FirstDetectionInspection(500),				
		};
		
		Inspector inspector = Inspector.getInstance();
		inspector.inspect(configId, inspections);
		
		Object [] reports = new Object[inspections.length];
		for (int i=0; i<inspections.length; i++) {
			reports[i] = inspections[i];
		}
		response.getWriter().print(new Gson().toJson(reports));
	} catch (Exception e) {
		e.printStackTrace();
		response.getWriter().print(new Gson().toJson(new GoodBad().bad(e)));
	}
%>