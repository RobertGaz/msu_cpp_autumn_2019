#include <stdexcept>
#include <new>

template <class T>
class Allocator
{   
public:
    using size_type = size_t;
    using value_type = T;
    using pointer = T*;
    using const_reference = const T&;
    
    pointer allocate(size_type n) {
        pointer result = static_cast <pointer> (malloc(n * sizeof(value_type)));
        if (result == nullptr) 
            throw std::bad_alloc();
    }
    
    void deallocate(pointer p, size_type n) {
        free(p);
    }
    
    void construct(pointer p, const_reference val) {
        new((void *) p) value_type(val);
    }   
    
    void destroy(pointer p) {
        p->~value_type();
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
    
    
    explicit Vector(size_type count = 0) : memory(Allocator()), vec_size(count), reserved(count) {
        vec = memory.allocate(vec_size);
        for (size_type i = 0; i < vec_size; ++i)
            memory.construct(vec + i, value_type());
    }
    
    Vector(size_type count, const_reference defaultValue) : memory(Allocator()), vec_size(count), reserved(count) {
        vec = memory.allocate(vec_size);
        for (size_type i = 0; i < vec_size; ++i)
            memory.construct(vec + i, defaultValue);
    }
    
    Vector(std::initializer_list<value_type> init) : memory(Allocator()), vec_size(init.size()), reserved(init.size()){
        vec = memory.allocate(vec_size);
        
        size_type i = 0; 
        auto init_it = init.begin();
        while (i < vec_size) {
            memory.construct(vec + i, *init_it);
            ++i;
            ++init_it;
        }         
    }
    
    ~Vector() {
        for (size_type i = 0; i < vec_size; ++i)
            memory.destroy(vec + i);
            
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
        resize(vec_size + 1, value);
    }
    
    void push_back(value_type&& value) {
        reserve(vec_size + 1);
        memory.construct(vec + vec_size, std::forward<value_type>(value));
        vec_size++;
    }
    
    void pop_back() {
        if (not empty())
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
                memory.construct(vec + i, defaultValue);
        } else {
            for (size_type i = newSize; i < vec_size; ++i)
                memory.destroy(vec + i);
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
            for (size_type i = 0; i < vec_size; ++i)
                memory.construct(newData + i, vec[i]);
            for (size_type i = 0; i < vec_size; ++i)
                memory.destroy(vec + i);
                   
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
