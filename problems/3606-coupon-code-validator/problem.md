id: 3606
slug: coupon-code-validator
title: Coupon Code Validator
difficulty: easy
tags: array, hash-table, string, sorting
signature: string[] validateCoupons(string[] code, string[] businessLine, bool[] isActive)
---
A store keeps its `n` discount coupons in three arrays of the same length,
read side by side: coupon `i` has the code `code[i]`, belongs to the line
of business `businessLine[i]`, and is switched on exactly when
`isActive[i]` is `true`.

A coupon can be **redeemed** when it passes all three of these checks, and
not otherwise:

- `code[i]` has at least one character, and every character in it is an
  English letter (`a`-`z`, `A`-`Z`), a digit (`0`-`9`) or `_`. Any other
  character anywhere in the code, such as a space or a dash (`-`), rules it
  out.
- `businessLine[i]` is `"electronics"`, `"grocery"`, `"pharmacy"` or
  `"restaurant"`, character for character. A capital letter, a missing or
  extra character, or a space before or after rules it out.
- `isActive[i]` is `true`.

Return the codes of the coupons that can be redeemed, each exactly as it
was given. The `"electronics"` coupons come first, then the `"grocery"`
ones, then `"pharmacy"`, and `"restaurant"` last. Within one line of
business, the codes are in increasing order, comparing them character by
character by ASCII code: digits come before capital letters, capitals
before `_`, and `_` before small letters, and a code that is a prefix of a
longer one comes before it.

Each coupon that can be redeemed gives its own entry, even when another
one has the same code. If no coupon can be redeemed, return an empty array.

@examples

## Constraints

- `n == code.length == businessLine.length == isActive.length`
- `1 <= n <= 100`
- `0 <= code[i].length, businessLine[i].length <= 100`
- `code[i]` and `businessLine[i]` consist of printable ASCII characters
  (codes 32 to 126, the space included).
