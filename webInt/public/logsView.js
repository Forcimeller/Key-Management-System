logButton.onclick = function() {onClickLogs()};

function onClickLogs(){
    if (!logButton.disabled){
        keyButton.disabled = false;
        logButton.disabled = true;
        settingsButton.disabled = false;
        keyButton.className = "sidebarButton";
        logButton.className = "sidebarButtonInDisabled";
        settingsButton.className = "sidebarButton";
        header.innerText = "Logs";
    }
}