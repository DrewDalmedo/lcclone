/* Right but O(n^2): for each node of the first half, walk on to its mirror
 * image. */
bool isPalindrome(struct ListNode* head) {
    struct ListNode *p, *q;
    int n = 0, i, j;

    for (p = head; p; p = p->next)
        n++;
    p = head;
    for (i = 0; i < n / 2; i++, p = p->next) {
        q = p;
        for (j = i; j < n - 1 - i; j++)
            q = q->next;
        if (p->val != q->val)
            return false;
    }
    return true;
}
