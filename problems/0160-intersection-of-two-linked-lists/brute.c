/* For each node of A in turn, look for it in all of B. */
struct ListNode *getIntersectionNode(struct ListNode *headA, struct ListNode *headB) {
    struct ListNode *a, *b;

    for (a = headA; a; a = a->next)
        for (b = headB; b; b = b->next)
            if (a == b)
                return a;
    return NULL;
}
