//Event Handling for button
logButton.onclick = function() {onClickLogs()};

//Process button click
function onClickLogs(){
    if (!logButton.disabled){
        keyButton.disabled = false;
        logButton.disabled = true;
        settingsButton.disabled = false;
        keyButton.className = "sidebarButton";
        logButton.className = "sidebarButtonInDisabled";
        settingsButton.className = "sidebarButton";
        header.innerText = "Logs";
        keyContainer.style.display = "none";
        logContainer.style.display = "block";
        keySettingsContainer.style.display = "none"
        settingsContainer.style.display = "none";

        ///* Then make something like this with JSON.stringify
        const logsFromDB = [
            {description:"lorem ipsum", datetime: "1/2/3"},
            {description:"dolor sit amet", datetime: "4/5/6"}
        ];
        //*/

        showAllLogs(logsFromDB);
    }
}

function showAllLogs(logs) {
    let html = "";
    for(let log in logs){
        html +=
            "<div class=\"logDiv\">\n" +
            "<div class=\"logDescription\">" + log["description"] +"</div>\n" +
            "<div class=\"logDate\">" + log["datetime"] +"</div>\n" +
            "</div>"
    }

    logContainer.innerHTML = html;
}