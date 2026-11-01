id: 1431
slug: kids-with-the-greatest-number-of-candies
title: Kids With the Greatest Number of Candies
difficulty: easy
tags: array
signature: bool[] kidsWithCandies(int[] candies, int extraCandies)
---
Child `i` of a group of `n` children (numbered from `0`) holds
`candies[i]` candies, and you have a bag of `extraCandies` more.

Ask the same question about each child `i` in turn: if the whole bag went
to child `i`, and nobody else got anything, would child `i` then hold at
least as many candies as each of the other children? Drawing level with
the child who holds the most is enough for a yes.

Every question starts afresh: the bag is full again for the next child,
and the other children always keep exactly what they hold now.

Return `n` booleans, where the one at index `i` is `true` when the answer
for child `i` is yes, and `false` otherwise.

@examples

## Constraints

- `n == candies.length`
- `2 <= n <= 100`
- `1 <= candies[i] <= 100`
- `1 <= extraCandies <= 50`
