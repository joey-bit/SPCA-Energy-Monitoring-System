var chartEnergy = new Highcharts.Chart({
    chart: { renderTo: 'chart-energy' },
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
          if (chartEnergy.series[i].data.length > 20) {
            chartEnergy.series.addPoint([x, y[i]], true, true, true);
          } else {
            chartEnergy.series.addPoint([x, y[i]], true, false, true);
          }
        }
      }
    };
    xhttp.open("GET", "/energy", true);
    xhttp.send();
  }, 3000);