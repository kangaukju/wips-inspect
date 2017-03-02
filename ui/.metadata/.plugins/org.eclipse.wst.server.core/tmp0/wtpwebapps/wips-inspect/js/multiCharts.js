/**
 * multiCharts
 * dependencies: c3.js, d3.js, jquery
 */

var multiCharts = {
	height: 150,
	width: 800,
	gradations: 500, // gradations count
	ids: [], // chart div name
	items: [],	
	_charts: {},
	
	clear: function(id, item) {
		var x = this._column_dic[item+"_x"];
		var y = this._column_dic[item+"_y"];
		
		x.splice(1, x.length); // or x.length = 1 ?
		y.splice(1, y.length);
		
		this._charts[id].load({
			columns: [x, y]
		});
	},
	
	push: function(id, item, xdata, ydata) {
		var x = this._column_dic[item+"_x"];
		var y = this._column_dic[item+"_y"];
		
		x.push(xdata);
		y.push(ydata);
		
		this._charts[id].load({
			columns: [x, y]
		});
	},
	
	_xs: {},
	_build_xs: function() {
		this._xs = { "base_y": "base_x" };
		for (var i=0; i<this.items.length; i++) {
			this._xs[this.items[i]+"_y"] = this.items[i]+"_x";
		}
	},
	
	_columns: [],
	_column_dic: [],
	_build_columns: function() {
		var base_x = ['base_x', ];
		var base_y = ['base_y', ];
		for (var i=0; i<this.gradations; i++) {
			base_x.push(i);
			if (i % 100 == 0) {
				base_y.push(-90);
			} else {
				base_y.push(0);
			}
		}
		this._columns = [ base_x, base_y ];
		for (var i=0; i<this.items.length; i++) {
			var x = [this.items[i]+"_x", ];
			var y = [this.items[i]+"_y", ];
			this._columns.push(x);
			this._columns.push(y);
			this._column_dic[this.items[i]+"_x"] = x;
			this._column_dic[this.items[i]+"_y"] = y;
		}
	},
	
	_types: {},
	_build_types: function() {
		this._types = { "base_y": "bar" };
		for (var i=0; i<this.items.length; i++) {
			this._types[this.items[i]+"_y"] = "line";
		}
	},
	
	_names: {},
	_build_names: function() {
		this._names = { "base_y": "sec" };
		for (var i=0; i<this.items.length; i++) {
			this._names[this.items[i]+"_y"] = this.items[i];
		}		
	},
	
	_create_chart: function(n) {
		return c3.generate({
			bindto: "#"+this.ids[n],
			size: {
				height: this.height,
				width:  this.width
			},
			data: {
				xs: this._xs, 
				columns: this._columns,
				types: this._types,
				names: this._names,
			},
			transition: {
				duration: 0,
			},
			axis: {
				x: {
					tick: {
						format: function(x) {
							return (x/100);
						},
						culling: { max: 5 }
					}
				},
				y: {
					tick: {
						values: [0, -45, -90]
					},
					show: true,
				}
			},
			bar: {
				width: 2
			},
			zoom: {
				enabled: true
			},
		    tooltip: {
				grouped: false,
				format: {
					title: function (d) { return (d/100) + " sec"; },
					value: function (value, ratio, id) {
						if (id === "base_y") {
							return undefined;
						} else {
							return value;
						}
					}
				}
		    }
		});
	},
	generate: function() {
		this._build_xs();
		this._build_columns();
		this._build_types();
		this._build_names();
		this._charts = [];
		for (var i=0; i<this.ids.length; i++) {
			this._charts[this.ids[i]] = this._create_chart(i);
		}
	}
};




/*
var aircapture_msec_x = ['msec_x', ];
var aircapture_msec_y = ['msec_y', ];
var aircapture_x = ['x', ];
var aircapture_y = ['y', ];
var aircapture_result_chart = c3.generate({
	bindto: '#aircapture_result_chart',
	size: {
		height: 150,
		width:  800
	},
	data : {
		xs: {
			"msec_y": "msec_x",
			"y": "x",
		},
		columns : [ 
			aircapture_msec_x,
			aircapture_msec_y,
			aircapture_x,			
			aircapture_y 
		],
		types: {
			msec_y: "bar",
			y: "line",
		},
		names: {
			y: "signal",
			msec_y: "sec",
		},
	},
	axis: {
		x: {
			tick: {
				format: function(x) {
					return (x/100);
				},
				culling: { max: 5 }
			}
		},
		y: {
			tick: {
				values: [0, -45, -90]
			},
			show: true,
		}
	},
	bar: {
		width: 2
	},
    tooltip: {
		grouped: false,
		format: {
			title: function (d) { return (d/100) + " sec"; },
			value: function (value, ratio, id) {
				if (id === "msec_y") {
					return undefined;
				} else {
					return value;
				}
			}
		}
    },
	zoom: {
		enabled: true
	}
});
*/