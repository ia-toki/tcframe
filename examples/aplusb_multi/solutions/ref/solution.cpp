#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    for (int i = 1; i <= T; i++) {
        long long A, B;
        cin >> A >> B;
        cout << "Case #" << i << ": " << A + B << '\n';
    }
}
