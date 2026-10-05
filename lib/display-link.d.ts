import EventEmitter from 'bare-events'
import { tag, handle, Handle, Wrapper } from 'bare-foundation-registry'

/**
 * A timer that fires with the refresh of a display, as a `CADisplayLink`. It emits `tick` once it
 * has been added to a run loop.
 */
interface CoreAnimationDisplayLink extends EventEmitter<CoreAnimationDisplayLink.Events> {
  readonly [tag]: number

  readonly [handle]: Handle

  /** Whether ticks are held back. */
  paused: boolean

  /** When the last frame was displayed, in seconds. */
  readonly timestamp: number

  /** When the next frame will be displayed, in seconds. */
  readonly targetTimestamp: number

  /** The time between frames, in seconds. */
  readonly duration: number

  /** Start ticking on `runLoop` in `mode`, a `RunLoop.MODE` value from `bare-foundation`. */
  addToRunLoop(runLoop: Wrapper, mode: string): this

  /** Stop ticking on `runLoop` in `mode`. */
  removeFromRunLoop(runLoop: Wrapper, mode: string): this

  /** Stop ticking for good, and let go of every run loop. */
  invalidate(): this
}

declare class CoreAnimationDisplayLink {
  /**
   * Create a display link. On macOS it follows the display of `source`, a view, a window or a
   * screen, through `displayLinkWithTarget:selector:`. On iOS it follows the main display, and
   * there is no `source`.
   */
  constructor(source?: Wrapper)
}

declare namespace CoreAnimationDisplayLink {
  export interface Events {
    tick: []
  }
}

export = CoreAnimationDisplayLink
