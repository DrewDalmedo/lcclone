id: 2367
slug: number-of-arithmetic-triplets
title: Number of Arithmetic Triplets
difficulty: easy
tags: array, hash-table, two-pointers, enumeration
signature: int arithmeticTriplets(int[] nums, int diff)
---
The values of `nums` are sorted in strictly increasing order, and `diff` is
a positive integer. Three positions `i < j < k` of `nums` make a
**ladder** when each value climbs exactly `diff` above the one before it:

- `nums[j] = nums[i] + diff`, and
- `nums[k] = nums[j] + diff`.

The positions don't have to be next to each other, and a climb of any other
size, `2 * diff` included, doesn't count.

Return how many ladders `nums` holds. Ladders may share positions, and each
one counts on its own: with `diff = 5`, the values `10, 15, 20, 25` hold two
ladders, `10, 15, 20` and `15, 20, 25`.

@examples

## Constraints

- `3 <= nums.length <= 200`
- `0 <= nums[i] <= 200`
- `1 <= diff <= 50`
- `nums` is strictly increasing.
