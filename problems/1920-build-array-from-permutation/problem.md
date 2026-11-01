id: 1920
slug: build-array-from-permutation
title: Build Array from Permutation
difficulty: easy
tags: array, simulation
signature: int[] buildArray(int[] nums)
---
The `n` entries of `nums` are the numbers `0` to `n - 1`, each exactly
once, in some order. Read every entry as an arrow that points at a
position of `nums` itself: position `i` (counting from `0`) points at
position `nums[i]`.

Start at position `i`, follow its arrow, and then follow the arrow of the
position you arrive at. Return an array `ans` of length `n` in which
`ans[i]` is the position where these two steps from `i` end, for every
`i`. In index terms, `ans[i] = nums[nums[i]]`.

@examples

## Constraints

- `1 <= nums.length <= 1000`
- `0 <= nums[i] < nums.length`
- The entries of `nums` are all different.

## Follow-up

Can you build the answer inside `nums` itself, with only `O(1)` extra
memory, and return `nums`? Every entry is below `1000`, far less than an
`int` can hold.
