#include <emp-tool/emp-tool.h>

#include "emp-ag2pc/emp-ag2pc.h"
#include "iostream"
#include "sstream"
#include "string"

using namespace emp;
using namespace std;

int port, party;

// TODO: please modify this path of generated circuit file
string curcuit_file = "emp/emp-tool/max.txt";

void test(int party, NetIO* io, string name, vector<int> bids) {
    // read in the circuit from the location where it was generated
    int int_size = 32;
    string file = name;
    cout << file << endl;
    BristolFormat cf(file.c_str());
    //
    // initialize some timing stuff?
    auto t1 = clock_start();
    C2PC<NetIO> twopc(io, party, &cf);
    io->flush();
    cout << "one time:\t" << party << "\t" << time_from(t1) << endl;

    // preprocessing?
    t1 = clock_start();
    twopc.function_independent();
    io->flush();
    cout << "inde:\t" << party << "\t" << time_from(t1) << endl;

    // more preprocessing?
    t1 = clock_start();
    twopc.function_dependent();
    io->flush();
    cout << "dep:\t" << party << "\t" << time_from(t1) << endl;

    // create and fill in input vectors (to all zeros with memset)
    bool* in = new bool[max(cf.n1, cf.n2)];
    cout << "input size: max " << cf.n1 << "\t" << cf.n2 << "\t"
         << max(cf.n1, cf.n2) << endl;
    for (size_t i = 0; i < bids.size(); i++) {
        int_to_bool(in + i * int_size, bids[i], int_size);
    }

    bool* out = new bool[cf.n3];
    memset(out, false, cf.n3);

    // online protocol execution
    t1 = clock_start();
    twopc.online(in, out, true);
    cout << "online:\t" << party << "\t" << time_from(t1) << endl;

    for (size_t i = 0; i < bids.size(); i++) {
        int sum = bool_to_int<int>(out + i * int_size);
        cout << "A[" << i << "]: " << sum << endl;
    }

    for (size_t i = bids.size(); i < 2 * bids.size(); i++) {
        int sum = bool_to_int<int>(out + i * int_size);
        cout << "B[" << i - bids.size()<< "]: " << sum << endl;
    }

    for (size_t i = 2 * bids.size(); i < 3 * bids.size(); i++) {
        int sum = bool_to_int<int>(out + i * int_size);
        cout << "A[" << i - 2*bids.size() << "]+"
             << "B[" << i - 2*bids.size() << "]: " << sum << endl;
    }

    delete[] in;
    delete[] out;
}

int main(int argc, char** argv) {
    parse_party_and_port(argv, &party, &port);
    NetIO* io = new NetIO(party == ALICE ? nullptr : "127.0.0.1", port);

    vector<int> bids;
    for (int i = 3; i < argc; i++) {
        stringstream ss;
        ss << argv[i];
        int bid = 0;
        ss >> bid;
        cout << i << " bid: " << bid << endl;
        bids.push_back(bid);
    }
    test(party, io, curcuit_file, bids);

    delete io;
}