#pragma once

template <typename T>
class Vector {

private:
    size_t m_size;

    size_t m_capacity;

    T* m_buffer;

private:
    void reallocate_capacity(size_t new_capacity) {

        T* new_buffer = (T*)::operator new(new_capacity * sizeof(T));
        size_t old_size = m_size;

        if (new_capacity < m_size) {
            m_size = new_capacity;
        }

        for (size_t i = 0; i < m_size; i++) {
            new(&new_buffer[i]) T(std::move(m_buffer[i]));
        }

        for (size_t i = 0; i < old_size; i++) {
            m_buffer[i].~T();
        }

        ::operator delete(m_buffer, m_capacity * sizeof(T));
        m_capacity = new_capacity;
        m_buffer = new_buffer;
    }

public:

    Vector(): m_size(0), m_capacity(0), m_buffer(nullptr) {
    }

    Vector(const Vector& other): m_size(other.m_size), m_capacity(other.m_capacity) {

        m_buffer = (T*)::operator new(m_capacity * sizeof(T));

        for (size_t i = 0; i < m_size; i++) {
            new(&m_buffer[i]) T(other.m_buffer[i]);
        }
    }

    Vector<T>& operator=(const Vector<T>& other) {

        if (this != &other) {
            for (size_t i = 0; i < m_size; i++) {
                m_buffer[i].~T();
            }
            ::operator delete(m_buffer, m_capacity * sizeof(T));

            m_size = other.m_size;
            m_capacity = other.m_capacity;
            m_buffer = (T*)::operator new(m_capacity * sizeof(T));

            for (size_t i = 0; i < m_size; i++) {
                new(&m_buffer[i]) T(other.m_buffer[i]);
            }
        }

        return *this;
    }

    Vector(Vector&& other) noexcept {

        if (this != &other) {
            for (size_t i = 0; i < m_size; i++) {
                m_buffer[i].~T();
            }
            ::operator delete(m_buffer, m_capacity * sizeof(T));
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            m_buffer = other.m_buffer;

            other.m_size = 0;
            other.m_capacity = 0;
            other.m_buffer = nullptr;
        }
    }

    Vector& operator=(Vector&& other) noexcept {

        if (this != &other) {
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            m_buffer = other.m_buffer;

            other.m_size = 0;
            other.m_capacity = 0;
            other.m_buffer = nullptr;
        }

        return *this;
    }

    void push_back(const T& new_element) {
        if (m_size >= m_capacity) {
            reallocate_capacity(m_capacity + 0.5 * m_capacity + 1);
        }
        m_buffer[m_size++] = new_element;
    }

    void push_back(const T&& new_element) {
        if (m_size >= m_capacity) {
            reallocate_capacity(m_capacity + 0.5 * m_capacity + 1);
        }
        m_buffer[m_size++] = std::move(new_element);
    }

    template<typename... Args>
    void emplace_back(Args&&... args) {
        if (m_size >= m_capacity) {
            reallocate_capacity(m_capacity + 0.5 * m_capacity + 1);
        }
        new(&m_buffer[m_size]) T(std::forward<Args>(args)...);
        m_size++;
    }

    void pop_back() {
        if (m_size > 0) {
            m_buffer[--m_size].~T();
        }
    }

    void clear() noexcept {

        for (size_t i = 0; i < m_size; i++) {
            m_buffer[i].~T();
        }
        m_size = 0;
    }

    bool empty() {
        return m_size == 0;
    }

    size_t size() {
        return m_size;
    }

    T& back() {
        return m_buffer[m_size - 1];
    }

    T& operator[](size_t index) {
        return m_buffer[index];
    }

    const T& operator[](size_t index) const {
        return m_buffer[index];
    }

    ~Vector() {
        clear();
        ::operator delete(m_buffer, m_capacity * sizeof(T));
    }
};
