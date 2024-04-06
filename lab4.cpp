#include <iostream>
#include <vector>

///CLASSES
class CRational {
public:
    explicit CRational(int num = 0, unsigned denom = 1)
            : numerator_(num), denominator_(denom) {}

    int numerator() const {
        return numerator_;
    }

    unsigned denominator() const {
        return denominator_;
    }

    friend std::ostream &operator<<(std::ostream &stream, const CRational &value);

    CRational &operator=(const CRational &other) {
        if (&other == this) {
            return *this;
        }
        numerator_ = other.numerator_;
        denominator_ = other.denominator_;
        return *this;
    }

    CRational &operator++() {
        numerator_ += denominator_;
        return *this;
    }

    const CRational operator++(int) {
        CRational tmp(*this);
        operator++();
        return tmp;
    }

private:
    int numerator_;
    unsigned denominator_;
};

bool operator==(const CRational &lhs, const CRational &rhs) {
    return lhs.numerator() * rhs.denominator() ==
           rhs.numerator() * lhs.denominator();

}

bool operator!=(const CRational &lhs, const CRational &rhs) {
    return !operator==(lhs, rhs);
}

bool operator<(const CRational &lhs, const CRational &rhs) {
    return lhs.numerator() * rhs.denominator() < rhs.numerator() *
                                                 lhs.denominator();
}

bool operator>(const CRational &lhs, const CRational &rhs) {
    return (rhs < lhs);
}

bool operator<=(const CRational &lhs, const CRational &rhs) {
    return !operator>(lhs, rhs);
}

bool operator>=(const CRational &lhs, const CRational &rhs) {
    return !operator<(lhs, rhs);
}

std::ostream &operator<<(std::ostream &stream, const CRational &value) {
    stream << value.numerator() << '/' << value.denominator();
    return stream;
}

class polynomial {
public:
    explicit polynomial(int n = 0, std::vector<float> coeffs = {0}) :
            deg_(n) {
        int zeros = 0;
        while (coeffs[zeros] == 0 and zeros < coeffs.size() - 1) {
            zeros++;
        }
        for (int i = zeros; i < coeffs.size(); i++) {
            coeffs_.push_back(coeffs[i]);
        }
    }

    explicit polynomial(std::vector<float> coeffs) {
        int zeros = 0;
        while (coeffs[zeros] == 0 and zeros < coeffs.size() - 1) {
            zeros++;
        }
        for (int i = zeros; i < coeffs.size(); i++) {
            coeffs_.push_back(coeffs[i]);
        }
        deg_ = coeffs_.size() - 1;
    }

    polynomial(const polynomial &other) :
            deg_(other.deg_) {
        for (float coeff : other.coeffs_) {
            coeffs_.push_back(coeff);
        }
    }

    ~polynomial() = default;

    friend std::ostream &operator<<(std::ostream &os, const polynomial &pol);

    int deg() const {
        return deg_;
    }

    polynomial &operator=(const polynomial &other) {
        if (&other == this)
            return *this;
        deg_ = other.deg_;
        coeffs_.clear();
        for (float coeff : other.coeffs_) {
            coeffs_.push_back(coeff);
        }
        return *this;
    }

    bool operator==(const polynomial &other) {
        if (deg_ != other.deg_) {
            return false;
        } else {
            for (int i = 0; i < deg_ + 1; i++) {
                if (coeffs_[i] != other.coeffs_[i]) {
                    return false;
                }
            }
            return true;
        }
    }

    bool operator!=(const polynomial &other) {
        return !(*this == other);
    }

    polynomial operator+(const polynomial &other) {
        polynomial ress = *this;
        ress += other;
        return ress;
    }

    polynomial operator+(float a) {
        polynomial result = *this;
        result.coeffs_[deg_] += a;
        return result;
    }

    void operator+=(const polynomial &other) {
        std::vector<float> res;
        if (deg_ <= other.deg_) {
            unsigned int t = other.deg_ - deg_;
            for (int i = 0; i < t; i++) {
                res.push_back(other.coeffs_[i]);
            }
            for (int i = 0; i < coeffs_.size(); i++) {
                res.push_back(coeffs_[i] + other.coeffs_[i + t]);
            }
        } else {
            unsigned int t = deg_ - other.deg_;
            for (int i = 0; i < t; i++) {
                res.push_back(coeffs_[i]);
            }
            for (int i = 0; i < other.coeffs_.size(); i++) {
                res.push_back(other.coeffs_[i] + coeffs_[i + t]);
            }
        }
        polynomial result(res);
        *this = result;
    }

    void operator+=(float a) {
        coeffs_[deg_] += a;
    }


    polynomial operator-(const polynomial &other) {
        polynomial ress = *this;
        ress -= other;
        return ress;
    }

    polynomial operator-() {
        polynomial result = *this;
        for (float &coeff : result.coeffs_) {
            coeff *= -1;
        }
        return result;
    }

    void operator-=(const polynomial &other) {
        std::vector<float> res;
        if (deg_ <= other.deg_) {
            unsigned int t = other.deg_ - deg_;
            for (int i = 0; i < t; i++) {
                res.push_back(-other.coeffs_[i]);
            }
            for (int i = 0; i < coeffs_.size(); i++) {
                res.push_back(coeffs_[i] - other.coeffs_[i + t]);
            }
        } else {
            unsigned int t = deg_ - other.deg_;
            for (int i = 0; i < t; i++) {
                res.push_back(coeffs_[i]);
            }
            for (int i = 0; i < other.coeffs_.size(); i++) {
                res.push_back(coeffs_[i + t] - other.coeffs_[i]);
            }
        }
        polynomial result(res);
        *this = result;
    }

    void operator-=(float a) {
        coeffs_[deg_] -= a;
    }

    polynomial operator*(const polynomial &other) {
        polynomial ress = *this;
        ress *= other;
        return ress;
    }

    void operator*=(const polynomial &other) {
        std::vector<float> res(deg_ + other.deg_ + 1);
        for (int i = 0; i < coeffs_.size(); i++) {
            for (int j = 0; j < other.coeffs_.size(); j++) {
                res[i + j] += coeffs_[i] * other.coeffs_[j];
            }
        }
        polynomial result(res);
        *this = result;
    }

    polynomial operator*(float a) const {
        polynomial result;
        if (a != 0) {
            result = *this;
            for (float &coeff:result.coeffs_) {
                coeff *= a;
            }
        }
        return result;
    }

    bool operator<(polynomial a) const {
        if (this->deg_ > a.deg_) {
            return false;
        } else if (this->deg_ < a.deg_) {
            return true;
        } else {
            int i = 0;
            while (this->coeffs_[i] == a.coeffs_[i] and i <= this->deg_) {
                ++i;
            }
            return this->coeffs_[i] < a.coeffs_[i];
        }
    }

    bool operator>(polynomial a) const {
        if (this->deg_ < a.deg_) {
            return false;
        } else if (this->deg_ > a.deg_) {
            return true;
        } else {
            int i = 0;
            while (this->coeffs_[i] == a.coeffs_[i] and i <= this->deg_) {
                ++i;
            }
            return this->coeffs_[i] > a.coeffs_[i];
        }
    }

    bool operator>=(const polynomial &a) const {
        return !(*this < a);
    }

    bool operator<=(const polynomial &a) const {
        return !(*this > a);
    }

    polynomial operator/(float a) {
        polynomial ress = *this;
        ress /= a;
        return ress;
    }

    void operator/=(float a) {
        if (a == 0) {
            std::cerr << "divided by zero";
        } else {
            for (float &coeff:coeffs_) {
                coeff /= a;
            }
        }
    }

    float operator[](int n) {
        return coeffs_[coeffs_.size() - n - 1];
    }

private:
    unsigned int deg_;
    std::vector<float> coeffs_;
};

std::ostream &operator<<(std::ostream &os, const polynomial &pol) {
    if (pol.deg_ == 0) {
        os << pol.coeffs_[0] << ' ';
    } else {
        os << pol.coeffs_[0] << "x^" << pol.deg_ << ' ';
        for (int i = 1; i < pol.coeffs_.size() - 1; i++) {
            if (pol.coeffs_[i] > 0) {
                os << '+' << pol.coeffs_[i] << "x^" << pol.deg_ - i << ' ';
            } else if (pol.coeffs_[i] < 0) {
                os << pol.coeffs_[i] << "x^" << pol.deg_ - i << ' ';
            }
        }
        if (pol.coeffs_[pol.deg_] > 0) {
            os << '+' << pol.coeffs_[pol.deg_] << ' ';
        } else if (pol.coeffs_[pol.deg_] < 0) {
            os << pol.coeffs_[pol.deg_] << ' ';
        }
    }
    return os;
}

///TEMPLATES
template<typename InputIterator, typename Predicate>
bool All_of(InputIterator first, InputIterator last, Predicate p) {
    if (first == last) {
        return false;
    }
    while (first != last) {
        if (!p(*first)) {
            return false;
        }
        ++first;
    }
    return true;
}

template<typename InputIterator, typename Predicate>
bool Any_of(InputIterator first, InputIterator last, Predicate p) {
    while (first != last) {
        if (p(*first)) {
            return true;
        }
        ++first;
    }
    return false;
}

template<typename InputIterator, typename Predicate>
bool None_of(InputIterator first, InputIterator last, Predicate p) {
    return !Any_of(first, last, p);
}

template<typename InputIterator, typename Predicate>
bool One_of(InputIterator first, InputIterator last, Predicate p) {
    int counter = 0;
    while (first != last) {
        if (p(*first)) {
            ++counter;
        }
        if (counter > 1) {
            return false;
        }
        ++first;
    }
    if (counter == 1) {
        return true;
    }
    return false;
}

template<typename ForwardIterator, class Compare>
bool Is_sorted(ForwardIterator first, ForwardIterator last, Compare comp) {
    if (first == last) {
        return true;
    }
    ForwardIterator prev = first;
    ++first;
    while (first != last) {
        if (!comp(*prev, *first)) {
            return false;
        }
        prev = first;
        ++first;
    }
    return true;
}

template<typename ForwardIterator>
bool Is_sorted(ForwardIterator first, ForwardIterator last) {
    return Is_sorted(first, last, std::less<>());
}

template<class InputIt, class Predicate>
bool Is_partitioned(InputIt first, InputIt last, Predicate p) {
    while (first != last) {
        if (!p(*first)) {
            break;
        }
        ++first;
    }
    while (first != last) {
        if (p(*first)) {
            return false;
        }
        ++first;
    }
    return true;
}

template<typename InputIterator, typename T>
InputIterator Find_not(InputIterator first, InputIterator last, const T &value) {
    while (first != last) {
        if (*first != value) {
            return first;
        }
        ++first;
    }
    return last;
}

template<typename InputIterator, typename T>
InputIterator Find_backward(InputIterator first, InputIterator last, const T &value) {
    InputIterator temp = last;
    --last;
    --first;
    while (last != first) {
        if (*last == value) {
            return last;
        }
        --last;
    }
    return temp;
}

template<typename Bidirectional, class Predicate>
bool Is_palindrome(Bidirectional first, Bidirectional last, Predicate p) {
    if (first == last) {
        return true;
    }
    while (first != last) {
        --last;
        if (first == last) {
            return true;
        }
        if (!p(*first, *last)) {
            return false;
        }
        ++first;
    }
    return true;
}

template<typename Bidirectional>
bool Is_palindrome(Bidirectional first, Bidirectional last) {
    return Is_palindrome(first, last, std::equal_to<>());
}

int main() {
    std::vector<int> aaa = {1, 2, 4, 1};
    std::vector<std::string> qwe = {"aaa", "bb", "cc"};
    std::vector<CRational> asd = {CRational(1, 5), CRational(2, 3), CRational(4, 4)};
    std::vector<polynomial> zxc = {polynomial({-4, -2, -1, 0, 34}), polynomial({1, 0, 6}),
                                   polynomial({1, 1, 1, 1, 1})};

    std::cout << "All_of:\n";
    std::cout << All_of(qwe.begin(), qwe.end(), [](const std::string &i) { return i.size() == 3; }) << '\n';
    std::cout << All_of(asd.begin(), asd.end(), [](CRational i) { return i < CRational(1); }) << '\n';
    std::cout << All_of(zxc.begin(), zxc.end(), [](polynomial i) { return i != polynomial({1, 0, 66}); }) << '\n';
    std::cout << '\n';

    std::cout << "Any_of:\n";
    std::cout << Any_of(qwe.begin(), qwe.end(), [](const std::string &i) { return i.size() == 3; }) << '\n';
    std::cout << Any_of(asd.begin(), asd.end(), [](CRational i) { return i < CRational(1); }) << '\n';
    std::cout << Any_of(zxc.begin(), zxc.end(), [](polynomial i) { return i == polynomial({1, 0, 66}); }) << '\n';
    std::cout << '\n';

    std::cout << "None_of:\n";
    std::cout << None_of(qwe.begin(), qwe.end(), [](const std::string &i) { return i.size() == 2; }) << '\n';
    std::cout << None_of(asd.begin(), asd.end(), [](CRational i) { return i > CRational(1); }) << '\n';
    std::cout << None_of(zxc.begin(), zxc.end(), [](polynomial i) { return i != polynomial({1, 0, 66}); }) << '\n';
    std::cout << '\n';

    std::cout << "One_of:\n";
    std::cout << One_of(qwe.begin(), qwe.end(), [](const std::string &i) { return i.size() == 2; }) << '\n';
    std::cout << One_of(asd.begin(), asd.end(), [](CRational i) { return i == CRational(1); }) << '\n';
    std::cout << One_of(zxc.begin(), zxc.end(), [](polynomial i) { return i != polynomial({1, 0, 6}); }) << '\n';
    std::cout << '\n';

    std::cout << "Is_sorted:\n";
    std::cout << Is_sorted(aaa.begin(), aaa.end()) << '\n';
    std::cout << Is_sorted(qwe.begin(), qwe.end()) << '\n';
    std::cout << Is_sorted(asd.begin(), asd.end()) << '\n';
    std::cout << Is_sorted(zxc.begin(), zxc.end()) << '\n';
    std::cout << '\n';

    std::cout << "Is_partitioned:\n";
    std::cout << Is_partitioned(aaa.begin(), aaa.end(), [](int i) { return i < 3; }) << '\n';
    std::cout << Is_partitioned(qwe.begin(), qwe.end(), [](const std::string &i) { return i.size() < 3; }) << '\n';
    std::cout << Is_partitioned(asd.begin(), asd.end(), [](CRational i) { return i >= CRational(1); }) << '\n';
    std::cout << Is_partitioned(zxc.begin(), zxc.end(), [](const polynomial &i) { return i < polynomial({1, 1, 1}); })
              << '\n';
    std::cout << '\n';

    std::cout << "Find_not:\n";
    if (Find_not(qwe.begin(), qwe.end(), "aaa") != qwe.end()) {
        std::cout << *Find_not(qwe.begin(), qwe.end(), "aaa") << '\n';
    } else {
        std::cout << "No such element\n";
    }
    if (Find_not(asd.begin(), asd.end(), CRational(2, 4)) != asd.end()) {
        std::cout << *Find_not(asd.begin(), asd.end(), CRational(2, 4)) << '\n';
    } else {
        std::cout << "No such element\n";
    }
    if (Find_not(zxc.begin(), zxc.end(), polynomial({1, 0, 6})) != zxc.end()) {
        std::cout << *Find_not(zxc.begin(), zxc.end(), polynomial({1, 0, 6})) << '\n';
    } else {
        std::cout << "No such element\n";
    }
    std::cout << '\n';

    std::cout << "Find_backward:\n";
    if (Find_backward(qwe.begin(), qwe.end(), "aaa") != qwe.end()) {
        std::cout << *Find_backward(qwe.begin(), qwe.end(), "aaa") << '\n';
    } else {
        std::cout << "No such element\n";
    }
    if (Find_backward(asd.begin(), asd.end(), CRational(2, 4)) != asd.end()) {
        std::cout << *Find_backward(asd.begin(), asd.end(), CRational(2, 4)) << '\n';
    } else {
        std::cout << "No such element\n";
    }
    if (Find_backward(zxc.begin(), zxc.end(), polynomial({1, 0, 66})) != zxc.end()) {
        std::cout << *Find_backward(zxc.begin(), zxc.end(), polynomial({1, 0, 66})) << '\n';
    } else {
        std::cout << "No such element\n";
    }
    std::cout << '\n';

    std::cout << "Is_palindrome:\n";
    std::cout << Is_palindrome(aaa.begin(), aaa.end()) << '\n';
    std::cout << Is_palindrome(qwe.begin(), qwe.end(),
                               [](const std::string &i, const std::string &j) { return i.size() == j.size(); }) << '\n';
    std::cout << Is_palindrome(asd.begin(), asd.end()) << '\n';
    std::cout << Is_palindrome(zxc.begin(), zxc.end(),
                               [](polynomial i, const polynomial &j) { return i.deg() == j.deg(); })
              << '\n';
    std::cout << '\n';
    return 0;
}