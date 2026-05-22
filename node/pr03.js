import http from "http";
import fs from "fs"; // knjižnice za delo z datotekami
import path from "path"; // knjižnica za delo s potmi do datotek
import { fileURLToPath } from "url"; // knjižnica za pretvorbo URL v pot do datoteke

// nadomestek za __dirname
// pretvori URL -> v normalno pot v operacijskem sistemu
// npr. file:///C:/projekti/server/app.js
// v            C:\projekti\server\app.js
const __filename = fileURLToPath(import.meta.url);
// iz poti odstrani ime datoteke, vrne samo mapo
// iz __filenm ="C:\\projekti\\server\\app.js"
// po path.dirname(__filename)
// dobimo samo "C:\\projekti\\server" - dobimo mapo v kateri je trenutna datoteka
const __dirname = path.dirname(__filename);


const hostname = "192.168.1.198";
const port = 8080;

const server = http.createServer((req, res) => {

    if (req.url === "/"){ // pogledamo, ali je "request" enak "/"
        fs.readFile(path.join(__dirname, "pr03.html"), (err, data) => {
            if(err){
                res.writeHead(500, {"Content-Type": "text/plain; charset=utf-8"});
                res.end("Napaka pri branju datoteke s html stranjo. Branje datoteke pr03.html ni uspelo.");
            } else {
                res.writeHead(200, {"Content-Type": "text/html; charset=utf-8"});
                res.end(data);
            }
        });
    }
    else if (req.url === "/stranx"){ // pogledamo, ali je "request" enak "/"
        fs.readFile(path.join(__dirname, "pr03x.html"), (err, data) => {
            if(err){
                res.writeHead(500, {"Content-Type": "text/plain; charset=utf-8"});
                res.end("Napaka pri branju datoteke s html stranjo. Branje datoteke pr03x.html ni uspelo.");
            } else {
                res.writeHead(200, {"Content-Type": "text/html; charset=utf-8"});
                res.end(data);
            }
        });
    }
    else if (req.url === "/strany"){ // pogledamo, ali je "request" enak "/"
        fs.readFile(path.join(__dirname, "pr03y.html"), (err, data) => {
            if(err){
                res.writeHead(500, {"Content-Type": "text/plain; charset=utf-8"});
                res.end("Napaka pri branju datoteke s html stranjo. Branje datoteke pr03y.html ni uspelo.");
            } else {
                res.writeHead(200, {"Content-Type": "text/html; charset=utf-8"});
                res.end(data);
            }
        });
    }                  
});

server.listen(port, hostname, () => {
    // v oklepaju zapišemo narekovaje z Alt+Gr + 7
    // da dobimo izpis IP naslova in vrat
    console.log(`Strežnik teče na http://${hostname}:${port}/`);
});