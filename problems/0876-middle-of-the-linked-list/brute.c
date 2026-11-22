/* Count the nodes, then walk n / 2 steps from the head. */
struct ListNode* middleNode(struct ListNode* head) {
    struct ListNode *p;
    int n = 0, i;

    for (p = head; p; p = p->next)
        n++;
    for (i = 0; i < n / 2; i++)
        head = head->next;
    return head;
}
