#include "window_logic.hpp"

long long greatestCommonDivisor (long long num1, long long num2) {
    if (num1 < num2) {
        long long aux { num1 };
        num1 = num2;
        num2 = aux;
    }

    while (num2 != 0) {
        long long aux { num1 };

        num1 = num2;
        num2 = aux % num2;
    }

    return num1;
}

aspectRatio getAspectRatio (short xAxisLength, short yAxisLength) {
    auto gcd = static_cast<short>(greatestCommonDivisor(xAxisLength, yAxisLength));

    return {static_cast<short>(xAxisLength / gcd), static_cast<short>(yAxisLength / gcd)};
}
