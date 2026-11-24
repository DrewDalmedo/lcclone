id: 257
slug: binary-tree-paths
title: Binary Tree Paths
difficulty: easy
tags: tree, depth-first-search, string, backtracking, binary-tree
signature: string[] binaryTreePaths(TreeNode root)
compare: unordered 0
---
For each **leaf** of the binary tree `root` (a node without children), write
the breadcrumb that leads to it: the values met on the way from the root down
to that leaf, in that order, joined by the two characters `->` with no
spaces. Values are in plain decimal and a negative one keeps its minus sign,
so a route through `-7` and then `3` reads `-7->3`. A tree that is only a root
has one breadcrumb, its value alone, with no arrow.

The result holds exactly one breadcrumb per leaf, listed in whatever order you
like. When two leaves produce the same text, that text appears twice.

An example shows the tree row after row from the top, with `null` holding the
place of an absent child.

@examples

## Constraints

- The tree has between `1` and `100` nodes.
- `-100 <= Node.val <= 100`
