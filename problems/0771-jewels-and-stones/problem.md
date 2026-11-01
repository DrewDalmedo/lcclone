id: 771
slug: jewels-and-stones
title: Jewels and Stones
difficulty: easy
tags: hash-table, string
signature: int numJewelsInStones(string jewels, string stones)
---
A prospector tips a sack of rough stones out onto the table. The string
`stones` describes them one character per stone, and the character tells
what kind of stone it is. The characters of `jewels` name the kinds that
are worth keeping; no kind is named there twice.

Return the number of stones on the table whose kind is named in
`jewels`.

Each stone counts on its own, so four stones of a kind worth keeping add
4 to the answer. Kinds are told apart by the exact character, case
included: `r` and `R` are two different kinds, and naming one of them
says nothing about the other. A kind named in `jewels` that the sack
doesn't hold just adds nothing.

@examples

## Constraints

- `1 <= jewels.length, stones.length <= 50`
- `jewels` and `stones` consist of English letters only (`a`-`z` and
  `A`-`Z`).
- The characters of `jewels` are all different.
