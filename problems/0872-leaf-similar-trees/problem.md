id: 872
slug: leaf-similar-trees
title: Leaf-Similar Trees
difficulty: easy
tags: tree, depth-first-search, binary-tree
signature: bool leafSimilar(TreeNode root1, TreeNode root2)
---
A **leaf** is a node with no children at all: a node with one child is not a
leaf, and a root standing alone is. Trace a binary tree's outline from left to
right and its leaves come up in a fixed order, the order in which a depth-first
walk that goes left before right reaches them. The list of their values, in
that order, is the tree's **fringe**. How deep a leaf lies has no effect on its
place in the fringe.

Return `true` if `root1` and `root2` have the same fringe, meaning the same
number of leaves and equal values at every position, and `false` otherwise.
Everything else about the two trees, such as their shapes, sizes and inner
values, may differ.

In an example, each of the two lines is a tree spelled out level by level,
where `null` means that child isn't there.

@examples

## Constraints

- Each tree has between `1` and `200` nodes.
- `0 <= Node.val <= 200`
