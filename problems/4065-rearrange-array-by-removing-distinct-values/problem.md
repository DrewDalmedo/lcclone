id: 4065
slug: rearrange-array-by-removing-distinct-values
title: Rearrange Array by Removing Distinct Values
difficulty: easy
tags: array, hash-table, counting, ordered-set, sorting, simulation, heap-priority-queue
signature: int[] rearrangeArray(int[] nums)
---
`nums` holds positive integers, and a value may occur in it more than
once. Take `nums` apart in **rounds**, collecting what you take into an
array `ans` that is empty at the start. In each round, every value still
present in `nums` gives up exactly one of its copies, however many it has;
the values given up in that round go onto the end of `ans` in increasing
order. Keep going until `nums` has nothing left, and return `ans`.

So a value with `c` copies is given up once in each of the first `c`
rounds, and `ans` holds exactly the values of the original `nums`, each as
often as before, only in another order.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 100`
