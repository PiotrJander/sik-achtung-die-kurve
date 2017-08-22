//
// Created by Piotr Jander on 22/08/2017.
//

#ifndef PROJECT_CRC32C_H
#define PROJECT_CRC32C_H

#include <cstddef>
#include <cstdint>

uint32_t crc32c(uint32_t crc, const unsigned char *buf, size_t len);

#endif //PROJECT_CRC32C_H
