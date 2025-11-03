
#include <iostream>

#include "MNISTLoader.hpp"
#include "mathlib.hpp"

using namespace mathlib;

int main(int argc, char** argv) {
  std::cout << "=== MNIST Dataset Demo ===" << std::endl;

  try {
    // Load MNIST training data
    std::cout << "Loading MNIST training dataset..." << std::endl;
    auto train_data = MNISTLoader::loadTraining();
    std::cout << "✓ Loaded " << train_data.images.rows() << " training images" << std::endl;

    // Load MNIST test data
    std::cout << "Loading MNIST test dataset..." << std::endl;
    auto test_data = MNISTLoader::loadTest();
    std::cout << "✓ Loaded " << test_data.images.rows() << " test images" << std::endl;

    // Display some statistics
    std::cout << "\n=== Dataset Statistics ===" << std::endl;
    std::cout << "Training set: " << train_data.images.rows() << " samples" << std::endl;
    std::cout << "Test set: " << test_data.images.rows() << " samples" << std::endl;
    std::cout << "Image dimensions: 28x28 pixels (" << train_data.images.cols() << " features)" << std::endl;

    // Show first few samples
    std::cout << "\n=== Sample Images ===" << std::endl;
    for (size_t i = 0; i < 30 && i < train_data.labels.length(); ++i) {
      std::cout << "\nSample " << i + 1 << " - Label: " << train_data.labels[i] << std::endl;
      auto image = MNISTLoader::getImage(train_data, i);
      MNISTLoader::printImage(image);
    }

    // Basic statistics using MathLib
    std::cout << "\n=== Mathematical Analysis ===" << std::endl;

    // Get first image as vector for analysis
    Vector<double> first_image(784);
    for (size_t j = 1; j <= 784; ++j) {
      first_image(j) = train_data.images(1, j);
    }

    std::cout << "First image statistics:" << std::endl;
    std::cout << "  Mean pixel value: " << mean(first_image) << std::endl;
    std::cout << "  Standard deviation: " << mathlib::std(first_image) << std::endl;
    std::cout << "  Min pixel value: " << min(first_image) << std::endl;
    std::cout << "  Max pixel value: " << max(first_image) << std::endl;

    // Count digits in training set
    std::cout << "\n=== Digit Distribution ===" << std::endl;
    std::vector<int> digit_counts(10, 0);
    for (size_t i = 0; i < train_data.labels.length(); ++i) {
      digit_counts[static_cast<size_t>(train_data.labels[i])]++;
    }

    for (int digit = 0; digit <= 9; ++digit) {
      std::cout << "Digit " << digit << ": " << digit_counts[static_cast<size_t>(digit)] << " samples" << std::endl;
    }

    std::cout << "\n✓ MNIST dataset successfully loaded and analyzed!" << std::endl;
    std::cout << "Ready for machine learning algorithms!" << std::endl;

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}