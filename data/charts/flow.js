var chartFlowRT = new Highcharts.Chart({
    chart: { renderTo: 'chart-flow-rt' },
    title: { text: 'Hot Water Flow Rate' },
    series: [{
        type: "line",
        showInLegend: true,
        name: "Hot Water Flow",
        data: []
    }],
    plotOptions: {
        line: {
            animation: false,
            dataLabels: { enabled: true }
        },
    },
    xAxis: {
        type: 'datetime',
    },
    yAxis: {
        title: { text: 'Flow (L/min)' }
    },
    credits: { enabled: false }
});
setInterval(function () {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            var x = (new Date()).getTime(),
                y = parseFloat(this.responseText);
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
        showInLegend: true,
        name: "Hot Water Flow",
        data: []
    }],
    plotOptions: {
        line: {
            animation: false,
            dataLabels: { enabled: true }
        },
    },
    xAxis: {
        type: 'datetime',
    },
    yAxis: {
        title: { text: 'Flow (L/min)' }
    },
    credits: { enabled: false }
});
setInterval(function () {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            var x = (new Date()).getTime(),
                y = parseFloat(this.responseText);
            if (chartFlowRT.series[0].data.length > 30) {
                chartFlowRT.series[0].addPoint([x, y], true, true, true);
            } else {
                chartFlowRT.series[0].addPoint([x, y], true, false, true);
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
        showInLegend: true,
        name: "Hot Water Flow",
        data: []
    }],
    plotOptions: {
        line: {
            animation: false,
            dataLabels: { enabled: true }
        },
    },
    xAxis: {
        type: 'datetime',
    },
    yAxis: {
        title: { text: 'Flow (L/min)' }
    },
    credits: { enabled: false }
});