id: 1313
slug: decompress-run-length-encoded-list
title: Decompress Run-Length Encoded List
difficulty: easy
tags: array
signature: int[] decompressRLElist(int[] nums)
---
`nums` holds a list in packed form. Read it two numbers at a time:
`nums[0]` and `nums[1]` are the first pair, `nums[2]` and `nums[3]` the
second, and so on. In each pair the first number is a **count** and the
second a **value**, and the pair stands for the value written out count
times.

Unpack the list: write out the copies of the first pair, then those of the
second pair right after them, and so on up to the last pair. Return the
result.

A pair always starts at an even index, so `nums[1]` and `nums[2]` never
form a pair. Different pairs may carry the same value; their copies are
written out like any others.

@examples

## Constraints

- `2 <= nums.length <= 100`
- `nums.length` is even.
- `1 <= nums[i] <= 100`
