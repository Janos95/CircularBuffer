# CircularBuffer

Leightweight deque implementation by means of a Circular Buffer. The Circular Buffer grows automatically if necessary
and does not overwrite itself. On the other hand it does not automatically shrink, so it might consume a lot more memory than necessary.
To reclaim the used memory there is a shrinkToFit method though.
The Circular Buffer always allocates memory in powers of two so that adressing operations can be done with a quick masking operation.

# Benchmarks

In my benchmarks the Circular Buffer is about as fast as a std::deque if you have approximately as many pops as push operations.
The Circular Buffers performance degrades compared to the std::deque as the ratio of pushes to pops goes up though.
But feel free to play around with the benchmarking code which is located in the main.cpp.

On the good side the Circular Buffer is a lot faster than the std::dequeu in debug builds throughout the bank.
Also it compiles around twice as fast as the std::deque. In my measurements a file with a single instantiation of std::deque<int>
takes around 200ms while the same file containing a CurcularBuffer<int> compiles in around 150ms (Compiler startup time is around 100ms on 
my mashine).

# Dependancies

This depends on the [Corrade Containers library](https://doc.magnum.graphics/corrade/namespaceCorrade_1_1Containers.html).


