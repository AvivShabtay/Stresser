// Application entry point
const express = require("express");
const bodyParser = require("body-parser");
const cors = require("cors");
const settings = require("./config/settings");

// TODO: Move to different module:
const { HOST_EVENT } = require("./subscribers/EventTypes");
const UserService = require("./services/UserService");
const HostService = require("./services/HostService");

// TODO: move event registration to separate file.
HostService.registerListener(HOST_EVENT.QUERY_HOST, UserService.notifyManager);
HostService.registerListener(HOST_EVENT.NEW_HOST, UserService.notifyManager);

const port = settings.port;
const app = express();

app.use(bodyParser.json());
app.use(cors());
app.use(require("./router"));

app.set("db", require("./models/index"));
app.set("port", port);

let server = app.listen(port, () =>
  console.log(`The server running under port: ${port}`)
);

module.exports = server;
