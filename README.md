# event-loop-delay

A minimal overhead event loop delay sampler.

```
npm install event-loop-delay
```

Bundles a prebuilt N-API addon that makes the overhead of the sampler very minimal (around 0.2% on my machine, ymmv).

If the addon fails for some reason a JS fallback is used with a bit higher overhead (around 1-2% on my machine).

## Usage

``` js
const loopDelay = require('event-loop-delay')

const sampler = loopDelay()

setInterval(function () {
  console.log('Last delay in ms is', sampler.delay)
  console.log('Last delay was detected on', sampler.time)
  console.log('Accumulated delay in ms is', sampler.accumulatedDelay)
  console.log('Accumulated times a delay was detected', sampler.count)
}, 1000)

setInterval(function () {
  for (let i = 0; i < 1e7; i++) {
    // simulate event loop blocking
  }
}, 3000)
```

Running the above should run the sampler measuring the event loop delay.

## API

#### `sampler = loopDelay()`

Make a new sampler. Will sample the event loop every 10ms.

#### `sampler.delay`

Last measured event loop delay in ms.

#### `sampler.time`

Timestamp in ms when we last measured event loop delay.

#### `sampler.accumulatedDelay`

Accumulated event loop delay in ms measured since sampler was created.

#### `sampler.count`

Accumulated times the event loop delay was detected.

#### `sampler.destroy()`

Destroys the sampler.

#### `sampler.native`

Boolean telling you if the native addon is used.

## License

MIT
