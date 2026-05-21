const express = require("express");
const http = require("http");
const WebSocket = require("ws");
const bodyParser = require("body-parser");
const cors = require("cors");

// ======================
// INIT
// ======================
const app = express();
app.use(cors());
app.use(bodyParser.json());

const server = http.createServer(app);
const wss = new WebSocket.Server({ server });

// ======================
// CLIENTS
// ======================
let clients = [];

// latest traffic state (for dashboard sync)
let latestState = {
    north: "red",
    south: "red",
    east: "red",
    west: "red"
};

// ======================
// WEBSOCKET
// ======================
wss.on("connection", (ws) => {
    console.log("🟢 WebSocket Client Connected");

    clients.push(ws);

    // send latest state immediately
    ws.send(JSON.stringify({
        type: "init",
        data: latestState
    }));

    ws.on("close", () => {
        clients = clients.filter(c => c !== ws);
        console.log("🔴 WebSocket Client Disconnected");
    });
});

// ======================
// BROADCAST FUNCTION
// ======================
function broadcast(data) {
    clients.forEach(ws => {
        if (ws.readyState === WebSocket.OPEN) {
            ws.send(JSON.stringify(data));
        }
    });
}

// ======================
// ESP32 ENDPOINT
// ======================
app.post("/api/light", (req, res) => {

    const data = req.body;

    console.log("\n🚦 TRAFFIC UPDATE RECEIVED:");
    console.log(data);

    latestState = data;

    broadcast({
        type: "traffic_update",
        data
    });

    res.json({
        status: "ok",
        received: data
    });
});

// ======================
// CONTROL API (optional future use)
// ======================
app.post("/api/control", (req, res) => {

    const command = req.body;

    console.log("\n🟡 CONTROL COMMAND:");
    console.log(command);

    broadcast({
        type: "control",
        data: command
    });

    res.json({ status: "sent", command });
});

// ======================
// HEALTH CHECK
// ======================
app.get("/", (req, res) => {
    res.send("🚦 Smart Traffic Server Running");
});

// ======================
// START SERVER
// ======================
const PORT = 3000;

server.listen(PORT, () => {
    console.log(`🚀 Server running on http://localhost:${PORT}`);
});