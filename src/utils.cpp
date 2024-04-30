#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "logging.hpp"

std::vector<uint8_t> loadExeFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate); // Open file in binary mode and seek to the end
    if (!file) {
        LOG_ERROR("Failed to open file");
        return std::vector<uint8_t>();
    }

    std::streamsize size = file.tellg(); // Get the size of the file
    file.seekg(0, std::ios::beg); // Seek back to the beginning of the file

    std::vector<uint8_t> buffer(size); // Allocate a buffer of the appropriate size
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) { // Read the file into the buffer
        throw std::runtime_error("Failed to read file");
    }

    return buffer; // Return the buffer containing the file data
}

