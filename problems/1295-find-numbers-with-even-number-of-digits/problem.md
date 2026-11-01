id: 1295
slug: find-numbers-with-even-number-of-digits
title: Find Numbers with Even Number of Digits
difficulty: easy
tags: array, math
signature: int findNumbers(int[] nums)
---
The **length** of a positive integer is how many characters it takes to
write in decimal, with no leading zeros: `7` has length 1, `40` has length
2 and `100000` has length 6.

Return how many entries of `nums` have an even length. Each entry counts on
its own, so a value that appears three times counts three times.

@examples

## Constraints

- `1 <= nums.length <= 500`
- `1 <= nums[i] <= 10^5`
