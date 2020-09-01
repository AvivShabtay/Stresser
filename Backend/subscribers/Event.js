// https://www.freecodecamp.org/news/how-to-code-your-own-event-emitter-in-node-js-a-step-by-step-guide-e13b7e7908e1/
class Event {
  // Key-value pair
  constructor() {
    // TODO: Add event object to wrapping the messages when emitting.
    this.listeners = {};
  }

  /**
   * The addListener event checks if the event is already registered.
   * If yes, returns the array, otherwise empty array.
   * @param {*} eventName
   * @param {*} fn
   */
  addListener(eventName, fn) {
    this.listeners[eventName] = this.listeners[eventName] || [];
    this.listeners[eventName].push(fn);
    return this;
  }

  /**
   * an alias to the ‘addListener’ method
   * @param {*} eventName
   * @param {*} fn
   */
  on(eventName, fn) {
    return this.addListener(eventName, fn);
  }

  /**
   * Removes listener from the event array.
   * If the event has multiple listeners then other listeners will not be impacted.
   * @param {*} eventName
   * @param {*} fn
   */
  removeListener(eventName, fn) {
    let lis = this.listeners[eventName];
    if (!lis) return this;
    for (let i = lis.length; i > 0; i--) {
      if (lis[i] === fn) {
        lis.splice(i, 1);
        break;
      }
    }
    return this;
  }

  /**
   * An alias to the ‘removeListener’ method.
   * @param {an alias to the ‘removeListener’ method. } eventName
   * @param {*} fn
   */
  off(eventName, fn) {
    this.removeListener(eventName, fn);
  }

  /**
   * Adds a one-time listener function for the event named eventName.
   * The next time eventName is triggered, this listener is removed and then invoked.
   * Use for setup/init kind of events.
   * @param {*} eventName
   * @param {*} fn
   */
  once(eventName, fn) {
    this.listeners[event] = this.listeners[eventName] || [];
    const onceWrapper = () => {
      fn();
      this.off(eventName, onceWrapper);
    };
    this.listeners[eventName].push(onceWrapper);
    return this;
  }

  /**
   * Synchronously calls each of the listeners registered for the event named eventName,
   * in the order they were registered, passing the supplied arguments to each.
   * Returns true if the event had listeners, false otherwise.
   * @param {*} eventName
   * @param {...any} args
   */

  emit(eventName, ...args) {
    let fns = this.listeners[eventName];
    if (!fns) return false;
    fns.forEach((f) => {
      f(eventName, ...args);
    });
    return true;
  }

  /**
   * Returns the number of listeners listening to the event named eventName.
   * @param {*} eventName
   */
  listenerCount(eventName) {
    let fns = this.listeners[eventName] || [];
    return fns.length;
  }

  /**
   * Returns a copy of the array of listeners for the event named eventName,
   * including any wrappers (such as those created by .once()).
   * The once wrappers in this implementation will not be available if the event has been emitted once.
   * @param {*} eventName
   */
  rawListener(eventName) {
    return this.listeners[eventName];
  }
}

module.exports = Event;
