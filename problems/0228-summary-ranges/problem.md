id: 228
slug: summary-ranges
title: Summary Ranges
difficulty: easy
tags: array
signature: string[] summaryRanges(int[] nums)
---
The integers in `nums` are all different and listed in increasing order.
Cut the list into **runs**: groups of neighbouring elements in which each
element is exactly one more than the element before it. Make every run as
long as it can be, so that the last element of one run and the first
element of the next never differ by exactly one.

Describe each run with a string:

- a run of a single number `a` is written as `a` alone, so the run `7` is
  `"7"`;
- a run from `a` up to `b`, with `b > a`, is written as `a`, then the two
  characters `->`, then `b`, so the run `-6, -5, -4` is `"-6->-4"`.

Numbers are written in ordinary decimal, with a `-` before a negative
number, and no `+` sign, leading zeros or spaces.

Return the strings of all the runs, from the first run to the last. An empty
`nums` has no runs, so the answer is then an empty array.

@examples

## Constraints

- `0 <= nums.length <= 20`
- `-2^31 <= nums[i] <= 2^31 - 1`
- The values of `nums` are distinct and sorted in increasing order.
