
<script type="text/javascript">

$(function() {
	$(".link").click(function() {
		var param = "";
		var call = $(this).attr("call");
		if (call != undefined) {
			param = eval(call+"()")
			param = "?"+eval(call+"()");
		}
		var page = $(this).attr("page");
		if (page != undefined) {
			$("body").animate({
				"opacity": "0",
				"top": "10px"
			},400, function () {
				location.href = page+param;
			});
		}
	});
});

</script>