id: 3477
slug: fruits-into-baskets-ii
title: Fruits Into Baskets II
difficulty: easy
tags: segment-tree, array, binary-search, ordered-set, simulation
signature: int numOfUnplacedFruits(int[] fruits, int[] baskets)
---
Fruit arrives in `n` batches, and a row of `n` empty baskets waits for
it. Batch `i` has `fruits[i]` pieces of fruit, and basket `j` can take up
to `baskets[j]` pieces.

Handle the batches in order: batch `0` first, then batch `1`, and so on.
Each batch goes, whole, into the leftmost basket (the one with the
smallest index) that is still empty and big enough for it, meaning its
capacity is at least the size of the batch. From then on that basket is
taken, however much room it has left. When no empty basket is big
enough, the batch is left out for good: it takes no basket and is not
tried again later.

Return the number of batches left out.

The rule fixes where every batch goes, so the answer can be more than the
fewest possible: a small batch may take a big basket that a later batch
needed.

@examples

## Constraints

- `n == fruits.length == baskets.length`
- `1 <= n <= 100`
- `1 <= fruits[i], baskets[j] <= 1000`
