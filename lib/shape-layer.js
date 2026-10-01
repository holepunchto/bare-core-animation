const binding = require('../binding')
const registry = require('bare-foundation-registry')
const CoreAnimationLayer = require('./layer')

module.exports = exports = class CoreAnimationShapeLayer extends CoreAnimationLayer {
  constructor(opts = {}) {
    super({ tag: opts.tag ?? binding.shapeLayerInit() })
  }

  set path(path) {
    binding.shapeLayerPath(this._tag, path === null ? null : registry.adopt(binding, path))
  }

  set fillColor(color) {
    if (color === null) return binding.shapeLayerFillColor(this._tag)

    const { red = 0, green = 0, blue = 0, alpha = 1 } = color

    binding.shapeLayerFillColor(this._tag, red, green, blue, alpha)
  }

  set strokeColor(color) {
    if (color === null) return binding.shapeLayerStrokeColor(this._tag)

    const { red = 0, green = 0, blue = 0, alpha = 1 } = color

    binding.shapeLayerStrokeColor(this._tag, red, green, blue, alpha)
  }

  set lineWidth(value) {
    binding.shapeLayerLineWidth(this._tag, value)
  }

  set lineDashPattern(pattern) {
    binding.shapeLayerLineDashPattern(this._tag, pattern)
  }

  [Symbol.for('bare.inspect')]() {
    return {
      __proto__: { constructor: CoreAnimationShapeLayer }
    }
  }
}
