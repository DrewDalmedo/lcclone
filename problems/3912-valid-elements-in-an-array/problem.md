id: 3912
slug: valid-elements-in-an-array
title: Valid Elements in an Array
difficulty: easy
tags: array
signature: int[] findValidElements(int[] nums)
---
Picture `nums` as a row of `n` posts standing from left to right, where
post `i` is `nums[i]` high. Someone who looks along the row from its left
end can see post `i` when it is **strictly taller** than every post before
it, `nums[0]` to `nums[i - 1]`. Someone who looks from the right end can see
post `i` when it is strictly taller than every post after it, `nums[i + 1]`
to `nums[n - 1]`. So from either end, a post is hidden by any post between
it and that end that is as tall as it or taller: an equal height is enough
to hide it. Nothing stands between the leftmost post and the left end, or
between the rightmost post and the right end, so those two are always seen.

Return the heights of the posts that can be seen from **at least one** of
the two ends, in their order along the row. A post seen from both ends is
listed once, so when `n == 1` the answer has a single entry. Two posts can
have the same height, and each one that is seen is listed, so a height may
appear more than once.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 100`
