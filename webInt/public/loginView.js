//Event Handling for button
logoutButton.onclick = function() {onClickLogout()};
loginButton.onclick = function () {onClickLoginButton()};

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
        keyContainer.style.display = "none";
        keySettingsContainer.style.display = "none"
        logContainer.style.display = "none";
        loginContainer.style.display="grid"
        settingsContainer.style.display = "none";
    }
}

function onClickLoginButton() {
    //get password
    let password = passwordInput.value;
    let passwordCorrect = checkPassword(password);
    //if password == password
    //password = passwordInput.innerText.trim();

    //loginContainer.style.display="none";
    //keyButton.disabled = false;
    //onClickKeys();
}

