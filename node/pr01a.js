import http from "http";

const hostname = "192.168.1.198";
const port = 8080;

const server = http.createServer((req, res) => {
    res.statusCode = 200; // vse je OK
    res.setHeader("Content-Type", "text/plain");
    res.end("Pozdravljen svet VSC v marcu!");
});

server.listen(port, hostname, () => {
    // v oklepaju zapišemo narekovaje z Alt+Gr + 7
    // da dobimo izpis IP naslova in vrat
    // ´ - BackTick Alt Gr + 7 - povratni narekovaj
    console.log(`Strežnik teče na http://${hostname}:${port}`);
});