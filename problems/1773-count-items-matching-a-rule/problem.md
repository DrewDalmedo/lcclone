id: 1773
slug: count-items-matching-a-rule
title: Count Items Matching a Rule
difficulty: easy
tags: array, string
signature: int countMatches(string[][] items, string ruleKey, string ruleValue)
---
Picture `items` as a table with three columns. Each row describes one item
with three words, always in the same order: the item's **type** (column 0),
its **color** (column 1) and its **name** (column 2).

A query picks one column by its title and asks for one word. `ruleKey` is
the title, `"type"`, `"color"` or `"name"`, and `ruleValue` is the word. A
row passes the query when the word it has in that column is exactly
`ruleValue`: the same letters, the same length, so a longer or shorter word
that merely starts or ends the same way doesn't pass. The other two columns
play no part, even when one of them holds `ruleValue`.

Return how many rows pass. Rows are counted one by one, so rows that are
exact copies of each other each count.

@examples

## Constraints

- `1 <= items.length <= 10^4`
- `items[i].length == 3`
- `1 <= items[i][j].length, ruleValue.length <= 10`
- `ruleKey` is `"type"`, `"color"` or `"name"`.
- Every string consists of lowercase English letters.
