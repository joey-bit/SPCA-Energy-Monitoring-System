var chartTemp = new Highcharts.Chart({
    chart: { renderTo: 'chart-temperature' },
    title: { text: 'System Temperatures' },
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
        name: "Hot Water",
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
        name: "Solar Preheat",
        data: []
      },
      {
        type: "line",
        showInLegend: true,
        name: "Hybrid Tank",
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
      dateTimeLabelFormats: { second: '%H:%M' }
    },
    yAxis: {
      title: { text: 'Temperature (Celsius)' }
    },
    credits: { enabled: false }
  });
  setInterval(function () {
    var xhttp = new XMLHttpRequest(); //Create a data request
    xhttp.onreadystatechange = function () { //Callback function
      if (this.readyState == 4 && this.status == 200) { //When ready to receive
        var x = (new Date()).getTime(), //Current time
          y = this.responseText.split(',').map(Number); //Get the data as an array of floats
        for (let i = 0; i < 5; i++) {
          if (chartTemp.series[i].data.length > 20) { //If there are more than 20 points
            chartTemp.series[i].addPoint([x, y[i]], true, true, true); //Add a point and shift
          } else {
            chartTemp.series[i].addPoint([x, y[i]], true, false, true); //Add a point
          }
        }
      }
    };
    xhttp.open("GET", "/temperature", true); //Open the data request
    xhttp.send();   //Send the data request
  }, 3000);  //Repeat every 3 seconds