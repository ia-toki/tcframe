---
toc_max_heading_level: 4
---

# Core validators

## Validating a scalar value

A scalar type is defined as:

```cpp
template<typename T>
using ScalarType = std::enable_if_t<std::is_arithmetic_v<T>>;
```

Or in simple words, it includes `int`, `long long`, `double`, and `char`.

To instantiate the validator for a value, say `val`, call the following function:

### `valueOf(val)`

```cpp
template<typename T, typename = ScalarType<T>>
ScalarValidator<T> valueOf(T val);
```

This validator contains the following function:

#### `.isBetween()`

```cpp
bool ScalarValidator<T>::isBetween(T minVal, T maxVal);
```

Returns whether `val` is between `minVal` and `maxVal`, inclusive.

---

## Validating each element of a vector

To instantiate the validator for a each element of a vector, say `vec`, call the following function:

### `eachElementOf(vec)`

```cpp
template<typename T, typename = ScalarType<T>>
VectorElementValidator<T> eachElementOf(const std::vector<T>& vec);
```

This validator contains the following functions:

#### `.isBetween()`

```cpp
bool VectorElementValidator<T>::isBetween(T minVal, T maxVal);
```

Returns whether each element of `vec` is between `minVal` and `maxVal`, inclusive.

#### `.satisfies()`

```cpp
bool VectorElementValidator<T>::satisfies(std::function<bool(T)> predicate);
```

Returns whether `predicate(e)` returns true for each `e` element of `vec`.

---

## Validating each element of a matrix

To instantiate the validator for a each element of a matrix, say `mat`, call the following function:

### `eachElementOf(mat)`

```cpp
template<typename T, typename = ScalarType<T>>
MatrixElementValidator<T> eachElementOf(const std::vector<std::vector<T>>& mat);
```

This validator contains the following functions:

#### `.isBetween()`

```cpp
bool MatrixElementValidator<T>::isBetween(T minVal, T maxVal);
```

Returns whether each element of `mat` is between `minVal` and `maxVal`, inclusive.

#### `.satisfies()`

```cpp
bool MatrixElementValidator<T>::satisfies(std::function<bool(T)> predicate);
```

Returns whether `predicate(e)` returns true for each `e` element of `mat`.

---

## Validating each character of a string

To instantiate the validator for a each character of a string, say `str`, call the following function:

### `eachCharacterOf(str)`

```cpp
StringElementValidator eachCharacterOf(const std::string& str);
```

This validator contains the following functions:

#### `.isBetween()`

```cpp
bool StringElementValidator::isBetween(char minVal, char maxVal);
```

Returns whether each character of `str` is between `minVal` and `maxVal`, inclusive.

---

## Validating all elements of a vector

To instantiate the validator for all elements of a vector, say `vec`, call the following function:

### `elementsOf(vec)`

```cpp
template<typename T, typename = ScalarType<T>>
VectorElementsValidator<T> elementsOf(const std::vector<T>& vec);
```

This validator contains the following functions:

#### `.areAscending()`

```cpp
bool VectorElementsValidator<T>::areAscending();
```

Returns whether the elements of `vec` are in a strictly ascending order.

#### `.areDescending()`

```cpp
bool VectorElementsValidator<T>::areDescending();
```

Returns whether the elements of `vec` are in a strictly descending order.

#### `.areNonAscending()`

```cpp
bool VectorElementsValidator<T>::areNonAscending();
```

Returns whether the elements of `vec` are in a non-ascending order.

#### `.areNonDescending()`

```cpp
bool VectorElementsValidator<T>::areNonDescending();
```

Returns whether the elements of `vec` are in a non-descending order.

#### `.areUnique()`

```cpp
bool VectorElementsValidator<T>::areUnique();
```

Returns whether the elements of `vec` are unique (all different).
