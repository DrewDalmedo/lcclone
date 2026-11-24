id: 563
slug: binary-tree-tilt
title: Binary Tree Tilt
difficulty: easy
tags: tree, depth-first-search, binary-tree
signature: int findTilt(TreeNode root)
---
Imagine each node of the binary tree `root` as the pivot of a balance. Its
left pan carries the total of **all** values in its left subtree, and its
right pan the total of all values in its right subtree. A missing subtree
makes an empty pan, worth 0, and the pivot's own value sits in neither.

A node's **tilt** is how far apart its two pans are: the larger total minus
the smaller, so a leaf always has tilt 0. Values can be negative, and the
pans are compared as signed totals. Add up the tilts of all the nodes and
return the result; an empty tree gives 0.

Examples show the tree level by level, using `null` for an absent child.

@examples

## Constraints

- The tree has between `0` and `10^4` nodes.
- `-1000 <= Node.val <= 1000`
- The answer fits in a 32-bit signed integer.
