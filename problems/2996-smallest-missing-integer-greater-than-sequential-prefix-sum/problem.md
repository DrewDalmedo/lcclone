id: 2996
slug: smallest-missing-integer-greater-than-sequential-prefix-sum
title: Smallest Missing Integer Greater Than Sequential Prefix Sum
difficulty: easy
tags: array, hash-table, sorting
signature: int missingInteger(int[] nums)
---
Read `nums` from the left, starting at `nums[0]`, and carry on as long as
each element is exactly `1` more than the element before it. Stop at the
first element that isn't: it and everything after it are left out, even if
some later elements count up by one again. The elements read, from
`nums[0]` to just before where you stopped, form the **sequential prefix**
of `nums`. It always holds at least `nums[0]`.

Let `s` be the sum of the sequential prefix. Return the smallest integer
`x` with `x >= s` that does not appear anywhere in `nums`, the prefix
included.

Despite the title, `x` may be equal to `s`: if `s` itself is missing from
`nums`, the answer is `s`.

@examples

## Constraints

- `1 <= nums.length <= 50`
- `1 <= nums[i] <= 50`
