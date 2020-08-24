#include "CircularBuffer.h"

#include <deque>
#include <chrono>
#include <random>
#include <cstdio>

#include <cmath>

using my_clock = std::chrono::steady_clock;
using my_dur = std::chrono::duration<double, std::ratio<1>>;

constexpr float pushProb = 0.5f;
constexpr std::size_t n = 1'00'000'000;

template<class T>
CORRADE_ALWAYS_INLINE void doNotOptimize(T const& value) {
    asm volatile("" : : "r,m"(value) : "memory");
}

namespace Cr = Corrade;

using T = int;
using Deque = std::deque<T>;

using FuncPtr1 = void(*)(Deque&);
using FuncPtr2 = void(*)(CircularBuffer<T>&);

using OpType = std::pair<FuncPtr1, FuncPtr2>;

static FuncPtr1 pushesDeque[] = {
        +[](Deque& q){ doNotOptimize(q.emplace_back(1)); },
        +[](Deque& q){ doNotOptimize(q.emplace_back(2)); },
        +[](Deque& q){ doNotOptimize(q.emplace_front(2)); },
        +[](Deque& q){ doNotOptimize(q.emplace_front(1)); },
};

static FuncPtr2 pushesCircularBuffer[] = {
        +[](CircularBuffer<T>& q){ q.emplaceBack(1); },
        +[](CircularBuffer<T>& q){ q.emplaceBack(2); },
        +[](CircularBuffer<T>& q){ q.emplaceFront(2); },
        +[](CircularBuffer<T>& q){ q.emplaceFront(1); },
};

static FuncPtr1 popsDeque[] = {
        +[](Deque& q){ doNotOptimize(q.front()); q.pop_front(); },
        +[](Deque& q){ doNotOptimize(q.back()); q.pop_back(); },
};

static FuncPtr2 popsCircularBuffer[] = {
        +[](CircularBuffer<T>& q){ doNotOptimize(q.popFront()); },
        +[](CircularBuffer<T>& q){ doNotOptimize(q.popBack()); },
};

Cr::Containers::Array<OpType> generateOps(std::size_t& pushCount, std::size_t& popCount) {
    std::mt19937 gen(0);

    std::bernoulli_distribution bern(pushProb);
    std::uniform_int_distribution<int> uniform(0,3);

    Cr::Containers::Array<OpType> ops(n);

    std::size_t max = 0;

    for(int i = 0; i < n; ++i) {
        int idx = uniform(gen);
        if(pushCount == popCount || bern(gen)) {
            ops[i] = std::pair{pushesDeque[idx], pushesCircularBuffer[idx]};
            ++pushCount;
        } else {
            ops[i] = std::pair{popsDeque[idx%2], popsCircularBuffer[idx%2]};
            ++popCount;
        }
        max = std::max(max, pushCount - popCount);
    }

    Cr::Utility::Debug{} << "Peak element count" << max;
    return ops;
}


int main() {
    std::size_t a = 0, b = 0;
    auto ops = generateOps(a, b);

    Deque q1;
    CircularBuffer<T> q2;

    {
        auto begin = my_clock::now();
        for(int i = 0; i < n; ++i) {
            ops[i].second(q2);
        }
        auto end = my_clock::now();

        my_dur total = end - begin;
        printf("CircularBuffer: %zu pushes and %zu pops took %f seconds\n", a, b, total.count());
    }

    {
        auto begin = my_clock::now();
        for(int i = 0; i < n; ++i) {
            ops[i].first(q1);
        }
        auto end = my_clock::now();

        my_dur total = end - begin;
        printf("StdQueue: %zu pushes and %zu pops took %f seconds\n", a, b, total.count());
    }

    CORRADE_ASSERT(q1.size() == q2.size(), "Queues dont have the same size", 1);
    for(std::size_t i = 0; i < q1.size(); ++i) {
        CORRADE_ASSERT(q1[i] == q2[i], "Queues dont contain the same elements", 1);
    }

    return 0;
}
