import http from "http";
import express from "express";
import ip from "ip";
import path from "path"; // knjižnica za delo s potmi do datotek
import { fileURLToPath } from "url"; // knjižnica za pretvorbo URL v pot do datoteke
import { join, dirname } from "path"; // knjižnica za združevanje poti do datotek

const __dirname = path.dirname(fileURLToPath(import.meta.url)); // pridobimo pot do trenutne datoteke

import WebSocket, { WebSocketServer } from "ws"; // za permanetno dvosmerno povezavo med strežnikom in klientom

const app = express(); // objekt aplikacije Express

const hostname = "192.168.1.198";
const port = 80; // standardni port za http strežnike

const server = http.createServer(app).listen(80); // espress strežnik posluša na portu 80

const wss1 = new WebSocketServer({port: 8888}); // ko se bomo povezali s Chrome brskalnikom na strežnik bo webSocket povezava med Chrome brskalnikom in node.js strežnikom vzpostavljena preko porta 8888

const wss2 = new WebSocketServer({port: 8811}); // esp32 povezava preko vtičnika 8811 | tu se poveže esp32 z node.js strežniko preko porta 8811

app.get("/", (req, res) => {
    res.sendFile(join(__dirname, "pr08.html")); // pošlje datoteko pr07.html kot odgovor na zahtevo ("request - req")
});

// ***************************************************************
// Brskalnik Chrome na wss1 komunicira z node.js preko porta 8888
// ***************************************************************
wss1.on("connection", function(ws, req){
    console.log("Klient - brskalnik Chrome se je povezal z dvosmerno povezavo prek vrat (porta) 8888.");

    ws.on("message", function(msgString){
        console.log("Sporočilo iz Chrome brskalnika prejeto, vsebina sporočila: " + msgString);

        // izluščimo dele json sporočila in jih izpišemo
        let msg = JSON.parse(msgString); // msg je sedaj JSON objekt; s funkcijo parse razčlenimo string, ki smo ga poslali iz html strani klenta
        console.log("Tip sporočila: " + msg.tipSporočila);
        console.log("Pin: " + msg.pin);
        console.log("Vrednost: " + msg.vrednost);

    }); // konec ws.on("message"...)

}); // konec wss1.on("connection"...)

// ************************************************************
// esp32 na wss2, vrata 8811
// ************************************************************
wss2.on("connection", function(ws, req){
    console.log("esp32 se je povezal z dvosmerno povezavo preko vrat oz. vtičnika 8811");
}); // konec wss2.on("connection"...)

console.log("Strežnik zagnan.");
console.log("IP=" + ip.address());