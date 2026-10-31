// lc_rt.cpp: the C++ harness runtime. It follows lc_rt_core.h in every
// C++ build and gives the generated harness lc_get(), which reads a value
// of any signature type, and lc_put(), which writes one: overloads for the
// scalars, and templates for vectors of them. It must stay valid C++11.

// ------------------------------------------------------------------
// Reading
// ------------------------------------------------------------------

LC_RT void lc_get(int &v)
{
    v = (int)lc_rt_ll(INT_MIN, INT_MAX);
}

LC_RT void lc_get(long long &v)
{
    v = lc_rt_ll(LLONG_MIN, LLONG_MAX);
}

LC_RT void lc_get(uint32_t &v)
{
    v = (uint32_t)lc_rt_ll(0, 4294967295LL);
}

LC_RT void lc_get(double &v)
{
    v = lc_rt_double();
}

LC_RT void lc_get(bool &v)
{
    v = lc_rt_bool() != 0;
}

LC_RT void lc_get(char &v)
{
    v = (char)(unsigned char)lc_rt_ll(0, 255);
}

LC_RT void lc_get(std::string &v)
{
    size_t len;
    char *s = lc_rt_string(&len);

    v.assign(s, len);
    free(s);
}

template <class T> static void lc_get(std::vector<T> &v)
{
    int n = lc_rt_count();

    v.clear();
    v.reserve((size_t)n);
    for (int i = 0; i < n; i++) {
        T x = T();
        lc_get(x);
        v.push_back(x);
    }
}

// ------------------------------------------------------------------
// Writing
// ------------------------------------------------------------------

LC_RT void lc_put(int v)
{
    lc_rt_put_ll(v);
}

LC_RT void lc_put(long long v)
{
    lc_rt_put_ll(v);
}

LC_RT void lc_put(uint32_t v)
{
    lc_rt_put_ull(v);
}

LC_RT void lc_put(double v)
{
    lc_rt_put_double(v);
}

LC_RT void lc_put(bool v)
{
    lc_rt_put_ll(v ? 1 : 0);
}

LC_RT void lc_put(char v)
{
    lc_rt_put_ll((unsigned char)v);
}

LC_RT void lc_put(const std::string &v)
{
    lc_rt_put_string(v.data(), v.size());
}

template <class T> static void lc_put(const std::vector<T> &v)
{
    lc_rt_put_ll((long long)v.size());
    for (size_t i = 0; i < v.size(); i++)
        lc_put(v[i]);
}
