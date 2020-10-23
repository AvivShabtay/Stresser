const BaseController = require("../controllers/BaseController");
const HostService = require("../services/HostService");

/**
 * Table name: hosts
 * Table Fields: HostID, HostName, HostIP
 */
class HostsController extends BaseController {
  constructor() {}
  static async getHostById(req, res) {
    try {
      const hostID = req.params.id;

      // TODO: validates incoming data.

      // Calls BL to get the host data:
      const result = await HostService.getHostById(hostID);

      // TODO: return a uniform result.
      return res.status(200).json({
        type: "success",
        result,
      });
    } catch (error) {
      console.log(error);
    }
  }

  static async createHost(req, res) {
    try {
      // Get host data without ID:
      const host = req.body.host;

      // TODO: validates incoming data.

      // Calls BL to create new host:
      const result = await HostService.createHost(host.hostName, host.hostIP);

      // TODO: return a uniform result.
      return res.status(200).json({
        type: "success",
        result,
      });
    } catch (error) {
      console.log(error);
    }
  }
}

module.exports = HostsController;
