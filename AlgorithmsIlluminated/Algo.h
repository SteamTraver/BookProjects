#pragma once
#include <vector>
#include <algorithm>

using std::vector;
using std::copy;
using std::swap;

// 合并两个有序列表
vector<int> Merge(const vector<int>& a, const vector<int>& b)
{
    vector<int> result;
    result.assign(a.size() + b.size(), 0);

    vector<int>::const_iterator j = a.begin();
    vector<int>::const_iterator k = b.begin();

    for (auto i = result.begin(); i != result.end(); i++) {
        if (j == a.end() && k != b.end()) {
            copy(k, b.end(), i);
            break;
        }
        if (j != a.end() && k == b.end()) {
            copy(j, a.end(), i);
            break;
        }

        if (*j <= *k) {
            *i = (*j);
            j++;
        } else {
            *i = (*k);
            k++;
        }
    }

    return result;
}

vector<int> MergeSort(vector<int>& in)
{
    if (in.size() < 2) {
        return in;
    } else if (in.size() == 2 && in[0] > in[1]) {
        swap(in[0], in[1]);
        return in;
    }

    vector<int> left, right;
    left.assign(in.begin(), in.begin() + in.size() / 2);
    right.assign(in.begin() + in.size() / 2, in.end());

    vector<int> a = MergeSort(left);
    vector<int> b = MergeSort(right);

    return Merge(a, b);
}