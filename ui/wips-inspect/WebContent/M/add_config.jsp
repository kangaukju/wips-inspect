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
	padding: 6px;
}
</style>
</head>
<body>
	<fieldset class="main_fieldset">
		<legend>New Config</legend>
		<div>
			<input type="text" id="new_config_name">
			<img class="action_img" src="/img/M/save.png" id="save_config">
		</div>
		
		<form id="config_form">
			<div id="tabs" class="tab_header">
				<ul>
					<li>
						<a href="#shooter_tab">
							&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
							Shooter
							&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
						</a>
						<img src="/img/gree_add.png" class="table_img add_config_img" target="shooter_tab">
					</li>
					<li>
						<a href="#capture_tab">
							&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
							Capture
							&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
						</a>
						<img src="/img/gree_add.png" class="table_img add_config_img" target="capture_tab">
					</li>
				</ul>
				<div id="shooter_tab">
				</div>
				<div id="capture_tab">
				</div>
			</div>
		</form>
	</fieldset>

	<%-- sample tag html --%>
	<table id="sample_tag">
		<thead></thead>
		<tbody>
			<tr>
				<td></td>
				<td align="right"><img src="/img/red_remove.png" class="table_img del_tag_img"></td>
			</tr>
			<tr>
				<td>id</td>
				<td><input type="text" name="tag_id"></td>
			</tr>
			<tr>
				<td>length</td>
				<td><input type="text" name="tag_length"></td>
			</tr>
			<tr>
				<td>value</td>
				<td><input type="text" name="tag_value"></td>
			</tr>
		</tbody>
	</table>

	<%-- sample config html --%>
	<fieldset id="sample_config" class="sub_fieldset sample_config" style="display: none;">
		<legend>Config</legend>
		<img class="table_img del_config_img" src="/img/M/trash.png" align="right">
		<table>
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
					<td valign="top">tag <img src="/img/gree_add.png" class="table_img add_tag_img"></td>
					<td>
						<div class="tag_div"></div>
					</td>
				</tr>
			</tbody>
		</table>
	</fieldset>

	<a id="submit" href="#">submit</a>
</body>

<script type="text/javascript">
$(document).ready(function() {
	$( "#tabs" ).tabs({
	});
	
	
	$(".add_tag_img").click(function() {
		var c = $("#sample_tag").clone(true);
		c.find("input").val("");
		var $pp = $(this).parent().parent();
		var $t = $pp.find(".tag_div");
		c.css({display:'none'});
		c.prependTo($t).show("slow");
	});
	
	$(".del_tag_img").click(function() {
		var $pppp = $(this).parent().parent().parent().parent();
		$pppp.hide("slow", function() { $pppp.remove(); });
	});
	
	$(".add_config_img").click(function() {
		var c = $("#sample_config").clone(true);
		c.find("input").val("");
		var t = $(this).attr("target");
		c.css({display:'none'});
		c.prependTo($("#"+t)).slideDown("slow");
	});
	
	$(".del_config_img").click(function() {
		var $p = $(this).parent();
		$p.slideUp("slow", function() { $p.remove(); });
	});
	
	$("#submit").click(function() {
		$.post("submit.jsp", $("#config_form").serialize())
			.done(function(result) {
				alert("ok");
		});
	});
	
	
	$(".addr").hide();
	$(".address").change(function() {
		var $p = $(this).parent();
		
		$p.find(".addr").hide();
		
		var type = $(this).val();
		if (type == "any_addr") {
			$p.find(".any_addr").slideDown("slow");
		} else if (type == "apsta") {
			$p.find(".apsta").slideDown("slow");
		} else if (type == "addr1") {
			$p.find(".addr1").slideDown("slow");
		} else if (type == "addr2") {
			$p.find(".addr2").slideDown("slow");
		} else if (type == "addr3") {
			$p.find(".addr3").slideDown("slow");
		} else if (type == "addr4") {
			$p.find(".addr4").slideDown("slow");
		}
	});	
});
</script>
<%@include file="/M/footer.jsp"%>
</html>
