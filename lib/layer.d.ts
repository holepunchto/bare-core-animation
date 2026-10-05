import { tag, handle, Handle, Wrapper } from 'bare-foundation-registry'

/** A Core Animation layer: a rectangle that draws content and can hold other layers. */
interface CoreAnimationLayer {
  /** How round the corners are, in points. */
  cornerRadius: number

  /** The width of the border, in points. */
  borderWidth: number

  /** How opaque the layer is, from 0 to 1. */
  opacity: number

  /** The position of the layer on the z axis. */
  zPosition: number

  /** How many pixels the content has per point. */
  contentsScale: number

  /** How opaque the shadow is, from 0 to 1. */
  shadowOpacity: number

  /** How blurred the shadow is, in points. */
  shadowRadius: number

  /** Whether sublayers are clipped to the bounds of the layer. */
  masksToBounds: boolean

  /** Which corners `cornerRadius` applies to, as `CORNER_MASK` flags combined with `|`. */
  maskedCorners: number

  /** The background color. A component that is left out is 0, except `alpha`, which is 1. */
  get backgroundColor(): CoreAnimationLayer.Color
  set backgroundColor(color: Partial<CoreAnimationLayer.Color>)

  /** The border color. A component that is left out is 0, except `alpha`, which is 1. */
  get borderColor(): CoreAnimationLayer.Color
  set borderColor(color: Partial<CoreAnimationLayer.Color>)

  /** The shadow color. A component that is left out is 0, except `alpha`, which is 1. */
  get shadowColor(): CoreAnimationLayer.Color
  set shadowColor(color: Partial<CoreAnimationLayer.Color>)

  /** The position and size of the layer in its superlayer. Missing fields are 0. */
  get frame(): CoreAnimationLayer.Rect
  set frame(frame: Partial<CoreAnimationLayer.Rect>)

  /** The position and size of the layer in its own coordinates. Missing fields are 0. */
  get bounds(): CoreAnimationLayer.Rect
  set bounds(bounds: Partial<CoreAnimationLayer.Rect>)

  /** The position of the anchor point in the superlayer. Missing fields are 0. */
  get position(): CoreAnimationLayer.Point
  set position(position: Partial<CoreAnimationLayer.Point>)

  /** The point the layer is positioned and transformed around, from 0 to 1 on each axis. */
  get anchorPoint(): CoreAnimationLayer.Point
  set anchorPoint(anchorPoint: Partial<CoreAnimationLayer.Point>)

  /** The 3D transform, as sixteen numbers in the order CSS uses for `matrix3d()`. */
  transform: number[]

  /** How far the shadow is moved from the layer, in points. Missing fields are 0. */
  get shadowOffset(): CoreAnimationLayer.Size
  set shadowOffset(shadowOffset: Partial<CoreAnimationLayer.Size>)

  /** A layer whose opaque parts decide which parts of this layer are shown, or `null`. */
  get mask(): CoreAnimationLayer | null
  set mask(layer: Wrapper | null)

  /** Add `layer` on top of the other sublayers. */
  addSublayer(layer: Wrapper): this

  /** Add `layer` at `index` among the other sublayers. */
  insertSublayer(layer: Wrapper, index: number): this

  /** Remove the layer from its superlayer. */
  removeFromSuperlayer(): this

  /** Ask the layer to draw its content again. */
  setNeedsDisplay(): this

  readonly [tag]: number

  readonly [handle]: Handle
}

declare class CoreAnimationLayer {
  /** Create a new, empty layer. */
  constructor()

  /**
   * Return the layer of a view from another addon, such as an `NSView` or a `UIView`. Returns
   * `null` if the view has no layer yet. On macOS, set `wantsLayer` on the view first.
   * @throws A `TypeError` when the object is not a view.
   */
  static of(view: Wrapper): CoreAnimationLayer | null

  /** The corner flags used by `maskedCorners`. */
  static readonly CORNER_MASK: CoreAnimationLayer.Corners
}

declare namespace CoreAnimationLayer {
  export interface Color {
    red: number
    green: number
    blue: number
    alpha: number
  }

  export interface Point {
    x: number
    y: number
  }

  export interface Size {
    width: number
    height: number
  }

  export interface Rect extends Point, Size {}

  export interface Corners {
    readonly MIN_X_MIN_Y: number
    readonly MAX_X_MIN_Y: number
    readonly MIN_X_MAX_Y: number
    readonly MAX_X_MAX_Y: number
  }
}

export = CoreAnimationLayer
