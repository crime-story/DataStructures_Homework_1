#include <iostream>
using namespace std;

int v[100000], fr[100000], b[100000];

int i, n, k, mini = 999;

int main()
{
    cout << "Introduceti numarul de elemente: ";
    cin >> n;
    for(i=0;i<n;i++)
        cin >> v[i];
    for(i=0;i<n;i++)
    {
        if(v[i]< mini)
            mini = v[i];
        if(v[i]>k)
            k = v[i];
    }
    cout << " Mini = " << mini << endl;
    cout << " MAXI = " << k << endl;
    // v = [0,1,1,0,2,5,4,0,2,8,7,7,9,2,0,1,9], n este egal cu 17
    // fr[0] = 4
    // fr[1] = 3
    // fr[2] = 3
    // fr[4] = 1
    // fr[5] = 5
    // fr[7] = 2
    // fr[8] = 1
    // fr[9] = 2
    for(i=0;i<n;i++)
        ++fr[v[i]];
    for(i=1;i<=k;i++)
        fr[i] += fr[i-1];
    for(i=n-1;i>=0; i--)
    {
        b[fr[v[i]]]=v[i];
        fr[v[i]]-=1;
    }
    for(i=0;i<n;i++)
        cout << b[i] << " ";
    return 0;
}