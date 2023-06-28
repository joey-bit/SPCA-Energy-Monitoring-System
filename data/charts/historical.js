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
        if(rowNum < 1) 
        {
            rowNum = 1;
        }
        for (rowNum; rowNum < rows.length; rowNum++) {
            var columns = rows[rowNum].split(',');
            for (let colNum = 1; colNum < 6; colNum++) {
                var y = parseFloat(columns[colNum]);
                var time = Date.parse(columns[0]);
                chartTempDay.series[colNum-1].addPoint([time, y], true, false, true);
            }
            var y = parseFloat(columns[6]);
            var time = Date.parse(columns[0]);
            chartFlowDay.addPoint([time, y], true, false, true);
        }
    }
}