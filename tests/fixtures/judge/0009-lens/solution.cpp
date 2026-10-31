class Solution {
public:
    vector<long long> lens(vector<int>& a, vector<long long>& b, vector<uint32_t>& c, vector<double>& d, vector<bool>& e, vector<char>& f, vector<string>& g) {
        long long sa = 0, sb = 0, te = 0, tg = 0;
        for (size_t i = 0; i < a.size(); i++) sa += a[i];
        for (size_t i = 0; i < b.size(); i++) sb += b[i];
        for (size_t i = 0; i < e.size(); i++) te += e[i];
        for (size_t i = 0; i < g.size(); i++) tg += (long long)g[i].size();
        vector<long long> out;
        out.push_back((long long)a.size()); out.push_back(sa);
        out.push_back((long long)b.size()); out.push_back(sb);
        out.push_back((long long)c.size()); out.push_back((long long)d.size());
        out.push_back((long long)e.size()); out.push_back((long long)f.size());
        out.push_back((long long)g.size()); out.push_back(tg + te);
        return out;
    }
};
