id: 17
slug: bad-count
title: Bad Count
difficulty: easy
signature: int dedup(int[] nums)
check: prefix nums
---
Keep each value of the sorted `nums` once, at the front. This reference
solution returns a count larger than the array when `nums` is empty.

@examples
