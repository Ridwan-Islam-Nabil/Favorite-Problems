/*
Problem Link: https://codeforces.com/gym/103373/problem/F
Prerequisites: Segment tree lazy propagation.
*/

#include <bits/stdc++.h>
using namespace std;

#define endl '\n'
#define ll long long
#define fastIO() ({                   \
    ios_base::sync_with_stdio(false); \
    cin.tie(NULL);                    \
})

const int N = 2e5 + 100, inf = 1e9;

struct node
{
    int rightmost, leftmost;
    ll sflen, pflen;
    ll tot_ans, tot_sub_len;
};
int a[N];
struct ST
{
#define lc (n << 1)
#define rc ((n << 1) + 1)

    node t[4 * N];
    int lazy[4 * N];
    node mergee(node l, node r)
    {
        if (l.leftmost == inf)
            return r;
        if (r.leftmost == inf)
            return l;
        node ans;
        ans.rightmost = r.rightmost;
        ans.leftmost = l.leftmost;
        ans.tot_sub_len = l.tot_sub_len + r.tot_sub_len;

        if (l.rightmost == r.leftmost)
        { // Not alternating
            ans.tot_ans = l.tot_ans + r.tot_ans;
            ans.sflen = r.sflen;
            ans.pflen = l.pflen;
        }
        else
        {
            ll cal_ans = l.sflen + r.pflen; // Calculating ans of the intersecting part
            cal_ans = (cal_ans * (cal_ans + 1)) / 2LL;
            l.tot_ans -= (l.sflen * (l.sflen + 1)) / 2LL; // Subtracting, because it's already
            r.tot_ans -= (r.pflen * (r.pflen + 1)) / 2LL; // being calculated in the cal_ans variable
            ans.tot_ans = cal_ans + l.tot_ans + r.tot_ans;

            ans.pflen = l.pflen;
            if (l.pflen == l.tot_sub_len)
                ans.pflen += r.pflen;
            ans.sflen = r.sflen;
            if (r.sflen == r.tot_sub_len)
                ans.sflen += l.sflen;
        }
        return ans;
    }
    inline void push(int n, int b, int e)
    { // change this
        if (lazy[n] % 2 == 0)
            return;
        t[n].leftmost = 1 - t[n].leftmost;
        t[n].rightmost = 1 - t[n].rightmost;
        if (b != e)
        {
            lazy[lc] = lazy[lc] + lazy[n];
            lazy[rc] = lazy[rc] + lazy[n];
        }
        lazy[n] = 0;
    }
    void build(int n, int b, int e)
    {
        if (b == e)
        {
            t[n].rightmost = t[n].leftmost = a[b];
            t[n].sflen = t[n].pflen = t[n].tot_ans = t[n].tot_sub_len = 1;
            return;
        }
        int mid = (b + e) >> 1, l = n << 1, r = l | 1;
        build(l, b, mid);
        build(r, mid + 1, e);
        t[n] = mergee(t[l], t[r]); // change this
    }
    void upd(int n, int b, int e, int i, int j)
    {
        push(n, b, e);
        if (j < b || e < i)
            return;
        if (i <= b && e <= j)
        {
            lazy[n] += 1; // set lazy
            push(n, b, e);
            return;
        }
        int mid = (b + e) >> 1;
        upd(lc, b, mid, i, j);
        upd(rc, mid + 1, e, i, j);
        t[n] = mergee(t[lc], t[rc]);
    }
    node query(int n, int b, int e, int i, int j)
    {
        push(n, b, e);
        if (b > j || e < i)
            return {inf, inf, inf, inf, inf}; // return appropriate value
        if (b >= i && e <= j)
            return t[n];
        int mid = (b + e) >> 1, l = n << 1, r = l | 1;
        return mergee(query(l, b, mid, i, j), query(r, mid + 1, e, i, j)); // change this
    }
} t;

int main()
{
    fastIO();
    int test = 1;
    // cin >> test;
    for (int tc = 1; tc <= test; tc++)
    {

        int n, q;
        cin >> n >> q;
        for (int i = 1; i <= n; i++)
            cin >> a[i];
        t.build(1, 1, n);
        while (q--)
        {
            int ty, l, r;
            cin >> ty >> l >> r;
            if (ty == 1)
            { // update
                t.upd(1, 1, n, l, r);
            }
            else
            {
                cout << t.query(1, 1, n, l, r).tot_ans << endl;
            }
        }
    }

    return 0;
}