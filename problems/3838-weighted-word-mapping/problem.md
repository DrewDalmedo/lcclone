id: 3838
slug: weighted-word-mapping
title: Weighted Word Mapping
difficulty: easy
tags: array, string, simulation
signature: string mapWordWeights(string[] words, int[] weights)
---
`weights` holds a weight for each lowercase letter: `weights[k]` is the
weight of the letter `k` places after `a` in the alphabet, so `weights[0]`
is the weight of `a` and `weights[25]` that of `z`. To weigh a word, go
through it letter by letter and add up what each letter weighs. Repeats
are not merged: `"bob"` weighs the weight of `b`, plus that of `o`, plus
that of `b` once more.

Every word of `words` gets a one-letter **code**. Divide the word's weight
by `26` and keep the remainder `r`, a number from `0` to `25`. Now write
the alphabet backwards, `zyxwvutsrqponmlkjihgfedcba`: the code is the
letter at position `r` of that line, counting from `0`. So `z` is the code
for a remainder of `0`, `y` for `1`, and so on down to `a` for `25`.

Return the codes of the words, in the order of `words`, joined into one
string with nothing between them. A word that occurs more than once in
`words` contributes its code each time.

@examples

## Constraints

- `1 <= words.length <= 100`
- `1 <= words[i].length <= 10`
- `weights.length == 26`
- `1 <= weights[i] <= 100`
- `words[i]` consists of lowercase English letters.
