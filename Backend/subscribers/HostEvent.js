const EventEmitter = require("events");
const emitter = new EventEmitter();

class HostEvent {
  static hostCreated(host) {
    //Raise an event:
    emitter.emit("hostCreated", host);
  }
  static getHostEmitter() {
    return emitter;
  }
}

module.exports = HostEvent;
