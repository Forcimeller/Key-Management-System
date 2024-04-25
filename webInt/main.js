// Importing Node Modules
import express from 'express';
import bodyParser from "body-parser";
import mongodb from 'mongodb';

//Express App
const app = express();

//Serve the contents of "public" When the root folder is called
app.use(express.static('public'));

//App Extensions
app.use(bodyParser.json);
//app.set('view engine', 'ejs')
app.get('/', (request, response) => {
        response.send("");
});

app.listen(8080);
console.log("Express listening on port 8080");