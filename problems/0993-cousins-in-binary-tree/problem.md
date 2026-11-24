id: 993
slug: cousins-in-binary-tree
title: Cousins in Binary Tree
difficulty: easy
tags: tree, depth-first-search, breadth-first-search, binary-tree
signature: bool isCousins(TreeNode root, int x, int y)
---
Read the binary tree `root` as a company's org chart. Each node is an employee
whose value is a badge number, and no two badges are alike. A node's parent is
that employee's manager, and the root has no manager. An employee's **rank** is
how many managers stand above them: `0` for the root, and one more than their
manager's rank for anyone else.

Two employees are **cousins** when they have the same rank but different
managers. Nothing else matters, so they may work in entirely separate parts of
the company. Given the badges `x` and `y` of two different employees, return
`true` if they are cousins and `false` if not. Swapping `x` and `y` never
changes the answer, and the root has no cousins.

In the examples, the first line is the chart written top rank first, left to
right, with `null` in a free child position; then come `x` and `y`.

@examples

## Constraints

- The tree has between `2` and `100` nodes.
- `1 <= Node.val <= 100`, and all values are distinct.
- `x != y`, and both are values of nodes in the tree.
