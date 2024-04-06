#include <iostream>
#include <vector>
#include <cmath>

class dot {
public:
    explicit dot(float x = 0, float y = 0)
            : x_(x), y_(y) {
    }

    dot(const dot &other)
            : x_(other.x_), y_(other.y_) {
    }

    ~dot() = default;

    float x() const {
        return x_;
    }

    float y() const {
        return y_;
    }

    dot &operator=(const dot &other) {
        if (&other == this) {
            return *this;
        }
        x_ = other.x_;
        y_ = other.y_;
        return *this;
    }

    bool operator==(const dot &other) const {
        if (other.x() == x_ and other.y() == y_) {
            return true;
        } else {
            return false;
        }
    }

private:
    float x_;
    float y_;
};

class chain {
public:
    explicit chain(std::vector<dot> dots = {}) {
        for (const auto &dot : dots) {
            dots_.push_back(dot);
        }
    }

    chain(const chain &other) {
        for (int i = 0; i < other.num(); i++) {
            dots_.push_back(other.dots_[i]);
        }
    }

    ~chain() = default;

    virtual void type() {
        std::cout << "chain\n";
    }

    chain &operator=(const chain &other) {
        if (&other == this) {
            return *this;
        } else {
            dots_.clear();
            for (int i = 0; i < other.num(); i++) {
                dots_.push_back(other.dots_[i]);
            }
        }
    }

    int num() const {
        return dots_.size();
    }

    void getCoors() {
        for (int i = 0; i < dots_.size(); i++) {
            std::cout << "x:" << dots_[i].x() << " y:" << dots_[i].y() << '\n';
        }
    }

    float perimeter() const {
        float p = 0;
        for (int i = 1; i < dots_.size(); i++) {
            p += sqrt(pow((dots_[i].x() - dots_[i - 1].x()), 2) + pow((dots_[i].y() - dots_[i - 1].y()), 2));
        }
        return p;
    };


protected:
    std::vector<dot> dots_;
};

class closed_chain : public chain {
public:
    explicit closed_chain(std::vector<dot> dots) : chain(dots) {}

    void type() {
        std::cout << "closed_chain\n";
    }


};

class polygon : public chain {
public:
    polygon() {
        dots_ = casual();
    }

    explicit polygon(std::vector<dot> dots) : chain(dots) {
        if (dots_.size() < 3) {
            std::cerr << "polygon: not enough vertex\n";
            dots_ = casual();
        } else {
            for (int i = 0; i < dots_.size() - 1; i++) {
                for (int j = i + 1; j < dots_.size(); j++) {
                    if (dots_[i] == dots_[j]) {
                        std::cerr << "polygon: repeated vertex\n";
                        dots_ = casual();
                        break;
                    }
                    if (j != i + 1 and i != (j + 1) % dots_.size() and
                        crossing(dots_[i], dots_[i + 1], dots_[j], dots_[(j + 1) % dots_.size()])) {
                        std::cerr << "polygon: crossed lines\n";
                        dots_ = casual();
                        break;
                    }
                }
            }
        }

    }

    void type() {
        std::cout << "polygon\n";
    }

    float square() {
        float s;
        for (int i = 0; i < dots_.size(); i++) {
            s += dots_[i].x() * dots_[(i + 1) % dots_.size()].y();
            s -= dots_[(i + 1) % dots_.size()].x() * dots_[i].y();
        }
        return 0.5 * std::abs(s);
    }

protected:
    virtual std::vector<dot> casual() {
        dot a(0, 0);
        dot b(0.5, sqrt(3) / 2);
        dot c(1, 0);
        return {a, b, c};
    }

    bool crossing(dot a, dot b, dot c, dot d) {
        float x, y;
        float ax = b.x() - a.x();
        float ay = b.y() - a.y();
        float cx = d.x() - c.x();
        float cy = d.y() - c.y();
        if (cy * ax - cx * ay == 0) {
            return false;
        } else if (cx == 0) {
            x = c.x();
            y = a.y() + ay * (c.x() - a.x()) / ax;
        } else {
            x = a.x() + (a.y() - c.y() - (a.x() - c.x()) * cy / cx) * ax / (cy * ax / cx - ay);
            y = a.y() + (a.y() - c.y() - (a.x() - c.x()) * cy / cx) * ay / (cy * ax / cx - ay);
        }
        if (std::abs(x - a.x()) + std::abs(x - b.x()) == std::abs(b.x() - a.x()) and
            std::abs(y - a.y()) + std::abs(y - b.y()) == std::abs(b.y() - a.y())) {
            return true;
        } else {
            return false;
        }
    }
};

class triangle : public polygon {
public:
    triangle() : polygon() {

    }

    explicit triangle(std::vector<dot> dots) : polygon(dots) {
        if (dots_.size() != 3) {
            std::cerr << "triangle: wrong number of vertex\n";
            dots_ = casual();
        }
    }

    void type() {
        std::cout << "triangle\n";
    }
};

class trapezoid : public polygon {

    std::vector<dot> casual() {
        std::vector<dot> temp;
        temp.push_back(dot(0, 0));
        temp.push_back(dot(1, 1));
        temp.push_back(dot(2, 1));
        temp.push_back(dot(0, 3));
        return temp;
    }

public:
    trapezoid() {
        dots_ = casual();
    }

    explicit trapezoid(std::vector<dot> dots) : polygon(dots) {
        if (dots.size() != 4) {
            std::cerr << "trapezoid: wrong number of vertex\n";
            dots_ = casual();
        } else {
            float ax = dots_[1].x() - dots_[0].x();
            float ay = dots_[1].y() - dots_[0].y();
            float bx = dots_[2].x() - dots_[1].x();
            float by = dots_[2].y() - dots_[1].y();
            float cx = dots_[3].x() - dots_[2].x();
            float cy = dots_[3].y() - dots_[2].y();
            float dx = dots_[0].x() - dots_[3].x();
            float dy = dots_[0].y() - dots_[3].y();
            if (!((cy * ax - cx * ay == 0 and by * dx - bx * dy != 0) or
                  (cy * ax - cx * ay != 0 and by * dx - bx * dy == 0))) {
                std::cerr << "trapezoid: it is not a trapezoid\n";
                dots_.clear();
                dots_ = casual();
            }
        }
    }

    void type() {
        std::cout << "trapezoid\n";
    }
};

class regular_polygon : public polygon {
public:
    regular_polygon() : polygon() {

    }

    explicit regular_polygon(std::vector<dot> dots) : polygon(dots) {
        float section = sqrt(pow((dots_[1].x() - dots_[0].x()), 2) + pow((dots_[1].y() - dots_[0].y()), 2));
        float chord = sqrt(pow((dots_[2].x() - dots_[0].x()), 2) + pow((dots_[2].y() - dots_[0].y()), 2));
        for (int i = 1; i < dots_.size(); i++) {
            float side = sqrt(pow((dots_[i].x() - dots_[i - 1].x()), 2) + pow((dots_[i].y() - dots_[i - 1].y()), 2));
            float third = sqrt(pow((dots_[(i + 2) % dots_.size()].x() - dots_[i].x()), 2) +
                               pow((dots_[(i + 2) % dots_.size()].y() - dots_[i].y()), 2));
            if (side != section or third != chord) {
                std::cerr << "regular_polygon: different length of sides\n";
                dots_ = casual();
                break;
            }
        }
    }

    void type() {
        std::cout << "regular_polygon\n";
    }
};

int main() {
    dot a;
    dot b(1, 2);
    dot c(2, 2);
    dot d(3, 0);
    dot e(0, 2);
    dot f(2, 0);
    std::cout << d.x() << ' ' << d.y() << '\n';
    chain *chs[6];
    chs[0] = new chain({a, b});
    chs[1] = new closed_chain({a, d, c, b});
    chs[2] = new polygon({a, b, c, d});
    chs[3] = new triangle({a, b, c, d});
    chs[4] = new trapezoid({a, b, c, d});
    chs[5] = new regular_polygon({a, e, c, f});
    for (auto &ch : chs) {
        ch->type();
        ch->getCoors();
    }
    std::cout << chs[0]->perimeter() << '\n';
    trapezoid asd({a, b, c, d});
    std::cout << asd.square();
    return 0;
}
