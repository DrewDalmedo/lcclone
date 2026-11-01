id: 3411
slug: maximum-subarray-with-equal-products
title: Maximum Subarray With Equal Products
difficulty: easy
tags: array, math, enumeration, number-theory, sliding-window
signature: int maxLength(int[] nums)
---
Take a non-empty list of positive integers. On one side, multiply all of
its elements together. On the other, multiply its **gcd**, the largest
integer that divides every element, by its **lcm**, the smallest positive
integer that every element divides. Call the list **fair** when both sides
come out equal. For a list of one element `x`, the gcd and the lcm are
both `x`.

Return the length of the longest fair **subarray** of `nums`: a run of one
or more elements that sit next to each other in `nums`.

@examples

## Constraints

- `2 <= nums.length <= 100`
- `1 <= nums[i] <= 10`
