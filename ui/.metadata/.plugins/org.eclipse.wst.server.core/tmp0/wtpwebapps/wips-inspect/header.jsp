<%--
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ taglib prefix="fmt" uri="http://java.sun.com/jsp/jstl/fmt" %>
<%@ taglib prefix="x" uri="http://java.sun.com/jsp/jstl/xml" %>
<%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions" %>
<%@ taglib prefix="sql" uri="http://java.sun.com/jsp/jstl/sql" %> 
--%>
<%@page import="air.wips.inspect.log.D"%>
<%@page import="java.util.Date"%>
<%@page import="air.wips.inspect.Login.Admin"%>
<%@page import="air.wips.inspect.servlet.HttpGet"%>
<%@page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<head>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>Wips-Inspect</title>
	<!-- jquery -->
	<script type="text/javascript" src="/js/jquery/1.11.1/jquery-1.11.1.min.js"></script>
	<link rel="stylesheet" href="/js/jquery/ui/jquery-ui-1.12.1/jquery-ui.css">
	<script type="text/javascript" src="/js/jquery/ui/jquery-ui-1.12.1/jquery-ui.js"></script>
	<!-- json2html -->	
	<script type="text/javascript" src="/js/jquery.json2html.js"></script>
	<script type="text/javascript" src="/js/json2html.js"></script>		
	<!-- https://cdnjs.com/libraries/c3  -->
	<link rel="stylesheet" href="/js/c3/0.4.11/c3.min.css"/>	
	<script type="text/javascript" src="/js/d3/d3.v3.min.js"></script>
	<script type="text/javascript" src="/js/c3/0.4.11/c3.min.js"></script>
	<!-- icheck -->
	<script type="text/javascript" src="/js/icheck/icheck.min.js"></script>
	<link rel="stylesheet" href="/js/icheck/skins/all.css">
	<!-- tablesorter -->
	<link rel="stylesheet" href="/js/tablesorter-master/css/theme.default.css">
	<script type="text/javascript" src="/js/tablesorter-master/js/jquery.tablesorter.js"></script>
	<script type="text/javascript" src="/js/tablesorter-master/js/jquery.tablesorter.widgets.js"></script>
	<!-- loading -->
	<script src="/js/loading/jquery.loading.min.js"></script>
	<link rel="stylesheet" href="/js/loading/jquery.loading.min.css">
	<!-- animatedModal -->
	<script src="/js/animatedModal/animatedModal.js"></script>
	<link rel="stylesheet" href="/js/animatedModal/normalize.min.css">
	<link rel="stylesheet" href="/js/animatedModal/animate.min.css">
	<!-- html2canvas -->
	<script src="/js/html2canvas/html2canvas.js"></script>
	<!-- my js -->
	<script src="/js/multiCharts.js"></script>
	<script src="/js/get_set.js"></script>
	<script src="/js/err.js"></script>
	<script src="/js/wifi_protocol.js"></script>	
	<script src="/js/inspectChart.js"></script>
	<%--
	<script type="text/javascript">
		var uAgent = navigator.userAgent.toLowerCase();
		var mobilePhones = new Array('iphone', 'ipod', 'android', 'blackberry', 'windows ce', 'nokia', 'webos', 'opera mini', 'sonyericsson', 'opera mobi', 'iemobile', 'windows phone');
		var isMobile = false;
		for( var i = 0 ; i < mobilePhones.length ; ++i ){
			if( uAgent.indexOf(mobilePhones[i]) > -1) {
				isMobile = true;
				break;
			}
		}
	</script>
	--%>
	<style type="text/css">
	* {		
		font: 12px Arial, Sans-serif;
		font-weight: bold;
	}
	body {
		background: url(/img/login/bg.png) repeat;
		font-family: "HelveticaNeue-Light", "Helvetica Neue Light", "Helvetica Neue", Helvetica, Arial, "Lucida Grande", sans-serif;
		font-weight:300;
		text-align: left;
		text-decoration: none;
	}
	table.tablesorter thead tr th {
		background: #407bbf;
		color: #fff;
	}
	table.tablesorter_r thead tr th {
		background: #ffa31a;
		color: #fff;
	}
	table.tablesorter thead tr, table.tablesorter tbody tr {
		cursor: pointer;
		height: 15px;
	}
	table.tablesorter tbody tr.selected td {
   	background: none repeat scroll 0 0 #ffa31a;
   	color: #fff;
	}
	table.tablesorter_r tbody tr.selected td {
   	background: none repeat scroll 0 0 #407bbf;
   	color: #fff;
	}
	img {
		vertical-align: middle;
	}
	input {
		vertical-align: middle;
	}
	input[type=text], input[type=password], select {
	    width: 90%;
	    padding: 3px 5px;
	    display: inline-block;
	    border: 1px solid #ccc;
	    border-radius: 4px;
	    box-sizing: border-box;
	    background: #fff;
	}
	.login_img {
		width: 60px;
	}
	.head_img {
		width: 20px;
	}
	.action_img {
		cursor: pointer;
		width: 30px;
	}
	.table_img {
		padding: 0px;
		margin: 0px;
		cursor: pointer;
		width: 20px;
	}
	.index_background {
		background-color: #407bbf;
	}	
	.main_fieldset {
		background: #ddd;
		border: 2px solid #407bbf;
		border-radius: 5px;
		margin-top: 10px;
		margin-bottom: 10px;
		padding: 10px;
	}
	.main_fieldset > legend {
		background: #407bbf;
		color: #fff;
		padding: 5px 10px ;
		font-size: 14px;
		border-radius: 5px;
		box-shadow: 0 0 0 5px #ddd;
		margin-left: 20px;
	}
	.main_fieldset > legend > span {
		font-size: 14px;
	}
	.sub_fieldset {
		background: #ddd;
		border: 2px solid #666C73;
		border-radius: 5px;
		margin-top: 10px;
		margin-bottom: 10px;
		padding: 10px;
	}
	.sub_fieldset > legend {
		background: #666C73;
		color: #fff;
		padding: 5px 10px ;
		font-size: 15px;
		border-radius: 5px;
		box-shadow: 0 0 0 5px #ddd;
		margin-left: 20px;
	}
	.sub_fieldset > legend > span {
		font-size: 15px;
	}
	.main_fieldset_r {
		background: #ddd;
		border: 2px solid #ffa31a;
		border-radius: 5px;
		margin-top: 10px;
		margin-bottom: 10px;
		padding: 10px;
	}
	.main_fieldset_r > legend {
		background: #ffa31a;
    color: #fff;
    padding: 5px 10px ;
    font-size: 14px;
    border-radius: 5px;
    box-shadow: 0 0 0 5px #ddd;
    margin-left: 20px;
	}
	.main_fieldset_r > legend > span {
		font-size: 14px;
	}
	.sub_fieldset_r {
		background: #ddd;
		border: 2px solid #666C73;
		border-radius: 5px;
		margin-top: 10px;
		margin-bottom: 10px;
		padding: 10px;
	}
	.sub_fieldset_r > legend {
		background: #666C73;
		color: #fff;
		padding: 5px 10px ;
		font-size: 15px;
		border-radius: 5px;
		box-shadow: 0 0 0 5px #ddd;
		margin-left: 20px;
	}
	.sub_fieldset_r > legend > span {
		font-size: 15px;
	}
	.title_outer {
		cursor: pointer;
		vertical-align: middle;
		text-align: center;
		padding: 5px;
	}
	.title_img {		
		width: 30px;
	}
	.hidden {
		display: none;
	}
	.where_link_img {
		width: 20px;
	}
	.head_link_img {
		width: 25px;
		cursor: pointer;
	}
	.icon_img {
		width: 25px;
		height: 25px;
		vertical-align: middle;
		cursor: pointer;
	}
	.pop_error {
		font: 12px Arial, Sans-serif;
		font-weight: bold;
		padding: 20px;
		background-color: #f44336; /* Red */
		color: white;
		margin-bottom: 15px;
	}
	.pop_success {
		font: 12px Arial, Sans-serif;
		font-weight: bold;
		padding: 20px;
		background-color: #4CAF50; /* Green */
		color: white;
		margin-bottom: 15px;
	}
	.pop_info {
		font: 12px Arial, Sans-serif;
		font-weight: bold;
		padding: 20px;
		background-color: #2196F3; /* Blue */
		color: white;
		margin-bottom: 15px;
	}
	.pop_warning {
		font: 12px Arial, Sans-serif;
		font-weight: bold;
		padding: 20px;
		background-color: #ff9800; /* Yellow */
		color: white;
		margin-bottom: 15px;
	}
	</style>
	<link rel="shortcut icon" href="/img/wifi.ico">
</head>
<body>
<table style="width: 100%;">
	<tr>
		<td class="title_outer">
			<div align="left">
				<span class="title">
					<img class="title_img" src="/img/wifi.svg">&nbsp;&nbsp;<b>WIPS INSPECTOR</b>
				</span>				
				<span id="where"></span>
			</div>
		</td>
		<td align="right">
			<img class="head_link_img home_link" id="users"        src="/img/users.png">
			<img class="head_link_img home_link" id="inspect"      src="/img/inspect.png">
			<img class="head_link_img home_link" id="profile_list" src="/img/profiles.png">
			<img class="head_link_img home_link" id="config_list"  src="/img/configs.png">
			<img class="head_link_img home_link" id="history_list" src="/img/database.png">
			<img class="head_link_img home_link" id="wifi_search"  src="/img/wifi_search.png">
			<img class="head_link_img home_link" id="setting"      src="/img/setting.png">
		</td>
	</tr>
</table>

<div id="custom-overlay" style="text-align: center; vertical-align: middle;">
	<img src="/img/loading.svg" style="width: 70px; height: 70px;">
</div>

<%
	Admin admin = null;
	String curPage = HttpGet.currentPage(request);
	if (!"index.jsp".equals(curPage) && !"".equals(curPage)) {
		if ((session == null) || (session.getAttribute("admin") == null)) {
			response.sendRedirect("index.jsp");
			return;
		}
		admin = (Admin)session.getAttribute("admin");
		long now = new Date().getTime();
		if ((now - admin.getSessionTimestamp()) > admin.getSessionTimeout()) {
			D.log("session timeout");
			response.sendRedirect("index.jsp");
			return;
		}
		admin.setSessionTimestamp(now);
	}
%>

<% boolean debug = HttpGet.sessionBoolean(request, "debug", true); %>
<script type="text/javascript">
var debug = <%= debug %>;
function CtrlAlt() {
	if (event.keyCode == 17) { return false; }
	if (event.keyCode == 18) { return false; }
	if (event.keyCode == 91) { event.keyCode = 505; }
	if (event.keyCode > 112 && event.keyCode < 123){ event.keyCode = 505; }
	if (event.keyCode == 505) { return false; }
	if(event.keyCode === 37 || event.keyCode === 38 || event.keyCode === 39 || event.keyCode === 40) {
		return false;
	}
}
if (!debug) {
	document.onkeydown = CtrlAlt;
	document.onmousedown = CtrlAlt;
	
	$(window.document).ready(function(){
		$(window.document).on("contextmenu", function(event){return false;});	//우클릭방지
		$(window.document).on("selectstart", function(event){return false;});	//더블클릭을 통한 선택방지
		$(window.document).on("dragstart"	, function(event){return false;});	//드래그
	});
	
	$(document).keydown(function (e) {
		// F5, ctrl + F5, ctrl + r 새로고침 막기
		var allowPageList   = new Array('');
		var bBlockF5Key     = true;
		for (number in allowPageList) {
			var regExp = new RegExp('^' + allowPageList[number] + '.*', 'i');
			if (regExp.test(document.location.pathname)) {
				bBlockF5Key = false;
				break;
			}
		}
	     
		if (bBlockF5Key) {
			if (e.which === 116) {
				if (typeof event == "object") {
					event.keyCode = 0;
				}
				return false;
			} else if (e.which === 82 && e.ctrlKey) {
				return false;
			}
		}
	});
}

$("body").loading({
	theme: 'dark',
	overlay: $("#custom-overlay"),
	onStart: function(loading) {
		loading.overlay.fadeIn(10);
	},
	onStop: function(loading) {
		loading.overlay.fadeOut(200);
	}
});

function load_ok() {
	$("body").loading('stop');
}

/*
$(window).on('unload ',function() {
	jQuery.ajax({
		url: "login_out.jsp",
		type: "POST",
		cache: false,
		async: false,
		dataType: "json",
		success: function(result) {
			gogo("/M");
		},
		error: function(e) {
			gogo("/M");
		}
	});
});
*/
<%
String currentID 
	= request.getServletPath().substring(
			request.getServletPath().indexOf("/")+1, 
			request.getServletPath().lastIndexOf(".jsp"));
%>

$(function() {
	$(".title").click(function() {
		gogo("main.jsp");
	});
	$(".home_link").click(function() {		
		gogo($(this).attr("id")+".jsp");		
	});
	
	$("#where").empty();
	var $currentID = $("#"+"<%=currentID%>");
	if (_defined_($currentID)) {
		if (_defined_($currentID.attr("src"))) {
			$("<span>")
				.html("&nbsp;&nbsp;>&nbsp;&nbsp;")
				.appendTo("#where");
			$("<img>")
				.attr("src", $currentID.attr("src"))
				.addClass("where_link_img")				
				.appendTo("#where");
			var naming = $currentID.attr("id").toUpperCase().replace("_", " ");
			$("<span>")
				.html("&nbsp;"+naming)
				.appendTo("#where");
		}
	}
});

function _defined_(v) {
	return (typeof v != "undefined");
}

function pop(msg, config) {
	var c = {};
	
	if (_defined_(config)) {
		c = config;
	}
	
	if (1) {
		var css = "pop_error";
		if (_defined_(c.type)) {
			if (c.type == "info") {
				css = "pop_info";
			} else if (c.type == "success") {
				css = "pop_success";
			} else if (c.type == "warning") {
				css = "pop_warning";
			}
		}
		var box_msg = "<div class='"+css+"'>"+msg+"</div>";
		
		$("body").loading({
			theme: 'dark',
			message: box_msg,
			onStart: function(loading) {
				loading.overlay.slideDown(350);
			},
			onClick: function(loading) {
				loading.overlay.slideUp(250);
				if (_defined_(c.focus)) {
					c.focus.focus();
				}
				if (_defined_(c.page)) {
					gogo(c.page);
				}
			}
		});
		$("body").loading();
	} else {		
		alert(msg);
		if (typeof $focus != "undefined") {
			$focus.focus();
		}
	}
}

function gogo(page) {
	$("body").animate({
		"opacity": "0",
		"top": "10px"
	},400, function () {
		location.href = page;
	});
}

function pN($this, n) {
	var p = $this;
	for (var i=0; i<n; i++) {
		p = p.parent();
	}
	return p;
}
</script>
</body>