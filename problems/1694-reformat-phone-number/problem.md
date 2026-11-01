id: 1694
slug: reformat-phone-number
title: Reformat Phone Number
difficulty: easy
tags: string
signature: string reformatNumber(string number)
---
Someone typed a phone number into `number` carelessly: besides the digits
it may hold spaces and dashes (`-`), anywhere and in any amount. Only the
digits count, in the order they appear. The spaces and dashes are thrown
away, together with whatever grouping they seemed to suggest.

Write the digits out again, still in their order, in groups such that:

- every group holds either 3 digits or 2 digits;
- all the groups of 3 come before all the groups of 2;
- there are as few groups of 2 as possible.

Put a single dash between neighbouring groups, with nothing before the
first group or after the last, and return the result.

So when the number of digits is a multiple of 3, every group has 3 digits.
When it leaves a remainder of 2 on division by 3, only the last group has
2. When it leaves a remainder of 1, the last two groups have 2 digits each
(4 digits make two groups of 2). A group of one digit never occurs. A `0`
is a digit like any other, and is kept even at the front.

@examples

## Constraints

- `2 <= number.length <= 100`
- `number` consists of digits, spaces (`' '`) and dashes (`'-'`).
- `number` holds at least two digits.
