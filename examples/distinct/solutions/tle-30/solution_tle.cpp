/* O(N²) — no early break: always scans all j < i.
   Passes subtask 1 (N <= 1000) but TLEs on subtask 2 (N <= 200000). */
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> a(n);
    for(int i = 0; i < n; ++i) cin >> a[i];

    int k = 0;
    for (int i = 0; i < n; i++) {
        int first = 1;
        for (int j = 0; j < i; j++)   // no break → guaranteed O(N²)
            if (a[j] == a[i]) first = 0;
        if (first) k++;
    }

    cout << k << '\n';
}
