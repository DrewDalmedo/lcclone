id: 1370
slug: increasing-decreasing-string
title: Increasing Decreasing String
difficulty: easy
tags: hash-table, string, counting
signature: string sortString(string s)
---
Write the letters of `s` out again in a new order, using every letter of
`s` exactly once (copies of the same letter count separately). The new
order comes from walking along the alphabet, back and forth:

- an **upward pass** visits the letters `a`, `b`, `c`, ..., `z`, in that
  order;
- a **downward pass** visits `z`, `y`, `x`, ..., `a`, in that order.

Make an upward pass, then a downward pass, then an upward pass again, and
so on. Each time a pass visits a letter of which `s` still has an unused
copy, use one copy (just one) and write it at the end of the answer. Stop
as soon as every letter of `s` has been used, and return what you wrote.

Every pass visits all 26 letters, including the one where the previous
pass made its last pick. So if the last letter an upward pass takes still
has an unused copy, the next downward pass takes that copy first, and the
answer has the letter twice in a row. The same happens at the bottom, where
a downward pass hands over to an upward one.

@examples

## Constraints

- `1 <= s.length <= 500`
- `s` consists of lowercase English letters only.
