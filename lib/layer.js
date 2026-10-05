const binding = require('../binding')
const registry = require('bare-foundation-registry')

module.exports = exports = class CoreAnimationLayer {
  constructor(opts = {}) {
    const { tag = null } = opts

    this._tag = tag === null ? binding.layerInit() : tag

    this._token = binding.claim(this._tag, this)
  }

  get [registry.tag]() {
    return this._tag
  }

  get [registry.handle]() {
    return binding.handle(this._tag)
  }

  static of(view) {
    return wrap(binding.layerOf(registry.adopt(binding, view)))
  }

  get cornerRadius() {
    return binding.layerCornerRadius(this._tag)
  }

  set cornerRadius(cornerRadius) {
    binding.layerCornerRadius(this._tag, cornerRadius)
  }

  get borderWidth() {
    return binding.layerBorderWidth(this._tag)
  }

  set borderWidth(borderWidth) {
    binding.layerBorderWidth(this._tag, borderWidth)
  }

  get opacity() {
    return binding.layerOpacity(this._tag)
  }

  set opacity(opacity) {
    binding.layerOpacity(this._tag, opacity)
  }

  get zPosition() {
    return binding.layerZPosition(this._tag)
  }

  set zPosition(zPosition) {
    binding.layerZPosition(this._tag, zPosition)
  }

  get contentsScale() {
    return binding.layerContentsScale(this._tag)
  }

  set contentsScale(contentsScale) {
    binding.layerContentsScale(this._tag, contentsScale)
  }

  get shadowOpacity() {
    return binding.layerShadowOpacity(this._tag)
  }

  set shadowOpacity(shadowOpacity) {
    binding.layerShadowOpacity(this._tag, shadowOpacity)
  }

  get shadowRadius() {
    return binding.layerShadowRadius(this._tag)
  }

  set shadowRadius(shadowRadius) {
    binding.layerShadowRadius(this._tag, shadowRadius)
  }

  get masksToBounds() {
    return binding.layerMasksToBounds(this._tag)
  }

  set masksToBounds(masksToBounds) {
    binding.layerMasksToBounds(this._tag, masksToBounds)
  }

  get maskedCorners() {
    return binding.layerMaskedCorners(this._tag)
  }

  set maskedCorners(maskedCorners) {
    binding.layerMaskedCorners(this._tag, maskedCorners)
  }

  get backgroundColor() {
    return binding.layerBackgroundColor(this._tag)
  }

  set backgroundColor(color) {
    const { red = 0, green = 0, blue = 0, alpha = 1 } = color

    binding.layerBackgroundColor(this._tag, red, green, blue, alpha)
  }

  get borderColor() {
    return binding.layerBorderColor(this._tag)
  }

  set borderColor(color) {
    const { red = 0, green = 0, blue = 0, alpha = 1 } = color

    binding.layerBorderColor(this._tag, red, green, blue, alpha)
  }

  get shadowColor() {
    return binding.layerShadowColor(this._tag)
  }

  set shadowColor(color) {
    const { red = 0, green = 0, blue = 0, alpha = 1 } = color

    binding.layerShadowColor(this._tag, red, green, blue, alpha)
  }

  get frame() {
    return binding.layerFrame(this._tag)
  }

  set frame(frame) {
    const { x = 0, y = 0, width = 0, height = 0 } = frame

    binding.layerFrame(this._tag, x, y, width, height)
  }

  get bounds() {
    return binding.layerBounds(this._tag)
  }

  set bounds(bounds) {
    const { x = 0, y = 0, width = 0, height = 0 } = bounds

    binding.layerBounds(this._tag, x, y, width, height)
  }

  get position() {
    return binding.layerPosition(this._tag)
  }

  set position(position) {
    const { x = 0, y = 0 } = position

    binding.layerPosition(this._tag, x, y)
  }

  get anchorPoint() {
    return binding.layerAnchorPoint(this._tag)
  }

  set anchorPoint(anchorPoint) {
    const { x = 0, y = 0 } = anchorPoint

    binding.layerAnchorPoint(this._tag, x, y)
  }

  get transform() {
    return binding.layerTransform(this._tag)
  }

  set transform(transform) {
    binding.layerTransform(this._tag, transform)
  }

  get shadowOffset() {
    return binding.layerShadowOffset(this._tag)
  }

  set shadowOffset(shadowOffset) {
    const { width = 0, height = 0 } = shadowOffset

    binding.layerShadowOffset(this._tag, width, height)
  }

  get mask() {
    return wrap(binding.layerMask(this._tag))
  }

  set mask(layer) {
    binding.layerMask(this._tag, layer === null ? null : registry.adopt(binding, layer))
  }

  addSublayer(layer) {
    binding.layerAddSublayer(this._tag, registry.adopt(binding, layer))
    return this
  }

  insertSublayer(layer, index) {
    binding.layerInsertSublayerAtIndex(this._tag, registry.adopt(binding, layer), index)
    return this
  }

  removeFromSuperlayer() {
    binding.layerRemoveFromSuperlayer(this._tag)
    return this
  }

  setNeedsDisplay() {
    binding.layerSetNeedsDisplay(this._tag)
    return this
  }

  [Symbol.for('bare.inspect')]() {
    return {
      __proto__: { constructor: CoreAnimationLayer }
    }
  }
}

function wrap(tag) {
  if (tag === null) return null

  return binding.wrapper(tag) ?? new exports({ tag })
}

exports.CORNER_MASK = {
  MIN_X_MIN_Y: binding.CORNER_MIN_X_MIN_Y,
  MAX_X_MIN_Y: binding.CORNER_MAX_X_MIN_Y,
  MIN_X_MAX_Y: binding.CORNER_MIN_X_MAX_Y,
  MAX_X_MAX_Y: binding.CORNER_MAX_X_MAX_Y
}
