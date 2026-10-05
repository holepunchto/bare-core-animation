# bare-core-animation

Core Animation for Bare. Use it to style the layer behind a view, with rounded corners, borders, shadows and transforms, or to draw shapes with a shape layer.

It works with views from other addons, such as `bare-app-kit` on macOS and `bare-ui-kit` on iOS. You hand it a view and get back that view's layer.

```
npm i bare-core-animation
```

## Usage

```js
const { View, BezierPath } = require('bare-app-kit')
const { Layer, ShapeLayer } = require('bare-core-animation')

const view = new View({ x: 0, y: 0, width: 200, height: 200 })

// On macOS, a view only has a layer once it asks for one.
view.wantsLayer = true

const layer = Layer.of(view)

layer.cornerRadius = 12
layer.maskedCorners = Layer.CORNER_MASK.MIN_X_MIN_Y | Layer.CORNER_MASK.MAX_X_MIN_Y
layer.backgroundColor = { red: 0.2, green: 0.4, blue: 1 }
layer.shadowOpacity = 0.3

const circle = new ShapeLayer()

circle.frame = { x: 50, y: 50, width: 100, height: 100 }
circle.path = BezierPath.withOval(0, 0, 100, 100)
circle.fillColor = { red: 1, green: 1, blue: 1 }

layer.addSublayer(circle)
```

## License

Apache-2.0
