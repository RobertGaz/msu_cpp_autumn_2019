#include <stdexcept>

template <class T>
class Allocator
{   
public:
    using size_type = size_t;
    using value_type = T;
    using pointer = T*;
    
    pointer allocate(size_type n) {
        return static_cast <pointer> (malloc(n * sizeof(value_type)));
    }
    
    void deallocate(pointer p, size_type n) {
        for (size_type i = 0; i < n; ++i)
            p[i].~value_type();
        free(p);
    }  
};

template <class T>
class Iterator : public std::iterator<std::random_access_iterator_tag, T>
{   
public:
    
    using base = std::iterator<std::random_access_iterator_tag, T>;    
    using typename base::pointer;
    using typename base::reference;
    using const_reference = const reference;
    using typename base::difference_type;

    explicit Iterator(pointer init_ptr) : ptr(init_ptr) {}
    
    bool operator==(const Iterator<T>& other) const
    {
        return ptr == other.ptr;
    }

    bool operator!=(const Iterator<T>& other) const
    {
        return !(*this == other);
    }

    bool operator<(const Iterator<T>& other) const
    {
        return ptr < other.ptr;
    }
    
    bool operator>(const Iterator<T>& other) const
    {
        return ptr > other.ptr;
    }
    
    bool operator<=(const Iterator<T>& other) const
    {
        return ptr <= other.ptr;
    }
    
    bool operator>=(const Iterator<T>& other) const
    {
        return ptr >= other.ptr;
    }

    reference operator*()
    {
        return *ptr;
    }
    const_reference operator*() const
    {
        return *ptr;
    }
    
    Iterator& operator++()
    {
        ++ptr;
        return *this;
    }
    
    Iterator& operator--()
    {
        --ptr;
        return *this;
    }
    
    Iterator operator+(difference_type n)
    {   
        return Iterator(ptr + n);
    }
    
    Iterator operator-(difference_type n)
    {   
        return Iterator(ptr - n);
    }
    
    Iterator operator-(const Iterator<T>& other)
    {   
        return Iterator(ptr - other.ptr);
    }
    
private:
    
    pointer ptr;
};    
    
    
template <class T, class Allocator = Allocator<T>>
class Vector 
{     
public:
    using size_type = size_t;
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using allocator_type = Allocator;    
    
    using iterator = Iterator<T>;
    using reverse_iterator = std::reverse_iterator<Iterator<T>>;
    using const_iterator = Iterator<const T>;
    using const_reverse_iterator = std::reverse_iterator<Iterator<const T>>;
    
    
    explicit Vector(size_type count = 0) : vec_size(count), reserved(count) {
        memory = Allocator();
        vec = memory.allocate(vec_size);
    }
    
    Vector(size_type count, const_reference defaultValue) {
        *this = Vector(count);
        iterator it = this->begin();
        while (it < this->end()) {
            *it = defaultValue;
            ++it;
        }
    }
    
    Vector(std::initializer_list<value_type> init) {
        *this = Vector(init.size());
        iterator vec_it = this->begin();
        auto init_it = init.begin();
        while (vec_it < this->end()) {
            *vec_it = *init_it;
            ++vec_it;
            ++init_it;
        }  
    }
    
    ~Vector() {
        memory.deallocate(vec, vec_size);
    }
    
    iterator begin() noexcept {
        return iterator(vec);
    }
    iterator end() noexcept {
        return iterator(vec + vec_size);
    }
    
    reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
    }
    reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
    }
    
    reference operator[](size_type n) {
        return vec[n];
    }
    const_reference operator[](size_type n) const {
        return vec[n];
    }
    
    size_type size() const {
        return vec_size;
    }
    
    void push_back(const value_type& value) {
        reserve(vec_size + 1);
        vec[vec_size++] = value;
    }
    
    void push_back(value_type&& value) {
        reserve(vec_size + 1);
        vec[vec_size++] = std::forward<value_type>(value);
    }
    
    void pop_back() {
        resize(vec_size - 1);
    }
    
    bool empty() const {
        return vec_size == 0;
    }
    
    void clear() {
        resize(0);
    }
    
    void resize(size_type newSize, const value_type& defaultValue = value_type()) {
        if (newSize > vec_size) {
            reserve(newSize);
            for (size_type i = vec_size; i < newSize; ++i)
                vec[i] = defaultValue;    
        } else {
            for (size_type i = newSize; i < vec_size; ++i)
                vec[i].~value_type();
        }
        
        vec_size = newSize;
        
    }
    
    void reserve(size_type count) {
        if (count > reserved) {
            size_type to_reserve = count;
            if (reserved) {
                to_reserve = reserved;
                while (count > to_reserve)
                    to_reserve *=2;
            }        
                
            pointer newData = memory.allocate(to_reserve);
            std::copy(vec, vec + vec_size, newData);
            memory.deallocate(vec, vec_size);
            vec = newData;
            reserved = to_reserve;
        }
    }
    
    size_type capacity() const {
        return reserved;
    }
    
private:
    pointer vec;
    size_type vec_size;
    size_type reserved;
    allocator_type memory;    
};
