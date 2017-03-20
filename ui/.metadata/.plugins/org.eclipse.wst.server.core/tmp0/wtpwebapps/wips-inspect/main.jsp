<%@page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
<head>			
	<style type="text/css">
	.home_img {
		width: 100px;
		height: 100px;
		cursor: pointer;
		-webkit-transform: scale(1);
		transform: scale(0.8);
		-webkit-transition: .3s ease-in-out;
		transition: .3s ease-in-out;
	}
	.home_img:hover {
		-webkit-transform: scale(1.0);
		transform: scale(1.0);
	}
	.home_contain {
		width: 100%;
		height: 100%;
		vertical-align: middle;
		text-align: center;
	}
	</style>
</head>
<%@include file="header.jsp"%>
<body>
	<div align="center" style="height: 200px; width: 100%; padding-top: 50px;">
		<img src="/img/wireless-signal.png" style="width: 200px;">
	</div>
	<div align="center">
		<table class="home_contain" style="width: 600px;">
			<tr>
				<td>
					<img class="home_img home_link_users"        id="users"        src="/img/users.png" >
				</td>
				<td>	
					<img class="home_img home_link_inspect"      id="inspect"      src="/img/inspect.png">
				</td>
				<td>
					<img class="home_img home_link_profile_list" id="profile_list" src="/img/profiles.png">
				</td>
				<td>		
					<img class="home_img home_link_config_list"  id="config_list"  src="/img/configs.png">
				</td>
				<td>
					<img class="home_img home_link_wifi_search"  id="wifi_search"  src="/img/wifi_search.png">
				</td>
				<td>
					<img class="home_img home_link_setting_list" id="setting"      src="/img/setting.png">
				</td>				
			</tr>
			<tr>
				<td>Users</td>
				<td>Inspect</td>
				<td>Profile</td>
				<td>Config</td>
				<td>WiFi Search</td>
				<td>Setting</td>
			</tr>
		</table>
	</div>
</body>

<script type="text/javascript">
$(document).ready(function() {
	load_ok();
});
</script>
<%@include file="footer.jsp"%>
</html>
