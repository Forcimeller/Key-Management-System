keyButton.onclick = function() {onClickKeys()};

function onClickKeys(){
    if (!keyButton.disabled){
        keyButton.disabled = true;
        logButton.disabled = false;
        settingsButton.disabled = false;
        keyButton.className = "sidebarButtonInDisabled";
        logButton.className = "sidebarButton";
        settingsButton.className = "sidebarButton";
        header.innerText = "Keys"
    }
}