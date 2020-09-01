const BaseController = require("../controllers/BaseController");
const UserService = require("../services/UserService");

class UserController extends BaseController {
  constructor() {}
  static async getUserById(req, res) {
    try {
      const hostID = req.params.id;
      //const result = await super.getById(req, "hosts");
      const result = await UserService.getUserById(hostID);

      return res.status(200).json({
        type: "success",
        result,
      });
    } catch (error) {
      console.log(error);
    }
  }
}

module.exports = UserController;
