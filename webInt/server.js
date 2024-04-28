// Importing Node Modules
import express from 'express';
import bodyParser from "body-parser";
import { MongoClient, ServerApiVersion, ObjectId} from "mongodb";

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

//Express App
const app = express();

//Serve the contents of "public" When the root folder is called
app.use(express.static('public'));

//App Extensions
app.use(bodyParser.json());
app.set('view engine', 'ejs')
app.get('/', (request, response) => {
        response.send()
});

app.post('/login', verifyPassword);
app.get('/keys', getKeysFromDB);
app.get('/keys/*', getSingleKeyFromDB);
app.get('/logs', getLogsFromDB);

app.listen(8080);
console.log("Express listening on port 8080");

//let result = await find({username: 'cst3990'}, loginCollection);

//console.log(result[0]['password']);
