module.exports = function samplerJavascript () {
  let prev = now()

  const interval = setInterval(tick, 10)
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

    if (delta > 10) {
      delayer.count++
      delayer.time = time
      delayer.delay = delta - 10
      delayer.accumulatedDelay += delta - 10 // minus interval ms
    }
  }
}

function now () {
  const ts = process.hrtime()
  return (ts[0] * 1e3) + Math.floor((ts[1] / 1e6))
}
