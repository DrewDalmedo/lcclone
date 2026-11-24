/* Depth first with an explicit stack. One shared text buffer holds the
 * current route; each stack entry remembers how long the text was when the
 * node was pushed, so popping it cuts the text back to that length. */
char** binaryTreePaths(struct TreeNode* root, int* returnSize) {
    struct item {
        struct TreeNode* node;
        int len;
    } stack[128];
    char text[640];
    char** out = malloc(64 * sizeof *out);
    int top = 0, count = 0;

    stack[top].node = root;
    stack[top++].len = 0;
    while (top > 0) {
        struct item it = stack[--top];
        struct TreeNode* p = it.node;
        int len = it.len;

        if (len > 0) {
            text[len++] = '-';
            text[len++] = '>';
        }
        len += sprintf(text + len, "%d", p->val);
        if (!p->left && !p->right) {
            out[count] = malloc((size_t)len + 1);
            memcpy(out[count], text, (size_t)len + 1);
            count++;
            continue;
        }
        if (p->right) {
            stack[top].node = p->right;
            stack[top++].len = len;
        }
        if (p->left) {
            stack[top].node = p->left;
            stack[top++].len = len;
        }
    }
    *returnSize = count;
    return out;
}
