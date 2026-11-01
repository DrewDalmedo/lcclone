id: 1984
slug: minimum-difference-between-highest-and-lowest-of-k-scores
title: Minimum Difference Between Highest and Lowest of K Scores
difficulty: easy
tags: array, sorting, sliding-window
signature: int minimumDifference(int[] nums, int k)
---
`nums[i]` is the score that contestant `i` earned in a qualifying round,
and exactly `k` contestants go through to the final. To make the final as
even as possible, the organisers look at the **gap** of a group of
finalists: the highest score in the group minus the lowest score in it.

Return the smallest gap that a group of exactly `k` contestants can have.

Any `k` contestants may be picked: they need not stand next to each other
in `nums`. Contestants with equal scores are still different people, so
several of them can be picked together. A group of a single contestant has
a gap of `0`.

@examples

## Constraints

- `1 <= k <= nums.length <= 1000`
- `0 <= nums[i] <= 10^5`
