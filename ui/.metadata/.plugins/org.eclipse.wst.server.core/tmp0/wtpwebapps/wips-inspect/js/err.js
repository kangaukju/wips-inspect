/**
 * 
 */

function ajax_err_handle(e, errmsg) {
	if (e == undefined || 
		e == null || 
		e == "") {
		alert("Error! Unknown");
		return;
	}
	if (e.status == 200) {
		return;
	}
	console.log(e);
	if (errmsg == undefined || errmsg == null) {
		errmsg = "";
	}
	var msg = e.responseText.trim();
	if (msg == "") {
		msg = "Error! "+errmsg;
	} else {
		msg = "Error! "+msg;
	}
	alert(msg);
}