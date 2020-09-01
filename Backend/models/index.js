const fs = require("fs");
const path = require("path");
const Sequelize = require("sequelize");

const basename = path.basename(__filename);
const db = {};

let dbSettings;
if (process.env.NODE_ENV == "test") {
  dbSettings = require("../config/config.json").test;
} else {
  require("../config/settings").database;
}

let sequelize = new Sequelize({
  database: dbSettings.database,
  username: dbSettings.username,
  host: dbSettings.host,
  password: dbSettings.password,
  dialect: "mysql",
  operatorsAliases: 0,
  omitNull: 1,
});

fs.readdirSync(__dirname)
  .filter(
    (file) =>
      file.indexOf(".") !== 0 && file !== basename && file.slice(-3) === ".js"
  )
  .forEach((file) => {
    const model = sequelize.import(path.join(__dirname, file));
    db[model.name] = model;
  });

Object.keys(db).forEach((modelName) => {
  if (db[modelName].associate) {
    db[modelName].associate(db);
  }
});

db.sequelize = sequelize;
db.Sequelize = Sequelize;

module.exports = db;
