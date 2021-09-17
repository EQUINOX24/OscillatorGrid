#pragma once

#include <iostream>
#include <cmath>

struct Vec2 {
    float x, y;
    
    Vec2() {};
    
    Vec2(float x, float y) : x(x), y(y) {};
    
    float Norm() const {
        return sqrt(x*x + y*y);
    }
    
    float Quadrance() const {
        return x*x + y*y;
    }
};

Vec2 operator+(const Vec2& left, const Vec2& right) {
      return Vec2(left.x + right.x, left.y + right.y);
}

Vec2 operator-(const Vec2& left, const Vec2& right) {
      return Vec2(left.x - right.x, left.y - right.y);
}

Vec2 operator*(const float& left, const Vec2& right) {
    return Vec2(left*right.x, left*right.y);
}

Vec2 operator*(const Vec2& left, const float& right) {
      return Vec2(left.x*right, left.y / right);
}

Vec2 operator/(const Vec2& left, const float& right) {
      return Vec2(left.x / right, left.y / right);
}

// dot product
float operator*(const Vec2& left, const Vec2& right) {
    return left.x*right.x + left.y*right.y;
}

// cross product
float operator^(const Vec2& left, const Vec2& right) {
    return left.x*right.y - left.y*right.x;
}

Vec2 operator/(const float& left, const Vec2& right) {
    // return left*right / (right*right); // simpler implementation
    float normSq = right.x*right.x + right.y*right.y;
    return Vec2(left*right.x / normSq, left*right.y / normSq);
}

void operator+=(Vec2& left, const Vec2& right) {
    left.x += right.x;
    left.y += right.y;
}

void operator-=(Vec2& left, const Vec2& right) {
    left.x -= right.x;
    left.y -= right.y;
}

void operator*=(Vec2& left, const float& right) {
    left.x *= right;
    left.y *= right;
}

void operator/=(Vec2& left, const float& right) {
    left.x /= right;
    left.y /= right;
}

bool operator==(const Vec2& left, const Vec2& right) {
    return left.x == right.x && left.y == right.y;
}

bool operator!=(const Vec2& left, const Vec2& right) {
    return left.x != right.x || left.y != right.y;
}

std::ostream& operator<<(std::ostream& stream, const Vec2& other) {
    stream << '(' << other.x << ", " << other.y << ')';
    return stream;
}