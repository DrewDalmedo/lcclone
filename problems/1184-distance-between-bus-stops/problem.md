id: 1184
slug: distance-between-bus-stops
title: Distance Between Bus Stops
difficulty: easy
tags: array
signature: int distanceBetweenBusStops(int[] distance, int start, int destination)
---
A hiking trail runs in a closed loop past `n` shelters, numbered `0` to
`n - 1` in the order you reach them walking one way round: after shelter
`i` comes shelter `i + 1`, and after the last shelter, `n - 1`, the trail
leads back to shelter `0`. `distance[i]` is the length of the stretch of
trail from shelter `i` to the shelter after it.

You may walk round the loop in either direction, and a stretch is equally
long both ways. Return the length of the shortest walk from shelter
`start` to shelter `destination`. If they are the same shelter, there is
nothing to walk, so the answer is `0`.

@examples

## Constraints

- `1 <= n <= 10^4`
- `distance.length == n`
- `0 <= start, destination < n`
- `0 <= distance[i] <= 10^4`
