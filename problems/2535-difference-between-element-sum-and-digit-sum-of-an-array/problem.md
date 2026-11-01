id: 2535
slug: difference-between-element-sum-and-digit-sum-of-an-array
title: Difference Between Element Sum and Digit Sum of an Array
difficulty: easy
tags: array, math
signature: int differenceOfSum(int[] nums)
---
An adding machine should add up the numbers in `nums`, but its keypad is
faulty: it adds every key press as a number of its own. Typing `305` adds
`3`, then `0`, then `5`, so the machine counts `8` where three hundred and
five was meant.

Each number of `nums` is typed once, in decimal, one key per digit. Return
how much the machine's total falls short of the true total of `nums`, that
is, the true total minus the machine's total. A number of one digit is
added correctly, and no number ever counts for more than it is worth, so the
answer is never negative.

@examples

## Constraints

- `1 <= nums.length <= 2000`
- `1 <= nums[i] <= 2000`
