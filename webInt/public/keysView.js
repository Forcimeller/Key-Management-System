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
        keySettingsContainer.style.display = "none"
        //API CALL

        ///* Then make something like this with JSON.stringify
        let keysFromDB = [
            {keyName:"1", keySample: "loremIpsupm", keyType: ".pub"},
            {keyName:"2", keySample: "loremIpsupm", keyType: ".pub"}
        ]
        //*/
        populateKeyPage(keysFromDB);
    }
}

function populateKeyPage(keys){
    let html = "";
    for(let key in keys){
        html +=
            "<div class=\"keyDiv\" onclick=viewKey(" + key.keyName + ")>\n" +
                "<div class=\"keyName\">" + key["keyName"] +"</div>\n" +
                "<div class=\"keySample\">" + key["keySample"] +"</div>\n" +
                "<div class=\"keyType\">" + key["keyType"] +"</div>\n" +
            "</div>"
    }

    keyContainer.innerHTML = html;
}

function viewKey(keyName){
    //Show this specific key's details (delete and update)
    keyContainer.style.display = "none";
    keySettingsContainer.style.display = "block";
    keyButton.disabled = false;
    keyButton.className = "sidebarButton";

    //API call: get key data again

    const key = {keyName:"2", keySample: "loremIpsupm", keyType: ".pub"};

    html = "<h2>Key Details: " + key["keyName"] +"</h2>\n" +
        "<h5>Sample:</h5>\n" +
        "<p>" + key["keySample"] +"</p>\n" +
        "<button>Replace</button>\n" +
        "<button>Rename</button>\n" +
        "<button>Delete</button>" ;

    keySettingsContainer.innerHTML = html;
}