id: 3668
slug: restore-finishing-order
title: Restore Finishing Order
difficulty: easy
tags: array, hash-table
signature: int[] recoverOrder(int[] order, int[] friends)
---
`n` runners, wearing the bib numbers `1` to `n`, have finished a race.
`order` is the results sheet: `order[0]` is the bib number of the winner,
`order[1]` that of the runner who came second, and so on down to
`order[n - 1]`, the runner who came last. Every bib number is on the
sheet exactly once.

`friends` holds the bib numbers of the runners you know, sorted from the
smallest number to the largest. Rearrange these numbers by result, so
that the friend who finished earliest comes first, and return them. The
answer holds the bib numbers themselves (not places on the sheet), with
each friend's number exactly once.

@examples

## Constraints

- `n == order.length`
- `1 <= n <= 100`
- `order` holds each of the integers `1` to `n` exactly once.
- `1 <= friends.length <= min(8, n)`
- `1 <= friends[i] <= n`
- `friends` is strictly increasing.
