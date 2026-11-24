id: 112
slug: path-sum
title: Path Sum
difficulty: easy
tags: tree, depth-first-search, breadth-first-search, binary-tree
signature: bool hasPathSum(TreeNode root, int targetSum)
---
A ball is dropped onto the top node of the binary tree `root` and falls from
node to child. It scores the value of every node it touches, the first and the
last included; a negative value takes points away. At a node with two
children it may go either way, and at a node with one child it has to take
that child. It comes to rest only on a **leaf**, a node with nothing below it.

Return `true` if some fall ends with a score of exactly `targetSum`, and
`false` if none does. Having that score partway down counts for nothing
unless the ball is resting on a leaf. An empty tree allows no falls at all,
so its answer is `false` for every target, `0` included.

The examples write the tree one row at a time from the root down, with `null`
for an empty child slot.

@examples

## Constraints

- The tree has between `0` and `5000` nodes.
- `-1000 <= Node.val <= 1000`
- `-1000 <= targetSum <= 1000`
