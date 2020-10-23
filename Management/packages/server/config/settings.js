var path = require("path");

var settings = {
  path: path.normalize(path.join(__dirname, "..")),
  port: process.env.NODE_PORT || 5000,
  database: {
    host: "localhost",
    username: "stresser",
    password: "P@$$w0rd",
    database: "stresser",
    port: 3306,
    protocol: "mysql",
    poolSize: 100,
    timeout: 1000,
  },
};

module.exports = settings;
