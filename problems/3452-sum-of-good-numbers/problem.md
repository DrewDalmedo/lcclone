id: 3452
slug: sum-of-good-numbers
title: Sum of Good Numbers
difficulty: easy
tags: array
signature: int sumOfGoodNumbers(int[] nums, int k)
---
The element at index `i` of `nums` has up to two **rivals**: the element
exactly `k` places to its left, `nums[i - k]`, and the element exactly `k`
places to its right, `nums[i + k]`. A rival is there only when its index
lies inside the array, and the elements in between are not rivals at all.
Since `k` is at most half the length of `nums`, every element has at least
one rival.

An element is **good** when it is strictly greater than every rival it
has. A tie with a rival is not enough, and an element with a rival on one
side only has to beat just that one.

Return the sum of the values of the good elements, or `0` if none is good.
Equal values at different indices are different elements, so each of them
that is good adds its value.

@examples

## Constraints

- `2 <= nums.length <= 100`
- `1 <= nums[i] <= 1000`
- `1 <= k <= floor(nums.length / 2)`
