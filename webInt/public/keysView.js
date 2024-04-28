//Event Handling for button
keyButton.onclick = function() {onClickKeys()};

//Process button click
function onClickKeys(){
    if (!keyButton.disabled) {
        keyButton.disabled = true;
        logButton.disabled = false;
        settingsButton.disabled = false;
        logButton.disabled = false;
        keyButton.className = "sidebarButtonInDisabled";
        logButton.className = "sidebarButton";
        settingsButton.className = "sidebarButton";
        logoutButton.className = "sidebarButton";
        header.innerText = "Keys";
        keyContainer.style.display = "block";
        logContainer.style.display = "none";
        keySettingsContainer.style.display = "none"
        settingsContainer.style.display = "none";
        getKeys();
    }
}

function populateKeyPage(keys){
    let html = "";
    for (let keyIndex = 0; keyIndex < keys.length; keyIndex++) {
        html +=
            "<div class=\"keyDiv\" onclick=getSingleKey(\"" + keys[keyIndex].keyName + "\")>\n" +
                "<div class=\"keyName\">" + keys[keyIndex].keyName +"</div>\n" +
                "<div class=\"keySample\">" + keys[keyIndex].keySample +"</div>\n" +
                "<div class=\"keyType\">" + (keys[keyIndex].keyType === "" ? "-" : keys[keyIndex].keyType) +"</div>\n" +
            "</div>"
    }

    keyContainer.innerHTML = html;
}

function viewKey(key){

    //const key = {keyName:"2", keySample: "loremIpsupm", keyType: ".pub"};

    //Show this specific key's details (delete and update)
    keyContainer.style.display = "none";
    keySettingsContainer.style.display = "block";
    keyButton.disabled = false;
    keyButton.className = "sidebarButton";


    html = "<h2>Key Details: " + key[0].keyName +"</h2>\n" +
        "<h5>Sample:</h5>\n" +
        "<p>" + key[0].keySample +"</p>\n" +
        "<button id=\"replaceKeyButton\">Replace</button>\n" +
        "<button id=\"renameKeyButton\">Rename</button>\n" +
        "<button id=\"deleteKeyButton\">Delete</button>" ;

    keySettingsContainer.innerHTML = html;
}