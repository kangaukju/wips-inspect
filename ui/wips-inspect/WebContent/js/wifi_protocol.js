/**
 * 
 */

function make_frametype_selectbox(target) {
	target.empty();
	
	target.append("<option value=''>none</option>");
	var $group1 = $("<optgroup label='Management Frames'>");
	$group1.append("<option value='00'>Association Request</option>");
	$group1.append("<option value='01'>Association Response</option>");
	$group1.append("<option value='02'>Reassociation Request</option>");
	$group1.append("<option value='03'>Reassociation Response</option>");
	$group1.append("<option value='04'>Probe Request</option>");
	$group1.append("<option value='05'>Probe Response</option>");
	$group1.append("<option value='08'>Beacon</option>");
	$group1.append("<option value='09'>ATIM</option>");
	$group1.append("<option value='0a'>Disassociation</option>");
	$group1.append("<option value='0b'>Authentication</option>");
	$group1.append("<option value='0c'>Deauthentication</option>");
	$group1.append("<option value='0d'>Action</option>");
	target.append($group1);
	
	var $group2 = $("<optgroup label='Control Frames'>");
	$group2.append("<option value='18'>Block Ack Request</option>");
	$group2.append("<option value='19'>Block Ack</option>");
	$group2.append("<option value='1a'>PS-Poll</option>");
	$group2.append("<option value='1b'>RTS</option>");
	$group2.append("<option value='1c'>CTS</option>");
	$group2.append("<option value='1d'>ACK</option>");
	$group2.append("<option value='1e'>CF End</option>");
	$group2.append("<option value='1f'>CF End Ack</option>");
	target.append($group2);
	
	var $group3 = $("<optgroup label='Data Frames'>");
	$group3.append("<option value='20'>Data</option>");
	$group3.append("<option value='21'>Data Ack</option>");
	$group3.append("<option value='22'>Data Poll</option>");
	$group3.append("<option value='23'>Data Ack Poll</option>");
	$group3.append("<option value='24'>NULL</option>");
	$group3.append("<option value='25'>ACK</option>");
	$group3.append("<option value='26'>Poll</option>");
	$group3.append("<option value='27'>Ack Poll</option>");
	$group3.append("<option value='28'>Qos Data</option>");
	$group3.append("<option value='29'>Qos Data Ack</option>");
	$group3.append("<option value='2a'>Qos Data Poll</option>");
	$group3.append("<option value='2b'>Qos Data Ack Poll</option>");
	$group3.append("<option value='2c'>Qos NULL</option>");
	$group3.append("<option value='2e'>Qos Poll</option>");
	$group3.append("<option value='2f'>Qos Ack</option>");
	target.append($group3);
}

function get_frametype_string(_type) {
	var type = parseInt(_type);
	
	switch (type) {
	case 0:
		return "MGNT";
	case 1:
		return "CTRL";
	case 2:
		return "DATA";
	}
	return "";
}

function get_framesubtype_string(_type, _subtype) {
	var type    = parseInt(_type);
	var subtype = parseInt(_subtype);
	
	switch (type) {
	case 0:
		switch (subtype) {
		case 0: return "Assoc-Req";
		case 1: return "Assoc-Resp";
		case 2: return "Reassoc-Req";
		case 3: return "Reassoc-Resp";
		case 4: return "Probe-Request";
		case 5: return "Probe-Request";
		case 8: return "Beacon";
		case 9: return "ATIM";
		case 10: return "Disassoc";
		case 11: return "Auth";
		case 12: return "Deauth";
		case 13: return "Action";
		}
		break;
	case 1:
		switch (subtype) {
		case 8: return "Block-Ack-Req";
		case 9: return "Block-Ack";
		case 10: return "PS-Poll";
		case 11: return "RTS";
		case 12: return "CTS";
		case 13: return "ACK";
		case 14: return "CF-End";
		case 15: return "CF-End-Ack";		
		}
		break;
	case 2:
		switch (subtype) {
		case 0: return "Data";
		case 1: return "Data-Ack";
		case 2: return "Data-Poll";
		case 3: return "Data-Ack-Poll";
		case 4: return "NULL";
		case 5: return "ACK";
		case 6: return "Poll";
		case 7: return "Ack-Poll";
		case 8: return "Qos-Data";
		case 9: return "Qos-Data-Ack";
		case 10: return "Qos-Data-Poll";
		case 11: return "Qos-Data-Ack-Poll";
		case 12: return "Qos-NULL";
		case 14: return "Qos-Poll";
		case 15: return "Qos-Ack";
		}
		break;
	}
	return "";
}

function get_ds_string(_ds, is_data) {
	var ds = parseInt(_ds);
	if (0 == ds) {
		if (is_data == undefined || is_data == true) {
			return "ADHOC"; 
		} else {
			return "MGNT"
		}
	} else if (1 == ds) {
		return "ToDS";
	} else if (2 == ds) {
		return "FromDS";
	} else if (3 == ds) {
		return "WDS";
	}
	return "";
}

function get_fromtods_string(_fromds, _tods) {
	var fromds = parseInt(_fromds);
	var tods   = parseInt(_tods);	
	var ds     = (fromds << 1) | tods;
	
	switch (ds) {
	case 0: return "ADHOC (MGNT)"
	case 1: return "ToDS";
	case 2: return "FromDS";
	case 4: return "WDS";
	}
	return "";
}