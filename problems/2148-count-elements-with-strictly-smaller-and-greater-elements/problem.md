id: 2148
slug: count-elements-with-strictly-smaller-and-greater-elements
title: Count Elements With Strictly Smaller and Greater Elements
difficulty: easy
tags: array, counting, sorting
signature: int countElements(int[] nums)
---
Go through the indices of `nums` and ask two questions about each index
`i`: does `nums` hold a value strictly below `nums[i]`, and does it hold a
value strictly above `nums[i]`? Both searches cover the whole array: the
smaller value and the greater one can be at any positions, near `i` or far
from it, on the same side of it or on opposite sides.

Return the number of indices for which both answers are yes, or `0` if
there are none. Indices are counted, not distinct values: a value that
gets two yes answers and occurs three times adds `3` to the count.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `-10^5 <= nums[i] <= 10^5`
