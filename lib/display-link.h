#import <assert.h>
#import <js.h>
#import <objc/message.h>

#import <QuartzCore/QuartzCore.h>

#import "bridging.h"

@interface BareCoreAnimationDisplayLinkTarget : NSObject {
@public
  js_env_t *env;
  js_ref_t *ctx;
}

@end

@implementation BareCoreAnimationDisplayLinkTarget

- (void)dealloc {
  js_delete_reference(env, ctx);

  [super dealloc];
}

- (void)tick:(CADisplayLink *)link {
  int err;

  js_handle_scope_t *scope;
  err = js_open_handle_scope(env, &scope);
  assert(err == 0);

  js_value_t *receiver;
  err = js_get_reference_value(env, ctx, &receiver);
  assert(err == 0);

  // The context is weak, so it is empty once the wrapper has been collected.
  if (receiver != NULL) {
    js_value_t *fn;
    err = js_get_named_property(env, receiver, "_ontick", &fn);
    assert(err == 0);

    err = js_call_function(env, receiver, fn, 0, NULL, NULL);
    (void) err;
  }

  err = js_close_handle_scope(env, scope);
  assert(err == 0);
}

@end

// The run loop tells the common modes from a mode of that name by identity, so a
// string that merely spells it is turned back into the constant.
static NSRunLoopMode
bare_core_animation_display_link__mode(NSString *mode) {
  if ([mode isEqualToString:NSRunLoopCommonModes]) return NSRunLoopCommonModes;

  return mode;
}

static BareCoreAnimationDisplayLinkTarget *
bare_core_animation_display_link__target(js_env_t *env, js_value_t *receiver) {
  int err;

  BareCoreAnimationDisplayLinkTarget *target = [[[BareCoreAnimationDisplayLinkTarget alloc] init] autorelease];

  target->env = env;

  // Weak, so the link does not keep its own wrapper alive. Ticks are dropped
  // once the wrapper is collected.
  err = js_create_reference(env, receiver, 0, &target->ctx);
  assert(err == 0);

  return target;
}

// On iOS a display link follows the main display.
static js_value_t *
bare_core_animation_display_link_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  bare_foundation_registry_t *registry;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &registry);
  assert(err == 0);

  assert(argc == 1);

#if TARGET_OS_OSX
  err = js_throw_error(env, NULL, "A display link on macOS follows a view, a window or a screen");
  assert(err == 0);

  return NULL;
#else
  js_value_t *result;

  @autoreleasepool {
    BareCoreAnimationDisplayLinkTarget *target = bare_core_animation_display_link__target(env, argv[0]);

    result = bare_foundation_bridge(env, registry, [CADisplayLink displayLinkWithTarget:target selector:@selector(tick:)]);
  }

  return result;
#endif
}

// On macOS a display link follows the display of a view, a window or a screen,
// which comes from another addon and so is asked for one by selector. That
// keeps AppKit out of here.
static js_value_t *
bare_core_animation_display_link_of(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_foundation_registry_t *registry;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &registry);
  assert(err == 0);

  assert(argc == 2);

  void *handle;
  if (bare_foundation_read_tag(env, registry, argv[0], "handle", &handle) < 0) return NULL;

  SEL factory = @selector(displayLinkWithTarget:selector:);

  id source = (__bridge id) handle;

  if (![source respondsToSelector:factory]) {
    err = js_throw_type_error(env, NULL, "Expected a view, a window or a screen");
    assert(err == 0);

    return NULL;
  }

  js_value_t *result;

  @autoreleasepool {
    BareCoreAnimationDisplayLinkTarget *target = bare_core_animation_display_link__target(env, argv[1]);

    CADisplayLink *link = ((CADisplayLink * (*) (id, SEL, id, SEL)) objc_msgSend)(source, factory, target, @selector(tick:));

    result = bare_foundation_bridge(env, registry, link);
  }

  return result;
}

static js_value_t *
bare_core_animation_display_link_add_to_run_loop(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 3;
  js_value_t *argv[3];

  bare_foundation_registry_t *registry;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &registry);
  assert(err == 0);

  assert(argc == 3);

  void *handle;
  if (bare_foundation_read_tag(env, registry, argv[0], "handle", &handle) < 0) return NULL;

  void *loop;
  if (bare_foundation_read_tag(env, registry, argv[1], "runLoop", &loop) < 0) return NULL;

  @autoreleasepool {
    NSString *mode = bare_core_animation__read_string(env, argv[2], "mode");
    if (mode == nil) return NULL;

    [(__bridge CADisplayLink *) handle addToRunLoop:(__bridge NSRunLoop *) loop forMode:bare_core_animation_display_link__mode(mode)];
  }

  return NULL;
}

static js_value_t *
bare_core_animation_display_link_remove_from_run_loop(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 3;
  js_value_t *argv[3];

  bare_foundation_registry_t *registry;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &registry);
  assert(err == 0);

  assert(argc == 3);

  void *handle;
  if (bare_foundation_read_tag(env, registry, argv[0], "handle", &handle) < 0) return NULL;

  void *loop;
  if (bare_foundation_read_tag(env, registry, argv[1], "runLoop", &loop) < 0) return NULL;

  @autoreleasepool {
    NSString *mode = bare_core_animation__read_string(env, argv[2], "mode");
    if (mode == nil) return NULL;

    [(__bridge CADisplayLink *) handle removeFromRunLoop:(__bridge NSRunLoop *) loop forMode:bare_core_animation_display_link__mode(mode)];
  }

  return NULL;
}

static js_value_t *
bare_core_animation_display_link_invalidate(js_env_t *env, js_callback_info_t *info) {
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
    [(__bridge CADisplayLink *) handle invalidate];
  }

  return NULL;
}

static js_value_t *
bare_core_animation_display_link_paused(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_foundation_registry_t *registry;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &registry);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  void *handle;
  if (bare_foundation_read_tag(env, registry, argv[0], "handle", &handle) < 0) return NULL;

  CADisplayLink *link = (__bridge CADisplayLink *) handle;

  js_value_t *result = NULL;

  if (argc == 1) {
    err = js_get_boolean(env, link.paused, &result);
    assert(err == 0);
  } else {
    bool paused;
    if (!bare_core_animation__read_bool(env, argv[1], "paused", &paused)) return NULL;

    link.paused = paused;
  }

  return result;
}

#define BARE_CORE_ANIMATION_DISPLAY_LINK_TIME(name, property) \
  static js_value_t *name(js_env_t *env, js_callback_info_t *info) { \
    int err; \
\
    size_t argc = 1; \
    js_value_t *argv[1]; \
\
    bare_foundation_registry_t *registry; \
    err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &registry); \
    assert(err == 0); \
\
    assert(argc == 1); \
\
    void *handle; \
    if (bare_foundation_read_tag(env, registry, argv[0], "handle", &handle) < 0) return NULL; \
\
    js_value_t *result; \
    err = js_create_double(env, ((__bridge CADisplayLink *) handle).property, &result); \
    assert(err == 0); \
\
    return result; \
  }

BARE_CORE_ANIMATION_DISPLAY_LINK_TIME(bare_core_animation_display_link_timestamp, timestamp)
BARE_CORE_ANIMATION_DISPLAY_LINK_TIME(bare_core_animation_display_link_target_timestamp, targetTimestamp)
BARE_CORE_ANIMATION_DISPLAY_LINK_TIME(bare_core_animation_display_link_duration, duration)

#undef BARE_CORE_ANIMATION_DISPLAY_LINK_TIME
