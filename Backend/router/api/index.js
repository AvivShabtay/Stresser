const router = require("express").Router();

router.use("/hosts", require("./hostsRouter"));
router.use("/users", require("./usersRouter"));

module.exports = router;
