#import <QuartzCore/QuartzCore.h>

#import "bridging.h"

// The view comes from another addon, so it is asked for its layer by selector.
// That keeps AppKit and UIKit out of here.
static js_value_t *
bare_core_animation_layer_of(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  bare_foundation_registry_t *registry;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &registry);
  assert(err == 0);

  assert(argc == 1);

  void *handle;
  if (bare_foundation_read_tag(env, registry, argv[0], "handle", &handle) < 0) return NULL;

  js_value_t *result;

  @autoreleasepool {
    id view = (__bridge id) handle;

    if (![view respondsToSelector:@selector(layer)]) {
      err = js_throw_type_error(env, NULL, "Object has no layer");
      assert(err == 0);

      return NULL;
    }

    result = bare_foundation_bridge(env, registry, [view layer]);
  }

  return result;
}

static js_value_t *
bare_core_animation_layer_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  bare_foundation_registry_t *registry;
  err = js_get_callback_info(env, info, NULL, NULL, NULL, (void **) &registry);
  assert(err == 0);

  js_value_t *result;

  @autoreleasepool {
    result = bare_foundation_bridge(env, registry, [CALayer layer]);
  }

  return result;
}

#define V(name, expr, type, ctype, read) \
  static js_value_t * \
  bare_core_animation_layer_##name(js_env_t *env, js_callback_info_t *info) { \
    int err; \
    size_t argc = 2; \
    js_value_t *argv[2]; \
    bare_foundation_registry_t *registry; \
    err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &registry); \
    assert(err == 0); \
    assert(argc == 1 || argc == 2); \
    void *handle; \
    if (bare_foundation_read_tag(env, registry, argv[0], "handle", &handle) < 0) return NULL; \
    js_value_t *result = NULL; \
    @autoreleasepool { \
      CALayer *layer = (__bridge CALayer *) handle; \
      if (argc == 1) { \
        err = js_create_##type(env, layer.expr, &result); \
        assert(err == 0); \
      } else { \
        ctype value; \
        if (!bare_core_animation__read_##read(env, argv[1], #name, &value)) return NULL; \
        layer.expr = value; \
      } \
    } \
    return result; \
  } \
\
  static void \
  bare_core_animation_layer_##name##_typed(js_value_t *receiver, int32_t bare_tag, ctype value, js_typed_callback_info_t *info) { \
    int err; \
    bare_foundation_registry_t *registry; \
    err = js_get_typed_callback_info(info, NULL, (void **) &registry); \
    assert(err == 0); \
    id bare_object = bare_foundation_object(registry, bare_tag); \
    if (bare_object == nil) return; \
    @autoreleasepool { \
      ((CALayer *) bare_object).expr = value; \
    } \
  }

V(corner_radius, cornerRadius, double, double, double)
V(border_width, borderWidth, double, double, double)
V(opacity, opacity, double, double, double)
V(z_position, zPosition, double, double, double)
V(contents_scale, contentsScale, double, double, double)
V(shadow_opacity, shadowOpacity, double, double, double)
V(shadow_radius, shadowRadius, double, double, double)
V(masked_corners, maskedCorners, uint32, uint32_t, uint32)
#undef V

static js_value_t *
bare_core_animation_layer_masks_to_bounds(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_foundation_registry_t *registry;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &registry);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  void *handle;
  if (bare_foundation_read_tag(env, registry, argv[0], "handle", &handle) < 0) return NULL;

  js_value_t *result = NULL;

  @autoreleasepool {
    CALayer *layer = (__bridge CALayer *) handle;

    if (argc == 1) {
      err = js_get_boolean(env, layer.masksToBounds, &result);
      assert(err == 0);
    } else {
      bool value;
      if (!bare_core_animation__read_bool(env, argv[1], "masksToBounds", &value)) return NULL;

      layer.masksToBounds = value;
    }
  }

  return result;
}

static void
bare_core_animation_layer_masks_to_bounds_typed(js_value_t *receiver, int32_t bare_tag, bool value, js_typed_callback_info_t *info) {
  int err;

  bare_foundation_registry_t *registry;
  err = js_get_typed_callback_info(info, NULL, (void **) &registry);
  assert(err == 0);

  id bare_object = bare_foundation_object(registry, bare_tag);

  if (bare_object == nil) return;

  @autoreleasepool {
    ((CALayer *) bare_object).masksToBounds = value;
  }
}

// Colours cross as plain components, so there is no wrapper to own.
static js_value_t *
bare_core_animation__from_color(js_env_t *env, CGColorRef color) {
  int err;

  js_value_t *result;
  err = js_create_object(env, &result);
  assert(err == 0);

  const CGFloat *components = color == NULL ? NULL : CGColorGetComponents(color);
  size_t count = color == NULL ? 0 : CGColorGetNumberOfComponents(color);

#define V(name, value) \
  { \
    js_value_t *val; \
    err = js_create_double(env, value, &val); \
    assert(err == 0); \
    err = js_set_named_property(env, result, name, val); \
    assert(err == 0); \
  }

  // A grey has one component plus alpha. Anything else reads as opaque black.
  if (count >= 4) {
    V("red", components[0])
    V("green", components[1])
    V("blue", components[2])
    V("alpha", components[3])
  } else if (count == 2) {
    V("red", components[0])
    V("green", components[0])
    V("blue", components[0])
    V("alpha", components[1])
  } else {
    V("red", 0)
    V("green", 0)
    V("blue", 0)
    V("alpha", 0)
  }
#undef V

  return result;
}

#define V(name, expr) \
  static js_value_t * \
  bare_core_animation_layer_##name(js_env_t *env, js_callback_info_t *info) { \
    int err; \
    size_t argc = 5; \
    js_value_t *argv[5]; \
    bare_foundation_registry_t *registry; \
    err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &registry); \
    assert(err == 0); \
    assert(argc == 1 || argc == 5); \
    void *handle; \
    if (bare_foundation_read_tag(env, registry, argv[0], "handle", &handle) < 0) return NULL; \
    js_value_t *result = NULL; \
    @autoreleasepool { \
      CALayer *layer = (__bridge CALayer *) handle; \
      if (argc == 1) { \
        result = bare_core_animation__from_color(env, layer.expr); \
      } else { \
        double rgba[4]; \
        static const char *names[] = {"red", "green", "blue", "alpha"}; \
        for (size_t i = 0; i < 4; i++) { \
          if (!bare_core_animation__read_double(env, argv[i + 1], names[i], &rgba[i])) return NULL; \
        } \
        CGColorRef color = CGColorCreateSRGB(rgba[0], rgba[1], rgba[2], rgba[3]); \
        layer.expr = color; \
        CGColorRelease(color); \
      } \
    } \
    return result; \
  } \
\
  static void \
  bare_core_animation_layer_##name##_typed(js_value_t *receiver, int32_t bare_tag, double red, double green, double blue, double alpha, js_typed_callback_info_t *info) { \
    int err; \
    bare_foundation_registry_t *registry; \
    err = js_get_typed_callback_info(info, NULL, (void **) &registry); \
    assert(err == 0); \
    id bare_object = bare_foundation_object(registry, bare_tag); \
    if (bare_object == nil) return; \
    @autoreleasepool { \
      CGColorRef color = CGColorCreateSRGB(red, green, blue, alpha); \
      ((CALayer *) bare_object).expr = color; \
      CGColorRelease(color); \
    } \
  }

V(background_color, backgroundColor)
V(border_color, borderColor)
V(shadow_color, shadowColor)
#undef V

#define V(name, expr, make, count, ...) \
  static js_value_t * \
  bare_core_animation_layer_##name(js_env_t *env, js_callback_info_t *info) { \
    int err; \
    size_t argc = 1 + count; \
    js_value_t *argv[1 + count]; \
    bare_foundation_registry_t *registry; \
    err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &registry); \
    assert(err == 0); \
    assert(argc == 1 || argc == 1 + count); \
    void *handle; \
    if (bare_foundation_read_tag(env, registry, argv[0], "handle", &handle) < 0) return NULL; \
    js_value_t *result = NULL; \
    @autoreleasepool { \
      CALayer *layer = (__bridge CALayer *) handle; \
      if (argc == 1) { \
        result = bare_core_animation__from_##make(env, layer.expr); \
      } else { \
        double v[count]; \
        static const char *names[] = {__VA_ARGS__}; \
        for (size_t i = 0; i < count; i++) { \
          if (!bare_core_animation__read_double(env, argv[i + 1], names[i], &v[i])) return NULL; \
        } \
        layer.expr = bare_core_animation__make_##make(v); \
      } \
    } \
    return result; \
  }

static CGRect
bare_core_animation__make_rect(const double *v) {
  return CGRectMake(v[0], v[1], v[2], v[3]);
}

static CGPoint
bare_core_animation__make_point(const double *v) {
  return CGPointMake(v[0], v[1]);
}

static CGSize
bare_core_animation__make_size(const double *v) {
  return CGSizeMake(v[0], v[1]);
}

V(frame, frame, rect, 4, "x", "y", "width", "height")
V(bounds, bounds, rect, 4, "x", "y", "width", "height")
V(position, position, point, 2, "x", "y")
V(anchor_point, anchorPoint, point, 2, "x", "y")
V(shadow_offset, shadowOffset, size, 2, "width", "height")
#undef V

// A `CATransform3D` crosses as sixteen numbers, in the order CSS uses for
// `matrix3d()`.
#define BARE_CORE_ANIMATION_TRANSFORM(V) \
  V(0, m11) V(1, m12) V(2, m13) V(3, m14) \
  V(4, m21) V(5, m22) V(6, m23) V(7, m24) \
  V(8, m31) V(9, m32) V(10, m33) V(11, m34) \
  V(12, m41) V(13, m42) V(14, m43) V(15, m44)

static js_value_t *
bare_core_animation_layer_transform(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_foundation_registry_t *registry;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &registry);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  void *handle;
  if (bare_foundation_read_tag(env, registry, argv[0], "handle", &handle) < 0) return NULL;

  js_value_t *result = NULL;

  @autoreleasepool {
    CALayer *layer = (__bridge CALayer *) handle;

    if (argc == 1) {
      CATransform3D transform = layer.transform;

      err = js_create_array_with_length(env, 16, &result);
      assert(err == 0);

#define V(index, field) \
  { \
    js_value_t *value; \
    err = js_create_double(env, transform.field, &value); \
    assert(err == 0); \
    err = js_set_element(env, result, index, value); \
    assert(err == 0); \
  }
      BARE_CORE_ANIMATION_TRANSFORM(V)
#undef V
    } else {
      CATransform3D transform;

#define V(index, field) \
  { \
    js_value_t *value; \
    err = js_get_element(env, argv[1], index, &value); \
    assert(err == 0); \
    double component; \
    if (!bare_core_animation__read_double(env, value, #field, &component)) return NULL; \
    transform.field = component; \
  }
      BARE_CORE_ANIMATION_TRANSFORM(V)
#undef V

      layer.transform = transform;
    }
  }

  return result;
}

static js_value_t *
bare_core_animation_layer_mask(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_foundation_registry_t *registry;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &registry);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  void *handle;
  if (bare_foundation_read_tag(env, registry, argv[0], "handle", &handle) < 0) return NULL;

  js_value_t *result = NULL;

  @autoreleasepool {
    CALayer *layer = (__bridge CALayer *) handle;

    if (argc == 1) {
      result = bare_foundation_bridge(env, registry, layer.mask);
    } else {
      js_value_type_t type;
      err = js_typeof(env, argv[1], &type);
      assert(err == 0);

      if (type == js_null || type == js_undefined) {
        layer.mask = nil;
      } else {
        void *mask;
        if (bare_foundation_read_tag(env, registry, argv[1], "mask", &mask) < 0) return NULL;

        layer.mask = (__bridge CALayer *) mask;
      }
    }
  }

  return result;
}

static js_value_t *
bare_core_animation_layer_convert_point_to_layer(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 4;
  js_value_t *argv[4];

  bare_foundation_registry_t *registry;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &registry);
  assert(err == 0);

  assert(argc == 4);

  void *handle;
  if (bare_foundation_read_tag(env, registry, argv[0], "handle", &handle) < 0) return NULL;

  double x, y;
  if (!bare_core_animation__read_double(env, argv[1], "x", &x)) return NULL;
  if (!bare_core_animation__read_double(env, argv[2], "y", &y)) return NULL;

  js_value_type_t type;
  err = js_typeof(env, argv[3], &type);
  assert(err == 0);

  void *other = NULL;

  if (type != js_null && type != js_undefined) {
    if (bare_foundation_read_tag(env, registry, argv[3], "layer", &other) < 0) return NULL;
  }

  js_value_t *result;

  @autoreleasepool {
    CGPoint point = [(__bridge CALayer *) handle convertPoint:CGPointMake(x, y) toLayer:(__bridge CALayer *) other];

    result = bare_core_animation__from_point(env, point);
  }

  return result;
}

static js_value_t *
bare_core_animation_layer_convert_point_from_layer(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 4;
  js_value_t *argv[4];

  bare_foundation_registry_t *registry;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &registry);
  assert(err == 0);

  assert(argc == 4);

  void *handle;
  if (bare_foundation_read_tag(env, registry, argv[0], "handle", &handle) < 0) return NULL;

  double x, y;
  if (!bare_core_animation__read_double(env, argv[1], "x", &x)) return NULL;
  if (!bare_core_animation__read_double(env, argv[2], "y", &y)) return NULL;

  js_value_type_t type;
  err = js_typeof(env, argv[3], &type);
  assert(err == 0);

  void *other = NULL;

  if (type != js_null && type != js_undefined) {
    if (bare_foundation_read_tag(env, registry, argv[3], "layer", &other) < 0) return NULL;
  }

  js_value_t *result;

  @autoreleasepool {
    CGPoint point = [(__bridge CALayer *) handle convertPoint:CGPointMake(x, y) fromLayer:(__bridge CALayer *) other];

    result = bare_core_animation__from_point(env, point);
  }

  return result;
}

static js_value_t *
bare_core_animation_layer_convert_rect_to_layer(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 6;
  js_value_t *argv[6];

  bare_foundation_registry_t *registry;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &registry);
  assert(err == 0);

  assert(argc == 6);

  void *handle;
  if (bare_foundation_read_tag(env, registry, argv[0], "handle", &handle) < 0) return NULL;

  double v[4];
  static const char *names[] = {"x", "y", "width", "height"};

  for (size_t i = 0; i < 4; i++) {
    if (!bare_core_animation__read_double(env, argv[i + 1], names[i], &v[i])) return NULL;
  }

  js_value_type_t type;
  err = js_typeof(env, argv[5], &type);
  assert(err == 0);

  void *other = NULL;

  if (type != js_null && type != js_undefined) {
    if (bare_foundation_read_tag(env, registry, argv[5], "layer", &other) < 0) return NULL;
  }

  js_value_t *result;

  @autoreleasepool {
    CGRect rect = [(__bridge CALayer *) handle convertRect:bare_core_animation__make_rect(v) toLayer:(__bridge CALayer *) other];

    result = bare_core_animation__from_rect(env, rect);
  }

  return result;
}

static js_value_t *
bare_core_animation_layer_convert_rect_from_layer(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 6;
  js_value_t *argv[6];

  bare_foundation_registry_t *registry;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &registry);
  assert(err == 0);

  assert(argc == 6);

  void *handle;
  if (bare_foundation_read_tag(env, registry, argv[0], "handle", &handle) < 0) return NULL;

  double v[4];
  static const char *names[] = {"x", "y", "width", "height"};

  for (size_t i = 0; i < 4; i++) {
    if (!bare_core_animation__read_double(env, argv[i + 1], names[i], &v[i])) return NULL;
  }

  js_value_type_t type;
  err = js_typeof(env, argv[5], &type);
  assert(err == 0);

  void *other = NULL;

  if (type != js_null && type != js_undefined) {
    if (bare_foundation_read_tag(env, registry, argv[5], "layer", &other) < 0) return NULL;
  }

  js_value_t *result;

  @autoreleasepool {
    CGRect rect = [(__bridge CALayer *) handle convertRect:bare_core_animation__make_rect(v) fromLayer:(__bridge CALayer *) other];

    result = bare_core_animation__from_rect(env, rect);
  }

  return result;
}

static js_value_t *
bare_core_animation_layer_add_sublayer(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_foundation_registry_t *registry;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &registry);
  assert(err == 0);

  assert(argc == 2);

  void *handle;
  if (bare_foundation_read_tag(env, registry, argv[0], "handle", &handle) < 0) return NULL;

  void *sublayer;
  if (bare_foundation_read_tag(env, registry, argv[1], "sublayer", &sublayer) < 0) return NULL;

  @autoreleasepool {
    [(__bridge CALayer *) handle addSublayer:(__bridge CALayer *) sublayer];
  }

  return NULL;
}

static js_value_t *
bare_core_animation_layer_insert_sublayer_at_index(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 3;
  js_value_t *argv[3];

  bare_foundation_registry_t *registry;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &registry);
  assert(err == 0);

  assert(argc == 3);

  void *handle;
  if (bare_foundation_read_tag(env, registry, argv[0], "handle", &handle) < 0) return NULL;

  void *sublayer;
  if (bare_foundation_read_tag(env, registry, argv[1], "sublayer", &sublayer) < 0) return NULL;

  uint32_t index;
  err = js_get_value_uint32(env, argv[2], &index);
  assert(err == 0);

  @autoreleasepool {
    [(__bridge CALayer *) handle insertSublayer:(__bridge CALayer *) sublayer atIndex:index];
  }

  return NULL;
}

static js_value_t *
bare_core_animation_layer_remove_from_superlayer(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  bare_foundation_registry_t *registry;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &registry);
  assert(err == 0);

  assert(argc == 1);

  void *handle;
  if (bare_foundation_read_tag(env, registry, argv[0], "handle", &handle) < 0) return NULL;

  @autoreleasepool {
    [(__bridge CALayer *) handle removeFromSuperlayer];
  }

  return NULL;
}

static js_value_t *
bare_core_animation_layer_set_needs_display(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  bare_foundation_registry_t *registry;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &registry);
  assert(err == 0);

  assert(argc == 1);

  void *handle;
  if (bare_foundation_read_tag(env, registry, argv[0], "handle", &handle) < 0) return NULL;

  @autoreleasepool {
    [(__bridge CALayer *) handle setNeedsDisplay];
  }

  return NULL;
}
