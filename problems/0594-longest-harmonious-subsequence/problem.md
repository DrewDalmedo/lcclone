id: 594
slug: longest-harmonious-subsequence
title: Longest Harmonious Subsequence
difficulty: easy
tags: array, hash-table, counting, sorting, sliding-window
signature: int findLHS(int[] nums)
---
Call a list of numbers **harmonious** when its largest value is exactly
`1` more than its smallest one. So a list whose values are all the same
is not harmonious, and neither is one whose largest and smallest values
are `2` or more apart.

Pick some elements of `nums`, at least one, from anywhere in the array,
and keep them in the order in which they appear there: what you get is a
**subsequence** of `nums`. Return the largest number of elements in a
harmonious subsequence of `nums`, or `0` if no subsequence is
harmonious.

@examples

## Constraints

- `1 <= nums.length <= 2 * 10^4`
- `-10^9 <= nums[i] <= 10^9`

## Follow-up

Counting, for each element, how many elements equal it or are one more
than it takes `O(n^2)` time. Can you do better?
