id: 2899
slug: last-visited-integers
title: Last Visited Integers
difficulty: easy
tags: array, simulation
signature: int[] lastVisitedIntegers(int[] nums)
---
Each entry of `nums` is either a positive number or `-1`. Go through
`nums` from left to right, keeping a log of numbers that starts out empty:

- A positive entry is added to the end of the log. Nothing is ever taken
  out of the log, and a value that occurs several times is logged once for
  every occurrence.
- A `-1` entry is a lookup. Lookups that come right after one another, with
  no positive entry between them, form a **streak**. The `t`-th lookup of a
  streak (counting from `1`) answers with the `t`-th newest number of the
  log as it is at that moment: the first lookup of a streak gets the newest
  number in the log, the second one the number logged before that, and so
  on. If the log holds fewer than `t` numbers, the lookup answers `-1`.

Only a positive entry ends a streak; a lookup that answered `-1` does not.
The next lookup after a positive entry starts a new streak at `t = 1`.

Return the answers of all the lookups, in the order they were made. If
`nums` contains no `-1`, return an empty array.

@examples

## Constraints

- `1 <= nums.length <= 100`
- Each `nums[i]` is `-1` or a number from `1` to `100`.
