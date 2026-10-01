#import <assert.h>
#import <bare.h>
#import <js.h>

#import <QuartzCore/QuartzCore.h>

#import "bridging.h"

// `NSBezierPath` and `UIBezierPath` both answer `CGPath`, and neither of their
// headers can be used here.
@protocol BareCoreAnimationPath
@property (readonly) CGPathRef CGPath;
@end

static js_value_t *
bare_core_animation_shape_layer_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  bare_foundation_registry_t *registry;
  err = js_get_callback_info(env, info, NULL, NULL, NULL, (void **) &registry);
  assert(err == 0);

  js_value_t *result;

  @autoreleasepool {
    result = bare_foundation_bridge(env, registry, [CAShapeLayer layer]);
  }

  return result;
}

static js_value_t *
bare_core_animation_shape_layer_path(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_foundation_registry_t *registry;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &registry);
  assert(err == 0);

  assert(argc == 2);

  void *handle;
  if (bare_foundation_read_tag(env, registry, argv[0], "handle", &handle) < 0) return NULL;

  @autoreleasepool {
    CAShapeLayer *layer = (__bridge CAShapeLayer *) handle;

    js_value_type_t type;
    err = js_typeof(env, argv[1], &type);
    assert(err == 0);

    if (type == js_null || type == js_undefined) {
      layer.path = NULL;

      return NULL;
    }

    void *path;
    if (bare_foundation_read_tag(env, registry, argv[1], "path", &path) < 0) return NULL;

    id object = (__bridge id) path;

    if (![object respondsToSelector:@selector(CGPath)]) {
      err = js_throw_type_error(env, NULL, "Object has no path");
      assert(err == 0);

      return NULL;
    }

    layer.path = ((id<BareCoreAnimationPath>) object).CGPath;
  }

  return NULL;
}

#define V(name, expr) \
  static js_value_t * \
  bare_core_animation_shape_layer_##name(js_env_t *env, js_callback_info_t *info) { \
    int err; \
    size_t argc = 5; \
    js_value_t *argv[5]; \
    bare_foundation_registry_t *registry; \
    err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &registry); \
    assert(err == 0); \
    assert(argc == 1 || argc == 5); \
    void *handle; \
    if (bare_foundation_read_tag(env, registry, argv[0], "handle", &handle) < 0) return NULL; \
    @autoreleasepool { \
      CAShapeLayer *layer = (__bridge CAShapeLayer *) handle; \
      if (argc == 1) { \
        layer.expr = NULL; \
      } else { \
        double components[4]; \
        for (int i = 0; i < 4; i++) { \
          err = js_get_value_double(env, argv[i + 1], &components[i]); \
          assert(err == 0); \
        } \
        CGColorRef color = CGColorCreateSRGB(components[0], components[1], components[2], components[3]); \
        layer.expr = color; \
        CGColorRelease(color); \
      } \
    } \
    return NULL; \
  }

V(fill_color, fillColor)
V(stroke_color, strokeColor)
#undef V

static js_value_t *
bare_core_animation_shape_layer_line_width(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_foundation_registry_t *registry;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &registry);
  assert(err == 0);

  assert(argc == 2);

  void *handle;
  if (bare_foundation_read_tag(env, registry, argv[0], "handle", &handle) < 0) return NULL;

  double width;
  err = js_get_value_double(env, argv[1], &width);
  assert(err == 0);

  @autoreleasepool {
    ((__bridge CAShapeLayer *) handle).lineWidth = width;
  }

  return NULL;
}

static js_value_t *
bare_core_animation_shape_layer_line_dash_pattern(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_foundation_registry_t *registry;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &registry);
  assert(err == 0);

  assert(argc == 2);

  void *handle;
  if (bare_foundation_read_tag(env, registry, argv[0], "handle", &handle) < 0) return NULL;

  @autoreleasepool {
    CAShapeLayer *layer = (__bridge CAShapeLayer *) handle;

    uint32_t len;
    err = js_get_array_length(env, argv[1], &len);
    assert(err == 0);

    if (len == 0) {
      layer.lineDashPattern = nil;

      return NULL;
    }

    NSMutableArray *pattern = [NSMutableArray arrayWithCapacity:len];

    for (uint32_t i = 0; i < len; i++) {
      js_value_t *element;
      err = js_get_element(env, argv[1], i, &element);
      assert(err == 0);

      double value;
      err = js_get_value_double(env, element, &value);
      assert(err == 0);

      [pattern addObject:@(value)];
    }

    layer.lineDashPattern = pattern;
  }

  return NULL;
}
