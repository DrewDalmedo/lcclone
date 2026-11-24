id: 111
slug: minimum-depth-of-binary-tree
title: Minimum Depth of Binary Tree
difficulty: easy
tags: tree, depth-first-search, breadth-first-search, binary-tree
signature: int minDepth(TreeNode root)
time_limit_ms: 200
---
You stand on the root of the binary tree `root` and may only step down, from
a node to one of its children. Your walk can end only on a **leaf**, a node
with no children at all. Return the fewest nodes such a walk can visit,
counting the root and the leaf.

A node with exactly one child is not a leaf: you can't stop there, and the
side with no child is not a way out, so the walk has to go on through the
child that exists. A root with no children is itself a leaf, which makes the
answer 1. If the tree is empty there is nowhere to stand, and the answer is
0.

Example trees are given top level first, each level from left to right, with
`null` taking the place of a child that isn't there.

@examples

## Constraints

- The tree has between `0` and `10^5` nodes.
- `-1000 <= Node.val <= 1000`
