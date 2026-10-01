#pragma once

#import <assert.h>
#import <js.h>

#import <CoreGraphics/CoreGraphics.h>

#import "registry.h"

static bool
bare_core_animation__read_number(js_env_t *env, js_value_t *value, const char *name) {
  int err;

  js_value_type_t type;
  err = js_typeof(env, value, &type);
  assert(err == 0);

  if (type == js_number) return true;

  err = js_throw_type_errorf(env, NULL, "Expected a number for '%s'", name);
  assert(err == 0);

  return false;
}

static bool
bare_core_animation__read_bool(js_env_t *env, js_value_t *value, const char *name, bool *result) {
  int err;

  js_value_type_t type;
  err = js_typeof(env, value, &type);
  assert(err == 0);

  if (type != js_boolean) {
    err = js_throw_type_errorf(env, NULL, "Expected a boolean for '%s'", name);
    assert(err == 0);

    return false;
  }

  err = js_get_value_bool(env, value, result);
  assert(err == 0);

  return true;
}

static bool
bare_core_animation__read_uint32(js_env_t *env, js_value_t *value, const char *name, uint32_t *result) {
  if (!bare_core_animation__read_number(env, value, name)) return false;

  int err = js_get_value_uint32(env, value, result);
  assert(err == 0);

  return true;
}

static bool
bare_core_animation__read_double(js_env_t *env, js_value_t *value, const char *name, double *result) {
  if (!bare_core_animation__read_number(env, value, name)) return false;

  int err = js_get_value_double(env, value, result);
  assert(err == 0);

  return true;
}

static js_value_t *
bare_core_animation__from_size(js_env_t *env, CGSize size) {
  int err;

  js_value_t *result;
  err = js_create_object(env, &result);
  assert(err == 0);

#define V(name, n) \
  { \
    js_value_t *val; \
    err = js_create_double(env, n, &val); \
    assert(err == 0); \
    err = js_set_named_property(env, result, name, val); \
    assert(err == 0); \
  }

  V("width", size.width)
  V("height", size.height)
#undef V

  return result;
}

static js_value_t *
bare_core_animation__from_rect(js_env_t *env, CGRect rect) {
  int err;

  js_value_t *result;
  err = js_create_object(env, &result);
  assert(err == 0);

#define V(name, n) \
  { \
    js_value_t *val; \
    err = js_create_double(env, n, &val); \
    assert(err == 0); \
    err = js_set_named_property(env, result, name, val); \
    assert(err == 0); \
  }

  V("x", rect.origin.x)
  V("y", rect.origin.y)
  V("width", rect.size.width)
  V("height", rect.size.height)
#undef V

  return result;
}

static js_value_t *
bare_core_animation__from_point(js_env_t *env, CGPoint point) {
  int err;

  js_value_t *result;
  err = js_create_object(env, &result);
  assert(err == 0);

#define V(name, n) \
  { \
    js_value_t *val; \
    err = js_create_double(env, n, &val); \
    assert(err == 0); \
    err = js_set_named_property(env, result, name, val); \
    assert(err == 0); \
  }

  V("x", point.x)
  V("y", point.y)
#undef V

  return result;
}
