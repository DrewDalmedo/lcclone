id: 2558
slug: take-gifts-from-the-richest-pile
title: Take Gifts From the Richest Pile
difficulty: easy
tags: array, simulation, heap-priority-queue
signature: long pickGifts(int[] gifts, int k)
---
`gifts[i]` is the number of gifts in pile `i`. The following round is done
exactly `k` times:

- choose a pile that holds the most gifts right now (when several piles tie,
  any of them, since they hold the same number);
- if it holds `c` gifts, take gifts away until it holds `floor(sqrt(c))`:
  the largest whole number whose square is at most `c`. A pile of `10`
  keeps `3`; a pile of `1` keeps its `1`.

The same pile may be chosen in several rounds. Return the total number of
gifts left in all the piles after the `k` rounds.

@examples

## Constraints

- `1 <= gifts.length <= 10^3`
- `1 <= gifts[i] <= 10^9`
- `1 <= k <= 10^3`

## Follow-up

Scanning every pile for the largest one costs `O(n)` per round. Can you find
it in `O(log n)`?
