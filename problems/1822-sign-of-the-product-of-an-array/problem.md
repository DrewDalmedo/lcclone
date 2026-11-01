id: 1822
slug: sign-of-the-product-of-an-array
title: Sign of the Product of an Array
difficulty: easy
tags: array, math
signature: int arraySign(int[] nums)
---
Picture multiplying all the elements of `nums` together, exactly, with no
limit on how many digits the result may have. Return the sign of that
product:

- `1` if the product is greater than `0`,
- `-1` if it is less than `0`,
- `0` if it equals `0`.

No other value is a correct answer, so a product of `-210` gives `-1`.

The exact product can be enormous: a thousand factors of size `100` make a
number with 2001 digits, far beyond the range of `int`, `long long` and even
`double`. The answer is the sign of that exact product, not of a value that
overflowed along the way.

@examples

## Constraints

- `1 <= nums.length <= 1000`
- `-100 <= nums[i] <= 100`
