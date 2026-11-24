id: 94
slug: binary-tree-inorder-traversal
title: Binary Tree Inorder Traversal
difficulty: easy
tags: stack, tree, depth-first-search, binary-tree
signature: int[] inorderTraversal(TreeNode root)
---
Press the binary tree `root` flat onto one horizontal line, so that at every
node the whole left subtree lands to the node's left and the whole right
subtree to its right. Only one arrangement obeys this at all nodes at once.
It is called the tree's **inorder** sequence: the inorder sequence of the
left subtree, then the node, then the inorder sequence of the right subtree.

Return the values along that line, from left to right. With no nodes the
line is empty, and so is the result. This need not be a search tree and
values can repeat, so don't expect the result to be sorted.

The examples write a tree breadth-first, up to its last node, with `null` in
an empty child slot of a listed node.

@examples

## Constraints

- `0 <= number of nodes <= 100`
- `-100 <= Node.val <= 100`

## Follow-up

Can you build the line with a loop alone, and then in `O(1)` extra space by
threading temporary links through the tree?
