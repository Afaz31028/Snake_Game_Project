#include <bits/stdc++.h>
using namespace std;
const int N = 1e5;
int segTree[4 * N];
int a[N];
void makeTree(int node, int st, int end)
{
    if (st == end)
    {
        segTree[node] = a[st];
        return;
    }
    int mid = (st + end) / 2;
    int left = 2 * node;
    int right = 2 * node + 1;
    makeTree(left, st, mid);
    makeTree(right, mid + 1, end);
    segTree[node] = segTree[left] + segTree[right];
}
void update(int node, int st, int end, int idx, int val)
{
    if (st == end)
    {
        segTree[node] = val;
        return;
    }
    int mid = (st + end) / 2;
    int left = 2 * node;
    int right = 2 * node + 1;
    if (idx <= mid)
    {
        update(left, st, mid, idx, val);
    }
    else
    {
        update(right, mid + 1, end, idx, val);
    }
    segTree[node] = segTree[left] + segTree[right];
}
int Query(int node, int st, int end, int l, int r)
{
    if (st > r || end < l)
    {
        return 0;
    }
    if (st >= l && end <= r)
    {
        return segTree[node];
    }
    int mid = (st + end) / 2;
    int left = 2 * node;
    int right = 2 * node + 1;
    int q1 = Query(left, st, mid, l, r);
    int q2 = Query(right, mid + 1, end, l, r);
    return q1 + q2;
}
int Query2(int node, int st, int end, int l, int r)
{
    if (st > r || end < l)
    {
        return INT_MAX;
    }
    if (st >= l && end <= r)
    {
        return segTree[node];
    }
    int mid = (st + end) / 2;

    int left_seg = 2 * node;
    int q1 = Query2(left_seg, st, mid, l, r);

    int right_seg = 2 * node + 1;
    int q2 = Query2(right_seg, mid + 1, end, l, r);

    return min(q1, q2);
}
int main()
{

    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
    }
    makeTree(1, 0, n - 1);
    int q;
    cin >> q;
    while (q--)
    {
        int op;
        cin >> op;
        if (op == 1)
        {
            int l, r;
            cin >> l >> r;
            cout << Query(1, 0, n - 1, l - 1, r - 1) << endl;
        }
        else
        {
            int p, q;
            cin >> p >> q;
            p--, q--;
            if (p == 0)
            {
                cout << Query(1, 0, n - 1, p, q) << endl;
            }
            else
            {
                int ans1 = Query(1, 0, n - 1, 0, p - 1);
                int ans2 = Query2(1, 0, n - 1, 0, p - 1);
                int x = (p - q) + 1;
                long long z = x * ans2;
                long long y = ans1 + z;
                cout << y << endl;
            }
        }
    }
    return 0;
}