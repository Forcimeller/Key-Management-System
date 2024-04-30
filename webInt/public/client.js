
function clearLogs(){
    if(confirm("You are about to clear ALL Logs")) {
        let xhttpReq = new XMLHttpRequest();

        xhttpReq.onreadystatechange = function () {
            if (this.readyState === 4 && this.status === 200) {
                let data = JSON.parse(xhttpReq.responseText);
                alert("ALL Logs cleared");
            } else {
                console.error("There was an error: " + xhttpReq.status)
            }
        };

        xhttpReq.open('GET', "/clearLogs", true);
        xhttpReq.send();
    }
}

function clearKeys(){
    if(confirm("You are about to clear ALL keys")) {
        let xhttpReq = new XMLHttpRequest();

        xhttpReq.onreadystatechange = function () {
            if (this.readyState === 4 && this.status === 200) {
                let data = JSON.parse(xhttpReq.responseText);
                //Data Received here
                alert("ALL Keys cleared");
            } else {
                console.error("There was an error: " + xhttpReq.status)
            }
        };

        xhttpReq.open('GET', "/clearKeys", true);
        xhttpReq.send();
    }
}

async function checkPassword(passwordInput){
    let postData = JSON.stringify({password: passwordInput});
    let url = "/login";

    let xhttpReq = new XMLHttpRequest();

    xhttpReq.onreadystatechange = function() {
        if(this.readyState === 4 && this.status === 200) {
            let data = JSON.parse(xhttpReq.responseText);

            if(data['passwordCorrect']){
                loginContainer.style.display = "none";
                keyButton.disabled = false;
                onClickKeys();
            } else {
                alert("Incorrect Password");
            }
        } else {
            console.error("There was an error: " + xhttpReq.status)
        }
    };

    xhttpReq.open("POST", url, true);
    xhttpReq.setRequestHeader("Content-type", "application/json");
    xhttpReq.send(postData);

    if(response === ""){
        alert("Incorrect Password");
    } else {

    }
}

async function getKeys(){
    let xhttpReq = new XMLHttpRequest();
    let keyArray;
    xhttpReq.onreadystatechange =function() {
        if(this.readyState === 4 && this.status === 200) {
            keyArray = JSON.parse(xhttpReq.responseText);
            populateKeyPage(keyArray);
        } else {
            console.error("There was an error: " + xhttpReq.status)
        }
    };

    xhttpReq.open('GET', '/keys', true);
    xhttpReq.send();
}

async function getSingleKey(keyName){
    let xhttpReq = new XMLHttpRequest();
    let keyArray;
    xhttpReq.onreadystatechange =function() {
        if(this.readyState === 4 && this.status === 200) {
            keyArray = JSON.parse(xhttpReq.responseText);
            viewKey(keyArray);
        } else {
            console.error("There was an error: " + xhttpReq.status)
        }
    };

    xhttpReq.open('GET', '/keys/' + keyName, true);
    xhttpReq.send();
}

async function getLogs() {
    let xhttpReq = new XMLHttpRequest();
    let logArray;
    xhttpReq.onreadystatechange = function () {
        if (this.readyState === 4 && this.status === 200) {
            logArray = JSON.parse(xhttpReq.responseText);
            showAllLogs(logArray);
        } else {
            console.error("There was an error: " + xhttpReq.status)
        }
    };

    xhttpReq.open('GET', '/logs', true);
    xhttpReq.send();
}

async function deleteKey(keyName){
    let xhttpReq = new XMLHttpRequest();

    xhttpReq.onreadystatechange = function() {
        if(this.readyState === 4 && this.status === 200) {
            let data = JSON.parse(xhttpReq.responseText);
            //Data Received here
            deleteKeyConclusion(keyName);
        } else {
            console.error("There was an error: " + xhttpReq.status)
        }
    };

    xhttpReq.open("POST", "/delete/" + keyName, true);
    xhttpReq.setRequestHeader("Content-type", "application/json");
    xhttpReq.send(JSON.stringify({"keyName": keyName}));
}

async function renameKey(keyName){
    let xhttpReq = new XMLHttpRequest();

    let newKeyName = prompt("Please enter a new Key name:");

    if(newKeyName.length > 0) {
        let postData = JSON.stringify({existingName: keyName, newName: newKeyName});

        xhttpReq.onreadystatechange = function () {
            if (this.readyState === 4 && this.status === 200) {
                let data = JSON.parse(xhttpReq.responseText);
                //Data Received here
                renameKeyConclusion(keyName, newKeyName);
            } else {
                console.error("There was an error: " + xhttpReq.status)
            }
        };

        xhttpReq.open("POST", "/rename/" + keyName, true);
        xhttpReq.setRequestHeader("Content-type", "application/json");
        xhttpReq.send(postData);
    } else {
        alert("Do not leave the rename field empty..")
    }
}