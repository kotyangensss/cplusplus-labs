#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <GL/freeglut.h>

int WIDTH = 1000.0, HEIGHT = 800.0, DEGX = 0.0, DEGY = 0.0;

class Rubik {
public:

    Rubik() {
        this->generate();
    }

    explicit Rubik(std::string name) {
        std::ifstream s;
        s.open(name);
        if (!s.is_open()) {
            std::cerr << "Cannot open the file!\n";
            this->generate();
        } else {
            for (auto &i : cube) {
                for (auto &j : i) {
                    for (char &k : j) {
                        s >> k;
                    }
                }
            }
            if (!this->check()) {
                this->generate();
            }
        }
        s.close();
    }

    friend void Keyboard(unsigned char a, int x, int y);

    friend void draw(int size);

    friend void draw_side(int k, int size);

    void generate() {
        for (int i = 0; i < 6; ++i) {
            for (auto &j : cube[i]) {
                for (char &k : j) {
                    k = choose_color(i);
                }
            }
        }
        srand(time(nullptr));
        int t = rand() % 1000;
        for (int i = 0; i < t; ++i) {
            front_right();
            swap_side(rand() % 6);
            front_left();
            swap_side(rand() % 6);
        }
        swap_side(0);
        while (cube[up][1][1] != 'b') {
            swap_side(bottom);
            swap_side(right);
            swap_side(up);
        }
    }

    void solve() {
        std::ofstream s;
        s.open("solution.txt", std::ofstream::out | std::ofstream::trunc);
        s.close();
        is_solving = true;
        std::vector<char> sides(6);
        for (int i = 0; i < 6; ++i) {
            sides[i] = cube[i][1][1];
        }
        ////Первый крест
        for (int i = 0; i < 4; ++i) {
            if (sides[front] == cube[front][2][1]) {
                cube[front][2][1] = toupper(cube[front][2][1]);
                if (cube[bottom][0][1] != sides[bottom]) {
                    if (cube[bottom][0][1] == sides[right]) {
                        swap_side(bottom);
                        front_right();
                        front_right();
                        swap_side(bottom);
                        front_right();
                        swap_side(right);
                        front_right();
                        front_right();
                        swap_side(left);
                        swap_side(up);
                        swap_side(up);
                    } else if (cube[bottom][0][1] == sides[up]) {
                        swap_side(bottom);
                        front_right();
                        front_right();
                        swap_side(bottom);
                        front_right();
                        front_right();
                        swap_side(bottom);
                        front_right();
                        front_right();
                        swap_side(bottom);
                    } else if (cube[bottom][0][1] == sides[left]) {
                        swap_side(bottom);
                        front_right();
                        front_right();
                        swap_side(bottom);
                        front_left();
                        swap_side(left);
                        front_right();
                        front_right();
                        swap_side(right);
                        swap_side(up);
                        swap_side(up);
                    }
                }
            } else if (sides[front] == cube[front][0][1]) {
                cube[front][0][1] = toupper(cube[front][0][1]);
                if (cube[up][2][1] != sides[up]) {
                    if (cube[up][2][1] == sides[right]) {
                        swap_side(up);
                        front_right();
                        front_right();
                        swap_side(up);
                        front_left();
                        right_swap(right);
                        front_right();
                        front_right();
                        swap_side(left);
                        swap_side(up);
                    } else if (cube[up][2][1] == sides[bottom]) {
                        swap_side(up);
                        front_right();
                        front_right();
                        swap_side(up);
                        front_right();
                        front_right();
                        swap_side(up);
                        front_right();
                        front_right();
                    } else if (cube[up][2][1] == sides[left]) {
                        swap_side(up);
                        front_right();
                        front_right();
                        swap_side(up);
                        front_right();
                        left_swap(left);
                        front_right();
                        front_right();
                        swap_side(right);
                        swap_side(up);
                    }
                    swap_side(up);
                }
            } else if (sides[front] == cube[front][1][0]) {
                cube[front][1][0] = toupper(cube[front][1][0]);
                if (cube[left][1][2] != sides[left]) {
                    if (cube[left][1][2] == sides[right]) {
                        swap_side(left);
                        front_right();
                        front_right();
                        swap_side(left);
                        front_right();
                        front_right();
                        swap_side(left);
                        front_right();
                        front_right();
                        swap_side(left);
                    } else if (cube[left][1][2] == sides[up]) {
                        swap_side(left);
                        front_right();
                        front_right();
                        swap_side(left);
                        front_left();
                        swap_side(up);
                        front_right();
                        front_right();
                        swap_side(bottom);
                        swap_side(back);
                    } else if (cube[left][1][2] == sides[bottom]) {
                        swap_side(left);
                        front_right();
                        front_right();
                        swap_side(left);
                        front_right();
                        swap_side(bottom);
                        front_right();
                        front_right();
                        swap_side(up);
                        swap_side(back);
                    }
                }
            } else if (sides[front] == cube[front][1][2]) {
                cube[front][1][2] = toupper(cube[front][1][2]);
                if (cube[right][1][0] != sides[right]) {
                    if (cube[right][1][0] == sides[left]) {
                        swap_side(right);
                        front_right();
                        front_right();
                        swap_side(right);
                        front_right();
                        front_right();
                        swap_side(right);
                        front_right();
                        front_right();
                        swap_side(right);
                    } else if (cube[right][1][0] == sides[up]) {
                        swap_side(right);
                        front_right();
                        front_right();
                        swap_side(right);
                        front_right();
                        swap_side(up);
                        front_right();
                        front_right();
                        swap_side(bottom);
                        swap_side(back);
                    } else if (cube[right][1][0] == sides[bottom]) {
                        swap_side(right);
                        front_right();
                        front_right();
                        swap_side(right);
                        front_left();
                        swap_side(bottom);
                        front_right();
                        front_right();
                        swap_side(up);
                        swap_side(back);
                    }
                }
            } else if (sides[front] == cube[back][2][1]) {
                cube[back][2][1] = toupper(cube[back][2][1]);
                if (cube[bottom][2][1] == sides[bottom]) {
                    swap_side(bottom);
                    front_left();
                    front_left();
                    swap_side(up);
                } else if (cube[bottom][2][1] == sides[right]) {
                    swap_side(back);
                    front_right();
                    swap_side(left);
                    front_right();
                    front_right();
                    swap_side(left);
                } else if (cube[bottom][2][1] == sides[up]) {
                    swap_side(back);
                    front_right();
                    front_right();
                    swap_side(up);
                    front_right();
                    front_right();
                    swap_side(bottom);
                    swap_side(back);
                } else if (cube[bottom][2][1] == sides[left]) {
                    swap_side(back);
                    front_left();
                    swap_side(right);
                    front_right();
                    front_right();
                    swap_side(right);
                }
            } else if (sides[front] == cube[back][0][1]) {
                cube[back][0][1] = toupper(cube[back][0][1]);
                if (cube[up][0][1] == sides[up]) {
                    swap_side(up);
                    front_left();
                    front_left();
                    swap_side(bottom);
                } else if (cube[up][0][1] == sides[right]) {
                    swap_side(back);
                    front_left();
                    swap_side(left);
                    front_right();
                    front_right();
                    swap_side(left);
                } else if (cube[up][0][1] == sides[bottom]) {
                    swap_side(back);
                    front_left();
                    front_left();
                    swap_side(bottom);
                    front_left();
                    front_left();
                    swap_side(up);
                    swap_side(back);
                } else if (cube[up][0][1] == sides[left]) {
                    swap_side(back);
                    front_right();
                    swap_side(right);
                    front_right();
                    front_right();
                    swap_side(right);
                }
            } else if (sides[front] == cube[back][1][0]) {
                cube[back][1][0] = toupper(cube[back][1][0]);
                if (cube[right][1][2] == sides[right]) {
                    swap_side(right);
                    front_left();
                    front_left();
                    swap_side(left);
                } else if (cube[right][1][2] == sides[bottom]) {
                    swap_side(back);
                    front_left();
                    swap_side(bottom);
                    front_right();
                    front_right();
                    swap_side(up);
                    swap_side(back);
                } else if (cube[right][1][2] == sides[left]) {
                    swap_side(back);
                    front_right();
                    front_right();
                    swap_side(right);
                    front_right();
                    front_right();
                    swap_side(right);
                } else if (cube[right][1][2] == sides[up]) {
                    swap_side(back);
                    front_right();
                    swap_side(up);
                    front_right();
                    front_right();
                    swap_side(bottom);
                    swap_side(back);
                }
            } else if (sides[front] == cube[back][1][2]) {
                cube[back][1][2] = toupper(cube[back][1][2]);
                if (cube[left][1][0] == sides[left]) {
                    swap_side(left);
                    front_left();
                    front_left();
                    swap_side(right);
                } else if (cube[left][1][0] == sides[up]) {
                    swap_side(back);
                    front_left();
                    swap_side(up);
                    front_right();
                    front_right();
                    swap_side(bottom);
                    swap_side(back);
                } else if (cube[left][1][0] == sides[right]) {
                    swap_side(back);
                    front_right();
                    front_right();
                    swap_side(left);
                    front_right();
                    front_right();
                    swap_side(left);
                } else if (cube[left][1][0] == sides[bottom]) {
                    swap_side(back);
                    front_right();
                    swap_side(bottom);
                    front_right();
                    front_right();
                    swap_side(up);
                    swap_side(back);
                }
            } else if (sides[front] == cube[right][2][1] or sides[front] == cube[right][0][1] or
                       sides[front] == cube[right][1][0] or sides[front] == cube[right][1][2]) {
                swap_side(right);
                swap_side(bottom);
                swap_side(left);
                solvin_for_up_1(sides);
                swap_side(right);
                swap_side(up);
                swap_side(left);
            } else if (sides[front] == cube[left][2][1] or sides[front] == cube[left][0][1] or
                       sides[front] == cube[left][1][0] or sides[front] == cube[left][1][2]) {
                swap_side(left);
                swap_side(bottom);
                swap_side(right);
                solvin_for_up_1(sides);
                swap_side(left);
                swap_side(up);
                swap_side(right);
            } else if (sides[front] == cube[bottom][2][1] or sides[front] == cube[bottom][0][1] or
                       sides[front] == cube[bottom][1][0] or sides[front] == cube[bottom][1][2]) {
                swap_side(right);
                swap_side(bottom);
                swap_side(bottom);
                swap_side(left);
                solvin_for_up_1(sides);
                swap_side(right);
                swap_side(up);
                swap_side(up);
                swap_side(left);
            } else {
                solvin_for_up_1(sides);
            }
        }
        cube[front][0][1] = tolower(cube[front][0][1]);
        cube[front][1][0] = tolower(cube[front][1][0]);
        cube[front][1][2] = tolower(cube[front][1][2]);
        cube[front][2][1] = tolower(cube[front][2][1]);
        ////Первые углы
        for (int i = 0; i < 4; ++i) {
            if (sides[front] == cube[front][0][0] and sides[up] == cube[up][2][0]) {
                cube[front][0][0] = toupper(cube[front][0][0]);
                continue;
            }
            if (sides[front] == cube[front][0][2] and sides[up] == cube[up][2][2]) {
                cube[front][0][2] = toupper(cube[front][0][2]);
                continue;
            }
            if (sides[front] == cube[front][2][0] and sides[bottom] == cube[bottom][0][0]) {
                cube[front][2][0] = toupper(cube[front][2][0]);
                continue;
            }
            if (sides[front] == cube[front][2][2] and sides[bottom] == cube[bottom][0][2]) {
                cube[front][2][2] = toupper(cube[front][2][2]);
                continue;
            }
            if (sides[front] == cube[up][2][0] or sides[front] == cube[left][0][2] or
                sides[front] == cube[front][0][0]) {
                swap_side(up);
                front_right();
                swap_side(up);
                front_right();
                swap_side(bottom);
                front_left();
                swap_side(bottom);
            }
            if (sides[front] == cube[up][2][2] or sides[front] == cube[right][0][0] or
                sides[front] == cube[front][0][2]) {
                swap_side(up);
                front_left();
                swap_side(up);
                front_right();
                swap_side(bottom);
                front_right();
                swap_side(bottom);
            }

            if (sides[front] == cube[bottom][0][2] or sides[front] == cube[right][2][0] or
                sides[front] == cube[front][2][2]) {
                swap_side(bottom);
                front_right();
                swap_side(bottom);
                front_right();
                swap_side(up);
                front_left();
                swap_side(up);
            }
            if (sides[front] == cube[bottom][0][0] or sides[front] == cube[left][2][2] or
                sides[front] == cube[front][2][0]) {
                swap_side(bottom);
                front_left();
                swap_side(bottom);
                front_right();
                swap_side(up);
                front_right();
                swap_side(up);
            }

            while (sides[front] == cube[back][0][0] or sides[front] == cube[back][2][0] or
                   sides[front] == cube[back][0][2] or sides[front] == cube[back][2][2]) {
                if (sides[front] == cube[back][0][0] and cube[front][0][2] != toupper(sides[front])) {
                    swap_side(right);
                    front_right();
                    swap_side(right);
                    front_right();
                    front_right();
                    swap_side(left);
                    front_left();
                    swap_side(left);
                    break;
                }
                if (sides[front] == cube[back][2][0] and cube[front][2][2] != toupper(sides[front])) {
                    swap_side(right);
                    front_left();
                    swap_side(right);
                    front_right();
                    front_right();
                    swap_side(left);
                    front_right();
                    swap_side(left);
                    break;
                }
                if (sides[front] == cube[back][0][2] and cube[front][0][0] != toupper(sides[front])) {
                    swap_side(left);
                    front_left();
                    swap_side(left);
                    front_right();
                    front_right();
                    swap_side(right);
                    front_right();
                    swap_side(right);
                    break;
                }
                if (sides[front] == cube[back][2][2] and cube[front][2][0] != toupper(sides[front])) {
                    swap_side(left);
                    front_right();
                    swap_side(left);
                    front_right();
                    front_right();
                    swap_side(right);
                    front_left();
                    swap_side(right);
                    break;
                } else {
                    swap_side(back);
                    front_right();
                    swap_side(back);
                }
            }

            if (sides[front] == cube[right][0][2] or sides[front] == cube[right][2][2]) {
                swap_side(back);
                front_right();
                swap_side(back);
            } else if (sides[front] == cube[bottom][2][0] or sides[front] == cube[bottom][2][2]) {
                swap_side(back);
                front_right();
                front_right();
                swap_side(back);
            } else if (sides[front] == cube[left][0][0] or sides[front] == cube[left][2][0]) {
                swap_side(back);
                front_left();
                swap_side(back);
            }
            solvin_for_up_2(sides);
        }
        cube[front][0][0] = tolower(cube[front][0][0]);
        cube[front][2][0] = tolower(cube[front][2][0]);
        cube[front][0][2] = tolower(cube[front][0][2]);
        cube[front][2][2] = tolower(cube[front][2][2]);
        ////Боковые слои
        for (int i = 0; i < 8; ++i) {
            if (cube[up][1][0] == sides[up] and cube[up][1][2] == sides[up] and cube[left][0][1] == sides[left] and
                cube[left][2][1] == sides[left] and
                cube[right][0][1] == sides[right] and cube[right][2][1] == sides[right] and
                cube[bottom][1][0] == sides[bottom] and cube[bottom][1][2] == sides[bottom]) {
                break;
            }
            if (cube[up][1][0] == sides[left] and cube[left][0][1] == sides[up]) {
                solvin_for_up_3_l();
                swap_side(back);
                front_right();
                front_right();
                swap_side(back);
                solvin_for_up_3_l();
            }

            if (cube[up][1][2] == sides[right] and cube[right][0][1] == sides[up]) {
                solvin_for_up_3_r();
                swap_side(back);
                front_right();
                front_right();
                swap_side(back);
                solvin_for_up_3_r();
            }

            if (cube[up][1][0] == sides[up] and cube[left][0][1] == sides[right]) {
                solvin_for_up_3_l();
                swap_side(back);
                front_right();
                swap_side(bottom);
                swap_side(left);
                swap_side(bottom);
                solvin_for_up_3_l();
                swap_side(up);
                swap_side(left);
                swap_side(bottom);
            }

            if (cube[up][1][2] == sides[up] and cube[right][0][1] == sides[left]) {
                solvin_for_up_3_r();
                swap_side(back);
                front_left();
                swap_side(bottom);
                swap_side(right);
                swap_side(bottom);
                solvin_for_up_3_r();
                swap_side(up);
                swap_side(right);
                swap_side(bottom);
            }

            if (cube[up][1][0] == sides[right] and cube[left][0][1] == sides[up]) {
                solvin_for_up_3_l();
                swap_side(back);
                front_right();
                front_right();
                swap_side(back);
                solvin_for_up_3_r();
            }

            if (cube[up][1][2] == sides[left] and cube[right][0][1] == sides[up]) {
                solvin_for_up_3_r();
                swap_side(back);
                front_right();
                front_right();
                swap_side(back);
                solvin_for_up_3_l();
            }
            for (int j = 0; j < 8; ++j) {
                if (cube[up][0][1] == sides[up] and cube[back][0][1] != sides[back]) {
                    if (cube[back][0][1] == sides[left]) {
                        solvin_for_up_3_l();
                    } else {
                        solvin_for_up_3_r();
                    }
                }

                if (cube[up][1][0] != sides[up] and cube[up][1][0] != sides[back] and cube[left][0][1] != sides[back]) {
                    solvin_for_up_3_l();
                }
                if (cube[up][1][2] != sides[up] and cube[up][1][2] != sides[back] and
                    cube[right][0][1] != sides[back]) {
                    solvin_for_up_3_r();
                }
                swap_side(back);
                front_right();
                swap_side(back);
            }
            swap_side(bottom);
            swap_side(left);
            swap_side(up);
        }
        ////Второй крест
        if (cube[back][0][1] != sides[back] or cube[back][1][0] != sides[back] or cube[back][1][2] != sides[back] or
            cube[back][2][1] != sides[back]) {
            if (cube[back][0][1] != sides[back] and cube[back][1][0] != sides[back] and
                cube[back][1][2] != sides[back] and cube[back][2][1] != sides[back]) {
                solvin_for_up_krest();
            }
            if (cube[back][1][2] == sides[back] and cube[back][0][1] == sides[back]) {
                swap_side(back);
                front_right();
                swap_side(back);
            }
            if (cube[back][1][0] == sides[back] and cube[back][0][1] == sides[back]) {
                swap_side(back);
                front_right();
                front_right();
                swap_side(back);
            }
            if (cube[back][1][0] == sides[back] and cube[back][2][1] == sides[back]) {
                swap_side(back);
                front_left();
                swap_side(back);
            }
            if (cube[back][1][2] == sides[back] and cube[back][2][1] == sides[back]) {
                solvin_for_up_krest();
            }
            if (cube[back][0][1] == sides[back] and cube[back][2][1] == sides[back]) {
                swap_side(back);
                front_right();
                swap_side(back);
            }
            if (cube[back][1][0] == sides[back] and cube[back][1][2] == sides[back]) {
                solvin_for_up_krest();
            }
        }
        ////Правильный крест
        int same = 0;
        while (same < 2) {
            same = 0;
            swap_side(back);
            front_right();
            swap_side(back);
            if (cube[up][0][1] == sides[up]) {
                ++same;
            }

            if (cube[right][1][2] == sides[right]) {
                ++same;
            }

            if (cube[left][1][0] == sides[left]) {
                ++same;
            }

            if (cube[bottom][2][1] == sides[bottom]) {
                ++same;
            }
        }
        if (same == 2) {
            if (cube[up][0][1] == sides[up] and cube[bottom][2][1] == sides[bottom]) {
                solvin_r_b_krest();
                swap_side(back);
                front_left();
                swap_side(back);
            } else if (cube[left][1][0] == sides[left] and cube[right][1][2] == sides[right]) {
                swap_side(bottom);
                swap_side(right);
                swap_side(up);
                solvin_r_b_krest();
                swap_side(back);
                front_left();
                swap_side(back);
                swap_side(bottom);
                swap_side(left);
                swap_side(up);
            }
            if (cube[left][1][0] == sides[left] and cube[bottom][2][1] == sides[bottom]) {
                swap_side(bottom);
                swap_side(left);
                swap_side(up);
                solvin_r_b_krest();
                swap_side(bottom);
                swap_side(right);
                swap_side(up);
            } else if (cube[left][1][0] == sides[left] and cube[up][0][1] == sides[up]) {
                swap_side(bottom);
                swap_side(left);
                swap_side(left);
                swap_side(up);
                solvin_r_b_krest();
                swap_side(bottom);
                swap_side(right);
                swap_side(right);
                swap_side(up);
            } else if (cube[up][0][1] == sides[up] and cube[right][1][2] == sides[right]) {
                swap_side(bottom);
                swap_side(right);
                swap_side(up);
                solvin_r_b_krest();
                swap_side(bottom);
                swap_side(left);
                swap_side(up);
            } else if (cube[right][1][2] == sides[right] and cube[bottom][2][1] == sides[bottom]) {
                solvin_r_b_krest();
            }

        }
        ////Расстановка углов
        same = counter_for_corner(sides);
        while (same != 4) {
            if (same == 1) {
                if ((cube[back][0][0] == sides[back] or cube[back][0][0] == sides[up] or
                     cube[back][0][0] == sides[right]) and (
                            cube[up][0][2] == sides[back] or cube[up][0][2] == sides[up] or
                            cube[up][0][2] == sides[right]) and
                    (cube[right][0][2] == sides[back] or cube[right][0][2] == sides[up] or
                     cube[right][0][2] == sides[right])) {
                    swap_side(bottom);
                    swap_side(left);
                    swap_side(up);
                    solvin_for_corner();
                    swap_side(bottom);
                    swap_side(right);
                    swap_side(up);

                } else if ((cube[back][2][0] == sides[back] or cube[back][2][0] == sides[bottom] or
                            cube[back][2][0] == sides[right]) and (
                                   cube[bottom][2][2] == sides[back] or cube[bottom][2][2] == sides[bottom] or
                                   cube[bottom][2][2] == sides[right]) and
                           (cube[right][2][2] == sides[back] or cube[right][2][2] == sides[bottom] or
                            cube[right][2][2] == sides[right])) {
                    swap_side(bottom);
                    swap_side(left);
                    swap_side(left);
                    swap_side(up);
                    solvin_for_corner();
                    swap_side(bottom);
                    swap_side(right);
                    swap_side(right);
                    swap_side(up);
                } else if ((cube[back][2][2] == sides[back] or cube[back][2][2] == sides[bottom] or
                            cube[back][2][2] == sides[left]) and
                           (cube[bottom][2][0] == sides[back] or cube[bottom][2][0] == sides[bottom] or
                            cube[bottom][2][0] == sides[left]) and
                           (cube[left][2][0] == sides[back] or cube[left][2][0] == sides[bottom] or
                            cube[left][2][0] == sides[left])) {
                    swap_side(bottom);
                    swap_side(right);
                    swap_side(up);
                    solvin_for_corner();
                    swap_side(bottom);
                    swap_side(left);
                    swap_side(up);
                } else {
                    solvin_for_corner();
                }
            } else {
                solvin_for_corner();
            }
            same = counter_for_corner(sides);
        }
        ////Разворот углов
        while (cube[back][0][0] != sides[back] or cube[back][2][0] != sides[back] or cube[back][0][2] != sides[back] or
               cube[back][2][2] != sides[back]) {
            while (cube[back][0][0] != sides[back]) {
                swap_side(up);
                front_right();
                swap_side(bottom);
                front_right();
                swap_side(up);
                front_left();
                swap_side(bottom);
                front_left();
            }
            swap_side(back);
            front_right();
            swap_side(back);
        }

        while (cube[up][0][1] != sides[up]) {
            swap_side(back);
            front_right();
            swap_side(back);
        }
        front = 0;
        up = 1;
        right = 2;
        left = 3;
        bottom = 4;
        back = 5;
        is_solving = false;
    }

    void save(const std::string &name) {
        std::ofstream s;
        s.open(name);
        if (!s.is_open()) {
            std::cerr << "Cannot open the file!\n";
            return;
        }
        for (auto &i : cube) {
            for (auto &j : i) {
                for (char &k : j) {
                    s << k;
                }
                s << '\n';
            }
        }
        s.close();
    }

    void print() {
        std::cout << "-----------------------\n";
        for (int i = 0; i < 3; i++) {
            std::cout << "      ";
            for (int j = 0; j < 3; j++) {
                std::cout << cube[up][i][j] << ' ';
            }
            std::cout << '\n';
        }
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                std::cout << cube[left][i][j] << ' ';
            }
            for (int j = 0; j < 3; j++) {
                std::cout << cube[front][i][j] << ' ';
            }
            for (int j = 0; j < 3; j++) {
                std::cout << cube[right][i][j] << ' ';
            }
            for (int j = 0; j < 3; j++) {
                std::cout << cube[back][i][j] << ' ';
            }
            std::cout << '\n';
        }
        for (int i = 0; i < 3; i++) {
            std::cout << "      ";
            for (int j = 0; j < 3; j++) {
                std::cout << cube[bottom][i][j] << ' ';
            }
            std::cout << '\n';
        }
        std::cout << "-----------------------\n";
    }


private:
    char cube[6][3][3]{};
    short int front = 0;
    short int up = 1;
    short int right = 2;
    short int left = 3;
    short int bottom = 4;
    short int back = 5;
    bool is_solving = false;


    void front_right() {
        transpose(front);
        reverse_column(front);
        for (int i = 0; i < 3; i++) {
            char t = cube[up][2][i];
            cube[up][2][i] = cube[left][2 - i][2];
            cube[left][2 - i][2] = cube[bottom][0][2 - i];
            cube[bottom][0][2 - i] = cube[right][i][0];
            cube[right][i][0] = t;
        }
        if (is_solving) {
            std::ofstream s;
            s.open("solution.txt", std::ios::app);
            s << "F\n";
            s.close();
        }
    }

    void front_left() {
        transpose(front);
        reverse_line(front);
        for (int i = 0; i < 3; i++) {
            char t = cube[up][2][i];
            cube[up][2][i] = cube[right][i][0];
            cube[right][i][0] = cube[bottom][0][2 - i];
            cube[bottom][0][2 - i] = cube[left][2 - i][2];
            cube[left][2 - i][2] = t;
        }
        if (is_solving) {
            std::ofstream s;
            s.open("solution.txt", std::ios::app);
            s << "F'\n";
            s.close();
        }
    }

    void swap_side(int side) {
        std::ofstream s;
        if (is_solving) s.open("solution.txt", std::ios::app);

        if (side == right) {
            right_swap(side);
            s << "right ";
        } else if (side == left) {
            left_swap(side);
            s << "left ";
        } else if (side == back) {
            right_swap(right);
            right_swap(right);
            s << "back ";
        } else if (side == up) {
            up_swap(side);
            s << "up ";
        } else if (side == bottom) {
            for (int i = 0; i < 3; ++i) {
                up_swap(up);
            }
            s << "bottom ";
        }
        if (is_solving) s.close();
    }

    int change_choose_color(char a, char b = ' ', char c = ' ') {
        if (b == ' ') {
            int t = (int) a;
            switch (t) {
                case 119:
                    return 0;
                case 98:
                    return 1;
                case 114:
                    return 2;
                case 111:
                    return 3;
                case 103:
                    return 4;
                case 121:
                    return 5;
                default:
                    return 26;
            }
        } else if (c == ' ') {
            int t = (int) a * (int) b;
            switch (t) {
                case 119 * 98:
                    return 6;
                case 119 * 114:
                    return 7;
                case 119 * 111:
                    return 8;
                case 119 * 103:
                    return 9;
                case 121 * 98:
                    return 10;
                case 121 * 114:
                    return 11;
                case 121 * 111:
                    return 12;
                case 121 * 103:
                    return 13;
                case 98 * 114:
                    return 14;
                case 98 * 111:
                    return 15;
                case 103 * 114:
                    return 16;
                case 103 * 111:
                    return 17;
                default:
                    return 26;
            }
        } else {
            int t = (int) a * (int) b * (int) c;
            switch (t) {
                case 119 * 98 * 114:
                    return 18;
                case 119 * 98 * 111:
                    return 19;
                case 119 * 103 * 114:
                    return 20;
                case 119 * 103 * 111:
                    return 21;
                case 121 * 98 * 114:
                    return 22;
                case 121 * 98 * 111:
                    return 23;
                case 121 * 103 * 114:
                    return 24;
                case 121 * 103 * 111:
                    return 25;
                default:
                    return 26;
            }
        }
    }

    bool check() {
        /*
         * 0 - w; 1 - b; 2 - r; 3 - o; 4 - g; 5 - y;
         * 6 - wb; 7 - wr; 8 - wo; 9 - wg; 10 - yb; 11 - yr; 12 - yo; 13 - yg; 14 - br; 15 - bo; 16 - gr; 17 - go
         * 18 - wbr; 19 - wbo; 20 - wgr; 21 - wgo; 22 - ybr; 23 - ybo; 24 - ygr; 25 - ygo;
         * 26 - extra
         */
        std::vector<bool> elements(27, 0);
        elements[26] = true;
        for (int i = 0; i < 3; ++i) {
            if (elements[change_choose_color(cube[i][1][1])] or elements[change_choose_color(cube[5 - i][1][1])]) {
                std::cerr<< "wrong pieces";
                return false;
            }
            elements[change_choose_color(cube[i][1][1])] = true;
            elements[change_choose_color(cube[5 - i][1][1])] = true;
        }

        elements[change_choose_color(cube[front][0][1], cube[up][2][1])] = true;
        elements[change_choose_color(cube[front][2][1], cube[bottom][0][1])] = true;
        elements[change_choose_color(cube[front][1][0], cube[left][1][2])] = true;
        elements[change_choose_color(cube[front][1][2], cube[right][1][0])] = true;
        elements[change_choose_color(cube[back][0][1], cube[up][0][1])] = true;
        elements[change_choose_color(cube[back][2][1], cube[bottom][2][1])] = true;
        elements[change_choose_color(cube[back][1][0], cube[right][1][2])] = true;
        elements[change_choose_color(cube[back][1][2], cube[left][1][0])] = true;
        elements[change_choose_color(cube[up][1][0], cube[left][0][1])] = true;
        elements[change_choose_color(cube[up][1][2], cube[right][0][1])] = true;
        elements[change_choose_color(cube[bottom][1][0], cube[left][2][1])] = true;
        elements[change_choose_color(cube[bottom][1][2], cube[right][2][1])] = true;

        elements[change_choose_color(cube[front][0][0], cube[up][2][0], cube[left][0][2])] = true;
        elements[change_choose_color(cube[front][0][2], cube[up][2][2], cube[right][0][0])] = true;
        elements[change_choose_color(cube[front][2][0], cube[bottom][0][0], cube[left][2][2])] = true;
        elements[change_choose_color(cube[front][2][2], cube[bottom][0][2], cube[right][2][0])] = true;
        elements[change_choose_color(cube[back][0][0], cube[up][0][2], cube[right][0][2])] = true;
        elements[change_choose_color(cube[back][0][2], cube[up][0][0], cube[left][0][0])] = true;
        elements[change_choose_color(cube[back][2][0], cube[bottom][2][2], cube[right][2][2])] = true;
        elements[change_choose_color(cube[back][2][2], cube[bottom][2][0], cube[left][2][0])] = true;

        if (std::find(elements.begin(), elements.end(), false) == elements.end()) {
            char t[6][3][3];
            for (int i = 0; i < 6; ++i) {
                for (int j = 0; j < 3; ++j) {
                    for (int k = 0; k < 3; ++k) {
                        t[i][j][k] = cube[i][j][k];
                    }
                }
            }
            solve();
            for (int i = 0; i < 6; ++i) {
                for (int j = 0; j < 3; ++j) {
                    for (int k = 0; k < 3; ++k) {
                        if (cube[i][j][k] != choose_color(i)) {
                            return false;
                        }
                    }
                }
            }
            for (int i = 0; i < 6; ++i) {
                for (int j = 0; j < 3; ++j) {
                    for (int k = 0; k < 3; ++k) {
                        cube[i][j][k] = t[i][j][k];
                    }
                }
            }
            front = 0;
            up = 1;
            right = 2;
            left = 3;
            bottom = 4;
            back = 5;
            return true;
        } else {
            std::cerr<< "wrong pieces";
            return false;
        }
    }

    char choose_color(int n) {
        switch (n) {
            case 0 :
                return 'w';
            case 1 :
                return 'b';
            case 2 :
                return 'r';
            case 3 :
                return 'o';
            case 4 :
                return 'g';
            default:
                return 'y';
        }
    }

    void transpose(int side) {
        std::swap(cube[side][0][1], cube[side][1][0]);
        std::swap(cube[side][0][2], cube[side][2][0]);
        std::swap(cube[side][2][1], cube[side][1][2]);
    }

    void reverse_column(int side) {
        std::swap(cube[side][0][0], cube[side][0][2]);
        std::swap(cube[side][1][0], cube[side][1][2]);
        std::swap(cube[side][2][0], cube[side][2][2]);
    }

    void reverse_line(int side) {
        std::swap(cube[side][0][0], cube[side][2][0]);
        std::swap(cube[side][0][1], cube[side][2][1]);
        std::swap(cube[side][0][2], cube[side][2][2]);
    }


    void right_swap(int side) {
        right = back;
        back = left;
        left = front;
        front = side;
        transpose(up);
        reverse_column(up);
        transpose(bottom);
        reverse_line(bottom);
    }

    void left_swap(int side) {
        left = back;
        back = right;
        right = front;
        front = side;
        transpose(up);
        reverse_line(up);
        transpose(bottom);
        reverse_column(bottom);
    }

    void up_swap(int side) {
        up = back;
        back = bottom;
        bottom = front;
        front = side;
        transpose(right);
        reverse_line(right);
        transpose(left);
        reverse_column(left);
        reverse_line(back);
        reverse_column(back);
        reverse_line(up);
        reverse_column(up);
    }


    void solvin_for_up_1(std::vector<char> sides) {
        if (sides[front] == cube[up][2][1]) {
            cube[up][2][1] = toupper(cube[up][2][1]);
            if (cube[front][0][1] == sides[up]) {
                swap_side(up);
                front_left();
                swap_side(bottom);
                front_right();
                swap_side(right);
                front_left();
                swap_side(left);
                front_left();
            } else if (cube[front][0][1] == sides[right]) {
                swap_side(up);
                front_left();
                swap_side(right);
                front_left();
                swap_side(left);
                swap_side(bottom);
            } else if (cube[front][0][1] == sides[bottom]) {
                swap_side(up);
                front_right();
                front_right();
                swap_side(up);
                front_right();
                front_right();
                swap_side(up);
                front_right();
                swap_side(up);
                front_right();
                swap_side(left);
                front_left();
                swap_side(right);
                front_left();
            } else if (cube[front][0][1] == sides[left]) {
                swap_side(up);
                front_right();
                swap_side(left);
                front_right();
                swap_side(right);
                swap_side(bottom);
            }
        } else if (sides[front] == cube[up][0][1]) {
            cube[up][0][1] = toupper(cube[up][0][1]);
            if (cube[back][0][1] == sides[up]) {
                swap_side(back);
                front_right();
                swap_side(right);
                front_right();
                swap_side(up);
                front_left();
                swap_side(bottom);
                front_left();
                swap_side(right);
            } else if (cube[back][0][1] == sides[right]) {
                swap_side(up);
                front_right();
                swap_side(right);
                front_left();
                swap_side(left);
                front_left();
                swap_side(bottom);
            } else if (cube[back][0][1] == sides[bottom]) {
                swap_side(back);
                front_left();
                swap_side(left);
                front_right();
                swap_side(bottom);
                front_left();
                swap_side(up);
                front_left();
                swap_side(left);
            } else if (cube[back][0][1] == sides[left]) {
                swap_side(up);
                front_left();
                swap_side(left);
                front_right();
                swap_side(right);
                front_right();
                swap_side(bottom);
            }
        } else if (sides[front] == cube[up][1][0]) {
            cube[up][1][0] = toupper(cube[up][1][0]);
            if (cube[left][0][1] == sides[left]) {
                swap_side(left);
                front_right();
                swap_side(right);
            } else if (cube[left][0][1] == sides[bottom]) {
                front_right();
                swap_side(left);
                front_right();
                swap_side(right);
                front_left();
            } else if (cube[left][0][1] == sides[right]) {
                swap_side(up);
                front_right();
                front_right();
                swap_side(right);
                front_left();
                swap_side(left);
                front_right();
                front_right();
                swap_side(bottom);
            } else if (cube[left][0][1] == sides[up]) {
                front_left();
                swap_side(left);
                front_right();
                swap_side(right);
                front_right();
            }
        } else if (sides[front] == cube[up][1][2]) {
            cube[up][1][2] = toupper(cube[up][1][2]);
            if (cube[right][0][1] == sides[right]) {
                swap_side(right);
                front_left();
                swap_side(left);
            } else if (cube[right][0][1] == sides[bottom]) {
                front_left();
                swap_side(right);
                front_left();
                swap_side(left);
                front_right();
            } else if (cube[right][0][1] == sides[left]) {
                swap_side(up);
                front_right();
                front_right();
                swap_side(left);
                front_right();
                swap_side(right);
                front_right();
                front_right();
                swap_side(bottom);
            } else if (cube[right][0][1] == sides[up]) {
                front_right();
                swap_side(right);
                front_left();
                swap_side(left);
                front_left();
            }
        }
    }

    void solvin_for_up_2(std::vector<char> sides) {
        if (sides[front] == cube[up][0][0]) {
            cube[up][0][0] = toupper(cube[up][0][0]);
            if (cube[back][0][2] == sides[left]) {
                swap_side(bottom);
                front_left();
                swap_side(bottom);
                front_right();
                swap_side(up);
                front_right();
                swap_side(up);
            } else if (cube[back][0][2] == sides[bottom]) {
                swap_side(right);
                front_left();
                swap_side(right);
                front_left();
                front_left();
                swap_side(left);
                front_right();
                swap_side(left);
            } else if (cube[back][0][2] == sides[right]) {
                swap_side(back);
                front_left();
                swap_side(left);
                front_right();
                swap_side(right);
                front_right();
                swap_side(left);
                front_left();
                swap_side(left);
            } else if (cube[back][0][2] == sides[up]) {
                swap_side(back);
                front_left();
                swap_side(right);
                front_left();
                swap_side(left);
                front_right();
                swap_side(right);
                front_right();
                swap_side(right);
            }
        } else if (sides[front] == cube[up][0][2]) {
            cube[up][0][2] = toupper(cube[up][0][2]);
            if (cube[back][0][0] == sides[right]) {
                swap_side(bottom);
                front_right();
                swap_side(bottom);
                front_left();
                swap_side(up);
                front_left();
                swap_side(up);
            } else if (cube[back][0][0] == sides[bottom]) {
                swap_side(back);
                front_left();
                front_left();
                swap_side(bottom);
                front_left();
                swap_side(up);
                front_left();
                swap_side(bottom);
                front_right();
                swap_side(up);
                swap_side(back);
            } else if (cube[back][0][0] == sides[left]) {
                swap_side(back);
                front_left();
                swap_side(up);
                front_right();
                swap_side(bottom);
                front_right();
                front_right();
                swap_side(up);
                front_left();
                swap_side(bottom);
                swap_side(back);
            } else if (cube[back][0][0] == sides[up]) {
                swap_side(back);
                front_right();
                swap_side(left);
                front_right();
                swap_side(right);
                front_left();
                swap_side(left);
                front_left();
                swap_side(left);
            }
        }
    }

    void solvin_for_up_3_l() {
        swap_side(back);
        front_left();
        swap_side(right);
        front_left();
        swap_side(left);
        front_right();
        swap_side(right);
        front_right();
        swap_side(left);
        front_right();
        swap_side(up);
        front_right();
        swap_side(bottom);
        front_left();
        swap_side(up);
        front_left();
        swap_side(bottom);
        swap_side(back);
    }

    void solvin_for_up_3_r() {
        swap_side(back);
        front_right();
        swap_side(left);
        front_right();
        swap_side(right);
        front_left();
        swap_side(left);
        front_left();
        swap_side(right);
        front_left();
        swap_side(up);
        front_left();
        swap_side(bottom);
        front_right();
        swap_side(up);
        front_right();
        swap_side(bottom);
        swap_side(back);
    }

    void solvin_for_up_krest() {
        swap_side(up);
        front_right();
        swap_side(right);
        front_right();
        swap_side(up);
        front_right();
        swap_side(bottom);
        front_left();
        swap_side(up);
        front_left();
        swap_side(bottom);
        swap_side(left);
        front_left();
        swap_side(bottom);
    }

    void solvin_r_b_krest() {
        swap_side(right);
        front_right();
        swap_side(right);
        front_right();
        swap_side(left);
        front_left();
        swap_side(right);
        front_right();
        swap_side(left);
        front_right();
        swap_side(right);
        front_right();
        front_right();
        swap_side(left);
        front_left();
        swap_side(right);
        front_right();
        swap_side(left);
        swap_side(left);
    }

    void solvin_for_corner() {
        swap_side(right);
        front_right();
        swap_side(right);
        front_left();
        swap_side(right);
        front_left();
        swap_side(left);
        front_right();
        swap_side(left);
        front_left();
        swap_side(right);
        front_left();
        swap_side(right);
        front_right();
        swap_side(left);
        front_right();
        swap_side(back);
    }

    int counter_for_corner(std::vector<char> sides) {
        int same = 0;
        if ((cube[back][0][0] == sides[back] or cube[back][0][0] == sides[up] or cube[back][0][0] == sides[right]) and
            (cube[up][0][2] == sides[back] or cube[up][0][2] == sides[up] or cube[up][0][2] == sides[right]) and
            (cube[right][0][2] == sides[back] or cube[right][0][2] == sides[up] or cube[right][0][2] == sides[right])) {
            ++same;
        }
        if ((cube[back][2][0] == sides[back] or cube[back][2][0] == sides[bottom] or
             cube[back][2][0] == sides[right]) and
            (cube[bottom][2][2] == sides[back] or cube[bottom][2][2] == sides[bottom] or
             cube[bottom][2][2] == sides[right]) and
            (cube[right][2][2] == sides[back] or cube[right][2][2] == sides[bottom] or
             cube[right][2][2] == sides[right])) {
            ++same;
        }
        if ((cube[back][2][2] == sides[back] or cube[back][2][2] == sides[bottom] or
             cube[back][2][2] == sides[left]) and
            (cube[bottom][2][0] == sides[back] or cube[bottom][2][0] == sides[bottom] or
             cube[bottom][2][0] == sides[left]) and
            (cube[left][2][0] == sides[back] or cube[left][2][0] == sides[bottom] or cube[left][2][0] == sides[left])) {
            ++same;
        }
        if ((cube[back][0][2] == sides[back] or cube[back][0][2] == sides[up] or cube[back][0][2] == sides[left]) and
            (cube[up][0][0] == sides[back] or cube[up][0][0] == sides[up] or cube[up][0][0] == sides[left]) and
            (cube[left][0][0] == sides[back] or cube[left][0][0] == sides[up] or cube[left][0][0] == sides[left])) {
            ++same;
        }
        return same;
    }

};

Rubik r;

void Timer(int) {
    glutTimerFunc(20, Timer, 0);
    glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y) {
    key = tolower(key);

    switch (key) {
        case 'w':
            DEGY += 10;
            break;
        case 's':
            DEGY -= 10;
            break;
        case 'd':
            DEGX += 10;
            break;
        case 'a':
            DEGX -= 10;
            break;
        case '1':
            DEGX = 0;
            DEGY = 0;
            break;
        case '2':
            r.swap_side(r.up);
            break;
        case '3':
            r.swap_side(r.right);
            break;
        case '4':
            r.swap_side(r.left);
            break;
        case '5':
            r.swap_side(r.bottom);
            break;
        case '6':
            r.swap_side(r.back);
            break;
        case 'l':
            r.swap_side(r.right);
            r.front_right();
            r.swap_side(r.left);
            break;
        case 'k':
            r.swap_side(r.right);
            r.front_left();
            r.swap_side(r.left);
        case 'm':
            r.swap_side(r.bottom);
            r.front_right();
            r.swap_side(r.up);
            break;
        case 'n':
            r.swap_side(r.bottom);
            r.front_left();
            r.swap_side(r.up);
            break;
        case 'j':
            r.front_right();
            break;
        case 'h':
            r.front_left();
            break;
        case 'u':
            r.swap_side(r.up);
            r.front_right();
            r.swap_side(r.bottom);
            break;
        case 'y':
            r.swap_side(r.up);
            r.front_right();
            r.swap_side(r.bottom);
            break;
        case 'g':
            r.swap_side(r.left);
            r.front_right();
            r.swap_side(r.right);
            break;
        case 'f':
            r.swap_side(r.left);
            r.front_left();
            r.swap_side(r.right);
            break;
        case ';':
            r.swap_side(r.back);
            r.front_right();
            r.swap_side(r.back);
            break;
        case '\'':
            r.swap_side(r.back);
            r.front_left();
            r.swap_side(r.back);
            break;
        case 'z':
            r.print();
            break;
        case 'q':
            r.solve();
            break;
        case 'e':
            r.generate();
            break;
        case 'c':
            r.save("data.txt");
            break;
    }
}

void paint(char color = '0') {
    switch (color) {
        case 'w':
            glColor3f(1.0, 1.0, 1.0);
            break;
        case 'b':
            glColor3f(0.0, 0.0, 1.0);
            break;
        case 'r':
            glColor3f(1.0, 0.0, 0.0);
            break;
        case 'g':
            glColor3f(0.0, 1.0, 0.0);
            break;
        case 'o':
            glColor3f(1.0, 0.64, 0.0);
            break;
        case 'y':
            glColor3f(1.0, 1.0, 0.0);
            break;
        default:
            glColor3f(0.0, 0.0, 0.0);
            break;
    }
}

void draw_side(int k, int size) {
    glBegin(GL_QUADS);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            paint(r.cube[k][i][j]);
            glVertex2f((2 * j - 3) * size + (j - 1) * size / 8, (3 - 2 * i) * size + (1 - i) * size / 8);
            glVertex2f((2 * j - 3) * size + (j - 1) * size / 8, (1 - 2 * i) * size + (1 - i) * size / 8);
            glVertex2f((2 * j - 1) * size + (j - 1) * size / 8, (1 - 2 * i) * size + (1 - i) * size / 8);
            glVertex2f((2 * j - 1) * size + (j - 1) * size / 8, (3 - 2 * i) * size + (1 - i) * size / 8);
        }
    }
    glEnd();
}

void draw(int size = std::min(HEIGHT, WIDTH) / 20) {
    glPushMatrix();
    int minimap = 300;
    glTranslatef(minimap, minimap, 0);
    for (int k = 0; k < 6; ++k) {
        if (k == r.up) {
            glTranslatef(0, size * 51 / 32, 0);
            draw_side(k, size / 4);
            glTranslatef(0, -size * 51 / 32, 0);
        } else if (k == r.bottom) {
            glTranslatef(0, -size * 51 / 32, 0);
            draw_side(k, size / 4);
            glTranslatef(0, size * 51 / 32, 0);
        } else if (k == r.right) {
            glTranslatef(size * 51 / 32, 0, 0);
            draw_side(k, size / 4);
            glTranslatef(-size * 51 / 32, 0, 0);
        } else if (k == r.left) {
            glTranslatef(-size * 51 / 32, 0, 0);
            draw_side(k, size / 4);
            glTranslatef(size * 51 / 32, 0, 0);
        } else if (k == r.back) {
            glTranslatef(size * 51 / 16, 0, 0);
            draw_side(k, size / 4);
            glTranslatef(-size * 51 / 16, 0, 0);
        } else {
            draw_side(k, size / 4);
        }
    }
    glTranslatef(-minimap, -minimap, 0);
    glRotatef(DEGX, 0, 1, 0);
    glRotatef(DEGY, 1, 0, 0);
    glBegin(GL_QUADS);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                if (i == 2) {
                    paint(r.cube[r.front][j][k]);
                } else {
                    paint();
                }
                glVertex3f((2 * k - 3) * size + (k - 1) * 5, (3 - 2 * j) * size + (1 - j) * 5,
                           (2 * i - 1) * size + (i - 1) * 5);
                glVertex3f((2 * k - 3) * size + (k - 1) * 5, (1 - 2 * j) * size + (1 - j) * 5,
                           (2 * i - 1) * size + (i - 1) * 5);
                glVertex3f((2 * k - 1) * size + (k - 1) * 5, (1 - 2 * j) * size + (1 - j) * 5,
                           (2 * i - 1) * size + (i - 1) * 5);
                glVertex3f((2 * k - 1) * size + (k - 1) * 5, (3 - 2 * j) * size + (1 - j) * 5,
                           (2 * i - 1) * size + (i - 1) * 5);
            }
        }
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                if (i == 0) {
                    paint(r.cube[r.back][j][k]);
                } else {
                    paint();
                }
                glVertex3f((3 - 2 * k) * size + (1 - k) * 5, (3 - 2 * j) * size + (1 - j) * 5,
                           (2 * i - 3) * size + (i - 1) * 5);
                glVertex3f((3 - 2 * k) * size + (1 - k) * 5, (1 - 2 * j) * size + (1 - j) * 5,
                           (2 * i - 3) * size + (i - 1) * 5);
                glVertex3f((1 - 2 * k) * size + (1 - k) * 5, (1 - 2 * j) * size + (1 - j) * 5,
                           (2 * i - 3) * size + (i - 1) * 5);
                glVertex3f((1 - 2 * k) * size + (1 - k) * 5, (3 - 2 * j) * size + (1 - j) * 5,
                           (2 * i - 3) * size + (i - 1) * 5);
            }
        }
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                if (i == 2) {
                    paint(r.cube[r.up][j][k]);
                } else {
                    paint();
                }
                glVertex3f((2 * k - 3) * size + (k - 1) * 5,
                           (2 * i - 1) * size + (i - 1) * 5, (2 * j - 3) * size + (j - 1) * 5);
                glVertex3f((2 * k - 3) * size + (k - 1) * 5,
                           (2 * i - 1) * size + (i - 1) * 5, (2 * j - 1) * size + (j - 1) * 5);
                glVertex3f((2 * k - 1) * size + (k - 1) * 5,
                           (2 * i - 1) * size + (i - 1) * 5, (2 * j - 1) * size + (j - 1) * 5);
                glVertex3f((2 * k - 1) * size + (k - 1) * 5,
                           (2 * i - 1) * size + (i - 1) * 5, (2 * j - 3) * size + (j - 1) * 5);
            }
        }
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                if (i == 0) {
                    paint(r.cube[r.bottom][j][k]);
                } else {
                    paint();
                }
                glVertex3f((2 * k - 3) * size + (k - 1) * 5,
                           (2 * i - 3) * size + (i - 1) * 5, (3 - 2 * j) * size + (1 - j) * 5);
                glVertex3f((2 * k - 3) * size + (k - 1) * 5,
                           (2 * i - 3) * size + (i - 1) * 5, (1 - 2 * j) * size + (1 - j) * 5);
                glVertex3f((2 * k - 1) * size + (k - 1) * 5,
                           (2 * i - 3) * size + (i - 1) * 5, (1 - 2 * j) * size + (1 - j) * 5);
                glVertex3f((2 * k - 1) * size + (k - 1) * 5,
                           (2 * i - 3) * size + (i - 1) * 5, (3 - 2 * j) * size + (1 - j) * 5);
            }
        }
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                if (i == 2) {
                    paint(r.cube[r.right][j][k]);
                } else {
                    paint();
                }
                glVertex3f((2 * i - 1) * size + (i - 1) * 5, (3 - 2 * j) * size + (1 - j) * 5,
                           (3 - 2 * k) * size + (1 - k) * 5);
                glVertex3f((2 * i - 1) * size + (i - 1) * 5, (1 - 2 * j) * size + (1 - j) * 5,
                           (3 - 2 * k) * size + (1 - k) * 5);
                glVertex3f((2 * i - 1) * size + (i - 1) * 5, (1 - 2 * j) * size + (1 - j) * 5,
                           (1 - 2 * k) * size + (1 - k) * 5);
                glVertex3f((2 * i - 1) * size + (i - 1) * 5, (3 - 2 * j) * size + (1 - j) * 5,
                           (1 - 2 * k) * size + (1 - k) * 5);
            }
        }
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                if (i == 0) {
                    paint(r.cube[r.left][j][k]);
                } else {
                    paint();
                }
                glVertex3f((2 * i - 3) * size + (i - 1) * 5, (3 - 2 * j) * size + (1 - j) * 5,
                           (2 * k - 3) * size + (k - 1) * 5);
                glVertex3f((2 * i - 3) * size + (i - 1) * 5, (1 - 2 * j) * size + (1 - j) * 5,
                           (2 * k - 3) * size + (k - 1) * 5);
                glVertex3f((2 * i - 3) * size + (i - 1) * 5, (1 - 2 * j) * size + (1 - j) * 5,
                           (2 * k - 1) * size + (k - 1) * 5);
                glVertex3f((2 * i - 3) * size + (i - 1) * 5, (3 - 2 * j) * size + (1 - j) * 5,
                           (2 * k - 1) * size + (k - 1) * 5);
            }
        }
    }
    glEnd();
    glPopMatrix();
}

void display() {
    glPushMatrix();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw();
    glPopMatrix();
    glutSwapBuffers();
}


int main(int argc, char **argv) {
    Rubik s("data.txt");
    s.print();
    s.solve();
    s.print();

    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow("Rubik");
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();
    glOrtho(-WIDTH / 2, WIDTH / 2, -HEIGHT / 2, HEIGHT / 2, -400, 400);
    glutDisplayFunc(display);
    glutTimerFunc(10, Timer, 0);
    glutKeyboardFunc(Keyboard);

    glutMainLoop();

    return 0;
}
