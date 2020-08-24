# CircularBuffer

Leightweight dequeu implementation by means of a Circular Buffer. The Circular Buffer grows automatically if necessary
and does not overwrite itself. On the other hand it does not automatically shrink, so it might consume a lot more memory than necessary.
To reclaim the used memory there is a shrinkToFit method though.
The Circular Buffer always allocated memory in powers of two so that adressing operations can be done with a quick masking operation.

# Benchmarks

In my benchmarks the Circular Buffer is about as fast as a std::deque if you have approximately as many pops as push operations.
The Circular Buffers performance degrades compared to the std::deque as the ratio of pushes to pops goes up though.

On the good side the Circular Buffer is a lot faster than the std::dequeu in debug builds trhoughout the bank.
