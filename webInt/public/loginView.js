//Event Handling for button
logoutButton.onclick = function() {onClickLogout()};

//Process button click
function onClickLogout(){
    if (!logButton.disabled){
        keyButton.disabled = true;
        logButton.disabled = true;
        settingsButton.disabled = true;
        logoutButton.disabled = true;
        keyButton.className = "sidebarButtonInDisabled";
        logButton.className = "sidebarButtonInDisabled";
        settingsButton.className = "sidebarButtonInDisabled";
        logoutButton.className = "sidebarButtonInDisabled";
        header.innerText = "Sign In";
    }
}