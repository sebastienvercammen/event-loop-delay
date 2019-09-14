#include <node_api.h>
#include <napi-macros.h>
#include <uv.h>

#define DELAY_TIMER_RESOLUTION 10
#define DELAY_TIMER_RESOLUTION_NS DELAY_TIMER_RESOLUTION * 1e6
#define DELAY_TIMER_THRESHOLD_NS 5e6 + DELAY_TIMER_RESOLUTION_NS

typedef struct {
  uint32_t delay;
  uint32_t accumulatedDelay;
  uint32_t count;
  uint32_t time;
  uint64_t prev;
  uv_timer_t timer;
} delay_timer_t;

static void on_uv_interval (uv_timer_t *req) {
  delay_timer_t *delay = (delay_timer_t *) req->data;

  uint64_t time = uv_hrtime();
  uint64_t delta = time - delay->prev;

  if (delta > DELAY_TIMER_THRESHOLD_NS) {
    delta = (delta - DELAY_TIMER_RESOLUTION_NS) / 1e6;

    if (delta > 0xffffffff) delta = 0xffffffff;
    if (delta + delay->accumulatedDelay > 0xffffffff) delay->accumulatedDelay = 0;

    delay->delay = (uint32_t) delta;
    delay->accumulatedDelay += (uint32_t) delta;
    delay->time = (uint32_t) (time / 1e6);
    delay->count++;
  }

  delay->prev = time;
}

NAPI_METHOD(stop_delay_timer) {
  NAPI_ARGV(1)
  NAPI_ARGV_BUFFER_CAST(delay_timer_t *, delay, 0)

  uv_timer_stop(&(delay->timer));

  return NULL;
}

NAPI_METHOD(start_delay_timer) {
  NAPI_ARGV(1)
  NAPI_ARGV_BUFFER_CAST(delay_timer_t *, delay, 0)

  uv_timer_init(uv_default_loop(), &(delay->timer));
  delay->prev = uv_hrtime();
  delay->timer.data = delay;

  uv_timer_start(&(delay->timer), on_uv_interval, DELAY_TIMER_RESOLUTION, DELAY_TIMER_RESOLUTION);
  uv_unref((uv_handle_t *) &(delay->timer));

  return NULL;
}

NAPI_INIT () {
  NAPI_EXPORT_FUNCTION(start_delay_timer)
  NAPI_EXPORT_FUNCTION(stop_delay_timer)
  NAPI_EXPORT_SIZEOF(delay_timer_t)
}
