#include "emp-tool/emp-tool.h"
using namespace emp;
#include "iostream"

void max(int n, vector<int> bid_vec) {
    int int_size = 32;
    Integer *A = new Integer[n];
    Integer *B = new Integer[n];
    Integer *C = new Integer[n];
    Integer *D = new Integer[n];
    Integer *E = new Integer[n];

    for (int i = 0; i < n; ++i) {
        A[i] = Integer(int_size, bid_vec[i], ALICE);
        B[i] = Integer(int_size, bid_vec[i], BOB);
    }

    for (int i = 0; i < n; ++i) {
        C[i] = A[i];
        D[i] = B[i];
        E[i] = A[i] + B[i];
    }

    for (int i = 0; i < n; i++) {
        C[i].reveal<uint32_t>();
    }

    for (int i = 0; i < n; i++) {
        D[i].reveal<uint32_t>();
    }

    for (int i = 0; i < n; i++) {
        E[i].reveal<uint32_t>();
    }
}

int main(int argc, char **argv) {
    setup_plain_prot(true, "max.txt");
    vector<int> bids;
    int num_bid = 3;
    for (int i = 0; i < num_bid; i++) {
        bids.push_back(i);
    }
    max(num_bid, bids);
    finalize_plain_prot();
}