// Team F.I.R.E. - University of Idaho Computer Science (2019-2020)
// File: Crypto.h
// Functionality: Library to handle encryption/decryption of messages.
// Notes:
// Adapted by Brandon Foss from https://github.com/kokke/tiny-AES-c
// Encryption and Decryption is AES-128 bit utilizing the CBC mode of operation.
// The Message must be padded to an interval of 16 bytes by appending ^'s onto the end of a message.
// The encryption utilizes a 16 byte (128 bit) symmetric key and IV (initialization vector).

#ifndef CRYPTO_H
#define CRYPTO_H

// Libraries:
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Arduino.h>

// Defines:
#define AES_BLOCKLEN 16 // Defines the block length in bytes - 16 bytes = 128 bits
#define AES_KEYLEN 16   // Defines the key length in bytes - 16 bytes = 128 bits
#define AES_KEYEXPSIZE 176 // Defines the key expansion size
#define NB 4 // Defines the number of columns comprising a state in AES
#define NK 4 // Defines the number of 32 bit words in a key
#define NR 10 // Defines the number of rounds in AES the cipher
#define getSBoxValue(num) (sbox[(num)]) // Retrieves substitution box value
#define getSBoxInvert(num) (rsbox[(num)]) // Retrieves the inverse substitution box value
// Defines method used to multiply numbers in the field GF(2^8)
#define Multiply(x, y)                                \
      (  ((y & 1) * x) ^                              \
      ((y>>1 & 1) * xtime(x)) ^                       \
      ((y>>2 & 1) * xtime(xtime(x))) ^                \
      ((y>>3 & 1) * xtime(xtime(xtime(x)))) ^         \
      ((y>>4 & 1) * xtime(xtime(xtime(xtime(x))))))   \

// Crypto class provides encryption / decryption functionality to the Iridium 9523
class Crypto
{
    public:
        // Methods:
        Crypto();
        uint8_t* encrypt_cbc(uint8_t* str);
        uint8_t* decrypt_cbc(uint8_t* str);
        int compareArrays(uint8_t arr1[], uint8_t arr2[], int size);
        
    private:
        // Variables:
        uint8_t key[16] = { 0x74, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20, 0x74, 0x65, 0x73, 0x74, 
            0x20, 0x6b, 0x65, 0x79 }; // Key is 16 Bytes (128 bits)
        uint8_t iv[16]  = { 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x70, 0x71, 0x72, 
            0x73, 0x74, 0x75, 0x76 }; // Initialization vector is 16 Bytes (128 bits)
        typedef uint8_t state_t[4][4];  

        // Methods
        void AES_init_ctx(struct AES_ctx* ctx, const uint8_t* key);
        void AES_init_ctx_iv(struct AES_ctx* ctx, const uint8_t* key, const uint8_t* iv);
        void AES_ctx_set_iv(struct AES_ctx* ctx, const uint8_t* iv);
        void trimMessage(uint8_t* str);
        uint8_t* padString(uint8_t* str);
        uint8_t* removeStringPadding(uint8_t* str);
        void AES_CBC_encrypt_buffer(struct AES_ctx* ctx, uint8_t* buf, uint32_t length);
        void AES_CBC_decrypt_buffer(struct AES_ctx* ctx, uint8_t* buf, uint32_t length);
        void KeyExpansion(uint8_t* RoundKey, const uint8_t* Key);
        void AddRoundKey(uint8_t round, state_t* state, const uint8_t* RoundKey);
        void SubBytes(state_t* state);
        void ShiftRows(state_t* state);
        uint8_t xtime(uint8_t x);
        void MixColumns(state_t* state);
        void InvMixColumns(state_t* state);
        void InvSubBytes(state_t* state);
        void InvShiftRows(state_t* state);
        void Cipher(state_t* state, const uint8_t* RoundKey);
        void InvCipher(state_t* state, const uint8_t* RoundKey);
        void XorWithIv(uint8_t* buf, const uint8_t* Iv);
        char* strToCharArr(String str);
};

#endif // CRYPTO_H
