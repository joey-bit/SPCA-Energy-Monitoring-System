var chartEnergyRT = new Highcharts.Chart({
    chart: { renderTo: 'chart-energy-rt' },
    title: { text: 'Energy Monitoring' },
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
        type: 'datetime',
        dateTimeLabelFormats: { second: '%H:%M:%S' }
    },
    yAxis: {
        title: { text: 'Energy (kWh)' }
    },
    credits: { enabled: false }
});
setInterval(function () {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            var x = (new Date()).getTime(),
                y = parseFloat(this.responseText);
            for (let i = 0; i < 2; i++) {
                if (chartEnergyRT.series[i].data.length > 20) {
                    chartEnergyRT.series.addPoint([x, y[i]], true, true, true);
                } else {
                    chartEnergyRT.series.addPoint([x, y[i]], true, false, true);
                }
            }
        }
    };
    xhttp.open("GET", "/energy-rt", true);
    xhttp.send();
}, 3000);

function fillHistorical() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) { //When ready to receive
        var x = (new Date()).getTime(), //Current time
            y = this.responseText.split(',').map(Number); //Get the data as an array of strings like "1,2,3,4,5"
        for (let i = 0; i < 5; i++) {
            chartTempRT.series[i].setData(y[i]);
        }
    }
    }
}