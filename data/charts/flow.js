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
        dateTimeLabelFormats: { second: '%H:%M:%S' }
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
            if (chartFlowRT.series.data.length > 20) {
                chartFlowRT.series.addPoint([x, y], true, true, true);
            } else {
                chartFlowRT.series.addPoint([x, y], true, false, true);
            }
        }
    };
    xhttp.open("GET", "/flow", true);
    xhttp.send();
}, 3000);
