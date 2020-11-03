# emp-ag2pc-max-example

### What is this about
+ I am trying to use ag2pc to run the following simple example
<p>

    n = 3;
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

</p>

+ Both ALICE and BOB provides N (in this example, it's 3) numbers, and I try to
  reveal the original input (N numbers from both party) and the sum of them.

+ But it turns out that after transforming the output into nine integers, the
  results is a bit weird. Suppose ALICE inputs `1 2 3` and BOB inputs `4 5 6`.
  The expected output should be `A[0]: 1, A[1]: 2, A[2]: 3, B[0]: 4, B[1]: 5,
  B[2]: 6, A[0]+B[0]: 5, A[1]+B[1]: 7, A[2]+B[2]: 9`. 

+ However, the output from my side is `A[0]: 4, A[1]: 6, A[2]: 2, B[0]: 5, B[1]:
  1, B[2]: 3, A[0]+B[0]: 9, A[1]+B[1]: 7, A[2]+B[2]: 5`.

### Way I generate input for circuit
<p>

    bool* in = new bool[max(cf.n1, cf.n2)];
    for (size_t i = 0; i < bids.size(); i++) {
        int_to_bool(in + i * int_size, bids[i], int_size);
    }

</p>

+ The codes above describe how I generate the boolean array required by circuit,
  bascially I am using `int_to_bool()` function provides in `utils.hpp` and
  transform one input into the right place in the boolean array.


### How to run
+ Generate the circuit file by compiling `max-cuircuit-gen.cpp`, and execute it
  to generate the circuit file. The circuit file should be named as `max.txt`. I
  have also attached a circuit file generated from my machine
  (`max-circuit.txt`) for reference.

+ Please modify `line 14: string curcuit_file = "emp/emp-tool/max.txt";` in
  `max-ag2.cpp` and then compile it.

+ You can use this command to run the binary, `$PATH_TO_max-ag2 1 12345 1 2 3 &
  $PATH_TO_max-ag2 2 12345 4 5 6 > /dev/null` to see the output. The last three
  arguments are input for that party.

### Version
+ I am using the latest version of `emp-ag2pc`.