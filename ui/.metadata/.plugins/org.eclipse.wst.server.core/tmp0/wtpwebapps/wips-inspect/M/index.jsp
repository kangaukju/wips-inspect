<%@page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
<head>			
	<style type="text/css">
	.home_img {
		width: 100px;
		height: 100px;
		cursor: pointer;
	}
	.home_contain {
		width: 100%;
		height: 100%;
		vertical-align: middle;
		text-align: center;
	}
	</style>
</head>
<%@include file="/M/header.jsp"%>
<body>
	<div align="center" style="height: 200px; width: 100%; padding-top: 50px;">
		<img src="/img/M/wireless-signal.png" style="width: 200px;">
	</div>
	<div>
		<table class="home_contain">
			<tr>
				<td>
					<img class="home_img home_link_users"        id="users"        src="/img/M/analytics2.png" >
					<img class="home_img home_link_inspect"      id="inspect"      src="/img/M/analytics_balls.svg">
					<img class="home_img home_link_profile_list" id="profile_list" src="/img/M/edit2.svg">		
					<img class="home_img home_link_config_list"  id="config_list"  src="/img/M/config4.svg">
					<img class="home_img home_link_wifi_search"  id="wifi_search"  src="/img/M/wifi_rader.png">
				</td>
			</tr>
		</table>
	</div>
</body>

<script type="text/javascript">
$(document).ready(function() {
	load_ok();
});
</script>
<%@include file="/M/footer.jsp"%>
</html>
