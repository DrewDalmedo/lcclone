id: 572
slug: subtree-of-another-tree
title: Subtree of Another Tree
difficulty: easy
tags: tree, depth-first-search, binary-tree, string-matching, hash-function
signature: bool isSubtree(TreeNode root, TreeNode subRoot)
---
Snip the link between any node of the binary tree `root` and its parent, and
a **branch** falls off: that node together with everything below it. Leaving
`root` uncut also counts, so the whole tree is a branch too.

Return `true` if some branch of `root` is an exact match for the binary tree
`subRoot`, and `false` if none is. An exact match has equal values in the same
places, with left and right kept apart. It must cover the whole branch: when a
node of `root` looks like `subRoot` at the top but has more nodes where
`subRoot` stops, that branch doesn't match. Values can repeat, and `subRoot`
may be the larger tree.

An example gives `root` on its first line and `subRoot` on its second, each as
a level-by-level list with `null` for a missing child.

@examples

## Constraints

- `root` has between `1` and `2000` nodes.
- `subRoot` has between `1` and `1000` nodes.
- `-10^4 <= Node.val <= 10^4`

## Follow-up

Comparing at every node can take `O(n * m)` steps. Can you do it in
`O(n + m)`?
