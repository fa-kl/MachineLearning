#ifndef UTILS_COMPARISON_H
#define UTILS_COMPARISON_H

#include <cmath>
#include <limits>
#include <algorithm>

/**
 * @brief Default epsilon for floating-point comparisons
 */
constexpr double DEFAULT_EPSILON = 1e-9;

/**
 * @brief Default relative tolerance for floating-point comparisons
 */
constexpr double DEFAULT_RELATIVE_TOLERANCE = 1e-9;

/**
 * @brief Compare two doubles using absolute tolerance
 * @param a First value
 * @param b Second value
 * @param epsilon Absolute tolerance (default: DEFAULT_EPSILON)
 * @return true if |a - b| <= epsilon
 */
bool isEqual(double a, double b, double epsilon = DEFAULT_EPSILON);

/**
 * @brief Compare two doubles using relative tolerance
 * @param a First value
 * @param b Second value
 * @param relativeTolerance Relative tolerance (default: DEFAULT_RELATIVE_TOLERANCE)
 * @return true if |a - b| <= relativeTolerance * max(|a|, |b|)
 */
bool isEqualRelative(double a, double b, double relativeTolerance = DEFAULT_RELATIVE_TOLERANCE);

/**
 * @brief Compare two doubles using combined absolute and relative tolerance
 * @param a First value
 * @param b Second value
 * @param absoluteTolerance Absolute tolerance
 * @param relativeTolerance Relative tolerance
 * @return true if values are equal within either tolerance
 */
bool isEqualCombined(double a, double b, double absoluteTolerance = DEFAULT_EPSILON,
                     double relativeTolerance = DEFAULT_RELATIVE_TOLERANCE);

/**
 * @brief Check if first value is less than second with tolerance
 * @param a First value
 * @param b Second value
 * @param epsilon Absolute tolerance (default: DEFAULT_EPSILON)
 * @return true if a < b - epsilon
 */
bool isLess(double a, double b, double epsilon = DEFAULT_EPSILON);

/**
 * @brief Check if first value is less than or equal to second with tolerance
 * @param a First value
 * @param b Second value
 * @param epsilon Absolute tolerance (default: DEFAULT_EPSILON)
 * @return true if a <= b + epsilon
 */
bool isLessOrEqual(double a, double b, double epsilon = DEFAULT_EPSILON);

/**
 * @brief Check if first value is greater than second with tolerance
 * @param a First value
 * @param b Second value
 * @param epsilon Absolute tolerance (default: DEFAULT_EPSILON)
 * @return true if a > b + epsilon
 */
bool isGreater(double a, double b, double epsilon = DEFAULT_EPSILON);

/**
 * @brief Check if first value is greater than or equal to second with tolerance
 * @param a First value
 * @param b Second value
 * @param epsilon Absolute tolerance (default: DEFAULT_EPSILON)
 * @return true if a >= b - epsilon
 */
bool isGreaterOrEqual(double a, double b, double epsilon = DEFAULT_EPSILON);

/**
 * @brief Check if a value is effectively zero
 * @param value Value to check
 * @param epsilon Absolute tolerance (default: DEFAULT_EPSILON)
 * @return true if |value| <= epsilon
 */
bool isZero(double value, double epsilon = DEFAULT_EPSILON);

/**
 * @brief Check if a value is effectively zero using relative tolerance
 * @param value Value to check
 * @param relativeTolerance Relative tolerance (default: DEFAULT_RELATIVE_TOLERANCE)
 * @return true if |value| <= relativeTolerance
 */
bool isZeroRelative(double value, double relativeTolerance = DEFAULT_RELATIVE_TOLERANCE);

/**
 * @brief Get the absolute difference between two values
 * @param a First value
 * @param b Second value
 * @return |a - b|
 */
double absoluteDifference(double a, double b);

/**
 * @brief Get the relative difference between two values
 * @param a First value (reference value)
 * @param b Second value
 * @return |a - b| / |a|, or |a - b| / |b| if a is effectively zero, or 0 if both are zero
 */
double relativeDifference(double a, double b);

/**
 * @brief Check if two values have the same sign
 * @param a First value
 * @param b Second value
 * @return true if both positive, both negative, or both zero
 */
bool sameSign(double a, double b);

/**
 * @brief Clamp a value between min and max with tolerance
 * @param value Value to clamp
 * @param min Minimum value
 * @param max Maximum value
 * @param epsilon Tolerance for boundary checks
 * @return Clamped value
 */
double clamp(double value, double min, double max, double epsilon = DEFAULT_EPSILON);


#endif //UTILS_COMPARISON_H
