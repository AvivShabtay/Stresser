const host = require("../models").hosts;
const Event = require("../subscribers/Event");
const { HOST_EVENT } = require("../subscribers/EventTypes");
const modelName = "hosts";

let hostEvent = new Event();

class HostService {
  constructor() {}

  static registerListener(eventName, callback) {
    hostEvent.on(eventName, callback);
  }

  static async getHostById(id) {
    let result;
    try {
      // Using sequelize.js findByPk function:
      result = await host.findByPk(id);

      // Notify listeners:
      hostEvent.emit(HOST_EVENT.QUERY_HOST, result);
    } catch (err) {
      return Promise.reject(err);
    }
    return result;
  }

  static async createHost(hostName, hostIP) {
    let result;
    try {
      // Using sequelize.js create function:
      // TODO: use converter for boundary to entity:
      result = await host.create({ HostName: hostName, HostIP: hostIP });

      // Notify listeners:
      hostEvent.emit(HOST_EVENT.NEW_HOST, result);
    } catch (err) {
      return Promise.reject(err);
    }
    return result;
  }
}

module.exports = HostService;
