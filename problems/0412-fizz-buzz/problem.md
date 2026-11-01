id: 412
slug: fizz-buzz
title: Fizz Buzz
difficulty: easy
tags: math, string, simulation
signature: string[] fizzBuzz(int n)
---
Play one round of the counting game Fizz Buzz, counting from `1` up to `n`
inclusive. Each number `i` is called out as a string, chosen by the first
rule below that applies to it:

1. if `i` is a multiple of both 3 and 5 (that is, of 15), call `"FizzBuzz"`;
2. if `i` is a multiple of 3, call `"Fizz"`;
3. if `i` is a multiple of 5, call `"Buzz"`;
4. otherwise, call the number itself, written in decimal, such as `"8"` or
   `"1234"`.

Return the `n` calls in the order they are made: the string for `i` goes at
index `i - 1`, so the array starts with the call for `1` and ends with the
call for `n`.

The words are spelled exactly as shown: a capital `F` and `B`, the rest
lowercase, and no spaces.

@examples

## Constraints

- `1 <= n <= 10^4`
