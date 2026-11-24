id: 2331
slug: evaluate-boolean-binary-tree
title: Evaluate Boolean Binary Tree
difficulty: easy
tags: tree, depth-first-search, binary-tree
signature: bool evaluateTree(TreeNode root)
---
The binary tree `root` is a small logic circuit, and the signal leaving its
root is the circuit's output. No node has exactly one child.

- A node with no children is an input wire. Value `0` means it carries false,
  and value `1` means it carries true.
- A node with two children is a gate fed by its children's signals. Value `2`
  is an **OR** gate, which outputs true when at least one input is true. Value
  `3` is an **AND** gate, which outputs true only when both inputs are true.

Return the circuit's output. A circuit that is a single wire outputs what that
wire carries.

The examples list the nodes row by row, starting at the output, with `null`
filling the empty places under a wire.

@examples

## Constraints

- The tree has between `1` and `1000` nodes.
- `0 <= Node.val <= 3`
- A node's child count is `0` or `2`.
- A node with no children has value `0` or `1`.
- A node with two children has value `2` or `3`.
