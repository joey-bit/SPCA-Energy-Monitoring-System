var chartFlowRT = new Highcharts.Chart({
    chart: { renderTo: 'chart-flow-rt' },
    title: { text: 'Hot Water Flow Rate' },
    series: [{
        type: "line",
        showInLegend: false,
        name: "Hot Water Flow",
        data: []
    }],
    plotOptions: {
        line: {
            animation: false,
            dataLabels: { enabled: false }
        },
    },
    xAxis: {
        title: { text: 'Time' },
        type: 'datetime',
        dateTimeLabelFormats: { second: '%H:%M:%S' }
    },
    yAxis: {
        title: { text: 'Flow (L/min)' }
    },
    credits: { enabled: true }
});
setInterval(function () {
    let xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            let x = (new Date()).getTime(),
                y = parseFloat(this.responseText)/100;
            if (chartFlowRT.series[0].data.length > 20) {
                chartFlowRT.series[0].addPoint([x, y], true, true, true);
            } else {
                chartFlowRT.series[0].addPoint([x, y], true, false, true);
            }
        }
    };
    xhttp.open("GET", "/flow-rt", true);
    xhttp.send();
}, 6000);

var chartFlowHR = new Highcharts.Chart({
    chart: { renderTo: 'chart-flow-hour' },
    title: { text: 'Hourly Usage' },
    series: [{
        type: "line",
        showInLegend: false,
        name: "Hot Water Usage",
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
        title: { text: 'Water Used (L)' }
    },
    credits: { enabled: false }
});
setInterval(function () {
    let xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            let x = (new Date()).getTime(),
                y = parseFloat(this.responseText)/100;
            if (chartFlowHR.series[0].data.length > 30) {
                chartFlowHR.series[0].addPoint([x, y], true, true, true);
            } else {
                chartFlowHR.series[0].addPoint([x, y], true, false, true);
            }
        }
    };
    xhttp.open("GET", "/flow-hr", true);
    xhttp.send();
}, 120000);

var chartFlowDay = new Highcharts.Chart({
    chart: { renderTo: 'chart-flow-day' },
    title: { text: 'Daily Usage' },
    series: [{
        type: "line",
        showInLegend: false,
        name: "Hot Water Usage",
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
        title: { text: 'Water Used (L)' }
    },
    credits: { enabled: false }
});