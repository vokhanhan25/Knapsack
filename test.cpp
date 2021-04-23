#include <iostream>
#include <vector>

using namespace std;

int main() {
    vector<int> a = {10, 5, 7};
    vector<int> b = {2, 3, 4, 5, 6};
    a = b;
    a[0] = 1000;
    for (int i = 0; i < a.size(); i++)
        cout << a[i] << " ";
    cout << "\n";

    for (int i = 0; i < b.size(); i++)
        cout << b[i] << " ";
    cout << "\n";
}