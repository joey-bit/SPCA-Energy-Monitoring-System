Highcharts.setOptions({
    time: {
        useUTC: false,
        timezone: 'America/Vancouver',
    }
});
document.addEventListener('DOMContentLoaded', extractHistorical);
function extractHistorical() {
    var xhttp = new XMLHttpRequest(); //Create a data request
    xhttp.onreadystatechange = handleStateChange;
    xhttp.open("GET", "/historical-data"); //Open the data request
    xhttp.send();   //Send the data request

    function handleStateChange() { //Callback function 
        if (this.readyState == 4 && this.status == 200) { //When ready to receive
            extractData(this.responseText);
        }
    }
    function extractData(data) {
        var rows = data.split("\n");
        let rowNum = rows.length-24;
        if(rowNum < 1) //Can't have rowNum be 0 because the historical data's first line is a header
        {
            rowNum = 1;
        }
        for (rowNum; rowNum < rows.length; rowNum++) {
            let columns = rows[rowNum].split(',');
            let time = Date.parse(columns[0]);

            for (let colNum = 1; colNum < 6; colNum++) {
                let temp_y = parseFloat(columns[colNum]);  
                chartTempDay.series[colNum-1].addPoint([time, temp_y], true, false, true);
            }

            let water_used = parseFloat(columns[6]);
            chartFlowDay.series[0].addPoint([time, water_used], true, false, true);

            let source_temp = columns[4];
            let hot_temp = columns[5];
            let preheat_temp = columns[2];
            chartEnergyDay.series[0].addPoint([time, ((preheat_temp-source_temp)*water_used*4.186)/3600], true, false, true);
            chartEnergyDay.series[1].addPoint([time, ((hot_temp-preheat_temp)*water_used*4.186)/3600], true, false, true);
        }
    }
}