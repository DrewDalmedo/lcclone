/* Each new bit doubles what was read so far and adds itself. */
int getDecimalValue(struct ListNode* head) {
    int value = 0;

    for (; head; head = head->next)
        value = value * 2 + head->val;
    return value;
}
