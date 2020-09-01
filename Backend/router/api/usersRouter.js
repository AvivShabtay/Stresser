const router = require("express").Router();
const UsersController = require("../../controllers/UserController");

router.get("/:id", UsersController.getUserById);

module.exports = router;
