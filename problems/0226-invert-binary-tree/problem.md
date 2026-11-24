id: 226
slug: invert-binary-tree
title: Invert Binary Tree
difficulty: easy
tags: tree, depth-first-search, breadth-first-search, binary-tree
signature: TreeNode invertTree(TreeNode root)
---
Picture the binary tree `root` drawn on a sheet of glass. Turn the sheet over
around a vertical line through the root: the root stays on top, and whatever
was on one side of any node is now on the other. In terms of links, the left
and right children of **every** node trade places, even when one of them is
missing, so a lone left child becomes a lone right child.

Return the root of the turned tree. Values don't change. You may rearrange
the nodes you were given or build new ones. An empty tree stays empty.

Trees, in the input and the result alike, are written breadth-first:
`[6,null,2]` is a root 6 with no left child and 2 as its right child. Only
real nodes get child entries, and nothing trails the last one.

@examples

## Constraints

- `0 <= number of nodes <= 100`
- `-100 <= Node.val <= 100`
