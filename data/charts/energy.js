var chartEnergyRT = new Highcharts.Chart({
    chart: { renderTo: 'chart-energy-rt' },
    title: { text: 'Percentage of Water Heating from Solar' },
    series: [{
        type: "area",
        showInLegend: false,
        name: "Solar Thermal Input",
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
        title: { text: 'Energy from Solar [%]' },
    },
    credits: { enabled: false },
});
setInterval(function () {
    let xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if(this.readyState == 4 && this.status == 200)
        {
            let x = (new Date()).getTime(),
                y = parseFloat(this.responseText);
                if (chartEnergyRT.series[0].data.length > 20) {
                    chartEnergyRT.series[0].addPoint([x, y], true, true, true);
                } else {
                    chartEnergyRT.series[0].addPoint([x, y], true, false, true);
                }

        }
    }
    xhttp.open("GET", "/energy-rt", true); //Open the data request
    xhttp.send();   //Send the data request
}, 6000);

var chartEnergyHR = new Highcharts.Chart({
    chart: { renderTo: 'chart-energy-hour' },
    title: { text: 'Water Heating Energy' },
    series: [{
        type: "line",
        showInLegend: true,
        name: "Energy from Solar",
        data: []
    },
    {
        type: "line",
        showInLegend: true,
        name: "Energy from Hybrid HW Tank",
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
        title: { text: 'Energy (Wh)' }
    },
    credits: { enabled: false },
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
    title: { text: 'Water Heating Energy' },
    series: [{
        type: "line",
        showInLegend: true,
        name: "Energy from Solar",
        data: []
    },
    {
        type: "line",
        showInLegend: true,
        name: "Energy from Hybrid HW Tank",
        data: []
    }],
    plotOptions: {
        line: {
            animation: false,
            dataLabels: { 
                enabled: true,
                format: '{point.y:.0f}'
            }
        },
    },
    tooltip: {
        valueDecimals: 2
    },
    xAxis: {
        title: { text: 'Time' },
        type: 'datetime',
        dateTimeLabelFormats: { minute: '%H:%M' }
    },
    yAxis: {
        title: { text: 'Energy (Wh)' }
    },
    credits: { enabled: false },
});