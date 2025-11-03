/*****************************************************************************************
 * @file: MNISTLoader.hpp
 *
 * @brief: MNIST dataset loader for handwritten digit recognition
 *
 * @author: fakl
 * @date: November 2025
 *
 ****************************************************************************************/

#pragma once

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "mathlib.hpp"

class MNISTLoader {
public:
  struct Dataset {
    mathlib::Matrix<double> images;  // Each row is a flattened 28x28 image
    mathlib::Vector<int> labels;     // Corresponding labels (0-9)
  };

  /**
   * @brief Load MNIST training dataset
   * @param data_path Path to the data directory (containing the MNIST files)
   * @return Dataset containing training images and labels
   */
  static Dataset loadTraining(const std::string& data_path = "../data/mnist/");

  /**
   * @brief Load MNIST test dataset
   * @param data_path Path to the data directory (containing the MNIST files)
   * @return Dataset containing test images and labels
   */
  static Dataset loadTest(const std::string& data_path = "../data/mnist/");

  /**
   * @brief Get a single image as a 28x28 matrix
   * @param dataset The loaded dataset
   * @param index Index of the image to extract
   * @return 28x28 matrix representing the image
   */
  static mathlib::Matrix<double> getImage(const Dataset& dataset, size_t index);

  /**
   * @brief Print an image to console (for visualization)
   * @param image 28x28 matrix representing the image
   */
  static void printImage(const mathlib::Matrix<double>& image);

private:
  static uint32_t reverseBytes(uint32_t value);
  static std::vector<uint8_t> loadImageFile(const std::string& filename, uint32_t& num_images, uint32_t& rows,
                                            uint32_t& cols);
  static std::vector<uint8_t> loadLabelFile(const std::string& filename, uint32_t& num_labels);
};