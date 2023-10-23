/*========================================================================================
 Copyright (2021), Tomer Shalev (tomer.shalev@gmail.com, https://github.com/HendrixString).
 All Rights Reserved.
 License is a custom open source semi-permissive license with the following guidelines:
 1. unless otherwise stated, derivative work and usage of this file is permitted and
    should be credited to the project and the author of this project.
 2. Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
========================================================================================*/
#pragma once

namespace static_array_traits {

    template< class T > struct remove_reference      {typedef T type;};
    template< class T > struct remove_reference<T&>  {typedef T type;};
    template< class T > struct remove_reference<T&&> {typedef T type;};

    template <class _Tp> inline
    typename remove_reference<_Tp>::type&&
    move(_Tp&& __t) noexcept {
        typedef typename remove_reference<_Tp>::type _Up;
        return static_cast<_Up&&>(__t);
    }
    template <class _Tp> inline _Tp&&
    forward(typename remove_reference<_Tp>::type& __t) noexcept {
        return static_cast<_Tp&&>(__t);
    }
    template <class _Tp> inline _Tp&&
    forward(typename remove_reference<_Tp>::type&& __t) noexcept {
        return static_cast<_Tp&&>(__t);
    }

    template<typename T=unsigned char>
    class void_allocator {
    public:
        using value_type = T;
        using size_t = unsigned long;

        template<class U>
        explicit void_allocator(const void_allocator<U> & other) noexcept { };
        explicit void_allocator()=default;

        template <class U, class... Args>
        void construct(U* p, Args&&... args) {
            new(p) U(forward<Args>(args)...);
        }

        T * allocate(size_t n) { return nullptr; }
        void deallocate(T * p, size_t n=0) { }

        template<class U> struct rebind {
            typedef void_allocator<U> other;
        };
    };

    template<class T1, class T2>
    bool operator==(const void_allocator<T1>& lhs, const void_allocator<T2>& rhs ) noexcept {
        return true;
    }
}

/**
 * Static array, equivalent to std::array, but with some mods:
 * 1. contains FAKE allocator interface, so it can be compatible with many allocator-aware algorithms.
 *
 * @tparam T item type
 * @tparam N the fixed capacity
 * @tparam fake_allocator A fake allocator
 */
template<typename T, unsigned N, class fake_allocator=static_array_traits::void_allocator<>>
class static_array {
public:
    using value_type = T;
    using allocator_type = fake_allocator;
    using index = unsigned int;
    using type = T;
    using uint = unsigned int;

private:
    T _data[N];
    index _current = 0;

public:
    explicit static_array() = default;
    template<class Iterable>
    static_array(const Iterable & list) {
        for (const auto & item : list) push_back(item);
    }
    static_array(const allocator_type & allocator) {}
    static_array(const uint count, const T & value = T(),
                 const allocator_type & alloc = allocator_type()) {
        for (int ix = 0; ix < count; ++ix) push_back(value);
    }
    static_array(const static_array & container) {
        for(auto ix = 0; ix < container.size(); ix++)
            push_back(container[ix]);
    }
    static_array(static_array && container) noexcept {
        for(auto ix = 0; ix < container.size(); ix++)
            push_back(static_array_traits::move(container[ix]));
    }

    static_array & operator=(const static_array & container) {
        if(this==&container) return (*this);
        clear();
        for(index ix = 0; ix < container.size(); ix++)
            push_back(container[ix]);
        return (*this);
    }
    static_array & operator=(static_array && container) noexcept {
        if(this==&container) return (*this);
        clear();
        for(index ix = 0; ix < container.size(); ix++)
            push_back(static_array_traits::move(container[ix]));
        return (*this);
    }

    T& operator[](index i) { return _data[i]; }
    const T& operator[](index i) const { return _data[i]; }
    T* data() { return _data; }
    const T* data() const { return _data; }
    const T& peek() { return operator[](_current); }

    void push_back(const T & v) {
        if(_current==N-1) return;
        _data[_current++] = v;
    }
    void push_back(T && v) {
        if(_current==N-1) return;
        _data[_current++] = static_array_traits::move(v);
    }
    void pop_back() {
        if(_current==0) return;
        _data[_current--].~T();
    }

    T& back() noexcept { return _data[_current-1]; }

    void clear() {
        for (int ix = 0; ix < capacity(); ++ix)
            _data[ix].~T();
        _current = 0;
    }
    index size() const { return _current; }
    constexpr index capacity() const { return N; }
    allocator_type get_allocator() const { return allocator_type(); }

    T* begin() { return _data; }
    T* end() { return _data + _current; }
    const T* begin() const { return _data; }
    const T* end() const { return _data + _current; }
};
