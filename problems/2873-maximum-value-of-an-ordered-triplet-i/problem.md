id: 2873
slug: maximum-value-of-an-ordered-triplet-i
title: Maximum Value of an Ordered Triplet I
difficulty: easy
tags: array
signature: long maximumTripletValue(int[] nums)
---
Pick three indices `i < j < k` of `nums`. The **value** of that triple is
`(nums[i] - nums[j]) * nums[k]`.

Return the largest value that any triple reaches, but never less than
`0`: when even the best triple comes out negative, the answer is `0`.

@examples

## Constraints

- `3 <= nums.length <= 100`
- `1 <= nums[i] <= 10^6`

## Follow-up

Trying every triple takes `O(n^3)` time. For a fixed `k`, what do you need
to know about the elements before it? Can you do it in one pass?
