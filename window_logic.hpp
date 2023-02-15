#ifndef JOCUL_PATRATELOR_GLISANTE_WINDOW_LOGIC_HPP
#define JOCUL_PATRATELOR_GLISANTE_WINDOW_LOGIC_HPP

struct aspectRatio {
    short xAxis{};
    short yAxis{};
};

long long greatestCommonDivisor (long long num1, long long num2);
aspectRatio getAspectRatio (short xAxisLength, short yAxisLength);

#endif //JOCUL_PATRATELOR_GLISANTE_WINDOW_LOGIC_HPP
