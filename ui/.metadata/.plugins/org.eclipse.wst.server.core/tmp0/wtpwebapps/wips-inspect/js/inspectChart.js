/**
 * 
 */

(function (window) {
	'use strict';
	
	var inspectChart = { ver: 0.1};
	
	inspectChart.defaults = {
			count: 1,
			height: 150,
			width: 800,
			max_sec: 5,    // max seconds
			unit_sec: 100, // gradation msec
			items: [],
	};
	
	function def(def, user, item) {
		if (user[item] == null || user[item] == undefined)
			return def[item];
		return user[item];
	}
	function itemX(item) { return item+'_x'; }
	function itemY(item) { return item+'_y'; }
	
	inspectChart.generate = function (pname, config) {
		var $def = inspectChart.defaults;
		var $config;
		if (config == null || config.undefined) {
			$config = inspectChart.defaults;
		} else {
			$config = config;
		}
		
		/************************************
		 * chart
		 ************************************/
		var chart = {
				/* config of char */
				$height    : def($def, $config, 'height'),
				$width     : def($def, $config, 'width'),
				$count     : def($def, $config, 'count'),
				$max_sec   : def($def, $config, 'max_sec'),
				$items     : def($def, $config, 'items'),
				$unit_sec  : def($def, $config, 'unit_sec'),
				
				/* push data */
				push: function(item, x, y, reload) {
					var div = (x / (this.$max_sec * 1000));
					if (this.$charts.length < div) {
						this.appendNewChart();
					}
					
					var dataX = this.$cache[ itemX(item) ];
					var dataY = this.$cache[ itemY(item) ];					
					dataX.push(x);
					dataY.push(y);
					
					if (reload == false) return;
					
					var cur = this.$charts[this.$charts.length - 1];
					cur.load({
						columns: [dataX, dataY],
					});
				},
				
				refresh: function(item) {
					var dataX = this.$cache[ itemX(item) ];
					var dataY = this.$cache[ itemY(item) ];
					
					var cur = this.$charts[this.$charts.length - 1];
					cur.load({
						columns: [dataX, dataY],
					});
				},
				
				/* empty all charts in pname object */
				empty: function() {
					this.$xs = null;
					this.$cache = null;
					this.$columns = null;
					this.$types = null;
					this.$names = null;
					this.$charts = null;
					$('#'+pname).empty();
				},
				
				/* axis */
				$xs: {},
				build_xs: function() {
					this.$xs = null;
					this.$xs = { 'base_y' : 'base_x' };
					
					var it;
					for (var i in this.$items) {
						it = this.$items[i];
						/* ???_y = ???_x */
						this.$xs[itemY(it)] = itemX(it);
					}
				},
				
				/* base columns */
				$cache:[],
				$columns:[],
				build_columns: function() {
					this.$cache = null;
					this.$columns = null;
					//console.log("call build_columns");
					
					var base_x = ['base_x', ]; // header for columns X
					var base_y = ['base_y', ]; // header for columns Y
					var gradation = (1000 / this.$unit_sec); // 10
					var max     = this.$max_sec * gradation * (this.$charts.length + 1);
					var start_x = this.$max_sec * gradation * (this.$charts.length);
					//console.log("start x = "+start_x + " ~ end x = "+max);
					for (var i=start_x; i<max; i++) {
						//console.log(start_x +", " + (i * this.$unit_sec));
						base_x.push(i * this.$unit_sec);
						// Y axis is dbm (range: 0 ~ -90)
						if (i % gradation == 0) {
							base_y.push(-90);
						} else {
							base_y.push(0);
						}
					}
					
					this.$cache = [ ];
					this.$columns = [ base_x, base_y ];
					var it, itX, itY;
					for (var i in this.$items) {
						it = this.$items[i];
						itX = [ itemX(it) ]; this.$columns.push(itX); // header for columns X
						itY = [ itemY(it) ]; this.$columns.push(itY); // header for columns Y
						
						// cache for quick search data
						this.$cache[ itemX(it) ] = itX;
						this.$cache[ itemY(it) ] = itY;
					}
				},
				
				/* kind of chart graph */
				$types: {},
				build_types: function() {
					this.$types = null;
					this.$types = { 'base_y' : 'bar' };
					
					var it;
					for (var i in this.$items) {
						it = this.$items[i];
						this.$types[ itemY(it) ] = 'line';
					}
				},
				
				/* axis names */
				$names: {},
				build_names: function() {
					this.$names = null;
					this.$names = { 'base_y' : 'sec' };
					
					var it;
					for (var i in this.$items) {
						it = this.$items[i];
						this.$names[ itemY(it) ] = it;
					}
				},
				
				/* chart array */
				$charts: [],
				
				/* create new chart */
				newChart: function(bindid) {
					return c3.generate({
						bindto: '#'+bindid,
						size: { height : this.$height, width : this.$width },
						data: {
							xs:      this.$xs,
							columns: this.$columns,
							types:   this.$types,
							names:   this.$names,
						},
						transition: { duration: 0, },
						axis: {
							x: {
								tick: {
									format: function(x) { return (x/1000); },
									culling: { max: this.$max_sec },
									multiline: false,
								}
							},
							y: {
								tick: { values: [0, -45, -90] },
								show: true,
							}
						},
						bar: { width: 2 },
						zoom: { enabled: true },
						tooltip: {
							grouped: false,
							format: {
								title: function (data) { return (data/1000) + " sec"; },
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
				
				newChartId: function() {
					return pname+'_inspect_chart_'+this.$charts.length;
				},
				
				appendNewChart: function() {
					//console.log("call appendNewChart");
					this.build_xs();
					this.build_columns();
					this.build_types();
					this.build_names();
					
					var newid = this.newChartId();
					$("<div>", {
						id: newid,
						class: 'inspectChart',
					}).appendTo($('#'+pname));
					
					//console.log(newid);
					
					var chart = this.newChart(newid);
					this.$charts.push(chart);
					return chart;
				},
		};
		
		
		// first chart
		chart.appendNewChart();
		
		return chart;
	};

	
	if (typeof define === 'function' && define.amd) {
		define("inspectChart", ["d3", "c3"], function () { return inspectChart; });
	} else if ('undefined' !== typeof exports && 'undefined' !== typeof module) {
		module.exports = inspectChart;
	} else {
		window.inspectChart = inspectChart;
	}
	
})(window);