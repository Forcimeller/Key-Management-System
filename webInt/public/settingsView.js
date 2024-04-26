settingsButton.onclick = function() {onClickSettings()};

function onClickSettings(){
    if (!settingsButton.disabled){
        keyButton.disabled = false;
        logButton.disabled = false;
        settingsButton.disabled = true;
        keyButton.className = "sidebarButton";
        logButton.className = "sidebarButton";
        settingsButton.className = "sidebarButtonInDisabled";
        header.innerText = "Settings";
    }
}