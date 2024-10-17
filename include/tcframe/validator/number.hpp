namespace tcframe {

template<typename T>
bool isBetween(T &N, T &mn, T &mx) {
    return N >= mn && N <= mx;
}

}
