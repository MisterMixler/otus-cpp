#include <cassert>
#include <iostream>

#include "matrix.h"

int main() {
    Matrix<int, 0> matrix;

    for (int i = 0; i < 10; ++i) {
        matrix[i][i] = i;
        matrix[i][9 - i] = 9 - i;
    }

    for (int i = 1; i <= 8; ++i) {
        for (int j = 1; j <= 8; ++j) {
            if (j > 1)
                std::cout << ' ';
            std::cout << matrix[i][j];
        }
        std::cout << '\n';
    }

    std::cout << matrix.size() << '\n';

    for (auto [x, y, v] : matrix) {
        std::cout << '[' << x << ',' << y << "] = " << v << '\n';
    }

    return 0;
}
