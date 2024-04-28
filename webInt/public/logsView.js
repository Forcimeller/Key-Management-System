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

        getLogs();
    }
}

function showAllLogs(logs) {
    let html = "";

    for (let logIndex = 0; logIndex < logs.length; logIndex++){
        html +=
            "<div class=\"logDiv\">\n" +
            "<div class=\"logDescription\">" + logs[logIndex]["description"] +"</div>\n" +
            "<div class=\"logDate\">" + logs[logIndex]["datetime"] +"</div>\n" +
            "</div>"
    }

    logContainer.innerHTML = html;
}