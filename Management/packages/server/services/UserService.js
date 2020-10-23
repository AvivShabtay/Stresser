const user = require("../models").User;
const modelName = "users";

class UserService {
  constructor() {}

  static async getUserById(id) {
    let result;
    try {
      //result = await db[modelName].findByPk(id);
      result = await user.findByPk(id);
    } catch (err) {
      return Promise.reject(err);
    }
    return result;
  }

  static async notifyManager(event, message) {
    console.log(`Event: ${event}, message: ${JSON.stringify(message)}`);
  }
}

module.exports = UserService;
