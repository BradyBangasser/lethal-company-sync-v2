#include <openssl/evp.h>
#include <openssl/evperr.h>
#include "../constants.hpp"
#include "utils.h"
#include <string.h>
#include <stdint.h>

#define FILE_BUFFER_LENGTH 1024

int hash(const EVP_MD *algorithm, uint8_t *buffer, const char *msg, uint32_t *hashLen) {
    if (algorithm == NULL) return INVALID_ALGORITHM;
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    int result;

    result = EVP_DigestInit_ex(ctx, algorithm, NULL);
    if (result == 0) {
        EVP_MD_CTX_free(ctx);
        return ALGORITHM_INIT_ERROR;
    }

    result = EVP_DigestUpdate(ctx, msg, strlen(msg));
    if (result == 0) {
        EVP_MD_CTX_free(ctx);
        return ALGORITHM_UPDATE_ERROR;
    }

    result = EVP_DigestFinal_ex(ctx, buffer, hashLen);

    EVP_MD_CTX_free(ctx);

    if (result == 0) {
        return ALGORITHM_FINAL_ERROR;
    }

    return ALL_GOOD;
}

int hashFile(const EVP_MD *algorithm, uint8_t *result, const char *filePath, uint32_t *hashLen) {
    if (algorithm == NULL) return INVALID_ALGORITHM;
    int res;
    size_t n;
    char buffer[FILE_BUFFER_LENGTH] = { 0 };

    FILE *f = fopen(filePath, "rb");

    if (f == NULL) {
        return FAILED_TO_OPEN_FILE;
    }

    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    res = EVP_DigestInit_ex(ctx, algorithm, NULL);

    if (res == 0) {
        fclose(f);
        EVP_MD_CTX_free(ctx);
        return ALGORITHM_INIT_ERROR;
    }

    while ((n = fread(buffer, 1, sizeof(buffer), f))) {
        res = EVP_DigestUpdate(ctx, buffer, n);

        if (res == 0) {
            EVP_MD_CTX_free(ctx);
            fclose(f);
            return ALGORITHM_UPDATE_ERROR;
        }
    }

    res = EVP_DigestFinal_ex(ctx, result, hashLen);

    EVP_MD_CTX_free(ctx);
    fclose(f);

    if (res == 0) {
        return ALGORITHM_FINAL_ERROR;
    }

    return ALL_GOOD;
}