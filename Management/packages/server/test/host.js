process.env.NODE_ENV = "test";

const hostModel = require("../models").hosts;
const chai = require("chai");
const chaiHttp = require("chai-http");
const app = require("../app");

const Sequelize = require("sequelize");
const db = require("../models");
const path = require("path");
var Umzug = require("umzug");
// const config = require('../config/config.json');

const should = chai.should();
chai.use(chaiHttp);

// Mockup data to create:
const hostsToCreate = [
  { /*hostID: 1, */ HostName: "PC1", HostIP: "192.168.1.100" },
  { /*hostID: 2, */ HostName: "PC2", HostIP: "192.168.1.200" },
  { /*hostID: 3, */ HostName: "PC3", HostIP: "192.168.1.300" },
];

// Stores the mockup data:
let createdHosts = [];

/* Mocha hook, called before running the tests to create some mockup data. */
before((done) => {
  // hostsToCreate.map(async (host) => {
  //   const result = await hostModel.create(host);
  //   createdHosts.push(result);
  // });

  // Setup the database
  db.sequelize
    .getQueryInterface()
    .dropAllTables()
    .catch((res) => console.log(res));

  let umzug = new Umzug({
    migrations: {
      params: [db.sequelize.getQueryInterface(), Sequelize],
      path: "./migrations",
      pattern: /^\d+[\w-]+\.js$/,
    },
    storage: "sequelize",
    storageOptions: { sequelize: db.sequelize },
    logging: console.log,
  });

  umzug.pending().then((migrations) => {
    umzug
      .execute({
        migrations: migrations,
        method: "up",
      })
      .then((migrations) => {
        console.log(migrations);
        done();
      });
  });
});

// /* Mocha hook, called after running all the tests to clean the mockup data. */
// after(() => {
//   createdHosts.map(async (host) => {
//     hostModel.findByPk(host.HostID).then((hostToDelete) => {
//       const result = hostToDelete.destroy();
//     });
//   });
// });

describe("/GET host by ID", () => {
  it("get host data by it host-ID", (done) => {
    chai
      .request(app)
      .get("/api/hosts/1")
      .end((err, res) => {
        res.should.have.status(200);
        res.body.should.be.a("object");
        res.body.should.have.property("type").eq("success");
        res.body.should.have.property("result");

        const host = res.body.result;
        host.should.have.property("HostID");
        host.should.have.property("HostName");
        host.should.have.property("HostIP");
        done();
      });
  });
});

describe("/POST host", () => {
  it("creates a new host in the system", (done) => {
    // TODO: use entity template class.
    const host = { hostName: "AvivPC", hostIP: "192.168.1.100" };

    chai
      .request(app)
      .post("/api/hosts")
      .send({ host })
      .end((err, res) => {
        res.should.have.status(200);
        res.body.should.be.a("object");
        res.body.should.have.property("type").eq("success");
        res.body.should.have.property("result");

        const host = res.body.result;
        host.should.have.property("HostID");
        host.should.have.property("HostName");
        host.should.have.property("HostIP");

        done();
      });
  });
});
