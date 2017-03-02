/**
 * require jquery
 */

function _id_str(to, id, extra) {
	if (extra == null || extra == undefined) {
		return "#"+to+"_"+id;
	}
	return "#"+to+"_"+id+" "+extra;	
}

function _class_str(to, id, extra) {
	if (extra == null || extra == undefined) {
		return "."+to+"_"+id;	
	}
	return "."+to+"_"+id+" "+extra;
}

function _id(to, id, extra) {
	return $(_id_str(to, id, extra));
}

function _class(to, id, extra) {
	return $(_class_str(to, id, extra));
}

function _val(to, id, val) {
	if (val == null || val == undefined) {
		return _id(to, id).val();	
	}
	return _id(to, id).val(val);
}

function _int(val) {
	if (val == null || val == undefined) {
		return 0;
	}
	return parseInt(val);
}

function _isnull(val) {
	if (val == null || 
			val == undefined || 
			val == "" || 
			val.length == 0) {
		return true;
	}
	return false;
}

function _null(val) {
	if (_isnull(val)) {
		return null;
	}
	return val;
}

//
function extract_profile_id(tree_id) {
	var profile_id = "";
	if (tree_id.startsWith("p_")) {
		profile_id = tree_id.substring(2);
	} else if (tree_id.startsWith("c_")) {
		tree_id = tree_id.substring(2);
		profile_id = tree_id.split("_")[0];	
	}
	return profile_id;
}

function extract_config_id(tree_id) {
	var config_id = "";
	if (tree_id.startsWith("c_")) {
		tree_id = tree_id.substring(2);
		config_id = tree_id.split("_")[1];	
	}
	return config_id;
}