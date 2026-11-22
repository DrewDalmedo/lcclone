class Solution {
public:
    bool hasCycle(ListNode* head) {
        unordered_set<ListNode*> seen;
        for (; head; head = head->next)
            if (!seen.insert(head).second) return true;
        return false;
    }
};
