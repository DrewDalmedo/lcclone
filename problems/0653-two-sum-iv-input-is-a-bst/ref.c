/* Two in-order walks over the tree, one rising from the smallest value and
 * one falling from the largest, each on its own explicit stack: the
 * two-pointer search on a sorted list, in O(h) memory. The walks stop when
 * they reach the same node, so a node is never paired with itself. */
struct bst_walk {
    struct TreeNode** stack;
    size_t top, cap;
    int falling;
};

static void walk_descend(struct bst_walk* w, struct TreeNode* node) {
    while (node) {
        if (w->top == w->cap) {
            w->cap *= 2;
            w->stack = realloc(w->stack, w->cap * sizeof *w->stack);
        }
        w->stack[w->top++] = node;
        node = w->falling ? node->right : node->left;
    }
}

static struct TreeNode* walk_next(struct bst_walk* w) {
    struct TreeNode* node = w->stack[--w->top];

    walk_descend(w, w->falling ? node->left : node->right);
    return node;
}

bool findTarget(struct TreeNode* root, int k) {
    struct bst_walk up = { NULL, 0, 64, 0 }, down = { NULL, 0, 64, 1 };
    struct TreeNode* a, * b;
    bool found = false;

    up.stack = malloc(up.cap * sizeof *up.stack);
    down.stack = malloc(down.cap * sizeof *down.stack);
    walk_descend(&up, root);
    walk_descend(&down, root);
    a = walk_next(&up);
    b = walk_next(&down);
    while (a != b) {
        int sum = a->val + b->val;

        if (sum == k) {
            found = true;
            break;
        }
        if (sum < k)
            a = walk_next(&up);
        else
            b = walk_next(&down);
    }
    free(up.stack);
    free(down.stack);
    return found;
}
