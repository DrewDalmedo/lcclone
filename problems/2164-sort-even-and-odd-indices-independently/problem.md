id: 2164
slug: sort-even-and-odd-indices-independently
title: Sort Even and Odd Indices Independently
difficulty: easy
tags: array, sorting
signature: int[] sortEvenOdd(int[] nums)
---
Paint the positions of `nums` alternately red and blue, starting with red:
index `0` is red, index `1` blue, index `2` red, and so on. The red
positions are the even indices and the blue positions the odd ones.

Rearrange the numbers among the red positions so that, read from left to
right, they never go down. Rearrange the numbers among the blue positions
so that, read from left to right, they never go up. A number on a red
position stays on a red position, and a number on a blue position stays
on a blue one.

Return the array after both rearrangements.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 100`
