id: 3866
slug: first-unique-even-element
title: First Unique Even Element
difficulty: easy
tags: array, hash-table, counting
signature: int firstUniqueEven(int[] nums)
---
Scan `nums` from left to right and stop at the first entry that passes
both of these tests:

- its value is even;
- its value occurs exactly once in `nums`, counting the whole array, so
  no other entry, earlier or later, holds the same value.

Return that entry's value (the value, not its index). If no entry passes
both tests, return `-1`.

Because the scan stops at the first entry that passes, a passing entry
further right never wins, even if its value is smaller. An odd value
never passes, not even one that occurs only once.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 100`
