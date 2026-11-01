id: 819
slug: most-common-word
title: Most Common Word
difficulty: easy
tags: array, hash-table, string, counting
signature: string mostCommonWord(string paragraph, string[] banned)
---
An editor wants to know which word a draft leans on most. The draft is
the string `paragraph`. Little words such as "the" would win every time,
so `banned` lists the words to leave out of the count.

Only letters make up words. Every character of `paragraph` that isn't a
letter (a space, or one of the marks `! ? ' , ; .`) stands between words,
and several of them in a row separate two words just as one does. So
`"well,well"` holds two words, and `"it's"` holds the two words `it` and
`s`. Case is ignored: `Rain`, `RAIN` and `rain` are all the word `rain`.

Tally the words, leave out every word that `banned` lists, and return the
word with the biggest tally among those left, written in lower case. An
entry of `banned` rules out exactly the word it spells: `seen` is still
counted when only `see` is banned, and so is any other word that merely
contains a banned entry.

You can rely on two things: `paragraph` holds some word that `banned`
doesn't list, and no two of the words left in the tally share the top
count, so the answer is never in doubt.

@examples

## Constraints

- `1 <= paragraph.length <= 1000`
- `paragraph` consists of English letters, spaces `' '` and the marks
  `!?',;.`
- `0 <= banned.length <= 100`
- `1 <= banned[i].length <= 10`
- `banned[i]` consists of lowercase English letters.
