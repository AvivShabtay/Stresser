// redundant class.
// TODO: figure how to create interfaces / abstract methods.
class BaseController {
  constructor() {}

  static async getById(req, modelName) {
    // const modelID = req.params.id;
    // const db = req.app.get("db");
    // let result;
    // try {
    //   // result = await db[modelName].findByPk(modelID);
    //   result = await req.app.get("db")[modelName].findByPk(modelID);
    // } catch (err) {
    //   return Promise.reject(err);
    // }
    // return result;
  }
}

module.exports = BaseController;
