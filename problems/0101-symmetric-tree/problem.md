id: 101
slug: symmetric-tree
title: Symmetric Tree
difficulty: easy
tags: tree, depth-first-search, breadth-first-search, binary-tree
signature: bool isSymmetric(TreeNode root)
---
Any node of the binary tree `root` can be reached from the top by a list of
turns, such as left, right, right. Flip every turn in that list and you get
the position of the node's **counterpart**: here right, left, left. The root
takes no turns, so it is its own counterpart.

Return `true` if every node's counterpart position holds a node with the same
value, and `false` if some counterpart is missing or holds a different value.
The root's own value plays no part, and values can repeat.

Each example is a single list: the tree row by row, with `null` filling an
empty child position of a node in the row above.

@examples

## Constraints

- The tree has between `1` and `1000` nodes.
- `-100 <= Node.val <= 100`

## Follow-up

Write it once as a recursion, then once more with a queue or stack of your
own.
