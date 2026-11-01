id: 976
slug: largest-perimeter-triangle
title: Largest Perimeter Triangle
difficulty: easy
tags: greedy, array, math, sorting
signature: int largestPerimeter(int[] nums)
---
Each entry of `nums` is the length of a stick. Pick three sticks, at three
different positions of `nums` (their lengths may be equal, but no position
is used twice), and try to join them end to end into a triangle. This works when the
longest of the three is shorter than the other two put together. When it is
exactly as long as the other two together, the three sticks lie flat along
a line, and that does not count as a triangle.

Return the largest perimeter, that is the total length of the three
sticks, of a triangle you can make this way. If no three sticks make a
triangle, return `0`.

@examples

## Constraints

- `3 <= nums.length <= 10^4`
- `1 <= nums[i] <= 10^6`
