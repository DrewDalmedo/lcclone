/* Steps through the two fringes together, each with its own stack, and stops
 * at the first difference. */
static struct TreeNode *next_leaf(struct TreeNode **stack, int *top)
{
    while (*top > 0) {
        struct TreeNode *x = stack[--*top];

        if (!x->left && !x->right)
            return x;
        if (x->right)
            stack[(*top)++] = x->right;
        if (x->left)
            stack[(*top)++] = x->left;
    }
    return NULL;
}

bool leafSimilar(struct TreeNode* root1, struct TreeNode* root2)
{
    struct TreeNode *s1[201], *s2[201];
    int t1 = 0, t2 = 0;

    s1[t1++] = root1;
    s2[t2++] = root2;
    for (;;) {
        struct TreeNode *a = next_leaf(s1, &t1);
        struct TreeNode *b = next_leaf(s2, &t2);

        if (!a || !b)
            return a == b; /* both finished, or one has leaves left */
        if (a->val != b->val)
            return false;
    }
}
