#include <iostream>
#include <vector>

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

    friend std::istream &operator>>(std::istream &in, polynomial &pol);

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

std::istream &operator>>(std::istream &in, polynomial &pol) {
    in >> pol.deg_;
    pol.coeffs_.resize(pol.deg_ + 1);
    for (int i = 0; i < pol.deg_ + 1; i++) {
        in >> pol.coeffs_[i];
    }
    return in;
}

int main() {
    polynomial p({-4, -2, -1, 0, 34});
    polynomial d({4, 2, 1, 0, -34});
    polynomial s;
    s = p;
    std::cout << s << '\n';
    std::cout << (p == d) << '\n';
    std::cout << (p != d) << '\n';
    std::cout << p + 3 << '\n';
    std::cout << p + d << '\n';
    std::cout << -p << '\n';
    std::cout << p - d << '\n';
    p += s;
    std::cout << p << '\n';
    std::cout << s << '\n';
    p += 3;
    std::cout << p << '\n';
    p -= d;
    std::cout << p / 3 << '\n';
    polynomial s1({1, 1});
    std::cout << p * s1 << '\n';
    p *= s1;

    std::cout << p << '\n';
    p /= 3;
    std::cout << p << '\n';

    std::cin >> s;
    std::cout << s << '\n';
    std::cout << p[0] << '\n';
    polynomial a = p - d;
    polynomial b = d - p;
    std::cout << p << '\n';
    std::cout << d << '\n';
    std::cout << a << '\n';
    std::cout << b << '\n';
    return 0;
}
