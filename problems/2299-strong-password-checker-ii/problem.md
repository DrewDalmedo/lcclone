id: 2299
slug: strong-password-checker-ii
title: Strong Password Checker II
difficulty: easy
tags: string
signature: bool strongPasswordCheckerII(string password)
---
A sign-up form screens every new `password`. Each of its characters is of
one of four kinds:

- a small letter, `a` to `z`;
- a capital letter, `A` to `Z`;
- a digit, `0` to `9`;
- a symbol, which is one of these twelve characters and no other:
  `!` `@` `#` `$` `%` `^` `&` `*` `(` `)` `-` `+`

The form takes the password only if it passes every check:

1. it is 8 characters long or longer;
2. it holds at least one character of **each** of the four kinds;
3. no character is directly followed by the very same character.

For check 3, two characters are the same only when they are identical, so
`k` right after `K` is allowed, while `kk`, `33` or `--` side by side is
not. A character may come back later in the password, as long as its
copies never stand next to each other.

Return `true` if the form takes `password`, and `false` if any check fails.

@examples

## Constraints

- `1 <= password.length <= 100`
- Every character of `password` is a letter, a digit or one of the twelve
  symbols above.
