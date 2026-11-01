id: 3162
slug: find-the-number-of-good-pairs-i
title: Find the Number of Good Pairs I
difficulty: easy
tags: array, hash-table
signature: int numberOfPairs(int[] nums1, int[] nums2, int k)
---
Each number in `nums1` is the length of a plank, and each number in `nums2`
is a setting on a saw. The saw always cuts pieces `k` times as long as its
setting, so setting `nums2[j]` cuts pieces of length `nums2[j] * k`.

Plank `i` and setting `j` make a **good pair** when the plank can be cut
up entirely into pieces of that length, with nothing left over: that is,
when `nums1[i]` divided by `nums2[j] * k` leaves the remainder `0`. A plank
exactly one piece long qualifies, and a piece longer than the plank never
does.

Return the number of good pairs `(i, j)`, where `i` is any index of `nums1`
and `j` any index of `nums2`. Pairs are made of positions, not values, so
equal numbers at different positions of either array make separate pairs.

@examples

## Constraints

- `1 <= nums1.length, nums2.length <= 50`
- `1 <= nums1[i], nums2[j] <= 50`
- `1 <= k <= 50`
