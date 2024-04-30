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

async function verifyPassword(request, response){
        let passwordAttempt = request.body;

        let result = await find(passwordAttempt, loginCollection);

        if(result.length === 0 || result[0]['password'] !== passwordAttempt['password']){
                response.send(JSON.stringify({passwordCorrect: false}));
        } else {
                response.send(JSON.stringify({passwordCorrect: true}));
        }
}

async function getKeysFromDB(request, response){

        let result = await find({}, keysCollection);

        response.send(JSON.stringify(result));

}

async function getSingleKeyFromDB(request, response){

        let urlArray = request.url.split("/");
        let specificKeyName = urlArray[urlArray.length - 1];

        let result = await find({keyName: specificKeyName}, keysCollection);

        response.send(JSON.stringify(result));
}

async function getLogsFromDB(request, response){

        let result = await find({}, logsCollection);

        response.send(JSON.stringify(result));

}



async function replaceSingleKeyOnDB(request, response) {
        if (!request.files || Object.keys(request.files).length === 0) {
                console.log("File not found error");
                return response.status(400).send(
                    JSON.stringify({"uploaded": false, "error": "File not found"})
                )
        }

        let keyFile = request.files.replacementKey;

        keyFile.mv('./' + keyFile.name, function(err) {
                if (err)
                        return response.status(500).send('{"filename": "' +
                            keyFile.name + '", "upload": false, "error": "' +
                            JSON.stringify(err) + '"}');

        });

        let fileContents;

        fs.readFile('./' + keyFile.name, 'utf-8', (error, contents) => {
                if(error){
                  console.log("File read error")
                        return response.status(500).send(
                            JSON.stringify({"uploaded": false, "error": "File reading error"})
                        )
                }

                fileContents = contents;
        });

        //Query formation:
        let urlArray = request.url.split("/");
        let specificKeyName = urlArray[urlArray.length - 1];

        let query  = {keyName: specificKeyName};
        let replacementData = {$set: {key: fileContents}};

        let confirm = await update(query, replacementData, keysCollection);
        console.log(confirm);

        return response.status(200).send(
            JSON.stringify("Key was replaced!")
        )

}

app.get('/', (request, response) => {
        response.send()
});

app.post('/login', verifyPassword);
app.post('/replace/*', replaceSingleKeyOnDB)
app.get('/keys', getKeysFromDB);
app.get('/keys/*', getSingleKeyFromDB);
app.get('/logs', getLogsFromDB);

app.listen(8080);
console.log("Express listening on port 8080");

//let result = await find({username: 'cst3990'}, loginCollection);

//console.log(result[0]['password']);
