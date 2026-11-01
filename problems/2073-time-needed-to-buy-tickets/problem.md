id: 2073
slug: time-needed-to-buy-tickets
title: Time Needed to Buy Tickets
difficulty: easy
tags: queue, array, simulation
signature: int timeRequiredToBuy(int[] tickets, int k)
---
A box office with a single window has a strict rule: one ticket per visit.
`n` fans wait at the window in a single file, numbered `0` to `n - 1` from
the front of the file to the back, and fan `i` wants `tickets[i]` tickets
in all.

The window works one second at a time. In each second, the fan at the front
of the file buys exactly one ticket. Right after that sale, a fan who still
wants more walks round to the end of the file, behind everyone who is
waiting at that moment, while a fan who now has all their tickets goes home
for good. Walking round and going home take no time, and no one else is
served during a sale.

Return how many seconds have gone by when fan `k` buys their last ticket,
counting the second of that last sale.

@examples

## Constraints

- `n == tickets.length`
- `1 <= n <= 100`
- `1 <= tickets[i] <= 100`
- `0 <= k < n`
