id: 27
slug: remove-element
title: Remove Element
difficulty: easy
tags: array, two-pointers
signature: int removeElement(int[] nums, int val)
check: prefix nums
compare: unordered 0
---
Take every copy of `val` out of `nums` **in place**, by moving the other
elements to the front of the array, and return how many elements differ
from `val`.

If that count is `k`, then `nums[0]` to `nums[k - 1]` must be exactly the
elements of `nums` that differ from `val`, each value as many times as it
occurs in `nums`, in *any order*. Whatever `nums` holds after them doesn't
matter, and you don't need to shrink it. The examples show those `k`
elements as the output, in the order they had in `nums`, though any order
of them is accepted.

@examples

## Constraints

- `0 <= nums.length <= 100`
- `0 <= nums[i] <= 50`
- `0 <= val <= 100`

## Follow-up

Can you do it in a single pass over `nums`, with `O(1)` extra memory?
