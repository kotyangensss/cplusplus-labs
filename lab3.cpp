#include <iostream>
#include <vector>
#include <cmath>
#include "pugixml-1.11/src/pugixml.hpp"

class Coordinates {
public:
    explicit Coordinates(float x, float y) :
            x_(x), y_(y) {}

    bool operator<(const Coordinates &other) const {
        return (x_ * x_ + y_ * y_ < other.x_ * other.x_ + other.y_ * other.y_);
    }

    double x() const {
        return x_;
    }

    double y() const {
        return y_;
    }

private:
    double x_;
    double y_;
};

class Street {
public:
    explicit Street(std::string name) {
        int s = name.find("улица ");
        if (s != std::string::npos) {
            name.erase(s, 11);
            name.append(" ул.");
        } else {
            s = name.find(" улица");
            if (s != std::string::npos) {
                name.erase(s, 11);
                name.append(" ул.");
            } else if (name.find("ул.") == 0) {
                name.erase(0, 6);
                name.append(" ул.");
            } else {
                s = name.find("проспект ");
                if (s != std::string::npos) {
                    name.erase(s, 17);
                    name.append(" пр.");
                } else {
                    s = name.find(" проспект");
                    if (s != std::string::npos) {
                        name.erase(s, 17);
                        name.append(" пр.");
                    } else if (name.find("пр.") == 0) {
                        name.erase(0, 6);
                        name.append(" пр.");
                    } else {
                        s = name.find("шоссе ");
                        if (s != std::string::npos) {
                            name.erase(s, 11);
                            name.append(" ш.");
                        } else {
                            s = name.find(" шоссе");
                            if (s != std::string::npos) {
                                name.erase(s, 11);
                                name.append(" ш.");
                            } else if (name.find("ш.") == 0) {
                                name.erase(0, 4);
                                name.append(" ш.");
                            }
                        }
                    }
                }
            }
        }
        name_ = name;
    }

    void operator++() {
        ++stops_;
    }

    std::string name() const {
        return name_;
    }

    int stops() const {
        return stops_;
    }

private:
    std::string name_;
    int stops_ = 1;
};

class Route {
public:
    explicit Route(std::string n) :
            number(std::move(n)) {}

    void add_stop(const Coordinates &st) {
        stops_.push_back(st);
    }

    std::string name() const {
        return number;
    }

    int stops() const {
        return stops_.size();
    }

    double len() const {
        double sum = 0;
        for (int i = 0; i < stops_.size() - 1; i++) {
            double lat1 = stops_[i].x() * M_PI / 180;
            double lat2 = stops_[i + 1].x() * M_PI / 180;
            double lon1 = stops_[i].y() * M_PI / 180;
            double lon2 = stops_[i + 1].y() * M_PI / 180;
            sum += 2 * 6371 *
                   asin(sqrt(pow(sin((lat2 - lat1) / 2), 2) + cos(lat1) * cos(lat2) * pow(sin((lon2 - lon1) / 2), 2)));
        }
        return sum;
    }

    void Sort() {
        std::sort(stops_.begin(), stops_.end());
    }

private:
    std::string number;
    std::vector<Coordinates> stops_;
};

template<class T>
int Find(const T &st, const std::vector<T> &streets) {
    for (int i = 0; i < streets.size(); i++) {
        if (streets[i].name() == st.name()) {
            return i;
        }
    }
    return -1;
}

template<class T>
std::string Max(const std::vector<T> &streets) {
    T m = streets[0];
    for (int i = 1; i < streets.size(); i++) {
        if (streets[i].stops() > m.stops()) {
            m = streets[i];
        }
    }
    return m.name();
}

std::string Longest(const std::vector<Route> &r) {
    int nmax = 0;
    double max = r[0].len();
    for (int i = 1; i < r.size(); i++) {
        double t = r[i].len();
        if (t > max) {
            max = t;
            nmax = i;
        }
    }
    return r[nmax].name();
}

void add_street(const std::string &loc, std::vector<Street> &streets) {
    int finder = Find<Street>(Street(loc), streets);
    if (finder == -1) {
        streets.emplace_back(loc);
    } else {
        ++streets[finder];
    }
}

void add_rout(const std::string &rout, const Coordinates &coor, const std::string &type, std::vector<Route> &routes) {
    int finder = Find<Route>(Route(rout), routes);
    if (finder == -1) {
        routes.emplace_back(rout);
        routes[routes.size() - 1].add_stop(coor);
    } else {
        routes[finder].add_stop(coor);
    }
}

std::vector<std::string> route_pars(const std::string &buf) {
    std::vector<std::string> routes;
    int prev = 0;
    for (int i = 0; i < buf.size(); i++) {
        if (buf[i] == '.' or buf[i] == ',') {
            routes.push_back(buf.substr(prev, i - prev));
            prev = i + 1;
        }
    }
    routes.push_back(buf.substr(prev, buf.size() - prev));
    return routes;
}

int main() {
    std::vector<Street> streets;
    std::vector<Route> bus;
    std::vector<Route> tram;
    std::vector<Route> trolleybus;
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("data.xml");
    if (!result) {
        std::cout << "Error description: " << result.description() << "\n";
        return 0;
    }
    for (pugi::xml_node tool: doc.first_child()) {
        if (static_cast<std::string>(tool.child("object_type").child_value()) == "Остановка") {
            std::string loc = tool.child("location").child_value();
            if (!loc.empty()) {
                std::string loc1;
                if (loc.find(',') != std::string::npos) {
                    loc1 = loc.substr(loc.find(',') + 2, loc.size() - (loc.find(',') + 2));
                    loc.erase(loc.find(','), loc.size());
                }
                add_street(loc, streets);
                if (!loc1.empty()) {
                    add_street(loc1, streets);
                }
            }
            std::string buf = tool.child("routes").child_value();
            std::string coor = tool.child("coordinates").child_value();
            int comma = coor.find(',');
            Coordinates xy(std::stof(coor.substr(0, comma)), std::stof(coor.substr(comma + 1, coor.size() - comma)));
            std::vector<std::string> Troutes = route_pars(buf);
            for (auto &i : Troutes) {
                if (static_cast<std::string>(tool.child("type_of_vehicle").child_value()) == "Автобус")
                    add_rout(i, xy, tool.child("type_of_vehicle").child_value(), bus);
                else if (static_cast<std::string>(tool.child("type_of_vehicle").child_value()) == "Трамвай")
                    add_rout(i, xy, tool.child("type_of_vehicle").child_value(), tram);
                else if (static_cast<std::string>(tool.child("type_of_vehicle").child_value()) == "Троллейбус")
                    add_rout(i, xy, tool.child("type_of_vehicle").child_value(), trolleybus);
            }
        }
    }
    for (auto &i:bus) {
        i.Sort();
    }
    for (auto &i:trolleybus) {
        i.Sort();
    }
    for (auto &i:tram) {
        i.Sort();
    }
    std::cout << "Автобус с наибольшим числом остановок - " << Max<Route>(bus) << '\n';
    std::cout << "Трамвай с наибольшим числом остановок - " << Max<Route>(tram) << '\n';
    std::cout << "Троллейбус с наибольшим числом остановок - " << Max<Route>(trolleybus) << '\n';
    std::cout << "Автобус с самым длинным маршрутом - " << Longest(bus) << '\n';
    std::cout << "Трамвай с самым длинным маршрутом - " << Longest(tram) << '\n';
    std::cout << "Троллейбус с самым длинным маршрутом - " << Longest(trolleybus) << '\n';
    std::cout << "Улица с наибольшим числом остановок - " << Max<Street>(streets) << '\n';
    return 0;
}
