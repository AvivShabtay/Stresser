"use strict";
module.exports = {
  up: (queryInterface, Sequelize) => {
    return queryInterface.createTable("hosts", {
      HostID: {
        type: Sequelize.INTEGER,
        allowNull: false,
        primaryKey: true,
        autoIncrement: true,
      },
      HostName: {
        type: Sequelize.STRING,
        allowNull: false,
      },
      HostIP: {
        type: Sequelize.STRING,
        allowNull: false,
      },
    });
  },
  down: (queryInterface, Sequelize) => {
    return queryInterface.dropTable("hosts");
  },
};
