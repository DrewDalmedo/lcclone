id: 3318
slug: find-x-sum-of-all-k-long-subarrays-i
title: Find X-Sum of All K-Long Subarrays I
difficulty: easy
tags: array, hash-table, sliding-window, heap-priority-queue
signature: int[] findXSum(int[] nums, int k, int x)
---
In a list of numbers, the **popularity** of a value is how many times it
appears in the list. To find the **x-sum** of a list:

1. Rank its distinct values: a more popular value ranks above a less
   popular one, and of two equally popular values the larger ranks above
   the smaller.
2. Keep the `x` best-ranked values, or every value if the list has fewer
   than `x` distinct ones.
3. Add up every appearance of a kept value in the list. Appearances of the
   other values add nothing.

So `x` counts distinct values, not elements: a kept value that appears
three times contributes three times its value.

`nums` has `n` elements. Slide a window of `k` consecutive elements along
it, from `nums[0..k-1]` to `nums[n-k..n-1]`. Return an array of `n - k + 1`
numbers: entry `i` is the x-sum of the window that starts at index `i`.

@examples

## Constraints

- `1 <= n == nums.length <= 50`
- `1 <= nums[i] <= 50`
- `1 <= x <= k <= nums.length`
