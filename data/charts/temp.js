//Code that creates a chart of realtime temperature data
var chartTempRT = new Highcharts.Chart({
    chart: { renderTo: 'chart-temperature-rt' },
    title: { text: 'Temperature Monitoring' },
    series: [
        {
            type: "line",
            showInLegend: true,
            name: "Glycol",
            data: []
        },
        {
            type: "line",
            showInLegend: true,
            name: "Solar Preheat",
            data: []
        },
        {
            type: "line",
            showInLegend: true,
            name: "Room Ambient",
            data: []
        },
        {
            type: "line",
            showInLegend: true,
            name: "Cold Water",
            data: []
        },
        {
            type: "line",
            showInLegend: true,
            name: "Hot Water",
            data: []
        },
    ],
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
        title: { text: 'Temperature (Celsius)' }
    },
    credits: { enabled: false }
});
//Function that allows the chart to update every 6 seconds
setInterval(function () {
    var xhttp = new XMLHttpRequest(); //Create a data request
    xhttp.onreadystatechange = function () { //Callback function
        if (this.readyState == 4 && this.status == 200) { //When ready to receive
            var x = (new Date()).getTime(), //Current time
                y = this.responseText.split(',').map(Number); //Get the data as an array of floats
            for (let i = 0; i < 5; i++) {
                if (chartTempRT.series[i].data.length > 20) { //If there are more than 20 points
                    chartTempRT.series[i].addPoint([x, y[i]/100], true, true, true); //Add a point and shift
                } else {
                    chartTempRT.series[i].addPoint([x, y[i]/100], true, false, true); //Add a point
                }
            }
        }
    };
    xhttp.open("GET", "/temperature-rt", true); //Open the data request
    xhttp.send();   //Send the data request
}, 6000);  //Repeat every 6 seconds

//Code that creates a chart of historical temperature data (2 minute data)
var chartTempHR = new Highcharts.Chart({
    chart: { renderTo: 'chart-temperature-hour' },
    title: { text: 'Hourly System Temperatures' },
    series: [
        {
            type: "line",
            showInLegend: true,
            name: "Glycol",
            data: []
        },
        {
            type: "line",
            showInLegend: true,
            name: "Solar Preheat",
            data: []
        },
        {
            type: "line",
            showInLegend: true,
            name: "Room Ambient",
            data: []
        },
        {
            type: "line",
            showInLegend: true,
            name: "Hot Water",
            data: []
        },
        {
            type: "line",
            showInLegend: true,
            name: "Cold Water",
            data: []
        },
    ],
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
        title: { text: 'Temperature (Celsius)' }
    },
    credits: { enabled: false }
});
//Function that allows the chart to update every 2 minutes
setInterval(function () {
    var xhttp = new XMLHttpRequest(); //Create a data request
    xhttp.onreadystatechange = function () { //Callback function
        if (this.readyState == 4 && this.status == 200) { //When ready to receive
            var x = (new Date()).getTime(), //Current time
                y = this.responseText.split(',').map(Number); //Get the data as an array of floats
            for (let i = 0; i < 5; i++) {
                if (chartTempHR.series[i].data.length > 30) { //If there are more than 30 points
                    chartTempHR.series[i].addPoint([x, y[i]/100], true, true, true); //Add a point and shift
                } else {
                    chartTempHR.series[i].addPoint([x, y[i]/100], true, false, true); //Add a point
                }
            }
        }
    };
    xhttp.open("GET", "/temperature-hr", true); //Open the data request
    xhttp.send();   //Send the data request
}, 120000);  //Repeat every 2 minutes

var chartTempDay = new Highcharts.Chart({
    chart: { renderTo: 'chart-temperature-day' },
    title: { text: 'Daily System Temperatures' },
    series: [
        {
            type: "line",
            showInLegend: true,
            name: "Glycol",
            data: []
        },
        {
            type: "line",
            showInLegend: true,
            name: "Solar Preheat",
            data: []
        },
        {
            type: "line",
            showInLegend: true,
            name: "Room Ambient",
            data: []
        },
        {
            type: "line",
            showInLegend: true,
            name: "Cold Water",
            data: []
        },
        {
            type: "line",
            showInLegend: true,
            name: "Hot Water",
            data: []
        }
    ],
    plotOptions: {
        line: {
            animation: false,
            dataLabels: { enabled: false }
        },
    },
    xAxis: {
        title: { text: 'Time' },
        type: 'datetime',
    },
    yAxis: {
        title: { text: 'Temperature (Celsius)' }
    },
    credits: { enabled: false }
});