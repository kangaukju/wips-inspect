<%@page import="air.wips.inspect.servlet.HttpGet"%>
<%@page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<%@include file="/M/header.jsp"%>
<style type="text/css">
#tabs {
	padding-left: 0px;
	margin-top: 10px;
	background: none;
	border-width: 0px;
} 
#tabs .ui-tabs-nav { 
	padding-left: 0px;
	background: transparent;
}
#tabs img {
	vertical-align: middle;
}
#tabs .ui-tabs-active {
	background-color: #407bbf;
	border-width: 0px;
}
</style>
</head>
<body>
	<fieldset class="main_fieldset">
		<legend id="config_fieldset">
			<img class="head_img" src="/img/M/edit_banner.svg">
			<span>New Config</span>
		</legend>
		<form id="config_form" method="post">
			<div>
				<input type="hidden" name="config_id" id="config_id">
				<input type="text" name="config_name" id="config_name">
				<img class="action_img" src="/img/M/upload.svg" id="save_config">
				<a id="config_xml_link" href='#config_xml_modal'>
					<img class='action_img' src='/img/M/file-xml2.svg'>
				</a>
			</div>		
		
			<div id="tabs" class="tab_header">
				<ul>
					<li>
						<a href="#shooter">
							&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
							Shooter
							&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
							<img src="/img/M/add2.svg" class="table_img add_config_img" target="shooter">
						</a>
					</li>
					<li>
						<a href="#capture">
							&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
							Capture
							&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
							<img src="/img/M/add2.svg" class="table_img add_config_img" target="capture">
						</a>
					</li>
				</ul>
				<div id="shooter">
				</div>
				<div id="capture">
				</div>
			</div>
		</form>
	</fieldset>

	<%-- sample tag html --%>
	<table id="sample_tag" class="hidden">
		<thead></thead>
		<tbody>
			<tr>
				<td></td>
				<td align="right"><img src="/img/M/trash.svg" class="table_img del_tag_img"></td>
			</tr>
			<tr>
				<td>id</td>
				<td><input type="text" name="tag_id"></td>
			</tr>
			<tr>
				<td>val</td>
				<td><input type="text" name="tag_value"></td>
			</tr>
			<tr>
				<td>type</td>
				<td>
					<select name="tag_type">
						<option value="string">string</option>
						<option value="hex">hex</option>
					</select>
				</td>
			</tr>
			<tr>
				<td>len</td>
				<td><input type="text" name="tag_length"></td>
			</tr>
		</tbody>
	</table>

	<%-- sample config html --%>
	<fieldset id="sample_config" class="sub_fieldset sample_config hidden">
		<legend>Config</legend>
		<img class="table_img del_config_img" src="/img/M/trash.svg" align="right">
		<input type="hidden" name="target">
		<input type="hidden" name="tags">
		<table align="center">
			<thead></thead>
			<tbody>
				<tr>
					<td width="100px;">enable</td>
					<td>
						<select name="enable">
							<option value="1">enable</option>
							<option value="0">disable</option>
						</select>
					</td>
				</tr>
				<tr>
					<td>desc</td>
					<td><input type="text" name="desc"></td>
				</tr>
				<tr>
					<td>dwell (msec)</td>
					<td><input type="text" name="dwell"></td>
				</tr>
				<tr>
					<td>channel</td>
					<td><input type="text" name="channel"></td>
				</tr>
				<tr>
					<td>frame</td>
					<td>
						<select name="frame" data-placeholder="Choose a Frame Type..." class="frametype">
							<option value=''>Not set</option>
							<optgroup label='Management Frames'>
								<option value='00'>Association Request</option>
								<option value='01'>Association Response</option>
								<option value='02'>Reassociation Request</option>
								<option value='03'>Reassociation Response</option>
								<option value='04'>Probe Request</option>
								<option value='05'>Probe Response</option>
								<option value='08'>Beacon</option>
								<option value='09'>ATIM</option>
								<option value='0a'>Disassociation</option>
								<option value='0b'>Authentication</option>
								<option value='0c'>Deauthentication</option>
								<option value='0d'>Action</option>
							</optgroup>
							<optgroup label='Control Frames'>
								<option value='18'>Block Ack Request</option>
								<option value='19'>Block Ack</option>
								<option value='1a'>PS-Poll</option>
								<option value='1b'>RTS</option>
								<option value='1c'>CTS</option>
								<option value='1d'>ACK</option>
								<option value='1e'>CF End</option>
								<option value='1f'>CF End Ack</option>
							</optgroup>
							<optgroup label='Data Frames'>
								<option value='20'>Data</option>
								<option value='21'>Data Ack</option>
								<option value='22'>Data Poll</option>
								<option value='23'>Data Ack Poll</option>
								<option value='24'>NULL</option>
								<option value='25'>ACK</option>
								<option value='26'>Poll</option>
								<option value='27'>Ack Poll</option>
								<option value='28'>Qos Data</option>
								<option value='29'>Qos Data Ack</option>
								<option value='2a'>Qos Data Poll</option>
								<option value='2b'>Qos Data Ack Poll</option>
								<option value='2c'>Qos NULL</option>
								<option value='2e'>Qos Poll</option>
								<option value='2f'>Qos Ack</option>
							</optgroup>
						</select>
					</td>
				</tr>
				<tr>
					<td>ds</td>
						<td>
							<select name="ds">
								<option value="">Not Set</option>
								<option value="00">ADHOC (management)</option>
								<option value="10">From DS</option>
								<option value="01">To DS</option>
								<option value="11">WDS</option>
							</select>
						</td>
				</tr>
				<tr>
					<td>addr</td>
					<td>
						<select name="address" class="address">
							<option value="">Not set</option>
							<option value="any_addr">ANY</option>
							<option value="apsta">AP/STA</option>
							<option value="addr1">ADDRESS 1</option>
							<option value="addr2">ADDRESS 2</option>
							<option value="addr3">ADDRESS 3</option>
							<option value="addr4">ADDRESS 4</option>
						</select>
						<div class="addr addr1">
							<table>
								<tr>
									<td>da</td>
									<td><input type="text" name="addr1_da"></td>
								</tr>
							</table>
						</div>
						<div class="addr addr2">
							<table>
								<tr>
									<td>da</td>
									<td><input type="text" name="addr2_da"></td>
								</tr>
								<tr>
									<td>sa</td>
									<td><input type="text" name="addr2_sa"></td>
								</tr>
							</table>
						</div>
						<div class="addr addr3">
							<table>
								<tr>
									<td>bssid</td>
									<td><input type="text" name="addr3_bssid"></td>
								</tr>
								<tr>
									<td>da</td>
									<td><input type="text" name="addr3_da"></td>
								</tr>
								<tr>
									<td>sa</td>
									<td><input type="text" name="addr3_sa"></td>
								</tr>
							</table>
						</div>
						<div class="addr addr4">
							<table>
								<tr>
									<td>da</td>
									<td><input type="text" name="addr4_da"></td>
								</tr>
								<tr>
									<td>sa</td>
									<td><input type="text" name="addr4_sa"></td>
								</tr>
								<tr>
									<td>ta</td>
									<td><input type="text" name="addr4_ta"></td>
								</tr>
								<tr>
									<td>ra</td>
									<td><input type="text" name="addr4_ra"></td>
								</tr>
							</table>
						</div>
						<div class="addr apsta">
							<table>
								<tr>
									<td>ap</td>
									<td><input type="text" name="ap"></td>
								</tr>
								<tr>
									<td>sta</td>
									<td><input type="text" name="sta"></td>
								</tr>
							</table>
						</div>
						<div class="addr any_addr">
							<table>
								<tr>
									<td>any</td>
									<td><input type="text" name="any_addr"></td>
								</tr>
							</table>
						</div>
					</td>
				</tr>
				<tr>
					<td valign="top">tag <img src="/img/M/plus.svg" class="table_img add_tag_img"></td>
					<td>
						<div class="tag_div"></div>
					</td>
				</tr>
			</tbody>
		</table>
	</fieldset>
	
	<div id="config_xml_modal">
		<div style="width: 100%; text-align: center;">
			<img src="/img/M/close.svg" id="btn-close-modal" class="close-config_xml_modal" style="cursor: pointer; width:30px;">
		</div>
		<div id="config_xml_tabs" class="modal-content tab_header" style="background-color:rgba(0,0,0,0); border-color:rgba(0,0,0,0);">
			<ul style="background-color:rgba(0,0,0,0); border-color:rgba(0,0,0,0);">
				<li>
					<a href="#shooter_xml">
						&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
						Shooter
						&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					</a>
				</li>
				<li>
					<a href="#capture_xml">
						&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
						Capture
						&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					</a>
				</li>
			</ul>
			<div id="shooter_xml">
			</div>
			<div id="capture_xml">
			</div>
		</div>
	</div>
	
</body>

<script type="text/javascript">
var config_id = '<%= HttpGet.get(request, "config_id", "")%>';

function bind_add_tag() {
	// tag 설정 추가
	$(".add_tag_img").click(function() {
		var c = $("#sample_tag").clone(true);
		c.find("input").val("");
		var $p2 = pN($(this), 2);
		var $p5 = pN($(this), 5);
		
		var tags = $p5.find("input[name=tags]");
		tags.val(parseInt(tags.val()) + 1);
		
		var $t = $p2.find(".tag_div");
		c.css({display:'none'});
		c.prependTo($t).show("slow");
		c.removeClass("hidden");
	});
}

function bind_del_tag() {
	$(".del_tag_img").click(function() {
		var $p10 = pN($(this), 10);
		var tags = $p10.find("input[name=tags]");
		tags.val(parseInt(tags.val()) - 1);
		
		var $p4 = pN($(this), 4);
		$p4.hide("slow", function() { $p4.remove(); });
	});
}

function bind_add_config() {
	// config 설정 추가
	$(".add_config_img").click(function() {
		var c = $("#sample_config").clone(true);
		c.find(".addr").hide();
		c.find("input").val("");
		c.find(".tag_div").empty();
		var t = $(this).attr("target");
		c.css({display:'none'});
		// shooter or capture 종류 구분
		c.find("input[name=target]").val(t);
		c.find("input[name=tags]").val("0");
		c.prependTo($("#"+t)).slideDown("slow");
		c.removeClass("hidden");
	});
}

function bind_del_config() {
	$(".del_config_img").click(function() {
		var $p1 = pN($(this), 1);
		$p1.slideUp("slow", function() { $p1.remove(); });
	});
}

function bind_change_address() {
	$(".address").change(function() {
		var $p1 = pN($(this), 1);
		
		$p1.find(".addr").hide();
		
		var type = $(this).val();
		if (type == "any_addr") {
			$p1.find(".any_addr").show();
		} else if (type == "apsta") {
			$p1.find(".apsta").show();
		} else if (type == "addr1") {
			$p1.find(".addr1").show();
		} else if (type == "addr2") {
			$p1.find(".addr2").show();
		} else if (type == "addr3") {
			$p1.find(".addr3").show();
		} else if (type == "addr4") {
			$p1.find(".addr4").show();
		}
	});
}

function bind_all() {
	bind_add_tag();
	bind_del_tag();
	bind_add_config();
	bind_del_config();
	bind_change_address();
}

function build_config_list(target, list) {
	$.each(list, function(i, v){
		var c = $("#sample_config").clone(true);
		c.removeClass("hidden");
		c.find("input").val("");
		// shooter or capture 종류 구분
		c.find("input[name=target]").val(target);
		c.find("select[name=enable]").val(v.enable).change();
		c.find("input[name=desc]").val(v.desc);
		c.find("input[name=dwell]").val(v.dwell);
		c.find("input[name=channel]").val(v.channel);
		if (!_isnull(v.type) && !_isnull(v.subtype)) {
			var frame = v.type+""+parseInt(v.subtype).toString(16);
			c.find("select[name=frame]").val(frame).change();
		}
		if (!_isnull(v.fromds) && !_isnull(v.tods)) {
			var ds = v.fromds+""+v.tods;
			c.find("select[name=ds]").val(ds).change();
		}
		var addrtype = "";
		switch (_int(v.addrCount)) {
		case 1:
			c.find("input[name=addr1_da]").val(v.da);
			addrtype = "addr1";
			break;
		case 2:
			c.find("input[name=addr2_da]").val(v.da);
			c.find("input[name=addr2_sa]").val(v.sa);
			addrtype = "addr2";
			break;
		case 3:
			c.find("input[name=addr3_bssid]").val(v.bssid);
			c.find("input[name=addr3_da]").val(v.da);
			c.find("input[name=addr3_sa]").val(v.sa);
			addrtype = "addr3";
			break;
		case 4:
			c.find("input[name=addr4_ra]").val(v.ra);
			c.find("input[name=addr4_ta]").val(v.ta);
			c.find("input[name=addr4_da]").val(v.da);
			c.find("input[name=addr4_sa]").val(v.sa);
			addrtype = "addr4";
			break;
		default:
			if (!_isnull(v.anyAddr)) {
				c.find("input[name=any_addr]").val(v.anyAddr);
				addrtype = "any_addr";	
			}
			if (!_isnull(v.ap)) {
				c.find("input[name=ap]").val(v.ap);
				addrtype = "apsta";
			}
			if (!_isnull(v.sta)) {
				c.find("input[name=sta]").val(v.sta);
				addrtype = "apsta";
			}
		}
		c.find("select[name=address]").val(addrtype).change();
		
		var tag_div = c.find(".tag_div");
		tag_div.empty();
		c.find("input[name=tags]").val("0");
		if (!_isnull(v.taglist)) {
			c.find("input[name=tags]").val(v.taglist.length);
			$.each(v.taglist, function(k, t) {
				var ct = $("#sample_tag").clone(true);
				ct.removeClass("hidden");
				ct.find("input").val("");
				ct.find("input[name=tag_id]").val(t.id);
				ct.find("input[name=tag_length]").val(t.len);
				ct.find("input[name=tag_value]").val(t.data);
				ct.find("select[name=tag_type]").val(t.type);
				tag_div.append(ct);
			});
		}
		
		c.appendTo($("#"+target));
	});
}
$(document).ready(function() {
	$("#tabs").tabs();
	$(".addr").hide();
	
	bind_all();
	
	$("#save_config").click(function() {
		var $config_name = $("#config_name");
		if ($config_name.val() == "") {
			pop("missing config name...", {
				focus: $config_name,
			});
			return;
		}
		
		$("#config_form input[name=tag_id]").each(function() {
			if ($(this).val() == "") {
				var $p10 = pN($(this), 10);
				var t = $p10.find("input[name=target]").val();
				var s = (t == "shooter") ? 0 : 1
				$("#tabs").tabs("option", "active", s);
				$(this).focus();
			}
		});
		
		$.post("add_config.jsp", $("#config_form").serialize())
			.done(function(result) {
				if (result.good == false) {
					pop("Error register config: "+result.cause);
					return;
				} else {
					pop("success registered config: "+$("#config_name").val(), {
						page: "/M/config_list.jsp",
						type: "success",
					});
				}
		}, "json");
	});
		
	$("#config_xml_link").animatedModal({
		modalTarget: 'config_xml_modal',
		animatedIn: 'lightSpeedIn',
		animatedOut: 'bounceOutDown',
		beforeOpen: function() {
			var config_id = $("#config_id").val();
			jQuery.ajax({
				url: "/config_xml.jsp",
				data: { 'config_id': config_id },
				cache: false,
				dataType: "json",
				success: function(result) {
					if (result.good == false) {
						pop("Error loading for config XML.\n"+result.cause);
						return;
					}
					$("#config_xml_tabs").tabs();
					$("#shooter_xml").html(result.shooter);
					$("#capture_xml").html(result.capture);
				},
				error: function(e) {
					//ajax_err_handle(e);
					pop("Error loading for config XML.\nplease retry...");
				}
			});
		},
	});
	
	if (config_id != '') {
		$("#config_fieldset span").html("Edit Config");
		
		jQuery.ajax({
			url: "/get_config.jsp",
			data: {
				id: config_id,
				detail: true,
			},
			cache: false,
			dataType: "json",
			success: function(result) {
				if (result.good == false) {
					pop("Error loading for config: "+result.cause);
					return;
				}
				$("#config_id").val(result.id);
				$("#config_name").val(result.name);
				build_config_list("shooter", result.shooterXmlAirConfList);
				build_config_list("capture", result.captureXmlAirConfList);
				
				load_ok();
			},
			error: function(e) {
				load_ok();
				//ajax_err_handle(e);
				pop("Error loading for config.\nplease retry...");
			}
		});
	}
	else {
		$("#config_xml_link").addClass("hidden");
		load_ok();
	}
});
</script>
<%@include file="/M/footer.jsp"%>
</html>
