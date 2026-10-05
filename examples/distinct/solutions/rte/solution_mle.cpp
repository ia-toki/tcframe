/* Allocates a 1 GB sieve regardless of input size.
   With the 256 MB memory limit active, vector construction triggers SIGSEGV
   → grader reports MLE. */
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> a(n);
    for (int& x : a) cin >> x;

    // 1 GB sieve — triggers MLE under the 256 MB limit
    vector<char> seen(1'000'000'000, 0);

    int k = 0;
    for (int x : a) {
        if (!seen[x - 1]) { seen[x - 1] = 1; k++; }
    }

    cout << k << '\n';
}
