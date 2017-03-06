<%--
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ taglib prefix="fmt" uri="http://java.sun.com/jsp/jstl/fmt" %>
<%@ taglib prefix="x" uri="http://java.sun.com/jsp/jstl/xml" %>
<%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions" %>
<%@ taglib prefix="sql" uri="http://java.sun.com/jsp/jstl/sql" %> 
--%>
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
	<!-- my js -->
	<script src="/js/multiCharts.js"></script>
	<script src="/js/get_set.js"></script>
	<script src="/js/err.js"></script>
	<script src="/js/wifi_protocol.js"></script>	
	<script src="/js/inspectChart.js"></script>
	<style type="text/css">
	* {
		font: 13px/18px Arial, Sans-serif;
		font-weight: bold;
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
		height: 35px;
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
	input[type=text], select {
    width: 90%;
    padding: 3px 5px;
    display: inline-block;
    border: 1px solid #ccc;
    border-radius: 4px;
    box-sizing: border-box;
    background: #fff;
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
		font-size: 18px;
		border-radius: 5px;
		box-shadow: 0 0 0 5px #ddd;
		margin-left: 20px;
	}
	.main_fieldset > legend > span {
		font-size: 18px;
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
    font-size: 18px;
    border-radius: 5px;
    box-shadow: 0 0 0 5px #ddd;
    margin-left: 20px;
	}
	.main_fieldset_r > legend > span {
		font-size: 18px;
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
	.title {
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
		font: 18px Arial, Sans-serif;
		font-weight: bold;
		padding: 20px;
		background-color: #f44336; /* Red */
		color: white;
		margin-bottom: 15px;
	}
	.pop_success {
		font: 18px Arial, Sans-serif;
		font-weight: bold;
		padding: 20px;
		background-color: #4CAF50; /* Green */
		color: white;
		margin-bottom: 15px;
	}
	.pop_info {
		font: 18px Arial, Sans-serif;
		font-weight: bold;
		padding: 20px;
		background-color: #2196F3; /* Blue */
		color: white;
		margin-bottom: 15px;
	}
	.pop_warning {
		font: 18px Arial, Sans-serif;
		font-weight: bold;
		padding: 20px;
		background-color: #ff9800; /* Yellow */
		color: white;
		margin-bottom: 15px;
	}
	</style>
	<link rel="shortcut icon" href="/img/M/wifi.ico">
</head>
<body>
<table style="width: 100%;">
	<tr>
		<td class="title" width="150px;">
			<img class="title_img" src="/img/M/wifi.svg">&nbsp;&nbsp;<b>WIPS INSPECTOR</b>
		</td>
		<td align="right">
			<img class="head_link_img home_link_inspect"      src="/img/M/analytics_balls.svg">
			<img class="head_link_img home_link_profile_list" src="/img/M/edit2.svg">
			<img class="head_link_img home_link_config_list"  src="/img/M/config4.svg">
			<img class="head_link_img home_link_wifi_search"  src="/img/M/wifi_rader.png">
			<img class="head_link_img home_link_setting_list" src="/img/M/setting2.svg">
		</td>
	</tr>
</table>


<div id="custom-overlay" style="text-align: center; vertical-align: middle;">
	<img src="/img/M/loading.svg" style="width: 70px; height: 70px;">
</div>

<% boolean debug = HttpGet.sessionBoolean(request, "debug", false); %>
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
		var allowPageList   = new Array('/a.php', '/b.php');
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
		loading.overlay.fadeOut(1000);
	}
});

function load_ok() {
	$("body").loading('stop');
}

$(function() {
	$(".title").click(function() {
		gogo("/M/index.jsp");
	});
	$(".home_link_inspect").click(function() {
		gogo("/M/inspect.jsp");
	});
	$(".home_link_profile_list").click(function() {
		gogo("/M/profile_list.jsp");
	});
	$(".home_link_config_list").click(function() {
		gogo("/M/config_list.jsp");
	});
	$(".home_link_setting_list").click(function() {
		gogo("/M/setting.jsp");
	});
	$(".home_link_wifi_search").click(function() {
		gogo("/M/wifi_search.jsp");
	});
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