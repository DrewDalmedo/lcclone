id: 1046
slug: last-stone-weight
title: Last Stone Weight
difficulty: easy
tags: array, heap-priority-queue
signature: int lastStoneWeight(int[] stones)
---
A pile of stones is given by their weights, `stones`. As long as the pile
holds at least two stones, take its two heaviest and grind them against
each other:

- if their weights are `a >= b`, the lighter one is worn away completely
  and the heavier one keeps `a - b` of its weight;
- when `a == b`, that leaves nothing, and both stones are gone.

A stone that comes out of a grinding with weight `a - b` goes back into
the pile as just another stone: it may be the heaviest one left, the
lightest, or anywhere in between, and it is picked whenever it is among
the two heaviest. When several stones share the heaviest weight, it
doesn't matter which of them are picked.

When the pile is down to one stone or none, stop. Return the weight of
the stone still there, or `0` if every stone has been worn away.

@examples

## Constraints

- `1 <= stones.length <= 30`
- `1 <= stones[i] <= 1000`
