id: 543
slug: diameter-of-binary-tree
title: Diameter of Binary Tree
difficulty: easy
tags: tree, depth-first-search, binary-tree
signature: int diameterOfBinaryTree(TreeNode root)
---
Treat the binary tree `root` as a map of rooms: one corridor joins each node
to each of its children, and it can be walked in either direction. A
**route** goes from one room to another without entering any room twice. It
may climb for a while and then descend, turning at any room, so the room at
the top of the tree can be left out of it. Its length is the number of
corridors it uses.

How far apart can two rooms be? Return the length of the longest route in
the tree. A tree of one node has no corridors, so its answer is 0.

Examples give the rooms in level order, writing `null` for a child that is
missing.

@examples

## Constraints

- The tree has between `1` and `10^4` nodes.
- `-100 <= Node.val <= 100`
