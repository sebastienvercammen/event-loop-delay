module.exports = function samplerJavascript () {
  const DELAY_TIMER_RESOLUTION_MS = 10
  let prev = now()

  const interval = setInterval(tick, DELAY_TIMER_RESOLUTION_MS)
  interval.unref()

  const delayer = {
    native: false,
    delay: 0,
    accumulatedDelay: 0,
    count: 0,
    time: 0,
    destroy () {
      clearInterval(interval)
    }
  }

  return delayer

  function tick () {
    const time = now()
    const delta = time - prev

    prev = time

    if (delta > DELAY_TIMER_RESOLUTION_MS) {
      delta = delta - DELAY_TIMER_RESOLUTION_MS
      delayer.count++
      delayer.time = time
      delayer.delay = delta
      delayer.accumulatedDelay += delta
    }
  }
}

function now () {
  const ts = process.hrtime()
  return (ts[0] * 1e3) + Math.floor((ts[1] / 1e6))
}
