const router = require("express").Router();
const HostsControllers = require("../../controllers/HostsControllers");

router.get("/:id", HostsControllers.getHostById);
router.post("", HostsControllers.createHost);

module.exports = router;
