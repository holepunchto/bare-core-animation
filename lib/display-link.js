const EventEmitter = require('bare-events')
const binding = require('../binding')
const registry = require('bare-foundation-registry')

// A run loop keeps a link it schedules, and the link only reaches its wrapper
// weakly, so the wrapper is kept for as long as the link is scheduled.
const scheduled = new Set()

module.exports = exports = class CoreAnimationDisplayLink extends EventEmitter {
  constructor(source = null) {
    super()

    this._tag =
      source === null
        ? binding.displayLinkInit(this)
        : binding.displayLinkOf(registry.adopt(binding, source), this)

    this._token = binding.claim(this._tag, this)

    this._runLoops = 0
  }

  get [registry.tag]() {
    return this._tag
  }

  get [registry.handle]() {
    return binding.handle(this._tag)
  }

  get paused() {
    return binding.displayLinkPaused(this._tag)
  }

  set paused(paused) {
    binding.displayLinkPaused(this._tag, paused)
  }

  get timestamp() {
    return binding.displayLinkTimestamp(this._tag)
  }

  get targetTimestamp() {
    return binding.displayLinkTargetTimestamp(this._tag)
  }

  get duration() {
    return binding.displayLinkDuration(this._tag)
  }

  addToRunLoop(runLoop, mode) {
    binding.displayLinkAddToRunLoop(this._tag, registry.adopt(binding, runLoop), mode)

    this._runLoops++

    scheduled.add(this)

    return this
  }

  removeFromRunLoop(runLoop, mode) {
    binding.displayLinkRemoveFromRunLoop(this._tag, registry.adopt(binding, runLoop), mode)

    if (--this._runLoops <= 0) {
      this._runLoops = 0

      scheduled.delete(this)
    }

    return this
  }

  invalidate() {
    binding.displayLinkInvalidate(this._tag)

    this._runLoops = 0

    scheduled.delete(this)

    return this
  }

  _ontick() {
    this.emit('tick')
  }
}
