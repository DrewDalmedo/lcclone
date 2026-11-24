id: 637
slug: average-of-levels-in-binary-tree
title: Average of Levels in Binary Tree
difficulty: easy
tags: tree, depth-first-search, breadth-first-search, binary-tree
signature: double[] averageOfLevels(TreeNode root)
---
Take a census of the binary tree `root`, one generation at a time. The root
alone is the first generation, its children are the second, their children
the third, and so on: a generation is all the nodes the same number of steps
below the root, however many there are.

Return an array with one number per generation, oldest first: the mean of
that generation's values, which is their sum divided by how many nodes it
has. A mean need not be a whole number, and each entry may be off from the
true one by up to `10^-5`.

The trees below are spelled out generation by generation, with `null` for a
child that doesn't exist.

@examples

## Constraints

- The tree has between `1` and `10^4` nodes.
- `-2^31 <= Node.val <= 2^31 - 1`
