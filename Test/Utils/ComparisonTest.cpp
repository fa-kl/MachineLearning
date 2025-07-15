#include "Comparison.h"
#include <gtest/gtest.h>
#include <cmath>
#include <limits>

TEST(ComparisonTest, BasicEquality) {
    // Test exact equality
    EXPECT_TRUE(isEqual(1.0, 1.0));
    EXPECT_TRUE(isEqual(0.0, 0.0));
    EXPECT_TRUE(isEqual(-1.0, -1.0));

    // Test within tolerance
    EXPECT_TRUE(isEqual(1.0, 1.0 + 1e-10));
    EXPECT_TRUE(isEqual(1.0, 1.0 - 1e-10));

    // Test outside tolerance
    EXPECT_FALSE(isEqual(1.0, 1.1));
    EXPECT_FALSE(isEqual(1.0, 1.0 + 1e-8));
}

TEST(ComparisonTest, RelativeEquality) {
    // Test small numbers
    EXPECT_TRUE(isEqualRelative(1e-10, 1e-10));
    EXPECT_TRUE(isEqualRelative(1e-10, 1.1e-10, 0.2));

    // Test large numbers
    EXPECT_TRUE(isEqualRelative(1e10, 1e10 * (1 + 1e-10)));
    EXPECT_FALSE(isEqualRelative(1e10, 1e10 * (1 + 1e-8)));

    // Test zero handling
    EXPECT_TRUE(isEqualRelative(0.0, 0.0));
    EXPECT_TRUE(isEqualRelative(1e-20, 1e-20));
}

TEST(ComparisonTest, CombinedEquality) {
    // Should pass absolute tolerance test
    EXPECT_TRUE(isEqualCombined(1e-15, 2e-15, 1e-14, 1e-6));

    // Should pass relative tolerance test
    EXPECT_TRUE(isEqualCombined(1e10, 1e10 * (1 + 1e-10), 1e-5, 1e-9));

    // Should fail both tests
    EXPECT_FALSE(isEqualCombined(1.0, 2.0, 1e-9, 1e-9));
}

TEST(ComparisonTest, LessThanComparison) {
    EXPECT_TRUE(isLess(1.0, 2.0));
    EXPECT_FALSE(isLess(2.0, 1.0));
    EXPECT_FALSE(isLess(1.0, 1.0 + 1e-10)); // Within tolerance
}

TEST(ComparisonTest, LessThanOrEqualComparison) {
    EXPECT_TRUE(isLessOrEqual(1.0, 2.0));
    EXPECT_TRUE(isLessOrEqual(1.0, 1.0));
    EXPECT_TRUE(isLessOrEqual(1.0, 1.0 + 1e-10));
    EXPECT_FALSE(isLessOrEqual(2.0, 1.0));
}

TEST(ComparisonTest, GreaterThanComparison) {
    EXPECT_TRUE(isGreater(2.0, 1.0));
    EXPECT_FALSE(isGreater(1.0, 2.0));
    EXPECT_FALSE(isGreater(1.0, 1.0 - 1e-10)); // Within tolerance
}

TEST(ComparisonTest, GreaterThanOrEqualComparison) {
    EXPECT_TRUE(isGreaterOrEqual(2.0, 1.0));
    EXPECT_TRUE(isGreaterOrEqual(1.0, 1.0));
    EXPECT_TRUE(isGreaterOrEqual(1.0, 1.0 - 1e-10));
    EXPECT_FALSE(isGreaterOrEqual(1.0, 2.0));
}

TEST(ComparisonTest, ZeroChecks) {
    // Test absolute zero
    EXPECT_TRUE(isZero(0.0));
    EXPECT_TRUE(isZero(1e-10));
    EXPECT_FALSE(isZero(1e-8));

    // Test relative zero
    EXPECT_TRUE(isZeroRelative(0.0));
    EXPECT_TRUE(isZeroRelative(1e-10));
    EXPECT_FALSE(isZeroRelative(1e-8));
}

TEST(ComparisonTest, AbsoluteDifference) {
    EXPECT_NEAR(absoluteDifference(3.0, 1.0), 2.0, 1e-15);
    EXPECT_NEAR(absoluteDifference(1.0, 3.0), 2.0, 1e-15);
    EXPECT_NEAR(absoluteDifference(-1.0, 1.0), 2.0, 1e-15);
    EXPECT_NEAR(absoluteDifference(0.0, 0.0), 0.0, 1e-15);
}

TEST(ComparisonTest, RelativeDifference) {
  EXPECT_NEAR(relativeDifference(100.0, 110.0), 0.1, 1e-14);
  EXPECT_NEAR(relativeDifference(1.0, 1.1), 0.1, 1e-14);
  EXPECT_NEAR(relativeDifference(0.0, 0.0), 0.0, 1e-15);

  // Test with very small numbers
  EXPECT_NEAR(relativeDifference(1e-10, 1.1e-10), 0.1, 1e-14);

  // Test with zero as first value
  EXPECT_NEAR(relativeDifference(0.0, 1.0), 1.0, 1e-14);

  // Test symmetric cases
  EXPECT_NEAR(relativeDifference(1.0, 0.9), 0.1, 1e-14);
}


TEST(ComparisonTest, InfinityHandling) {
    double inf = std::numeric_limits<double>::infinity();

    // Test infinity equality
    EXPECT_TRUE(isEqual(inf, inf));
    EXPECT_TRUE(isEqual(-inf, -inf));
    EXPECT_FALSE(isEqual(inf, -inf));
    EXPECT_FALSE(isEqual(inf, 1.0));

    // Test relative comparison with infinity
    EXPECT_TRUE(isEqualRelative(inf, inf));
    EXPECT_FALSE(isEqualRelative(inf, -inf));
}

TEST(ComparisonTest, NaNHandling) {
    double nan = std::numeric_limits<double>::quiet_NaN();

    // Test NaN - should always return false for equality
    EXPECT_FALSE(isEqual(nan, nan));
    EXPECT_FALSE(isEqual(nan, 1.0));
    EXPECT_FALSE(isEqual(1.0, nan));

    // Test relative comparison with NaN
    EXPECT_FALSE(isEqualRelative(nan, nan));
    EXPECT_FALSE(isEqualRelative(nan, 1.0));

    // Test relative difference with NaN
    EXPECT_TRUE(std::isnan(relativeDifference(nan, 1.0)));
    EXPECT_TRUE(std::isnan(relativeDifference(1.0, nan)));
}

TEST(ComparisonTest, SameSignChecks) {
    // Test same positive signs
    EXPECT_TRUE(sameSign(1.0, 2.0));
    EXPECT_TRUE(sameSign(0.1, 100.0));

    // Test same negative signs
    EXPECT_TRUE(sameSign(-1.0, -2.0));
    EXPECT_TRUE(sameSign(-0.1, -100.0));

    // Test zero cases
    EXPECT_TRUE(sameSign(0.0, 0.0));
    EXPECT_TRUE(sameSign(0.0, 1e-10));
    EXPECT_TRUE(sameSign(1e-10, 0.0));

    // Test different signs
    EXPECT_FALSE(sameSign(1.0, -1.0));
    EXPECT_FALSE(sameSign(-1.0, 1.0));
}

TEST(ComparisonTest, ClampFunction) {
    // Test normal clamping
    EXPECT_NEAR(clamp(5.0, 0.0, 10.0), 5.0, 1e-15);
    EXPECT_NEAR(clamp(-5.0, 0.0, 10.0), 0.0, 1e-15);
    EXPECT_NEAR(clamp(15.0, 0.0, 10.0), 10.0, 1e-15);

    // Test with tolerance
    EXPECT_NEAR(clamp(1e-10, 0.0, 10.0, 1e-9), 0.0, 1e-15);
    EXPECT_NEAR(clamp(10.0 - 1e-10, 0.0, 10.0, 1e-9), 10.0, 1e-15);

    // Test swapped min/max (should be handled internally)
    EXPECT_NEAR(clamp(5.0, 10.0, 0.0), 5.0, 1e-15);
}

TEST(ComparisonTest, VerySmallNumbers) {
    double tiny = std::numeric_limits<double>::epsilon();

    EXPECT_TRUE(isEqual(tiny, tiny));
    EXPECT_TRUE(isEqualRelative(tiny, tiny * 1.1, 0.2));

    // Test denormal numbers
    double denormal = std::numeric_limits<double>::denorm_min();
    EXPECT_TRUE(isEqual(denormal, denormal));
}

TEST(ComparisonTest, VeryLargeNumbers) {
    double huge = std::numeric_limits<double>::max() / 2.0;

    EXPECT_TRUE(isEqual(huge, huge));
    EXPECT_TRUE(isEqualRelative(huge, huge * (1 + 1e-15)));
    EXPECT_FALSE(isEqualRelative(huge, huge * (1 + 1e-8)));
}

TEST(ComparisonTest, CustomTolerances) {
    // Test with custom absolute tolerance
    EXPECT_TRUE(isEqual(1.0, 1.05, 0.1));
    EXPECT_FALSE(isEqual(1.0, 1.05, 0.01));

    // Test with custom relative tolerance
    EXPECT_TRUE(isEqualRelative(100.0, 105.0, 0.1));
    EXPECT_FALSE(isEqualRelative(100.0, 105.0, 0.01));
}

TEST(ComparisonTest, ZeroVsSmallNumbers) {
    // Test comparison between zero and very small numbers
    EXPECT_TRUE(isEqual(0.0, 1e-10));
    EXPECT_FALSE(isEqual(0.0, 1e-8));

    // Test relative comparison with zero
    EXPECT_TRUE(isEqualRelative(0.0, 0.0));
    EXPECT_TRUE(isEqualRelative(1e-20, 1e-20));
}