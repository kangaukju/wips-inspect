<%@page import="air.wips.inspect.osdep.Tools"%>
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
<head>
	<meta charset="utf-8">
	<style type="text/css">
	.color_over_table tbody tr:hover td {
		background: none repeat scroll 0 0 #FFCF8B;
		color: #000000;
		cursor: pointer;
    }
	.inspect_progress_label {
		position: absolute;
		left: 50%;
		top: 4px;
		font-weight: bold;
		text-shadow: 1px 1px 0 #fff;
	}
	</style>
</head>
<body>

	<fieldset id="profile_fieldset_select" class="tree main_fieldset">
		<legend>All Profile List</legend>
		<div>
			<select data-placeholder="Choose Profile..." id="profile_select"></select>
		</div>
	</fieldset>
		
	<fieldset class="main_fieldset">
		<legend id="select_config_legend">Inspect Process</legend>
		<div>
			<img src="img/timer.svg" class="input_img">
			<select id="inspect_timer" style="width: 100px;">
				<option value="5">5</option>
				<option value="10">10</option>
				<option value="15">15</option>
				<option value="20">20</option>
				<option value="25">25</option>
				<option value="30">30</option>
			</select>
			<a href="#" class="btn btn-inline btn-small btn-primary" id="inspect_run_button"><span>start</span></a>
		</div>

		<table id="selected_config_table" class="color_over_table">		
			<thead>
				<tr>
					<th>id</th>
					<th>name</th>
					<th>capture</th>
					<th>shooter</th>
					<th>created</th>
					<th>updated</th>
				</tr>
			</thead>
			<tbody></tbody>
		</table>	
	</fieldset>
	
	<fieldset class="main_fieldset">
		<legend id="inspect_results_legend">Inspect Results</legend>
		<div id="inspect_results_div">
		</div>
	</fieldset>
	
	<div id="detail_log_dialog">
	</div>

	<div id="detail_analyze_dialog">
	</div>
		
</body>
<script type="text/javascript">

var profile_select_xlat = {
		tag: "option",
		html: "$\{name\}",
		value: "$\{id\}",
};

/**
*
*/
var config_xlat = {
		tag: "tr",
		children: [
			{tag:"td", html:"$\{id\}"},
			{tag:"td", html:"$\{name\}"},
			{tag:"td", html:"$\{capturexml\}", "class":"capture_xml"},
			{tag:"td", html:"$\{shooterxml\}", "class":"shooter_xml"},
			{tag:"td", html:"$\{created\}"},
			{tag:"td", html:"$\{updated\}"},
		]
};

/**
*
*/
var detail_log_xlat = {
		tag: "tr",
		children: [
			{tag:"td", html:"$\{xid\}"},
			{tag:"td", html:"$\{elapsed\}"},
			{tag:"td", html:"$\{pwr\}"},
			{tag:"td", html:get_framesubtype_string("$\{type\}", "$\{subtype\}")},
			{tag:"td", html:get_ds_string("$\{ds\}", ("$\{ds\}" == 2))},
			{tag:"td", html:"$\{addr1\}"},
			{tag:"td", html:"$\{addr2\}"},
			{tag:"td", html:"$\{addr3\}"},
			{tag:"td", html:"$\{addr4\}"},
			{tag:"td", html:"$\{seq\}"},
		]
};

/**
*
*/
var inspect_results_xlat = {
		tag: "fieldset",
		id: "$\{id\}",
		class:"sub_fieldset",
		children: [
			{tag:"legend", html:"$\{name\}"},
			{tag:"table",  html:inspect_results_table_html("$\{id\}")},
		]
};

/**
*
*/
function inspect_results_table_html(id) {
	var height = 100;
	var html = 
	"<tr>"+
		"<td>progressbar</td>"+
	"</tr>"+
	"<tr>"+
		"<td>"+
			"<img class='icon_img detail_log_img' align='left' src='img/list2.svg' config_id='"+id+"' target='inspect_log_table_"+id+"' />"+
			"<img class='icon_img detail_ana_img' align='left' src='img/ana.png'   config_id='"+id+"'/>"+
			"<img class='icon_img wireshark_img'  align='left' src='img/wireshark.png' />"+
		"</td>"+
	"</tr>"+
	"<tr>"+
		"<td>"+
			"<div id='inspect_log_div_"+id+"' style='height: "+height+"px; overflow-x: hidden;'>"+
				"<table id='inspect_log_table_"+id+"' class='inspect_log_table color_over_table'>"+
					"<thead>"+
						"<tr>"+
							"<th>xid</th>"+
							"<th>elapsed</th>"+
							"<th>pwr</th>"+
							"<th>subtype</th>"+
							"<th>ds</th>"+
							"<th>addr1</th>"+
							"<th>addr2</th>"+
							"<th>addr3</th>"+
							"<th>addr4</th>"+
							"<th>seq</th>"+
						"</tr>"+
					"</thead>"+
					"<tbody></tbody>"+
				"</table>"+
			"</div>"+
		"</td>"+
	"</tr>"+
	"<tr>"+
		"<td>"+
			"<div class='inspect_chart' id='inspect_chart_"+id+"' config='"+id+"'></div>"+
		"</td>"+
	"</tr>";
	return html;
}

/**
*
*/
function get_inspect_log_table_html(data) {
	var html = 
	"<tr>"+
		"<td>"+data.xid+"</td>"+
		"<td>"+data.elapsed+"</td>"+
		"<td>"+data.pwr+"</td>"+
		"<td>"+get_framesubtype_string(data.type, data.subtype)+"</td>"+
		"<td>"+get_ds_string(data.ds, (data.type == 2))+"</td>"+
		"<td>"+data.addr1+"</td>"+
		"<td>"+data.addr2+"</td>"+
		"<td>"+data.addr3+"</td>"+
		"<td>"+data.addr4+"</td>"+
		"<td>"+data.seq+"</td>"+
	"</tr>";
	return html;
}

/**
*
*/
function load_selected_profile(profile_id) {
	jQuery.ajax({
		url: "get_profile.jsp",
		data: {id: profile_id},
		cache: false,
		beforeSend: function() {
			$("#selected_config_table > tbody").empty();
		},
		dataType: "json",
		success: function(result) {
			if (result.good == false) {
				alert("load_selected_profile: "+result.cause);
				return;
			}
			$("#select_config_legend").html("Inspect Process ["+result.name+"]");			
			$("#selected_config_table > tbody").html(json2html.transform(result.configList, config_xlat));
			$("#selected_config_table").tablesorter({widgets: ["zebra"]}).trigger("update");
			$("#inspect_results_div").html(json2html.transform(result.configList, inspect_results_xlat));
			
			/* detail log table popup */
			bind_detail_log();
			/* furture */
			bind_detail_ana();
		},
		error: function(e) {
			ajax_err_handle(e);
		}
	});
}

/**
*
*/
function load_profile() {
	jQuery.ajax({
		url: "get_profile.jsp",
		cache: false,
		dataType: "json",
		success: function(result) {
			if (result.good == false) {
				alert("load_profile: "+result.cause);
				return;
			}
			/* profile list to select box */
			$("#profile_select").html(json2html.transform(result, profile_select_xlat));
			$("#profile_select").change();
		},
		error: function(e) {
			ajax_err_handle(e);
		}
	});
}

/**
*
*/
function load_wifidev() {
	jQuery.ajax({
		url: "wifidriver_json.jsp",
		cache: false,
		dataType: "json",
		success: function(result) {
			if (result.good == false) {
				alert("load_wifidev: "+result.cause);
				return;
			}
			/* complete */
		},
		error: function(e) {
			alert("load_wifidev: "+e.responseText);  
		}
	});
}	

/**
*
*/
function inspect_process_table(id, name, progress, progress_label) {
	return
	"<td>"+id+"</td>"+
	"<td>"+name+"</td>"+
	"<td>"+
		"<div id='"+progress+"'>"+
			"<div id='"+progress_label+"'>Ready...</div>"+
		"</div>"+
	"</td>"+
	"<td></td>";
}

function bind_detail_ana() {
	$(".detail_ana_img").each(function() {
		$(this).click(function() {
			var config_id = $(this).attr("config_id");

			console.log(config_id);
			
			jQuery.ajax({
				url: "inspect_analyze.jsp",
				data: {config_id: config_id},
				cache: false,
				beforeSend: function() {
				},
				dataType: "json",
				success: function(result) {
					if (result.good == false) {
						alert("detail_ana_log: "+result.cause);
						return;
					}
					$("#detail_analyze_dialog").empty();
					var title = $("#"+config_id).children("legend").text();
					$("#detail_analyze_dialog").dialog("option", "title", title);
					
					for (var a in result) {
						if (a.name == "BusyTimeInspection") {
							alert(a.name);
							
							/*
							$("<div>",{
								id: title+"_"+config_id,
							}).appendTo($("#detail_analyze_dialog"));
							
							var c = c3.generate({
								bindto: title+"_"+config_id,
								data: {
									columns: 
								},
							});
							*/	
						}
					}
					$("#detail_analyze_dialog").dialog("open");
				},
				error: function(e) {
					ajax_err_handle(e);
				}
			});
		});
	});
}

/**
*
*/
function bind_detail_log() {
	$(".detail_log_img").each(function() {
		$(this).click(function() {			
			var config_id = $(this).attr("config_id");
			var title = $("#"+config_id).children("legend").text();
			$("#detail_log_dialog").dialog("option", "title", title);
			$("#detail_log_dialog").dialog("option", "target", $(this).attr("target"));
			$("#detail_log_dialog").dialog("open");	
		});
	});
}

var inspect_charts = {};
var inspect_refresh_times = {};
var inspect_render_time = 1000; // msec

$(document).ready(function() {
	$(".table").tablesorter({widgets: ["zebra"]});
	
	/* Wi-Fi Driver load result */
	load_wifidev();
	
	/* profile list */
	load_profile();
	
	/* */
	$("#detail_analyze_dialog").dialog({
		autoOpen: false,
		modal: true,
		show: {
			effect: "blind",
			duration: 100
		},
		hide: {
			effect: "explode",
			duration: 100
		},
		width: "90%",
		height: "500"
	});
	
	/*  */
	$("#detail_log_dialog").dialog({
		autoOpen: false,
		modal: true,
		open: function(event, ui) {
			/*
		    var t = $(this).parent(), w = window;
		    t.offset({
		    	top: (w.height() / 2) - (t.height() / 2),
		    	left: (w.width() / 2) - (t.width() / 2)
		    });
		    */
			
			$("#detail_log_dialog").empty();
			var table = $("#detail_log_dialog").dialog("option", "target");
			var new_table = $("#"+table).clone().prependTo($("#detail_log_dialog"));
			new_table.tablesorter({widgets: ["zebra"]}).trigger("update");
		},
		show: {
			effect: "blind",
			duration: 100
		},
		hide: {
			effect: "explode",
			duration: 100
		},
		width: "90%",
		height: "300"
	});
	
	/* inspect web socket */	
	var ws;
	var host = '<%=request.getHeader("host")%>';
	$("#inspect_run_button").click(function() {
		var status = $(this).find("span").html();
		
		/* start inspect */
		if ("start" == status) {
			var profile_id = $("#profile_select option:selected").val();
			if (_isnull(profile_id)) {
				alert("please select the profile you want to inspect");
				return;
			}
			
			//ws = new WebSocket("ws://localhost:8080/wips-inspect/inspect");
			var url = "ws://"+host+"/inspect";
			ws = new WebSocket(url);
			ws.onopen = function(msg) {
				$("#inspect_run_button").find("span").html("stop");
				$(".inspect_log_table > tbody").empty();				
				// clear child div
				for (var div in inspect_charts) {
					inspect_charts[div].empty();
				}
				inspect_charts = {};
				inspect_refresh_times = {0: 0, 1:0};
				
				/* prepare chart drawing */
				if (0) {
					var max_sec = parseInt($("#inspect_timer").val()) + 2;
					$(".inspect_chart").each(function(i){
						var id = $(this).attr("config");
						inspect_charts[id] 
							= inspectChart.generate("inspect_chart_"+id, {
									items: ["shooter", "capture"],
									max_sec: max_sec,
								});
					});
				}
				
				var params = {
					state: "start",
					timer: $("#inspect_timer").val(),
					profileId: profile_id
				};
				ws.send(JSON.stringify(params));
			}
			ws.onmessage = function(msg){
				if (!_isnull(msg.data)) {
					var result = JSON.parse(msg.data);
					if (result.good == false) {
						alert("inspect websocket: "+result.cause);
						$(this).find("span").html("start");
						ws.close();
						return;
					}
					if (result.good == true) {
						return;
					}
					
					// append capture data to log-table
					if (1) {
						$("#inspect_log_table_"+result.conf+" > tbody:last").append(get_inspect_log_table_html(result));						
						$("#inspect_log_table_"+result.conf).tablesorter({widgets: ["zebra"]}).trigger("update");
						
						/* for scroll inspect log table */
						var _div = $("#inspect_log_div_"+result.conf);
						_div.scrollTop(_div[0].scrollHeight);
					}
					
					/* dynamic char drawing */
					if (1) {
						// append capture data to chart
						if (inspect_charts[result.conf] == undefined) {
							var max_sec = parseInt($("#inspect_timer").val()) + 2;
							inspect_charts[result.conf] 
								= inspectChart.generate("inspect_chart_"+result.conf, {
										items: ["shooter", "capture"],
										max_sec: max_sec,
									});
						}
						// capture or shooter result
						var x = result.elapsed;
						var y = result.pwr;
						var now = new Date().getTime();
						if (result.key == 1) {
							// capture chart
							inspect_charts[result.conf].push("capture", x, y, false);
							if (now - inspect_refresh_times[0] > inspect_render_time) {
								inspect_refresh_times[0] = now;
								inspect_charts[result.conf].refresh("capture");
							}
						} else if (result.key == 2) {
							// shooter chart
							inspect_charts[result.conf].push("shooter", x, y, false);
							if (now - inspect_refresh_times[1] > inspect_render_time) {
								inspect_refresh_times[1] = now;
								inspect_charts[result.conf].refresh("shooter");
							}
						}						
					}
				}
			}
			ws.onerror = function(msg) {
				$("#inspect_run_button").find("span").html("start");
				if (_isnull(msg) || "error" == msg.type) {
					alert("wins-inspect not running.");
				}
			}
			ws.onclose = function() {				
				$("#inspect_run_button").find("span").html("start");
				for (var conf in inspect_charts) {
					inspect_charts[conf].refresh("capture");
					inspect_charts[conf].refresh("shooter");
				}
			}
		}
		/* stop inspect */
		else if ("stop" == status) {
			$(this).find("span").html("start");
			ws.close();
		}
	});
	
	$("#profile_select").change(function() {
		var profile_id = $("#profile_select option:selected").val();
		load_selected_profile(profile_id);
	});
});

</script>
</html>