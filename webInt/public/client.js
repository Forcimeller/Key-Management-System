
function getRequest(url){
    let xhttpReq = new XMLHttpRequest();

    xhttpReq.onreadystatechange =function() {
        if(this.readyState === 4 && this.status === 200) {
            let data = JSON.parse(xhttpReq.responseText);
            //Data Received here
            console.log("Data received", data)
        } else {
            console.error("There was an error: " + xhttpReq.status)
        }
    };

    xhttpReq.open('GET', url, true);
    xhttpReq.send();
}

async function postRequest(url, postData){
    let xhttpReq = new XMLHttpRequest();

    xhttpReq.onreadystatechange = function() {
        if(this.readyState === 4 && this.status === 200) {
            let data = JSON.parse(xhttpReq.responseText);
            //Data Received here
            console.log("Data received", data)
        } else {
            console.error("There was an error: " + xhttpReq.status)
        }
    };

    xhttpReq.open("POST", url, true);
    xhttpReq.setRequestHeader("Content-type", "application/json");
    xhttpReq.send(postData);
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

async function getLogs(){
    let xhttpReq = new XMLHttpRequest();
    let logArray;
    xhttpReq.onreadystatechange =function() {
        if(this.readyState === 4 && this.status === 200) {
            logArray = JSON.parse(xhttpReq.responseText);
            showAllLogs(logArray);
        } else {
            console.error("There was an error: " + xhttpReq.status)
        }
    };

    xhttpReq.open('GET', '/logs', true);
    xhttpReq.send();
}

async function addReplacementKey(keyName){
    let xhttpReq = new XMLHttpRequest();

    xhttpReq.onreadystatechange = function() {
        if(this.readyState === 4 && this.status === 200) {
            let response = JSON.parse(xhttpReq.responseText);
            //Data Received here
            if("error" in response) {//Error from server
                alert("File upload failed")
            }else {
                keyReplacementConclusion(response);
            }
        } else {
            console.error("There was an error: " + xhttpReq.status)
        }
    };

    xhttpReq.open("POST", "/replace/"+keyName, true);
    xhttpReq.setRequestHeader("Content-type", "application/json");
    xhttpReq.send(postData);
}