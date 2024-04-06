#include <iostream>

template<typename T>
class Circular_buffer {
public:
    explicit Circular_buffer(size_t size = 0, T value = T()) : size_(size) {
        data = new T[size * 3];
        for (int i = 0; i < size * 3; ++i) {
            data[i] = value;
        }
        first = iterator(&data[size]);
        last = iterator(&data[size]);
    }

    ~Circular_buffer() {
        delete[] data;
    }

    class iterator : public std::iterator<std::random_access_iterator_tag, T> {
    public:
        explicit iterator(T *n) : node(n) {}

        ~iterator() = default;

        iterator operator++() {
            node++;
            return iterator(node);
        }

        iterator operator--() {
            node--;
            return iterator(node);
        }

        T &operator*() {
            return *node;
        }

        bool operator==(const iterator &itr) {
            return node == itr.node;
        }

        bool operator!=(const iterator &itr) {
            return this->node != itr.node;
        }

        void operator+=(int index) {
            node += index;
        }

        void operator-=(int index) {
            node -= index;
        }

        T &operator[](int index) {
            iterator it = *this;
            it += index;
            return *it;
        }

        iterator operator+(int index) {
            iterator it = *this;
            it += index;
            return it;
        }

        iterator operator-(int index) {
            iterator it = *this;
            it -= index;
            return it;
        }

        int operator-(const iterator &it) {
            return this->node - it.node;
        }

        bool operator<(const iterator &it) {
            return (*this - it) < 0;
        }

        bool operator>(const iterator &it) {
            return (*this - it) > 0;
        }

        bool operator>=(const iterator &it) {
            return !(*this < it);
        }

        bool operator<=(const iterator &it) {
            return !(this > it);
        }

    private:
        T *node;
    };


    [[nodiscard]] size_t size() const {
        return size_;
    }

    iterator begin() {
        return first;
    }

    iterator end() {
        return last;
    }

    T &operator[](int index) {
        return first[index];
    }

    void pop_front() {
        if (first != last) {
            ++first;
            --size_;
        }
    }

    void pop_back() {
        if (first != last) {
            --last;
            --size_;
        }
    }

    void push_back(T value) {
        if (last == iterator(&data[size_ * 3])) {
            T *temp = new T[size_ * 3];
            int i = 0;
            while (first != last) {
                temp[size_ + i] = *first;
                ++first;
                ++i;
            }
            delete[]data;
            data = temp;
            first = iterator(&data[size_]);
            last = iterator(&data[size_ + i]);
        }
        if (last - first == size_) {
            ++first;
        }
        *last = value;
        ++last;
    }

    void push_front(T value) {
        if (first == iterator(&data[0])) {
            T *temp = new T[size_ * 3];
            int i = 0;
            while (first != last) {
                temp[size_ + i] = *first;
                ++first;
                ++i;
            }
            delete[]data;
            data = temp;
            first = iterator(&data[size_]);
            last = iterator(&data[size_ + i]);
        }
        if (last - first == size_) {
            --last;
        }
        --first;
        *first = value;
    }

    void resize(size_t size) {
        T *temp = new T[size * 3];
        int i = 0;
        while (first != last and i < size) {
            temp[size + i] = *first;
            ++first;
            ++i;
        }
        delete[]data;
        data = temp;
        size_ = size;
        first = iterator(&data[size_]);
        last = iterator(&data[size_ + i]);
    }

private:
    iterator first = iterator(nullptr);
    iterator last = iterator(nullptr);;
    T *data;
    size_t size_;
};

int main() {
    Circular_buffer<int> ez(5);
    ez.push_back(3);
    ez.push_back(23);
    ez.push_back(33);
    ez.push_back(123);
    for (auto &i : ez) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
    ez.pop_front();
    ez.pop_back();
    ez.push_front(43);
    std::cout << ez.size() << '\n';
    for (auto &i : ez) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
    ez.resize(8);
    for (int i = 0; i < 7; ++i) {
        ez.push_front(2);
    }
    for (auto &i : ez) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
    std::cout << *std::min_element(ez.begin(), ez.end()) << '\n';
    std::cout << *std::find(ez.begin(), ez.end(), 43) << '\n';
    Circular_buffer<std::string> sss(3);
    sss.push_back("aaa");
    sss.push_back("bbb");
    sss.push_back("ccc");
    sss.pop_front();
    for (auto &i : sss) {
        std::cout << i << '\n';
    }
    return 0;
}
