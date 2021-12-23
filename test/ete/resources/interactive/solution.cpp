#include <bits/stdc++.h>
using namespace std;

int main() {
    int lo = 1, hi = 10;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        cout << mid << endl;

        string response;
        cin >> response;

        if (response == "TOO_SMALL") {
            lo = mid+1;
        } else if (response == "TOO_LARGE") {
            hi = mid-1;
        } else {
            break;
        }
    }
}
