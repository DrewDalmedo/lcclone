id: 104
slug: maximum-depth-of-binary-tree
title: Maximum Depth of Binary Tree
difficulty: easy
tags: tree, depth-first-search, breadth-first-search, binary-tree
signature: int maxDepth(TreeNode root)
---
You are given `root`, the root of a binary tree. Number the levels of the
tree: the root is on level 1, its children are on level 2, their children on
level 3, and so on. Return the number of the **deepest level that holds a
node**. An empty tree has no levels, so its answer is 0.

Trees in the examples are written in level order: the values are listed row
by row from left to right, and `null` stands for a missing child. Only nodes
that exist have their children listed, so a `null` has no entries below it,
and the `null`s at the end of the list are left out.

@examples

## Constraints

- The tree has between `0` and `10^4` nodes.
- `-100 <= Node.val <= 100`

## Follow-up

A tree with `10^4` nodes can be a single chain. How deep does a recursive
solution go on such a tree, and can you write one that visits the tree level
by level instead?
