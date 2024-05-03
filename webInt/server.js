// Importing Node Modules
import express from 'express';
import bodyParser from "body-parser";
import fileUpload from 'express-fileupload';
import fs from 'fs';
import { MongoClient, ServerApiVersion, ObjectId} from "mongodb";


//Express App
const app = express();

//Serve the contents of "public" When the root folder is called
app.use(express.static('public'));

//App Extensions
app.use(bodyParser.json());
app.use(fileUpload());
app.set('view engine', 'ejs')


const connectionUri = "mongodb://127.0.0.1:27017/?directConnection=true&serverSelectionTimeoutMS=2000&appName=mongosh+2.2.1";
const client = new MongoClient(connectionUri, {
        serverApi: {
                version: ServerApiVersion.v1,
                strict: false,
                deprecationErrors: true,
        }
});

//Collection Declarations
const database = client.db("KeyManBase");
const loginCollection = database.collection("login")
const keysCollection = database.collection("keys")
const logsCollection = database.collection("logs")

async function find(query, chosenCollection){

        //execute
        const results = await chosenCollection.find(query).toArray();

        //close Connection
        //await client.close();

        //Return result
        return results;
}

async function update(query, replacementData, chosenCollection){

        //execute
        const confirmation = await chosenCollection.updateOne(query, replacementData);

        //Return confirmation
        return confirmation;
}

async function deleteDocument(query, chosenCollection){

        //execute
        const confirmation = await chosenCollection.deleteOne(query);

        //Return confirmation
        return confirmation;
}

async function insert(data, chosenCollection){

        //execute
        const confirmation = await chosenCollection.insertOne(data);

        //Return confirmation
        return confirmation;
}

async function deleteMany(query, chosenCollection){

        //execute
        const confirmation = await chosenCollection.deleteMany(query);

        //Return confirmation
        return confirmation;
}

async function addLog(logDescription){
        let now = new Date();
        let log = {description: logDescription, datetime: now}

        return await insert(log, logsCollection);
}

async function verifyPassword(request, response){
        let passwordAttempt = request.body;

        let result = await find(passwordAttempt, loginCollection);

        if(result.length === 0 || result[0]['password'] !== passwordAttempt['password']){
                addLog("From Web GUI: Unsuccessful Login attempt");
                response.send(JSON.stringify({passwordCorrect: false}));
        } else {
                addLog("From Web GUI: Successful login");
                response.send(JSON.stringify({passwordCorrect: true}));

        }
}

async function getKeysFromDB(request, response){

        let result = await find({}, keysCollection);

        addLog("From Web GUI: All keys viewed");

        response.send(JSON.stringify(result));

}

async function getSingleKeyFromDB(request, response){

        let urlArray = request.url.split("/");
        let specificKeyName = urlArray[urlArray.length - 1];

        let result = await find({keyName: specificKeyName}, keysCollection);

        addLog("From Web GUI: Single key " + specificKeyName + " viewed");
        response.send(JSON.stringify(result));
}

async function getLogsFromDB(request, response){

        let result = await find({}, logsCollection);

        addLog("From Web GUI: all logs viewed by user");
        response.send(JSON.stringify(result));
}

async function replaceSingleKeyOnDB(request, response) {
        if (!request.files || Object.keys(request.files).length === 0) {
                console.log("File not found error");

                addLog("From Web GUI: failed attempt to replace a key");

                return response.status(400).send(
                    JSON.stringify({"uploaded": false, "error": "File not found"})
                )
        }

        let keyFile = request.files.replacementKey;
        let errorContent;

        keyFile.mv('./' + keyFile.name, function(err) {
                if (err)
                        errorContent = JSON.stringify(err);
                        addLog("From Web GUI: failed attempt to replace a key");
                        return response.status(500).send('{"filename": "' +
                            keyFile.name + '", "upload": false, "error": "' +
                            errorContent + '"}');

        });

        let fileContents;

        fs.readFile('./' + keyFile.name, 'utf-8', (error, contents) => {
                if(error){
                  console.log("File read error");
                  addLog("From Web GUI: failed attempt to replace a key");
                        return response.status(500).send(
                            JSON.stringify({"uploaded": false, "error": "File reading error"})
                        )
                }

                fileContents = contents;
                console.log(fileContents)
        });

        //Query formation:
        let urlArray = request.url.split("/");
        let specificKeyName = urlArray[urlArray.length - 1];

        let query  = {keyName: specificKeyName};
        let replacementData = {$set: {key: fileContents}};

        let confirm = await update(query, replacementData, keysCollection);
        console.log(confirm);

        if(confirm.upsertedCount === 1) {
                addLog("From Web GUI: Key (" + specificKeyName + ") was replaced");
                return response.status(200).send(
                    JSON.stringify("Key was replaced!")
                )
        }
}

async function deleteSingleKeyOnDB(request, response){

        let urlArray = request.url.split("/");
        let specificKeyName = urlArray[urlArray.length - 1];

        let confirmation = await deleteDocument({keyName: specificKeyName}, keysCollection)

        addLog("From Web GUI: key (" + specificKeyName + ") was deleted");
        response.send(JSON.stringify(confirmation));
}

async function renameSingleKeyOnDB(request, response){
        let keyNames = request.body;

        let requestNewName = keyNames.newName;
        let requestExistingName = keyNames.existingName;

        let confirmation = await update({keyName: requestExistingName},
            {$set: {keyName: requestNewName}}, keysCollection)

        addLog("From Web GUI: Key (" + requestExistingName + ") was renamed to '" + requestNewName + "'.");
        response.send(JSON.stringify(confirmation));
}

async function clearKeysFromDB(request, response){

        let confirmation = await deleteMany({}, keysCollection)

        addLog("From Web GUI: ALL Keys Deleted");

        response.send(JSON.stringify(confirmation))
}

async function clearLogsFromDB(request, response){

        let confirmation = await deleteMany({}, logsCollection)

        addLog("From Web GUI: ALL logs Deleted");

        response.send(JSON.stringify(confirmation))
}

app.get('/', (request, response) => {
        addLog("From Web GUI: Web GUI Launched");
        response.send()
});

app.post('/login', verifyPassword);
app.post('/replace/*', replaceSingleKeyOnDB)
app.post('/delete/*', deleteSingleKeyOnDB)
app.post('/rename/*', renameSingleKeyOnDB);
app.get('/keys', getKeysFromDB);
app.get('/keys/*', getSingleKeyFromDB);
app.get('/logs', getLogsFromDB);
app.get('/clearLogs', clearLogsFromDB);
app.get('/clearKeys', clearKeysFromDB);

app.listen(8080);
console.log("Express listening on port 8080");