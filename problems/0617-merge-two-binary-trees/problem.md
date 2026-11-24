id: 617
slug: merge-two-binary-trees
title: Merge Two Binary Trees
difficulty: easy
tags: tree, depth-first-search, breadth-first-search, binary-tree
signature: TreeNode mergeTrees(TreeNode root1, TreeNode root2)
---
Every node of a binary tree has an **address**: the sequence of left and
right steps that leads to it from the root of its own tree, so a root's
address is empty. Build one tree from `root1` and `root2`, address by
address. It has a node at each address used in at least one of them:

- if both trees have a node there, the new value is the sum of their values;
- if only one has, the new node keeps that value unchanged.

Since addresses start at each tree's own root, the two roots always pair up,
and where one tree has nothing, the other's whole subtree carries over
intact. A sum of `0` is a value like any other: its node stays.

Return the root of the combined tree, which is empty only when both inputs
are. You may reuse nodes of the inputs or allocate new ones.

Trees are listed level by level. Each real node gets two child slots, `null`
when empty, and the list stops at its last real node.

@examples

## Constraints

- `0 <= number of nodes in each tree <= 2000`
- `-10^4 <= Node.val <= 10^4`
