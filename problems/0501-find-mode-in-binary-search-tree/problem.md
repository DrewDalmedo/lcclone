id: 501
slug: find-mode-in-binary-search-tree
title: Find Mode in Binary Search Tree
difficulty: easy
tags: tree, depth-first-search, binary-search-tree, binary-tree
signature: int[] findMode(TreeNode root)
compare: unordered 0
---
The tree `root` keeps its values in order but allows repeats: at every node,
nothing in the left subtree is larger than the node's value, and nothing in
the right subtree is smaller. Copies of one value can therefore lie on both
sides of a node that holds it, and can be far apart in the tree.

The **count** of a value is the number of nodes holding it. A value is a
**mode** of the tree when no other value has a higher count. Return every
mode, each listed once, in any order. Ties are common: when no value repeats,
all of the tree's values are modes.

Inputs below are in level order; `null` says a node lacks that child.

@examples

## Constraints

- The tree has between `1` and `10^4` nodes.
- `-10^5 <= Node.val <= 10^5`
- At every node, all values in the left subtree are `<=` the node's value and all values in the right subtree are `>=` it.

## Follow-up

Can you find the modes with no memory beyond the returned list and the call
stack?
