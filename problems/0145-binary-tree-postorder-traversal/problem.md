id: 145
slug: binary-tree-postorder-traversal
title: Binary Tree Postorder Traversal
difficulty: easy
tags: stack, tree, depth-first-search, binary-tree
signature: int[] postorderTraversal(TreeNode root)
---
Take the binary tree `root` apart one node at a time. A node can be lifted
out only when nothing hangs from it any more. To clear a subtree, clear the
left branch of its top node, then the right branch, then lift the top node
itself.

Clear the whole tree this way. The answer lists the lifted values, earliest
lift first, a sequence known as the tree's **postorder**. An empty tree has
nothing to lift and yields an empty array. Two nodes may hold the same value.

In the examples, `[7,null,3]` is a root 7 whose only child, 3, is on its
right: nodes are listed top level first, left to right. `null` marks a spot
with no node under a listed parent, and the list ends at the final node.

@examples

## Constraints

- `0 <= number of nodes <= 100`
- `-100 <= Node.val <= 100`

## Follow-up

Without recursion, how can you tell, on coming back up to a node, whether
you are returning from its left branch or from its right one?
