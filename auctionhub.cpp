#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace std::chrono;

struct Bid {
    string bidderId;
    double amount;
    long long timestamp;
    int auctionId;
};

void printBids(const vector<Bid>& v, size_t limit = 10) {
    for (size_t i = 0; i < min(v.size(), limit); ++i)
        cout << "[" << v[i].bidderId << "] $" << v[i].amount
             << " @ " << v[i].timestamp << "\n";
    if (v.size() > limit) cout << "... (" << v.size() << " total)\n";
}

bool loadBids(const string& path, vector<Bid>& out) {
    ifstream f(path);
    if (!f) return false;
    Bid b;
    while (f >> b.bidderId >> b.amount >> b.timestamp >> b.auctionId)
        out.push_back(b);
    return true;
}

bool isLess(const Bid& a, const Bid& b) {
    if (a.amount != b.amount)
        return a.amount < b.amount;
    return a.timestamp < b.timestamp;
}

void insertionSort(vector<Bid>& v) {
    int n = static_cast<int>(v.size());
    for (int i = 1; i < n; ++i) {
        Bid key = v[i];
        int j = i - 1;
        while (j >= 0 && isLess(key, v[j])) {
            v[j + 1] = v[j];
            --j;
        }
        v[j + 1] = key;
    }
}

void selectionSort(vector<Bid>& v) {
    int n = static_cast<int>(v.size());
    for (int i = 0; i < n - 1; ++i) {
        int maxIdx = i;
        for (int j = i + 1; j < n; ++j) {
            if (v[j].amount > v[maxIdx].amount) {
                maxIdx = j;
            }
        }
        if (maxIdx != i) {
            swap(v[i], v[maxIdx]);
        }
    }
}

void interchangeSort(vector<Bid>& v) {
    int n = static_cast<int>(v.size());
    long long swapCount = 0;
    for (int i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (v[i].amount > v[j].amount) {
                swap(v[i], v[j]);
                ++swapCount;
            }
        }
    }
    cout << "Total swaps: " << swapCount << "\n";
}

bool bubbleSortEarlyStop(vector<Bid>& v) {
    int n = static_cast<int>(v.size());
    bool isStable = true;
    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; ++j) {
            if (v[j].amount > v[j + 1].amount) {
                swap(v[j], v[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) {
            break;
        }
        if (i == 0) {
            isStable = false;
        }
    }
    return isStable;
}

int partition(vector<Bid>& v, int lo, int hi) {
    int mid = lo + (hi - lo) / 2;
    if (isLess(v[mid], v[lo])) swap(v[lo], v[mid]);
    if (isLess(v[hi], v[lo])) swap(v[lo], v[hi]);
    if (isLess(v[mid], v[hi])) swap(v[mid], v[hi]);

    Bid pivot = v[hi];
    int i = lo - 1;
    for (int j = lo; j < hi; ++j) {
        if (isLess(v[j], pivot)) {
            ++i;
            swap(v[i], v[j]);
        }
    }
    swap(v[i + 1], v[hi]);
    return i + 1;
}

void quickSort(vector<Bid>& v, int lo, int hi) {
    if (lo < hi) {
        int p = partition(v, lo, hi);
        quickSort(v, lo, p - 1);
        quickSort(v, p + 1, hi);
    }
}

void quickSortWrapper(vector<Bid>& v) {
    if (!v.empty()) {
        quickSort(v, 0, static_cast<int>(v.size()) - 1);
    }
}

bool isSortedAsc(const vector<Bid>& v) {
    for (size_t i = 1; i < v.size(); ++i) {
        if (isLess(v[i], v[i - 1])) return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    vector<Bid> smallBids;
    if (!loadBids("test_data/small.txt", smallBids)) {
        cerr << "Failed to load test_data/small.txt\n";
        return 1;
    }

    cout << "=== Task A: Insertion Sort ===\n";
    vector<Bid> taskA = smallBids;
    insertionSort(taskA);
    printBids(taskA);

    cout << "\n=== Task B: Selection Sort (Top 3) ===\n";
    vector<Bid> taskB = smallBids;
    selectionSort(taskB);
    printBids(taskB, 3);

    cout << "\n=== Task C: Interchange Sort ===\n";
    vector<Bid> taskC = smallBids;
    interchangeSort(taskC);
    printBids(taskC);

    cout << "\n=== Task D: Bubble Sort Early Stop ===\n";
    vector<Bid> taskD1 = taskA;
    bool stable1 = bubbleSortEarlyStop(taskD1);
    cout << "Already-sorted input: " << (stable1 ? "STABLE" : "UNSTABLE") << "\n";

    vector<Bid> taskD2 = taskB;
    bool stable2 = bubbleSortEarlyStop(taskD2);
    cout << "Reversed input: " << (stable2 ? "STABLE" : "UNSTABLE") << "\n";

    cout << "\n=== Task E: Quick Sort (100k records) ===\n";
    vector<Bid> largeBids;
    if (!loadBids("test_data/large.txt", largeBids)) {
        cerr << "Failed to load test_data/large.txt\n";
        return 1;
    }

    auto start = high_resolution_clock::now();
    quickSortWrapper(largeBids);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    cout << "Sorted " << largeBids.size() << " records in " << duration.count() << " ms.\n";
    cout << "Verification: " << (isSortedAsc(largeBids) ? "PASSED" : "FAILED") << "\n";
    printBids(largeBids, 5);

    return 0;
}
