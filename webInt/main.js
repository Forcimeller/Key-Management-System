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
        console.log(passwordAttempt);

        let result = await find(passwordAttempt, loginCollection);

        if(result.length === 0 || result[0]['password'] !== passwordAttempt['password']){
                response.send(JSON.stringify({passwordCorrect: false}));
        } else {
                response.send(JSON.stringify({passwordCorrect: true}));
        }
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

app.listen(8080);
console.log("Express listening on port 8080");

//let result = await find({username: 'cst3990'}, loginCollection);

//console.log(result[0]['password']);
