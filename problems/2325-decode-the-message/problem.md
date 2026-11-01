id: 2325
slug: decode-the-message
title: Decode the Message
difficulty: easy
tags: hash-table, string
signature: string decodeMessage(string key, string message)
---
A spy hides a cipher alphabet in the string `key`. To recover it, read
`key` from left to right, pass over its spaces, and write down each letter
the first time you meet it; a letter you have already written down is
passed over too. `key` holds every letter from `a` to `z`, so you end up
with a list of all 26 letters in some order.

In that list, the letter at position `k` (counting from 1) **stands for**
the `k`-th letter of the ordinary alphabet: the first letter you wrote down
stands for `a`, the second for `b`, and so on up to the last, which stands
for `z`.

Decode `message` by replacing each of its letters with the letter it stands
for. Spaces are not encoded: each one stays a space, in the same place,
including spaces at either end and several in a row. Return the decoded
string, which has the same length as `message`.

@examples

## Constraints

- `26 <= key.length <= 2000`
- `key` consists of lowercase English letters and spaces, and contains each
  letter from `a` to `z` at least once.
- `1 <= message.length <= 2000`
- `message` consists of lowercase English letters and spaces.
