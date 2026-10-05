import { Wrapper } from 'bare-foundation-registry'
import CoreAnimationLayer = require('./layer')

/** A layer that draws a path. */
interface CoreAnimationShapeLayer extends CoreAnimationLayer {
  /**
   * The path to draw, from another addon, such as an `NSBezierPath` or a `UIBezierPath`. Set it to
   * `null` to draw nothing.
   * @throws A `TypeError` when the object is not a path.
   */
  set path(path: Wrapper | null)

  /** The color the path is filled with, or `null` for no fill. */
  set fillColor(color: Partial<CoreAnimationLayer.Color> | null)

  /** The color the path is stroked with, or `null` for no stroke. */
  set strokeColor(color: Partial<CoreAnimationLayer.Color> | null)

  /** The width of the stroke, in points. */
  set lineWidth(width: number)

  /** The lengths of the dashes and gaps in the stroke. An empty list draws a solid line. */
  set lineDashPattern(pattern: number[])
}

declare class CoreAnimationShapeLayer {
  /** Create a new shape layer with no path. */
  constructor()
}

export = CoreAnimationShapeLayer
