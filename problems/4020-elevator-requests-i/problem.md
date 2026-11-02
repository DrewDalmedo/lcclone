id: 4020
slug: elevator-requests-i
title: Elevator Requests I
difficulty: easy
tags: array, simulation
signature: int elevatorRequests(int n, int[] requests)
---
A building has `n` floors, numbered `0` (the ground floor) up to `n - 1`,
and one elevator, which waits at floor `0` when the clock starts.

The elevator works through the list `requests` from front to back. To
handle `requests[i]`, it travels directly from wherever it is to that
floor, and the moment it gets there it leaves for `requests[i + 1]`. A
request is handled only by the trip made for it: going past floor `5` on
the way somewhere else does not handle a request for floor `5` that comes
later in the list.

Moving one floor up or one floor down takes one second, so a trip from
floor `a` to floor `b` lasts `|a - b|` seconds. Nothing else takes time:
not stopping, not the doors, and not a request for the floor the elevator
is already at, which is handled in `0` seconds. Once the last request is
handled, the elevator stays where it is.

Return how many seconds pass from the start until the last request has been
handled.

@examples

## Constraints

- `1 <= n <= 100`
- `1 <= requests.length <= 100`
- `0 <= requests[i] <= n - 1`
