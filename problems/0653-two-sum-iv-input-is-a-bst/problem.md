id: 653
slug: two-sum-iv-input-is-a-bst
title: Two Sum IV - Input is a BST
difficulty: easy
tags: tree, depth-first-search, breadth-first-search, binary-search-tree, hash-table, two-pointers, binary-tree
signature: bool findTarget(TreeNode root, int k)
---
Choose two different nodes of the binary search tree `root` and add their
values. Can the total come out to exactly `k`? Return `true` if some choice of
two nodes does it, and `false` if none does.

The two nodes may be anywhere in the tree; they need no relation to each
other. One node can't be chosen twice, and since the values of a search tree
are all different (smaller ones to a node's left, larger ones to its right),
a value that is exactly half of `k` doesn't help on its own. A tree with a
single node therefore always gives `false`.

The first line of an example is `root`, read across each level in turn, with
`null` where a node has nothing on that side; the second line is `k`.

@examples

## Constraints

- The tree has between `1` and `10^4` nodes.
- `-10^4 <= Node.val <= 10^4`
- `-10^5 <= k <= 10^5`
- The tree is a valid binary search tree, so all its values are distinct.
