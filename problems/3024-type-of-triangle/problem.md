id: 3024
slug: type-of-triangle
title: Type of Triangle
difficulty: easy
tags: array, math, sorting
signature: string triangleType(int[] nums)
---
The three positive integers in `nums`, in no particular order, are offered
as the side lengths of a triangle. Decide what kind of triangle they give.

A triangle with these sides exists only when each length is **strictly**
smaller than the sum of the other two, and it is enough to test this for
the largest length. When the two smaller lengths add up to the largest one, or to less,
there is no triangle and the answer is `"none"`. A sum equal to the largest
length is no exception: those sides would only make a degenerate triangle,
one with zero area. Do this test first: lengths that fail it give `"none"`
even if some of them are equal.

Otherwise, name the triangle by its equal sides:

- `"equilateral"`: the three sides are equal;
- `"isosceles"`: two sides are equal and the third is not;
- `"scalene"`: no two sides are equal.

Return the answer as one of these four lowercase words, spelled exactly as
shown.

@examples

## Constraints

- `nums.length == 3`
- `1 <= nums[i] <= 100`
