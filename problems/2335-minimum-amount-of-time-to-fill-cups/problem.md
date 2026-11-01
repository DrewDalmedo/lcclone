id: 2335
slug: minimum-amount-of-time-to-fill-cups
title: Minimum Amount of Time to Fill Cups
difficulty: easy
tags: greedy, array, sorting, heap-priority-queue
signature: int fillCups(int[] amount)
---
A drinks machine has three nozzles, numbered `0`, `1` and `2`, each pouring
its own kind of drink. You need `amount[i]` cups from nozzle `i`.

The machine works in whole seconds. In any one second at most two of the
nozzles can run, and a running nozzle fills exactly one cup in that second.
So a second fills either one cup, or two cups from two different nozzles:
two cups from the same nozzle never fill in the same second.

Return the smallest number of seconds after which every cup you need has
been filled (`0` if you need none).

@examples

## Constraints

- `amount.length == 3`
- `0 <= amount[i] <= 100`
