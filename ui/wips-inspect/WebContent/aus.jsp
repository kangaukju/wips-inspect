<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Insert title here</title>
<script type="text/javascript" src="/js/jquery/1.11.1/jquery-1.11.1.min.js"></script>
<link rel="stylesheet" href="/js/jquery/ui/jquery-ui-1.12.1/jquery-ui.css">
<script type="text/javascript" src="/js/jquery/ui/jquery-ui-1.12.1/jquery-ui.js"></script>
</head>
<body>
<script type="text/javascript">
$(function() {
	var c = "<table name='user_tbl' action='update'><user id='unet' pwd='qwe123' dept='응급실' mail='unet@naver.com' tel='010-1234-1234'><mac>11:22:33:44:55:61</mac><mac>11:22:33:44:55:62</mac><mac>11:22:33:44:55:63</mac></user></table>";
	var xml = encodeURI(c);
	alert(xml);
	jQuery.ajax({
		url: "https://10.10.200.152:8443/xmlapi.jsp",
		data: {xml: xml},
		cache: false,	
		
		success: function(result) {
		},
		error: function(e) {
			ajax_err_handle(e);
		}
	});
});
</script>
</body>
</html>