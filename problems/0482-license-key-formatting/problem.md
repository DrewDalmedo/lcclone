id: 482
slug: license-key-formatting
title: License Key Formatting
difficulty: easy
tags: string
signature: string licenseKeyFormatting(string s, int k)
---
A license key `s` is a string of letters, digits and dashes (`-`). The
dashes can be anywhere: at either end, several in a row, or nowhere at all.
Rewrite the key in a tidy layout of blocks of `k` characters:

1. Drop every dash and turn each lowercase letter into its capital.
   Capitals and digits stay as they are, and the characters keep their
   order. Where the dashes used to be makes no difference from here on.
2. Cut those characters into blocks, working from the **right** end: each
   cut takes the next `k` characters, until fewer than `k` are left. The
   leftovers at the left end, if there are any, make one shorter block,
   which comes first. So only the first block can be short, and no block
   is ever empty.
3. Join the blocks in order with a single dash between each two, and no
   dash at the start or the end.

Return the result. If `s` holds no letters or digits at all, there are no
blocks, and the result is the empty string `""`.

@examples

## Constraints

- `1 <= s.length <= 10^5`
- `s` consists of English letters, digits and dashes (`-`).
- `1 <= k <= 10^4`
