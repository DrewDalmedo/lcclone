id: 2057
slug: smallest-index-with-equal-value
title: Smallest Index With Equal Value
difficulty: easy
tags: array
signature: int smallestEqual(int[] nums)
---
Walk along `nums` from the front with a counter that shows a single decimal
digit. At index `0` it shows `0`, and it goes up by one at each step; after
`9` it wraps round to `0`, so it shows `0` again at index `10`, `1` at index
`11`, and so on. In other words, at index `i` it shows `i % 10`, the last
digit of `i`.

Return the first index at which `nums` holds the digit the counter shows
there, or `-1` if the two never agree.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `0 <= nums[i] <= 9`
