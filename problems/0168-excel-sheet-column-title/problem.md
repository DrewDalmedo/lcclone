id: 168
slug: excel-sheet-column-title
title: Excel Sheet Column Title
difficulty: easy
tags: math, string
signature: string convertToTitle(int columnNumber)
---
Each column of a spreadsheet has a label made of capital letters. The
labels are handed out shortest first. The 26 one-letter labels go to
the first 26 columns, in the order `A` to `Z`. The `26 * 26` two-letter
labels go to the columns after those, sorted as in a dictionary, so the
first letter changes most slowly. Then come the `26^3` three-letter labels,
sorted the same way, and so on.

Columns are counted from 1. Return the label of column number
`columnNumber`.

In arithmetic terms, a label is the column number written in base 26 with
the digits 1 to 26 instead of 0 to 25: `A` stands for 1, `B` for 2, ...,
`Z` for 26, and no letter stands for zero. In a label of `k` letters, the
first letter is worth its value times `26^(k-1)`, the second its value
times `26^(k-2)`, and so on down to the last letter, which is worth just
its value. The column number is the total.

@examples

## Constraints

- `1 <= columnNumber <= 2^31 - 1`
