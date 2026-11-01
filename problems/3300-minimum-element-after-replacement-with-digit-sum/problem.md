id: 3300
slug: minimum-element-after-replacement-with-digit-sum
title: Minimum Element After Replacement With Digit Sum
difficulty: easy
tags: array, math
signature: int minElement(int[] nums)
---
The **digit sum** of a positive integer is the total of its decimal
digits: `4071` has digit sum `4 + 0 + 7 + 1 = 12`.

Work out the digit sum of every number in `nums`, and return the smallest
of those digit sums. The answer is the digit sum itself, not the number it
belongs to. Note that the number with the smallest digit sum need not be
the smallest number in `nums`: `300`, with digit sum `3`, beats `8`.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 10^4`
