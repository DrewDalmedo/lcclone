id: 747
slug: largest-number-at-least-twice-of-others
title: Largest Number At Least Twice of Others
difficulty: easy
tags: array, sorting
signature: int dominantIndex(int[] nums)
---
Call `nums[i]` **dominant** when doubling any other element of `nums` never
gives more than `nums[i]`: that is, `nums[i] >= 2 * nums[j]` for every
index `j` other than `i`. Exactly double is still fine: `10` is dominant
in `[10, 5]`, but `9` is not dominant in `[9, 5]`.

`nums` has one largest element, larger than all the others, and no other
element can be dominant. Return the index of the largest element if it is
dominant, or `-1` if it isn't. The index is its position in `nums` as
given, counting from `0`.

@examples

## Constraints

- `2 <= nums.length <= 50`
- `0 <= nums[i] <= 100`
- The largest value in `nums` occurs exactly once.
