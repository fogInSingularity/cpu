#ifndef BIN_FILE_UTILS_H
#define BIN_FILE_UTILS_H

#include <stdio.h>
#include <stdint.h>

typedef uint8_t byte_t;


/// @brief struct that holds file contents
struct BinData {
  byte_t* buf;  ///< buffer that holds file string
  size_t bufSz; ///< size of buffer in bytes
};


/**
 * @brief get data from file
 *
 * @param data FileData struct
 * @param file file to get data from
*/
void GetData(BinData* data, FILE* file);


/**
 * @brief put buffer to file
 *
 * @param data FileData struct
 * @param file file to put buffer to
*/
void PutBuf(BinData* data, FILE* file);


/**
 * @brief free all data members
 *
 * @param data FileData struct
*/
void FreeData(BinData* data);

#endif // BIN_FILE_UTILS_H
