id: 3769
slug: sort-integers-by-binary-reflection
title: Sort Integers by Binary Reflection
difficulty: easy
tags: array, sorting
signature: int[] sortByReflection(int[] nums)
---
To find the **binary reflection** of a positive integer `x`, spell `x` in
binary with no leading zeros, turn that string of bits around so that it
runs from the last bit to the first, and take the value of the new string
read in base 2.

Take `52`, whose bits are `110100`. Turned around they are `001011`; the
two zeros now in front change nothing, so the reflection of `52` is `11`
(`1011`). Doubling a number only appends a `0` to its bits, so `26` (bits
`11010`) reflects to `11` as well, and the reflection of any power of two
is `1`.

Return the elements of `nums` ordered by reflection, smallest reflection
first. Elements whose reflections are equal go from the smaller value to
the larger. The result has the same length as `nums` and holds the same
values; repeated values stay repeated.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 10^9`
