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
    let xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if(this.readyState == 4 && this.status == 200)
        {
            let x = (new Date()).getTime(),
                y = this.responseText.split(',').map(Number);
                if (chartEnergyRT.series[0].data.length > 20) {
                    chartEnergyRT.series[0].addPoint([x, y[0]/100], true, true, true);
                    chartEnergyRT.series[1].addPoint([x, y[1]/100], true, true, true);
                } else {
                    chartEnergyRT.series[0].addPoint([x, y[0]/100], true, false, true);
                    chartEnergyRT.series[1].addPoint([x, y[1]/100], true, false, true);
                }

        }
    }
    xhttp.open("GET", "/energy-rt", true); //Open the data request
    xhttp.send();   //Send the data request
}, 6000);

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
    let xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if(this.readyState == 4 && this.status == 200)
        {
            let x = (new Date()).getTime(),
                y = this.responseText.split(',').map(Number);
                if (chartEnergyHR.series[0].data.length > 30) {
                    chartEnergyHR.series[0].addPoint([x, y[0]/100], true, true, true);
                    chartEnergyHR.series[1].addPoint([x, y[1]/100], true, true, true);
                } else {
                    chartEnergyHR.series[0].addPoint([x, y[0]/100], true, false, true);
                    chartEnergyHR.series[1].addPoint([x, y[1]/100], true, false, true);
                }

        }
    }
    xhttp.open("GET", "/energy-hr", true); //Open the data request
    xhttp.send();   //Send the data request
},120000);

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