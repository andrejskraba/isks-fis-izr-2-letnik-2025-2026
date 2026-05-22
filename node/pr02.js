import http from "http";
import fs from "fs"; // knjižnice za delo z datotekami

const hostname = "192.168.1.198";
const port = 8080;

const server = http.createServer((req, res) => {
    fs.readFile("pr02.html", (err, data) => {
        if (err) {
            res.statusCode = 500; // napaka strežnika
            res.setHeader("Content-Type", "text/plain");
            res.end("Napaka pri branju datoteke.");
        } else {
            res.statusCode = 200; // vse je OK
            res.setHeader("Content-Type", "text/html");
            res.end(data); // pošljemo vsebino datoteke kot odgovor klientu
        }
    });           
});

server.listen(port, hostname, () => {
    // v oklepaju zapišemo narekovaje z Alt+Gr + 7
    // da dobimo izpis IP naslova in vrat
    console.log(`Strežnik teče na http://${hostname}:${port}/`);
});