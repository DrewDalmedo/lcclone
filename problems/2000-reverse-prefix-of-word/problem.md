id: 2000
slug: reverse-prefix-of-word
title: Reverse Prefix of Word
difficulty: easy
tags: stack, two-pointers, string
signature: string reversePrefix(string word, char ch)
---
Scan `word` from the left for the letter `ch` and stop at the first place
it turns up. The **head** of `word` is the stretch from its first letter up
to and including that `ch`; the letters after it form the **tail**.

Return the string you get by spelling the head backwards and leaving the
tail untouched after it. The result then starts with `ch`. Copies of `ch`
further along, in the tail, change nothing.

If `ch` does not occur in `word` at all, there is no head to turn around:
return `word` exactly as it is.

@examples

## Constraints

- `1 <= word.length <= 250`
- `word` consists of lowercase English letters.
- `ch` is a lowercase English letter.
