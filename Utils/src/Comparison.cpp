#include "Comparison.h"

#include <algorithm>
#include <cmath>
#include <limits>

bool isEqual(const double a, const double b, const double epsilon) {
  // Handle special cases
  if (std::isnan(a) || std::isnan(b)) {
    return false;
  }
  if (std::isinf(a) || std::isinf(b)) {
    return a == b;
  }
  return std::abs(a - b) <= epsilon;
}

bool isEqualRelative(const double a, const double b, const double relativeTolerance) {
  // Handle special cases
  if (std::isnan(a) || std::isnan(b)) {
    return false;
  }
  if (std::isinf(a) || std::isinf(b)) {
    return a == b;
  }
  // If both are zero or very close to zero, use absolute comparison
  if (std::abs(a) < std::numeric_limits<double>::epsilon() && std::abs(b) < std::numeric_limits<double>::epsilon()) {
    return true;
  }
  // Use relative tolerance
  const double maxValue = std::max(std::abs(a), std::abs(b));
  return std::abs(a - b) <= relativeTolerance * maxValue;
}

bool isEqualCombined(const double a, const double b, const double absoluteTolerance, const double relativeTolerance) {
  // Handle special cases
  if (std::isnan(a) || std::isnan(b)) {
    return false;
  }
  if (std::isinf(a) || std::isinf(b)) {
    return a == b;
  }
  // Check absolute tolerance first
  if (std::abs(a - b) <= absoluteTolerance) {
    return true;
  }
  // Check relative tolerance
  const double maxValue = std::max(std::abs(a), std::abs(b));
  return std::abs(a - b) <= relativeTolerance * maxValue;
}

bool isLess(const double a, const double b, const double epsilon) {
  return a < b - epsilon;
}

bool isLessOrEqual(const double a, const double b, const double epsilon) {
  return a <= b + epsilon;
}

bool isGreater(const double a, const double b, const double epsilon) {
  return a > b + epsilon;
}

bool isGreaterOrEqual(const double a, const double b, const double epsilon) {
  return a >= b - epsilon;
}

bool isZero(const double value, const double epsilon) {
  return std::abs(value) <= epsilon;
}

bool isZeroRelative(const double value, const double relativeTolerance) {
  return std::abs(value) <= relativeTolerance;
}

double absoluteDifference(const double a, const double b) {
  return std::abs(a - b);
}

double relativeDifference(const double a, const double b) {
  if (std::isnan(a) || std::isnan(b)) {
    return std::numeric_limits<double>::quiet_NaN();
  }

  if (std::isinf(a) || std::isinf(b)) {
    if (a == b) {
      return 0.0;
    }
    return std::numeric_limits<double>::infinity();
  }

  // If both values are effectively zero
  if (std::abs(a) < std::numeric_limits<double>::epsilon() && std::abs(b) < std::numeric_limits<double>::epsilon()) {
    return 0.0;
  }

  // If the first value is effectively zero, use the second value as reference
  if (std::abs(a) < std::numeric_limits<double>::epsilon()) {
    return std::abs(a - b) / std::abs(b);
  }

  // Calculate relative difference using the first value as reference
  return std::abs(a - b) / std::abs(a);
}

bool sameSign(const double a, const double b) {
  // Handle zero cases
  if (isZero(a) && isZero(b)) {
    return true;
  }
  // Check if both have same sign
  return (a >= 0 && b >= 0) || (a < 0 && b < 0);
}

double clamp(const double value, double min, double max, const double epsilon) {
  // Ensure min <= max
  if (min > max) {
    std::swap(min, max);
  }
  // Check if value is within tolerance of boundaries
  if (isLessOrEqual(value, min, epsilon)) {
    return min;
  }
  if (isGreaterOrEqual(value, max, epsilon)) {
    return max;
  }
  return value;
}