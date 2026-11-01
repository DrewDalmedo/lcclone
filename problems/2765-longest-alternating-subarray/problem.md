id: 2765
slug: longest-alternating-subarray
title: Longest Alternating Subarray
difficulty: easy
tags: array, enumeration
signature: int alternatingSubarray(int[] nums)
---
Picture each element of `nums` as the height of a post in a fence. A
**seesaw** is a stretch of at least two neighbouring posts whose heights
rock between just two values, a low one `v` and a high one `v + 1`,
beginning on the low one: the heights read `v`, `v + 1`, `v`, `v + 1`,
and so on.

Said with steps: going from each post of the stretch to the next, the
height first rises by exactly `1`, then falls by exactly `1`, then rises by
`1` again, and so on, rising and falling in turn. A seesaw may end on
either of its two values.

Return the number of posts in the longest seesaw in `nums`. If there is
none, which is the case exactly when no element is followed directly by a
value `1` larger than itself, return `-1`.

@examples

## Constraints

- `2 <= nums.length <= 100`
- `1 <= nums[i] <= 10^4`
