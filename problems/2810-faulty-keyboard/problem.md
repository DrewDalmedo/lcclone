id: 2810
slug: faulty-keyboard
title: Faulty Keyboard
difficulty: easy
tags: string, simulation
signature: string finalString(string s)
---
Someone has rewired the `i` key of a keyboard. It no longer types a
letter: pressing it turns the whole line typed so far back to front. Every
other key still works and adds its letter to the end of the line.

The line starts out empty, and the keys of `s` are pressed one after
another, from left to right. Return the line as it reads after the last
key.

So `i` never appears in the result. Also, a press of `i` only turns around
the letters typed before it; letters typed later are added to the end of
the turned line in the usual way.

@examples

## Constraints

- `1 <= s.length <= 100`
- `s` consists of lowercase English letters.
- The first letter of `s` is not `i`.
