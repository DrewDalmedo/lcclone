id: 144
slug: binary-tree-preorder-traversal
title: Binary Tree Preorder Traversal
difficulty: easy
tags: stack, tree, depth-first-search, binary-tree
signature: int[] preorderTraversal(TreeNode root)
---
An explorer enters the binary tree `root` at the top, carrying a notebook.
The moment they first stand on a node, they write down its value. They then
go into the node's left branch, if there is one, and stay until all of it
has been seen; after that they do the same with the right branch, and climb
back up.

Return the notebook's entries, first to last. This order is the tree's
**preorder**, which is a node, then the preorder of its left subtree, then
the preorder of its right subtree. With an empty tree nothing gets written,
so the result is an empty array. Values can repeat.

A `null` in an example tree holds the place of an absent child; levels are
listed from the top down, with no entries under a `null` and none after the
last node.

@examples

## Constraints

- `0 <= number of nodes <= 100`
- `-100 <= Node.val <= 100`

## Follow-up

With an explicit stack in place of recursion, which child has to go on the
stack first so that the left branch is explored first?
