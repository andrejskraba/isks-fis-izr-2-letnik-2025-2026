import http from "http";
import express from "express";
import ip from "ip";
import path from "path"; // knjižnica za delo s potmi do datotek
import { fileURLToPath } from "url"; // knjižnica za pretvorbo URL v pot do datoteke
import { join, dirname } from "path"; // knjižnica za združevanje poti do datotek

const __dirname = path.dirname(fileURLToPath(import.meta.url)); // pridobimo pot do trenutne datoteke

const app = express(); // objekt aplikacije Express

const hostname = "192.168.1.198";
const port = 80; // standardni port za http strežnike

const server = http.createServer(app).listen(80); // espress strežnik posluša na portu 80

app.get("/", (req, res) => {
    res.sendFile(join(__dirname, "pr04.html")); // pošlje datoteko pr04.html kot odgovor na zahtevo ("request - req")
});

console.log("Strežnik zagnan.");
console.log("IP=" + ip.address());