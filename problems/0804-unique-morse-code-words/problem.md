id: 804
slug: unique-morse-code-words
title: Unique Morse Code Words
difficulty: easy
tags: array, hash-table, string
signature: int uniqueMorseRepresentations(string[] words)
---
A telegraph operator sends every word of `words` in International Morse
Code. The chart below gives the code of each lowercase letter, written
with `'.'` for a dot and `'-'` for a dash:

```text
a  .-      h  ....    o  ---     v  ...-
b  -...    i  ..      p  .--.    w  .--
c  -.-.    j  .---    q  --.-    x  -..-
d  -..     k  -.-     r  .-.     y  -.--
e  .       l  .-..    s  ...     z  --..
f  ..-.    m  --      t  -
g  --.     n  -.      u  ..-
```

To send a word, the operator keys the codes of its letters one after
another, in order, with no pause between two letters. So what goes down
the wire, the word's **signal**, is the letters' codes joined end to end.
`tea`, for one, is sent as `-` then `.` then `.-`, which makes the signal
`-..-` (the code of `x` on its own).

Return how many different signals go down the wire once every word of
`words` has been sent. Two signals are the same only when they are the same
string of dots and dashes. Since nothing marks where one letter stops,
different words can make the same signal, as `tea` and `x` do, and such
words count as a single signal. A word listed more than once also counts
once.

@examples

## Constraints

- `1 <= words.length <= 100`
- `1 <= words[i].length <= 12`
- `words[i]` consists of lowercase English letters.
