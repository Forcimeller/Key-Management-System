function requestHandler(xhttpReq) {
    if(xhttpReq.readyState === 4 && xhttpReq.status === 200){
        const data = JSON.parse(xhttpReq.responseText);
        console.log("Data received", data)
    } else {
        console.error("There was an error: " + xhttpReq.status)
    }
}

function getRequest(url){
    let xhttpReq = new XMLHttpRequest();
    xhttpReq.open('GET', url, true);
    xhttpReq.onreadystatechange = requestHandler(xhttpReq);
    xhttpReq.send();
}

function postRequest(url, postData){
    let xhttpReq = new XMLHttpRequest();

    xhttpReq.onreadystatechange = function() {
        if(this.readyState === 4 && this.status === 200) {
            let data = JSON.parse(xhttpReq.responseText);
            console.log("Data received", data)

        } else {
            console.error("There was an error: " + xhttpReq.status)
        }
    };

    xhttpReq.open("POST", "/login", true);
    xhttpReq.setRequestHeader("Content-type", "application/json");
    xhttpReq.send(postData);
}

function checkPassword(passwordInput){
    let data = JSON.stringify({password: passwordInput});
    let url = "/login";
    postRequest(url, data);
}