var source_temp_rt;
var preheat_temp_rt;
var hot_temp_rt;
var flow_rt;

var chartEnergyRT = new Highcharts.Chart({
    chart: { renderTo: 'chart-energy-rt' },
    title: { text: 'Power Monitoring' },
    series: [{
        type: "line",
        showInLegend: true,
        name: "Solar Power Input",
        data: []
    },
    {
        type: "line",
        showInLegend: true,
        name: "Hybrid Tank Power Input",
        data: []
    }],
    plotOptions: {
        line: {
            animation: false,
            dataLabels: { enabled: true }
        },
    },
    xAxis: {
        title: { text: 'Time' },
        type: 'datetime',
        dateTimeLabelFormats: { second: '%H:%M:%S' }
    },
    yAxis: {
        title: { text: 'Power (kW)' }
    },
    credits: { enabled: false }
});
setInterval(function () {
    let x = (new Date()).getTime();
    if (chartEnergyRT.series[0].data.length > 20) {
        chartEnergyRT.series[0].addPoint([x, ((preheat_temp_rt-source_temp_rt)*4.186*flow_rt/60)], true, true, true);
        chartEnergyRT.series[1].addPoint([x, ((hot_temp_rt-preheat_temp_rt)*4.186*flow_rt/60)], true, true, true);
    } else {
        chartEnergyRT.series[0].addPoint([x, ((preheat_temp_rt-source_temp_rt)*4.186*flow_rt/60)], true, false, true);
        chartEnergyRT.series[1].addPoint([x, ((hot_temp_rt-preheat_temp_rt)*4.186*flow_rt/60)], true, false, true);
    }
}, 6500);


var source_temp_hr;
var preheat_temp_hr;
var hot_temp_hr;
var flow_hr;
var chartEnergyHR = new Highcharts.Chart({
    chart: { renderTo: 'chart-energy-hour' },
    title: { text: 'Energy Consumption and Generation' },
    series: [{
        type: "line",
        showInLegend: true,
        name: "Solar Energy Generated",
        data: []
    },
    {
        type: "line",
        showInLegend: true,
        name: "Hybrid Tank Energy In",
        data: []
    }],
    plotOptions: {
        line: {
            animation: false,
            dataLabels: { enabled: true }
        },
    },
    xAxis: {
        title: { text: 'Time' },
        type: 'datetime',
        dateTimeLabelFormats: { minute: '%H:%M' }
    },
    yAxis: {
        title: { text: 'Energy (kWh)' }
    },
    credits: { enabled: false }
});
setInterval(function () {
    let x = (new Date()).getTime();
    if (chartEnergyHR.series[i].data.length > 30) {
        chartEnergyHR.series[0].addPoint([x, ((preheat_temp_hr-source_temp_hr)*4.186*flow_hr)/3600], true, true, true);
        chartEnergyHR.series[1].addPoint([x, ((hot_temp_hr-preheat_temp_hr)*4.186*flow_hr)/3600], true, true, true);
    } else {
        chartEnergyHR.series[0].addPoint([x, ((preheat_temp_hr-source_temp_hr)*4.186*flow_hr)/3600], true, false, true);
        chartEnergyHR.series[1].addPoint([x, ((hot_temp_hr-preheat_temp_hr)*4.186*flow_hr)/3600], true, false, true);
    }
},120500);

var chartEnergyDay = new Highcharts.Chart({
    chart: { renderTo: 'chart-energy-day' },
    title: { text: 'Energy Consumption and Generation' },
    series: [{
        type: "line",
        showInLegend: true,
        name: "Solar Energy Generated",
        data: []
    },
    {
        type: "line",
        showInLegend: true,
        name: "Hybrid Tank Energy In",
        data: []
    }],
    plotOptions: {
        line: {
            animation: false,
            dataLabels: { enabled: true }
        },
    },
    xAxis: {
        title: { text: 'Time' },
        type: 'datetime',
        dateTimeLabelFormats: { minute: '%H:%M' }
    },
    yAxis: {
        title: { text: 'Energy (kWh)' }
    },
    credits: { enabled: false }
});

