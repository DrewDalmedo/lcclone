/* A pointer moving at double speed reaches the end when this one is halfway. */
struct ListNode* middleNode(struct ListNode* head) {
    struct ListNode *fast = head;

    while (fast && fast->next) {
        head = head->next;
        fast = fast->next->next;
    }
    return head;
}
