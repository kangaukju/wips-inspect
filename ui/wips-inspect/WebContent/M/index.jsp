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
	<div align="center">
		<img src="/img/M/quality-control.jpg" style="height: 300px;">
	</div>
	<div>
		<table class="home_contain">
			<tr>
				<td>
					<img class="home_img link" id="users"   src="/img/M/analytics2.png" >
					<img class="home_img link" id="inspect" src="/img/M/analytics_balls.svg">
					<img class="home_img link" id="profile" src="/img/M/edit2.svg">		
					<img class="home_img link" id="config"  src="/img/M/config2.svg">
				</td>
			</tr>
		</table>
	</div>
</body>

<script type="text/javascript">
$(function() {
	$("#inspect").click(function() {
		gogo("/M/inspect.jsp");
	});
	$("#profile").click(function() {
		gogo("/M/profile.jsp");
	});
	$("#config").click(function() {
		gogo("/M/config.jsp");
	});
});
</script>
<%@include file="/M/footer.jsp"%>
</html>