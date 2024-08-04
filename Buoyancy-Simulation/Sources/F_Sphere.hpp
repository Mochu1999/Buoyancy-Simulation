#pragma once

#include "Common.hpp"

//Creates the points of a Fibonacci Sphere with n points and a defined radius
std::vector<p3> addFibSphere(int n, float radius) {
    std::vector<p3> points;
    points.reserve(n);

    //points are separated horizontally at an angle (the golden angle) from the previous point. 
    const float goldenAngle = PI * (3.0 - std::sqrt(5.0));
    float nInv = 1.0f / n;

    for (int i = 0; i < n; ++i) {
        float y = -1.0 + (2.0 * i + 1.0) * nInv; //Points are distributed vertically from -1 to 1, evenly spaced

        // Calculate radius at this height to the vertical axis to ensure points lie on the sphere surface
        float r = std::sqrt(1.0 - y * y);

        float theta = goldenAngle * i;

        float x = r * std::cos(theta);
        float z = r * std::sin(theta);

        points.emplace_back(p3{ x * radius, y * radius, z * radius });
    }

    //printv3(points);
    return points;
}