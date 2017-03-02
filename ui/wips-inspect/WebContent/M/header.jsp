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
		background: #1F497D;
    color: #fff;
	}
	table.tablesorter thead tr, table.tablesorter tbody tr {
		cursor: pointer;
    height: 35px;
	}
	table.tablesorter tbody tr.selected td {
    background: none repeat scroll 0 0 #FF9900;
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
	.main_fieldset {
		background: #ddd;
		border: 2px solid #1F497D;
		border-radius: 5px;
		margin-top: 10px;
		margin-bottom: 10px;
		padding: 10px;
	}
	.main_fieldset > legend {
		background: #1F497D;
    color: #fff;
    padding: 5px 10px ;
    font-size: 18px;
    border-radius: 5px;
    box-shadow: 0 0 0 5px #ddd;
    margin-left: 20px;
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
	.title {
		cursor: pointer;
		vertical-align: middle;
		text-align: center;
		padding: 5px;
	}
	.title_img {		
		width: 30px;
	}
	</style>
</head>
<body>
<table class="title">
	<tr>
		<td><img class="title_img" src="/img/M/internet.svg"></td>
		<td><b>WIPS INSPECTOR</b></td>
	</tr>
</table>
<script type="text/javascript">
function gogo(page) {
	$("body").animate({
		"opacity": "0",
		"top": "10px"
	},400, function () {
		location.href = page;
	});
}

$(function() {
	$(".title").click(function() {
		gogo("index.jsp");
	});
});
</script>
</body>