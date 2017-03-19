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
	<fieldset id="wifidev_fieldset" class="main_fieldset">
		<legend>
			<img class="legend_img toggleable_img" target="wifidev_table">
			 Wi-Fi Drivers 
			<img class="closeable_img" target="wifidev_fieldset">
		</legend>
		<div id="wifidev">
			<table id="wifidev_table" class="table color_over_table">
				<thead>
					<tr>
						<th>PHY</th>
						<th>INTERFACE</th>
						<th>DRIVER</th>
						<th>CHIPSEET</th>
						<th>MONITOR</th>
					</tr>
				</thead>
				<tbody/>
			</table>
		</div>
	</fieldset>
	
	<fieldset id="profile_fieldset_select" class="tree main_fieldset">
		<legend>All Profile List</legend>
		<div>
			<select data-placeholder="Choose Profile..." id="profile_select"></select>
		</div>
	</fieldset>
	
	<fieldset id="profile_fieldset_jstree" class="tree main_fieldset">
		<legend>
			<img class="legend_img toggleable_img" target="profile_tree">
			 All Profile List 			
			<img class="closeable_img" target="profile_fieldset_jstree">
		</legend>
		<div id="profile_tree">
		</div>
	</fieldset>
		
	<fieldset class="main_fieldset">
		<legend id="select_config_legend">Inspect Process</legend>
		<div>
			<img src="img/timer.svg" class="input_img">
			<select id="inspect_timer" class="number_selectbox" style="width: 100px;">
				<option value="5">5</option>
				<option value="10">10</option>
				<option value="15">15</option>
				<option value="20">20</option>
				<option value="25">25</option>
				<option value="30">30</option>
			</select>
			<a href="#" class="btn btn-inline btn-small btn-primary" id="inspect_run_button"><span>start</span></a>
		</div>

		<table id="selected_config_table" class="table">		
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
		<%--
		<table id="detail_log_table" class="table">
			<thead>
				<tr>
					<th>xid</th>
					<th>elapsed</th>
					<th>pwr</th>
					<th>type</th>
					<th>subtype</th>
					<th>ds</th>
					<th>addr1</th>
					<th>addr2</th>
					<th>addr3</th>
					<th>addr4</th>
					<th>seq</th>
				</tr>
			</thead>
		</table>
		--%>
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
		class:"pointer",
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
		class:"pointer",
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
			"<img class='icon_img detail_log_img' src='img/list2.svg' align='left' target='inspect_log_table_"+id+"' target_id='"+id+"' />"+
			"<img class='icon_img detail_ana_img' src='img/ana.png' align='left' config_id='"+id+"'/>"+
			"<img class='icon_img' src='img/wireshart_icon.png' align='left' />"+
		"</td>"+
	"</tr>"+
	"<tr>"+
		"<td>"+
			"<div id='inspect_log_div_"+id+"' style='height: "+height+"px; overflow-x: hidden;'>"+
				"<table id='inspect_log_table_"+id+"' class='inspect_log_table'>"+
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
			$("#inspect_results_div").html("");			
			$("#inspect_results_div").html(json2html.transform(result.configList, inspect_results_xlat));
			
			bind_detail_log();
			detail_ana_log();
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
			if (1) {
				$("#profile_select").html(json2html.transform(result, profile_select_xlat));
				$("#profile_select").change();
			} else {
				$("#profile_fieldset_select").hide();
			}
			
			/* profile list to jstree */
			if (1) {
				var id = 0;
				var data = [];
				$.each(result, function(i, profile){
					data.push({
						id: "p_"+profile.id,
						parent: '#',
						text: profile.name,
					});
					$.each(profile.configList, function(k, config){
						data.push({
							id: "c_"+profile.id+"_"+config.id,
							text: config.name,
							parent: "p_"+profile.id,
						});
					});
				});
				
				$("#profile_tree").jstree("destroy")
				.jstree({
					plugins: ["themes","html_data","ui","crrm","search","types","hotkeys","contextmenu"],
			   		themes: {'theme' : 'default', 'dots' : false, 'icons' : true},
					search : {'case_insensitive' : true},
					core: {
						data: data,
					},
				})
				.bind('loaded.jstree', function(event, data){
		            //트리 로딩 롼료 이벤트
				})
				.bind('select_node.jstree', function(event, data){
					//노드 선택 이벤트
					var profile_id = extract_profile_id(data.selected[0]);
					var config_id = extract_config_id(data.selected[0]);
					if (config_id != "") {
						
					} else  if (profile_id != "") {
						load_selected_profile(profile_id);
					}
				})
			} else {
				$("#profile_fieldset_jstree").hide();
			}
		},
		error: function(e) {
			ajax_err_handle(e);
		}
	});
}

/**
*
*/
var wifidev_ifname_xlat = {
		"tag":"option","value":"$\{ifname\}","html":"$\{ifname\}"};		
var wifidev_list_xlat = {
	tag: "tr",
	children: [
		{"tag":"td","html":"$\{phy\}"},
		{"tag":"td","html":"$\{ifname\}"},
		{"tag":"td","html":"$\{driver\}"},
		{"tag":"td","html":"$\{chipset\}"},
		{"tag":"td","html":"$\{monitor\}"},
	]
};

/**
*
*/
function clear_wifidev() {
	$("#wifidev_table > tbody").empty();
}

/**
*
*/
function load_wifidev() {
	jQuery.ajax({
		url: "wifidriver_json.jsp",
		cache: false,
		beforeSend: clear_wifidev,
		dataType: "json",
		success: function(result) {
			if (result.good == false) {
				alert("load_wifidev: "+result.cause);
				return;
			}
			$("#wifidev_table > tbody").html(json2html.transform(result, wifidev_list_xlat));
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

function detail_ana_log() {	
	$(".detail_ana_img").each(function() {
		$(this).click(function() {			
			config_id = $(this).attr("config_id");
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
			target_id = $(this).attr("target_id");
			title = $("#"+target_id).children("legend").text();
			$("#detail_log_dialog").dialog("option", "title", title);
			$("#detail_log_dialog").dialog("option", "target", $(this).attr("target"));
			$("#detail_log_dialog").dialog("open");	
		});
	});
}

var inspect_charts = {};

$(document).ready(function() {
	$(".table").tablesorter({widgets: ["zebra"]});
	$(".number_selectbox").chosen();
	
	/**
	* Wi-Fi Driver load result
	*/
	load_wifidev();
	if (0) {
		$("#wifidev_fieldset").hide();
	}
	
	/* profile list */
	load_profile();
	
	/**
	*
	*/
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
	
	/**
	*
	*/
	$(".closeable_img").each(function() {
		var target = $(this).attr("target");
		$(this).attr({"src" : "img/remove.svg"});
		$(this).click(function() {
			$("#"+target).toggle("slow");
		});
	});
	
	
	/**
	*
	*/
	$(".toggleable_img").each(function() {
		var target = $(this).attr("target");		
		$(this).attr({"src" : "img/arrow-down.svg"});		
		$(this).click(function() {
			var img = $(this);
			$("#"+target).toggle("slow", function() {
				if (img.attr("src") == "img/arrow-down.svg") {
					img.attr({"src" : "img/arrow-up.svg"});
				} else {
					img.attr({"src" : "img/arrow-down.svg"});
				}
			});
		});
	});
	
	/* inspect web socket */	
	var ws;
	var host = '<%=request.getHeader("host")%>';
	$("#inspect_run_button").click(function() {
		var status = $(this).find("span").html();
		
		/* start inspect */
		if ("start" == status) {
			var profile_id = $('#profile_tree').jstree('get_selected');
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
				
				if (1) {
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
					profileId: extract_profile_id(profile_id[0])
				};
				ws.send(JSON.stringify(params));
			}
			ws.onmessage = function(msg){
				if (!_isnull(msg.data)) {
					var result = JSON.parse(msg.data);
					if (result.good == false) {
						alert("inspect websocket: "+result.cause);
						return;
					}
					if (result.good == true) {
						return;
					}
					
					// append capture data to log-table
					if (1) {
						$("#inspect_log_table_"+result.conf+" > tbody:last").append(get_inspect_log_table_html(result));						
						$("#inspect_log_table_"+result.conf).tablesorter({widgets: ["zebra"]}).trigger("update");
						
						var _div = $("#inspect_log_div_"+result.conf);
						_div.scrollTop(_div[0].scrollHeight);
					}
					
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
						if (result.key == 1) {
							// capture chart
							inspect_charts[result.conf].push("capture", x, y);
						} else if (result.key == 2) {
							// shooter chart
							inspect_charts[result.conf].push("shooter", x, y);
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