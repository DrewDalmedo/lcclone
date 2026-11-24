id: 404
slug: sum-of-left-leaves
title: Sum of Left Leaves
difficulty: easy
tags: tree, depth-first-search, breadth-first-search, binary-tree
signature: int sumOfLeftLeaves(TreeNode root)
---
Put two questions to each node of the binary tree `root`. Is it the left child
of its parent? Does it have no children of its own? A node that answers yes to
both is a **left leaf**. The root has no parent, so it is never one, even when
it stands alone, and a right child is never one, however bare.

Add up the values held by the left leaves and return the total, which is `0`
for a tree without any.

In an example the nodes appear breadth first, left to right, and `null` stands
in wherever a node lacks a child on that side.

@examples

## Constraints

- The tree has between `1` and `1000` nodes.
- `-1000 <= Node.val <= 1000`
