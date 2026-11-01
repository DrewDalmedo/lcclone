id: 2670
slug: find-the-distinct-difference-array
title: Find the Distinct Difference Array
difficulty: easy
tags: array, hash-table
signature: int[] distinctDifferenceArray(int[] nums)
---
Let `n` be the length of `nums`. For every index `i`, cut `nums` in two
just after position `i`:

- the **left part** is `nums[0], ..., nums[i]`, so it always holds
  `nums[i]` itself;
- the **right part** is `nums[i + 1], ..., nums[n - 1]`, which is empty when
  `i = n - 1`.

Count how many different values each part contains; an empty part contains
`0`. A value that appears several times in a part is counted once there.

Return an array `answer` of length `n` in which `answer[i]` is the count for
the left part minus the count for the right part, both taken at the cut
after `i`. Entries may be zero or negative.

@examples

## Constraints

- `1 <= nums.length <= 50`
- `1 <= nums[i] <= 50`
