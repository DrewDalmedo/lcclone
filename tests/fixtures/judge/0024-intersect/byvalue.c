/* Joins by value, not by node: [8,8,4] and [8,4] meet at A's first 8. */
struct ListNode* getIntersectionNode(struct ListNode* headA,
                                     struct ListNode* headB) {
    struct ListNode *a, *b;

    for (a = headA; a; a = a->next)
        for (b = headB; b; b = b->next)
            if (a->val == b->val)
                return a;
    return NULL;
}
