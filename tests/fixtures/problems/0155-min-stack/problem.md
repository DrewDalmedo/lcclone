id: 155
slug: min-stack
title: Min Stack
difficulty: medium
tags: stack, design
class: MinStack
api: MinStack(); void push(int val); void pop(); int top(); int getMin()
---
Build a stack of integers that can also report its smallest element at any
moment. Implement the `MinStack` class:

- `MinStack()` creates an empty stack.
- `void push(int val)` puts `val` on top.
- `void pop()` removes the top element.
- `int top()` returns the top element without removing it.
- `int getMin()` returns the smallest element on the stack.

Every operation must take constant time. `pop`, `top` and `getMin` are only
called when the stack is not empty.

---

At most `20000` operations are performed in total.
