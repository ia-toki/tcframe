---
toc_max_heading_level: 4
---

# Random number generator

## `BaseTestSpec::rnd`

`BaseTestSpec` exposes a random number generator object `rnd` that can be utilized to define test cases. The following methods are available on it:

---

### `.nextInt(minNum, maxNum)`

```cpp
int nextInt(int minNum, int maxNum);
```

Returns a uniformly distributed random integer (int) between **minNum** and **maxNum**, inclusive.


### `.nextInt(maxNumEx)`

```cpp
int nextInt(int maxNumEx);
```

Returns a uniformly distributed random integer (int) between 0 and **maxNumEx** - 1, inclusive.

### `.nextLongLong(minNum, maxNum)`

```cpp
long long nextLongLong(long long minNum, long long maxNum);
```

Returns a uniformly distributed random integer (long long) between **minNum** and **maxNum**, inclusive.

### `.nextLongLong(maxNumEx)`

```cpp
long long nextLongLong(long long maxNumEx);
```

Returns a uniformly distributed random integer (long long) between 0 and **maxNumEx** - 1, inclusive.

### `.nextDouble(minNum, maxNum)`

```cpp
double nextDouble(double minNum, double maxNum);
```

Returns a uniformly distributed random real number (double) between **minNum** and **maxNum**, inclusive.

### `.nextDouble(maxNum)`

```cpp
double nextDouble(double maxNum);
```

Returns a uniformly distributed random real number (double) between 0 and **maxNum**, inclusive.

### `.shuffle(first, last)`

```cpp
void shuffle(std::RandomAccessIterator first, std::RandomAccessIterator last);
```

Randomly shuffles the elements in [**first**, **last**). Use this instead of `std::random_shuffle()`.
