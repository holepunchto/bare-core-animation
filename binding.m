#import <assert.h>
#import <bare.h>
#import <js.h>

#import "lib/layer.h"
#import "lib/shape-layer.h"

static js_value_t *
bare_core_animation_exports(js_env_t *env, js_value_t *exports) {
  int err;

  bare_foundation_registry_t *registry = bare_foundation_registry_create(env, exports);

#define T(name, fn, signature, typed) \
  { \
    js_value_t *val; \
    err = js_create_typed_function(env, name, -1, fn, signature, typed, registry, &val); \
    assert(err == 0); \
    err = js_set_named_property(env, exports, name, val); \
    assert(err == 0); \
  }

#define V(name, fn) \
  { \
    js_value_t *val; \
    err = js_create_function(env, name, -1, fn, registry, &val); \
    assert(err == 0); \
    err = js_set_named_property(env, exports, name, val); \
    assert(err == 0); \
  }

  V("claim", bare_foundation_claim)
  V("wrapper", bare_foundation_wrapper)
  V("registrySize", bare_foundation_registry_size)
  V("handle", bare_foundation_handle)
  V("adopt", bare_foundation_adopt)

  V("layerInit", bare_core_animation_layer_init)

  V("shapeLayerInit", bare_core_animation_shape_layer_init)
  V("shapeLayerPath", bare_core_animation_shape_layer_path)
  V("shapeLayerFillColor", bare_core_animation_shape_layer_fill_color)
  V("shapeLayerStrokeColor", bare_core_animation_shape_layer_stroke_color)
  V("shapeLayerLineWidth", bare_core_animation_shape_layer_line_width)
  V("shapeLayerLineDashPattern", bare_core_animation_shape_layer_line_dash_pattern)
  V("layerOf", bare_core_animation_layer_of)
  V("layerMask", bare_core_animation_layer_mask)
  V("layerAddSublayer", bare_core_animation_layer_add_sublayer)
  V("layerInsertSublayerAtIndex", bare_core_animation_layer_insert_sublayer_at_index)
  V("layerRemoveFromSuperlayer", bare_core_animation_layer_remove_from_superlayer)
  V("layerSetNeedsDisplay", bare_core_animation_layer_set_needs_display)

#define SCALAR(name, fn, arg) \
  T( \
    name, \
    fn, \
    &((js_callback_signature_t){ \
      .version = 0, \
      .result = js_undefined, \
      .args_len = 3, \
      .args = (int[]){js_object, js_int32, arg}, \
    }), \
    fn##_typed \
  )

  SCALAR("layerCornerRadius", bare_core_animation_layer_corner_radius, js_float64)
  SCALAR("layerBorderWidth", bare_core_animation_layer_border_width, js_float64)
  SCALAR("layerOpacity", bare_core_animation_layer_opacity, js_float64)
  SCALAR("layerZPosition", bare_core_animation_layer_z_position, js_float64)
  SCALAR("layerContentsScale", bare_core_animation_layer_contents_scale, js_float64)
  SCALAR("layerShadowOpacity", bare_core_animation_layer_shadow_opacity, js_float64)
  SCALAR("layerShadowRadius", bare_core_animation_layer_shadow_radius, js_float64)
  SCALAR("layerMaskedCorners", bare_core_animation_layer_masked_corners, js_uint32)
  SCALAR("layerMasksToBounds", bare_core_animation_layer_masks_to_bounds, js_boolean)
#undef SCALAR

#define COLOR(name, fn) \
  T( \
    name, \
    fn, \
    &((js_callback_signature_t){ \
      .version = 0, \
      .result = js_undefined, \
      .args_len = 6, \
      .args = (int[]){js_object, js_int32, js_float64, js_float64, js_float64, js_float64}, \
    }), \
    fn##_typed \
  )

  COLOR("layerBackgroundColor", bare_core_animation_layer_background_color)
  COLOR("layerBorderColor", bare_core_animation_layer_border_color)
  COLOR("layerShadowColor", bare_core_animation_layer_shadow_color)
#undef COLOR

  V("layerFrame", bare_core_animation_layer_frame)
  V("layerBounds", bare_core_animation_layer_bounds)
  V("layerPosition", bare_core_animation_layer_position)
  V("layerAnchorPoint", bare_core_animation_layer_anchor_point)
  V("layerShadowOffset", bare_core_animation_layer_shadow_offset)
  V("layerTransform", bare_core_animation_layer_transform)

#undef V
#undef T

#define V(name, n) \
  { \
    js_value_t *val; \
    err = js_create_uint32(env, n, &val); \
    assert(err == 0); \
    err = js_set_named_property(env, exports, name, val); \
    assert(err == 0); \
  }

  V("CORNER_MIN_X_MIN_Y", kCALayerMinXMinYCorner)
  V("CORNER_MAX_X_MIN_Y", kCALayerMaxXMinYCorner)
  V("CORNER_MIN_X_MAX_Y", kCALayerMinXMaxYCorner)
  V("CORNER_MAX_X_MAX_Y", kCALayerMaxXMaxYCorner)
#undef V

  return exports;
}

BARE_MODULE(bare_core_animation, bare_core_animation_exports)
