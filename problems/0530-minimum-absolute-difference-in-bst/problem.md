id: 530
slug: minimum-absolute-difference-in-bst
title: Minimum Absolute Difference in BST
difficulty: easy
tags: tree, depth-first-search, breadth-first-search, binary-search-tree, binary-tree
signature: int getMinimumDifference(TreeNode root)
---
Each node of the binary search tree `root` holds a different non-negative
integer: the values in a node's left subtree are smaller than its own, and
those in its right subtree are larger.

Two nodes holding the values `a` and `b` have a **gap** of `|a - b|`. Return
the smallest gap the tree contains. Any two nodes form a pair, however far
apart they sit and whether or not one is the other's parent, but a node can't
be paired with itself.

The examples flatten the tree into one list, a level at a time; a `null`
there is a child slot with nothing in it.

@examples

## Constraints

- The tree has between `2` and `10^4` nodes.
- `0 <= Node.val <= 10^5`
- The tree is a valid binary search tree, so all its values are distinct.
