/*****************************************************************************************
 * @file: MNISTLoader.cpp
 *
 * @brief: MNIST dataset loader implementation
 *
 * @author: fakl
 * @date: November 2025
 *
 ****************************************************************************************/

#include "MNISTLoader.hpp"

#include <algorithm>
#include <stdexcept>

uint32_t MNISTLoader::reverseBytes(uint32_t value) {
  return ((value & 0xFF000000) >> 24) | ((value & 0x00FF0000) >> 8) | ((value & 0x0000FF00) << 8) |
         ((value & 0x000000FF) << 24);
}

std::vector<uint8_t> MNISTLoader::loadImageFile(const std::string& filename, uint32_t& num_images, uint32_t& rows,
                                                uint32_t& cols) {
  std::ifstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("Cannot open file: " + filename);
  }

  // Read header
  uint32_t magic_number;
  file.read(reinterpret_cast<char*>(&magic_number), 4);
  magic_number = reverseBytes(magic_number);

  if (magic_number != 0x00000803) {
    throw std::runtime_error("Invalid MNIST image file format");
  }

  file.read(reinterpret_cast<char*>(&num_images), 4);
  file.read(reinterpret_cast<char*>(&rows), 4);
  file.read(reinterpret_cast<char*>(&cols), 4);

  num_images = reverseBytes(num_images);
  rows = reverseBytes(rows);
  cols = reverseBytes(cols);

  // Read image data
  size_t image_size = rows * cols;
  size_t total_size = num_images * image_size;
  std::vector<uint8_t> data(total_size);

  file.read(reinterpret_cast<char*>(data.data()), static_cast<std::streamsize>(total_size));
  file.close();

  return data;
}

std::vector<uint8_t> MNISTLoader::loadLabelFile(const std::string& filename, uint32_t& num_labels) {
  std::ifstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("Cannot open file: " + filename);
  }

  // Read header
  uint32_t magic_number;
  file.read(reinterpret_cast<char*>(&magic_number), 4);
  magic_number = reverseBytes(magic_number);

  if (magic_number != 0x00000801) {
    throw std::runtime_error("Invalid MNIST label file format");
  }

  file.read(reinterpret_cast<char*>(&num_labels), 4);
  num_labels = reverseBytes(num_labels);

  // Read label data
  std::vector<uint8_t> labels(num_labels);
  file.read(reinterpret_cast<char*>(labels.data()), static_cast<std::streamsize>(num_labels));
  file.close();

  return labels;
}

MNISTLoader::Dataset MNISTLoader::loadTraining(const std::string& data_path) {
  uint32_t num_images, rows, cols, num_labels;

  auto image_data = loadImageFile(data_path + "train-images-idx3-ubyte", num_images, rows, cols);
  auto label_data = loadLabelFile(data_path + "train-labels-idx1-ubyte", num_labels);

  if (num_images != num_labels) {
    throw std::runtime_error("Mismatch between number of images and labels");
  }

  // Convert to MathLib format
  size_t image_size = rows * cols;
  mathlib::Matrix<double> images(num_images, image_size);
  mathlib::Vector<int> labels(num_labels);

  // Fill images matrix (each row is a flattened image)
  for (size_t i = 0; i < num_images; ++i) {
    for (size_t j = 0; j < image_size; ++j) {
      // Normalize pixel values to [0, 1]
      images(i + 1, j + 1) = static_cast<double>(image_data[i * image_size + j]) / 255.0;
    }
    labels[i] = static_cast<int>(label_data[i]);
  }

  return {std::move(images), std::move(labels)};
}

MNISTLoader::Dataset MNISTLoader::loadTest(const std::string& data_path) {
  uint32_t num_images, rows, cols, num_labels;

  auto image_data = loadImageFile(data_path + "t10k-images-idx3-ubyte", num_images, rows, cols);
  auto label_data = loadLabelFile(data_path + "t10k-labels-idx1-ubyte", num_labels);

  if (num_images != num_labels) {
    throw std::runtime_error("Mismatch between number of images and labels");
  }

  // Convert to MathLib format
  size_t image_size = rows * cols;
  mathlib::Matrix<double> images(num_images, image_size);
  mathlib::Vector<int> labels(num_labels);

  // Fill images matrix (each row is a flattened image)
  for (size_t i = 0; i < num_images; ++i) {
    for (size_t j = 0; j < image_size; ++j) {
      // Normalize pixel values to [0, 1]
      images(i + 1, j + 1) = static_cast<double>(image_data[i * image_size + j]) / 255.0;
    }
    labels[i] = static_cast<int>(label_data[i]);
  }

  return {std::move(images), std::move(labels)};
}

mathlib::Matrix<double> MNISTLoader::getImage(const Dataset& dataset, size_t index) {
  if (index >= dataset.labels.length()) {
    throw std::out_of_range("Image index out of range");
  }

  mathlib::Matrix<double> image(28, 28);
  for (size_t i = 0; i < 28; ++i) {
    for (size_t j = 0; j < 28; ++j) {
      image(i + 1, j + 1) = dataset.images(index + 1, i * 28 + j + 1);
    }
  }
  return image;
}

void MNISTLoader::printImage(const mathlib::Matrix<double>& image) {
  std::cout << "┌";
  for (size_t j = 0; j < 28; ++j) std::cout << "─";
  std::cout << "┐" << std::endl;

  for (size_t i = 1; i <= 28; ++i) {
    std::cout << "│";
    for (size_t j = 1; j <= 28; ++j) {
      double pixel = image(i, j);
      if (pixel < 0.1)
        std::cout << " ";
      else if (pixel < 0.3)
        std::cout << "·";
      else if (pixel < 0.5)
        std::cout << "▪";
      else if (pixel < 0.7)
        std::cout << "▫";
      else if (pixel < 0.9)
        std::cout << "█";
      else
        std::cout << "█";
    }
    std::cout << "│" << std::endl;
  }

  std::cout << "└";
  for (size_t j = 0; j < 28; ++j) std::cout << "─";
  std::cout << "┘" << std::endl;
}