<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<%@include file="/M/header.jsp"%>
<style type="text/css">
.hidden {
	display: none;
}
</style>
</head>
<body>
	<fieldset class="main_fieldset">
		<legend>Config List</legend>
		<div align="right">
			<img class="action_img" src="/img/M/add-notes.png" id="add_config">
			<img class="action_img" src="/img/M/edit-notes.png" id="modify_config">
		</div>
		
		<table id="config_list_table">
			<thead>
				<tr>
					<th width="20px;"></th>
					<th>name</th>
					<th>capture</th>
					<th>shooter</th>
					<th>updated</th>
					<th></th>
				</tr>
			</thead>
			<tbody></tbody>
		</table>
	</fieldset>
</body>

<script type="text/javascript">
var config_list_table_xlat = {
		tag: "tr",
		id: "$\{id\}",
		class:"pointer",
		children: [
			{tag:"td", html:"<img class='config_check_img hidden table_img' src='/img/M/ok.png' id='img_$\{id\}'>"},
			{tag:"td", html:"$\{name\}"},
			{tag:"td", html:"$\{captureAirConf.desc\}"},
			{tag:"td", html:"$\{shooterAirConf.desc\}"},
			{tag:"td", html:"$\{updated\}"},
			{tag:"td", html:"<img class='config_del_img table_img' src='/img/M/trash.png'>"},
		]
};

function load_config_list() {
	jQuery.ajax({
		url: "/get_config.jsp",
		data: {detail: 'true'},
		cache: false,
		dataType: "json",
		success: function(result) {
			if (result.good == false) {
				alert("load_config_list: "+result.cause);
				return;
			}
			$("#config_list_table > tbody").html(
				json2html.transform(result, config_list_table_xlat)
			);
			$("#config_list_table").tablesorter({
				headers: {
					0: {sorter: false},
					5: {sorter: false},
				}
			}).trigger("update");
			$("#config_list_table > tbody > tr").click(function(e) {
				$("#config_list_table > tbody > tr").removeClass('selected');				
				$(this).toggleClass('selected');
				$(".config_check_img").addClass('hidden');
				$("#img_"+$(this).attr("id")).toggleClass('hidden');
			});
		},
		error: function(e) {
			ajax_err_handle(e);
		}
	});
}


$(document).ready(function() {
	load_config_list();
	
	$("#modify_config").click(function() {
		gogo("modify_config.jsp");
	});
	
	$("#add_config").click(function() {
		gogo("add_config.jsp");
	});
	
});
</script>
<%@include file="/M/footer.jsp"%>
</html>
