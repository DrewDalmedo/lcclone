id: 3917
slug: count-indices-with-opposite-parity
title: Count Indices With Opposite Parity
difficulty: easy
tags: array
signature: int[] countOppositeParity(int[] nums)
---
Say that two whole numbers are **mismatched** when one of them is even and
the other is odd: 4 and 7 are mismatched, and so are 9 and 2, but 3 and 5
are not, and neither are 6 and 8. What counts is the values themselves; it
makes no difference whether the indices they sit at are even or odd.

`nums` holds `n` positive integers. For each index `i`, look only at the
entries to its **right**, `nums[i + 1]` to `nums[n - 1]`, and count how many
of them are mismatched with `nums[i]`. Entries to the left of `i`, and
`nums[i]` itself, never take part.

Return these counts as an array `answer` of length `n`, with the count for
index `i` in `answer[i]`. Nothing lies to the right of the last index, so
`answer[n - 1]` is always `0`.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 100`
