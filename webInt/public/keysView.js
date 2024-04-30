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
                "<div class=\"keyType\">" + (keys[keyIndex].keyType === "" ? "-" : keys[keyIndex].keyType) +
                "</div>\n" +
            "</div>"
    }

    keyContainer.innerHTML = html;
}

function viewKey(key){

    //Show this specific key's details (delete and update)
    keyContainer.style.display = "none";
    keySettingsContainer.style.display = "block";
    keyButton.disabled = false;
    keyButton.className = "sidebarButton";


    html = "<h2>Key Details: " + key[0].keyName +"</h2>\n" +
        "<h5>Sample:</h5>\n" +
        "<p>" + key[0].keySample +"</p>\n" +
        "<button id=\"replaceKeyButton\" onclick='replaceKey(\"" +  key[0].keyName  + "\")'>Replace</button>\n" +
        "<button id=\"renameKeyButton\" onclick='renameKey(\"" +  key[0].keyName  + "\")'>Rename</button>\n" +
        "<button id=\"deleteKeyButton\" onclick='deleteKey(\"" +  key[0].keyName  + "\")'>Delete</button>\n";

    keySettingsContainer.innerHTML = html;
}

function replaceKey(keyName) {
    let formHtml =
        "<h2>Replace Key: " + keyName + "</h2>\n" +
        "<form action='/replace/" + keyName + "' method='post' " +
        "encType=\"multipart/form-data\">\n" +
        "        <input type=\"file\" name=\"replacementKey\" />\n" +
        "        <input type='submit' value='Upload' />\n" +
        "</form>";

    keySettingsContainer.innerHTML = formHtml;
}

function renameKeyConclusion(keyName, newKeyName){
    alert(keyName + " renamed to " + newKeyName);
}

function deleteKeyConclusion(keyName){
    alert(keyName + " Deleted");
}