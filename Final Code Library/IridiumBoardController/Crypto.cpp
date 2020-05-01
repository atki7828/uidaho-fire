// Team F.I.R.E. - University of Idaho Computer Science (2019-2020)
// File: Crypto.cpp
// Functionality: Library to handle encryption/decryption of messages.
// Notes:
// Adapted by Brandon Foss from https://github.com/kokke/tiny-AES-c
// Encryption and Decryption is AES-128 bit utilizing CBC mode of operation.
// The Message must be padded to an interval of 16 bytes by appending ^'s onto the end of a message.
// The encryption utilizes a 16 byte (128 bit) symmetric key and IV (initialization vector).

// Libraries:
#include "Crypto.h"

//#define SerialUSB Serial

// Struct to hold the roundkey and initialization vectors for encryption/decryption
struct AES_ctx
{
    uint8_t RoundKey[AES_KEYEXPSIZE];
    uint8_t Iv[AES_BLOCKLEN]; // Initialization vector of blocklen size
};

// Called on class creation
Crypto::Crypto()
{
    // Do nothing
}

// Initialize all necessary variables for cryptographic capabilites
void Crypto::init()
{
    // Do nothing
}

// This function produces Nb(Nr+1) round keys. The round keys are used in each round to decrypt the states.
void Crypto::KeyExpansion(uint8_t* RoundKey, const uint8_t* Key)
{
    unsigned i, j, k;
    uint8_t tempa[4]; // Used for the column/row operations
    // The round constant word array, Rcon[i], contains the values given by 
    // x to the power (i-1) being powers of x (x is denoted as {02}) in the field GF(2^8)
    uint8_t Rcon[11] = { 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 };
    // The substitution box
        uint8_t sbox[256] = {
        //0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
        0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
        0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
        0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
        0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
        0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
        0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
        0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
        0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
        0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
        0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
        0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
        0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
        0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
        0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
        0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
        0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };
    
    // The first round key is the key itself.
    for (i = 0; i < NK; ++i)
    {
        RoundKey[(i * 4) + 0] = Key[(i * 4) + 0];
        RoundKey[(i * 4) + 1] = Key[(i * 4) + 1];
        RoundKey[(i * 4) + 2] = Key[(i * 4) + 2];
        RoundKey[(i * 4) + 3] = Key[(i * 4) + 3];
    }

    // All other round keys are found from the previous round keys.
    for (i = NK; i < NB * (NR + 1); ++i)
    {
        {
        k = (i - 1) * 4;
        tempa[0]=RoundKey[k + 0];
        tempa[1]=RoundKey[k + 1];
        tempa[2]=RoundKey[k + 2];
        tempa[3]=RoundKey[k + 3];
        }

        if (i % NK == 0)
        {
        // This function shifts the 4 bytes in a word to the left once.
        // [a0,a1,a2,a3] becomes [a1,a2,a3,a0]

        // Function RotWord()
        {
            const uint8_t u8tmp = tempa[0];
            tempa[0] = tempa[1];
            tempa[1] = tempa[2];
            tempa[2] = tempa[3];
            tempa[3] = u8tmp;
        }

        // SubWord() is a function that takes a four-byte input word and 
        // applies the S-box to each of the four bytes to produce an output word.

        // Function Subword()
        {
            tempa[0] = getSBoxValue(tempa[0]);
            tempa[1] = getSBoxValue(tempa[1]);
            tempa[2] = getSBoxValue(tempa[2]);
            tempa[3] = getSBoxValue(tempa[3]);
        }

        tempa[0] = tempa[0] ^ Rcon[i/NK];
        }

        j = i * 4; k=(i - NK) * 4;
        RoundKey[j + 0] = RoundKey[k + 0] ^ tempa[0];
        RoundKey[j + 1] = RoundKey[k + 1] ^ tempa[1];
        RoundKey[j + 2] = RoundKey[k + 2] ^ tempa[2];
        RoundKey[j + 3] = RoundKey[k + 3] ^ tempa[3];
    }
}

// Initialize the struct / round key within
void Crypto::AES_init_ctx(struct AES_ctx* ctx, const uint8_t* key)
{
    KeyExpansion(ctx->RoundKey, key);
}

// Create the initialization vector
void Crypto::AES_init_ctx_iv(struct AES_ctx* ctx, const uint8_t* key, const uint8_t* iv)
{
    KeyExpansion(ctx->RoundKey, key);
    memcpy (ctx->Iv, iv, AES_BLOCKLEN);
}

// Assigns the initialization vector to our struct
void Crypto::AES_ctx_set_iv(struct AES_ctx* ctx, const uint8_t* iv)
{
    memcpy (ctx->Iv, iv, AES_BLOCKLEN);
}

// This function adds the round key to state
// The round key is added to the state by an XOR function
void Crypto::AddRoundKey(uint8_t round, state_t* state, const uint8_t* RoundKey)
{
    uint8_t i,j;
    for (i = 0; i < 4; ++i)
    {
        for (j = 0; j < 4; ++j)
        {
            (*state)[i][j] ^= RoundKey[(round * NB * 4) + (i * NB) + j];
        }
    }
}

// The SubBytes Function Substitutes the values in the
// state matrix with values in an S-box.
void Crypto::SubBytes(state_t* state)
{
    // The substitution box
        uint8_t sbox[256] = {
        //0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
        0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
        0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
        0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
        0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
        0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
        0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
        0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
        0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
        0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
        0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
        0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
        0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
        0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
        0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
        0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
        0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };
    uint8_t i, j;
    
    for (i = 0; i < 4; ++i)
    {
        for (j = 0; j < 4; ++j)
        {
            (*state)[j][i] = getSBoxValue((*state)[j][i]);
        }
    }
}


// The ShiftRows() function shifts the rows in the state to the left.
// Each row is shifted with different offset.
// Offset = Row number. So the first row is not shifted.
void Crypto::ShiftRows(state_t* state)
{
    uint8_t temp;

    // Rotate first row 1 columns to left  
    temp           = (*state)[0][1];
    (*state)[0][1] = (*state)[1][1];
    (*state)[1][1] = (*state)[2][1];
    (*state)[2][1] = (*state)[3][1];
    (*state)[3][1] = temp;

    // Rotate second row 2 columns to left  
    temp           = (*state)[0][2];
    (*state)[0][2] = (*state)[2][2];
    (*state)[2][2] = temp;

    temp           = (*state)[1][2];
    (*state)[1][2] = (*state)[3][2];
    (*state)[3][2] = temp;

    // Rotate third row 3 columns to left
    temp           = (*state)[0][3];
    (*state)[0][3] = (*state)[3][3];
    (*state)[3][3] = (*state)[2][3];
    (*state)[2][3] = (*state)[1][3];
    (*state)[1][3] = temp;
}

uint8_t Crypto::xtime(uint8_t x)
{
    return ((x<<1) ^ (((x>>7) & 1) * 0x1b));
}

// MixColumns function mixes the columns of the state matrix
void Crypto::MixColumns(state_t* state)
{
    uint8_t i;
    uint8_t Tmp, Tm, t;
    for (i = 0; i < 4; ++i)
    {  
        t   = (*state)[i][0];
        Tmp = (*state)[i][0] ^ (*state)[i][1] ^ (*state)[i][2] ^ (*state)[i][3] ;
        Tm  = (*state)[i][0] ^ (*state)[i][1] ; Tm = xtime(Tm);  (*state)[i][0] ^= Tm ^ Tmp ;
        Tm  = (*state)[i][1] ^ (*state)[i][2] ; Tm = xtime(Tm);  (*state)[i][1] ^= Tm ^ Tmp ;
        Tm  = (*state)[i][2] ^ (*state)[i][3] ; Tm = xtime(Tm);  (*state)[i][2] ^= Tm ^ Tmp ;
        Tm  = (*state)[i][3] ^ t ;              Tm = xtime(Tm);  (*state)[i][3] ^= Tm ^ Tmp ;
    }
}

// MixColumns function mixes the columns of the state matrix.
void Crypto::InvMixColumns(state_t* state)
{
    int i;
    uint8_t a, b, c, d;
    for (i = 0; i < 4; ++i)
    { 
        a = (*state)[i][0];
        b = (*state)[i][1];
        c = (*state)[i][2];
        d = (*state)[i][3];

        (*state)[i][0] = Multiply(a, 0x0e) ^ Multiply(b, 0x0b) ^ Multiply(c, 0x0d) ^ Multiply(d, 0x09);
        (*state)[i][1] = Multiply(a, 0x09) ^ Multiply(b, 0x0e) ^ Multiply(c, 0x0b) ^ Multiply(d, 0x0d);
        (*state)[i][2] = Multiply(a, 0x0d) ^ Multiply(b, 0x09) ^ Multiply(c, 0x0e) ^ Multiply(d, 0x0b);
        (*state)[i][3] = Multiply(a, 0x0b) ^ Multiply(b, 0x0d) ^ Multiply(c, 0x09) ^ Multiply(d, 0x0e);
    }
}

// The SubBytes Function Substitutes the values in the
// state matrix with values in an S-box.
void Crypto::InvSubBytes(state_t* state)
{
    // The inverse substitution box
    uint8_t rsbox[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };
    uint8_t i, j;
    
    for (i = 0; i < 4; ++i)
    {
        for (j = 0; j < 4; ++j)
        {
        (*state)[j][i] = getSBoxInvert((*state)[j][i]);
        }
    }
}

void Crypto::InvShiftRows(state_t* state)
{
    uint8_t temp;

    // Rotate first row 1 columns to right  
    temp = (*state)[3][1];
    (*state)[3][1] = (*state)[2][1];
    (*state)[2][1] = (*state)[1][1];
    (*state)[1][1] = (*state)[0][1];
    (*state)[0][1] = temp;

    // Rotate second row 2 columns to right 
    temp = (*state)[0][2];
    (*state)[0][2] = (*state)[2][2];
    (*state)[2][2] = temp;

    temp = (*state)[1][2];
    (*state)[1][2] = (*state)[3][2];
    (*state)[3][2] = temp;

    // Rotate third row 3 columns to right
    temp = (*state)[0][3];
    (*state)[0][3] = (*state)[1][3];
    (*state)[1][3] = (*state)[2][3];
    (*state)[2][3] = (*state)[3][3];
    (*state)[3][3] = temp;
}

// Cipher is the main function that encrypts the PlainText.
void Crypto::Cipher(state_t* state, const uint8_t* RoundKey)
{
    uint8_t round = 0;

    // Add the First round key to the state before starting the rounds.
    AddRoundKey(0, state, RoundKey);

    // There will be Nr rounds.
    // The first Nr-1 rounds are identical.
    // These Nr rounds are executed in the loop below.
    // Last one without MixColumns()
    for (round = 1; ; ++round)
    {
        SubBytes(state);
        ShiftRows(state);
        if (round == NR) {
        break;
        }
        MixColumns(state);
        AddRoundKey(round, state, RoundKey);
    }
    // Add round key to last round
    AddRoundKey(NR, state, RoundKey);
}

void Crypto::InvCipher(state_t* state, const uint8_t* RoundKey)
{
    uint8_t round = 0;

    // Add the First round key to the state before starting the rounds.
    AddRoundKey(NR, state, RoundKey);

    // There will be Nr rounds.
    // The first Nr-1 rounds are identical.
    // These Nr rounds are executed in the loop below.
    // Last one without InvMixColumn()
    for (round = (NR - 1); ; --round)
    {
        InvShiftRows(state);
        InvSubBytes(state);
        AddRoundKey(round, state, RoundKey);
        if (round == 0) {
        break;
        }
        InvMixColumns(state);
    }
}

void Crypto::XorWithIv(uint8_t* buf, const uint8_t* Iv)
{
  uint8_t i;
  for (i = 0; i < AES_BLOCKLEN; ++i) // The block in AES is always 128bit no matter the key size
  {
    buf[i] ^= Iv[i];
  }
}

void Crypto::AES_CBC_encrypt_buffer(struct AES_ctx *ctx, uint8_t* buf, uint32_t length)
{
    uintptr_t i;
    uint8_t *Iv = ctx->Iv;
    for (i = 0; i < length; i += AES_BLOCKLEN)
    {
        XorWithIv(buf, Iv);
        Cipher((state_t*)buf, ctx->RoundKey);
        Iv = buf;
        buf += AES_BLOCKLEN;
    }
    /* store Iv in ctx for next call */
    memcpy(ctx->Iv, Iv, AES_BLOCKLEN);
}

void Crypto::AES_CBC_decrypt_buffer(struct AES_ctx* ctx, uint8_t* buf,  uint32_t length)
{
    uintptr_t i;
    uint8_t storeNextIv[AES_BLOCKLEN];
    for (i = 0; i < length; i += AES_BLOCKLEN)
    {
        memcpy(storeNextIv, buf, AES_BLOCKLEN);
        InvCipher((state_t*)buf, ctx->RoundKey);
        XorWithIv(buf, ctx->Iv);
        memcpy(ctx->Iv, storeNextIv, AES_BLOCKLEN);
        buf += AES_BLOCKLEN;
    }
}

// Convert from a string to a char array
char* Crypto::strToCharArr(String str)
{
   // Convert the string to a c char array
  char* tmpMessage = new char [str.length() + 1];
  strcpy(tmpMessage, str.c_str());

  return tmpMessage;
}

// Encrypt the plaintext using AES 128 bit encryption - CBC Mode
uint8_t* Crypto::encrypt_cbc(String incomingMessage)
{
    SerialUSB.println("Encrypting the SBD Message.");

    char* plaintext = strToCharArr(incomingMessage);
  
    // Local Variables:
    struct AES_ctx ctx;
    int cipherTextLength = 0, plainLength = 0, i = 0;
    uint8_t* ciphertext;

    // Remove any extra characters like newlines, padBytes, etc. - Leaves a null byte at the end
    trimMessage((uint8_t*)plaintext);

    int plaintextLength = strlen((char*)plaintext); // Return the plaintext string length without counting null Byte

    // Now check to see if we need to pad the message
    // The input text must be an interval of 16 bytes - I cap it at 256 Bytes - This can be changed
    // Mod the length by 16 to check if it is not a multiple
    if(plaintextLength % 16 != 0)
    {
        // Pad the string if it is not a multiple
        ciphertext = padString((uint8_t*)plaintext);
        cipherTextLength = strlen((char*)ciphertext);
    }

    // Handles case where length is a multiple of 16
    else
    {
        cipherTextLength = strlen((char*)plaintext); // Length of new ciphertext array - Matches plaintextLength
        ciphertext = new uint8_t[(cipherTextLength + 1) * sizeof(plaintext[0])]; // Reserve enough memory for the string and null
        for(i = 0; i < cipherTextLength; i++)
        {
            ciphertext[i] = plaintext[i];
        }
        ciphertext[strlen((char*)plaintext)] = '\0'; // Adding a null to the end
    }

    // Initialize the IV in the ctx struct
    AES_init_ctx_iv(&ctx, key, iv);

    // Encrypt the plaintext message and store its value in our ciphertext Byte array
    AES_CBC_encrypt_buffer(&ctx, ciphertext, cipherTextLength);
    SerialUSB.print("Encrypted Ciphertext: "); SerialUSB.println((char*)ciphertext);

    return ciphertext; // Returns the hex version of our ciphertext
}

// Decrypt the given ciphertext
uint8_t* Crypto::decrypt_cbc(uint8_t *ciphertext)
{
    SerialUSB.println("Decrypting the SBD Message.");

    // Local Variables:
    struct AES_ctx ctx;
    int cipherTextLength = strlen((char*)ciphertext); // Length of the ciphertext without the null char
    int plainTextLength = 0, outputLength = 0, i = 0;
    uint8_t *plaintext, *unpaddedPlaintext;

    // Initialize our plaintext buffer
    plaintext = new uint8_t[(cipherTextLength + 1) * sizeof(ciphertext[0])];
    for(i = 0; i < cipherTextLength; i++)
    {
        plaintext[i] = ciphertext[i];
    }
    plaintext[i] = '\0'; // Adding a null to the end

    // Initialize the Initial Vector
    AES_init_ctx_iv(&ctx, key, iv);

    // Decrypt our ciphertext message and store it in our new plaintext message
    AES_CBC_decrypt_buffer(&ctx, plaintext, cipherTextLength);

    // Calculate the size of the plaintext buffer - Should match cipher text buffer size
    plainTextLength = strlen((char*)plaintext);

    unpaddedPlaintext = removeStringPadding(plaintext); // Unpad the string and return the result
    SerialUSB.print("Decypted Plaintext: "); SerialUSB.println((char*)unpaddedPlaintext);
	return unpaddedPlaintext; // Indicates program did exit properly 
}

// Compare two arrays to determine if the contents are equal
// Returns 1 if they are equal and 0 if they are not
int Crypto::compareArrays(uint8_t arr1[], uint8_t arr2[], int size)
{
    int i = 0;
    for(i = 0; i < size; i++)
    {
        // If there is a difference in content, return 0
        if(arr1[i] != arr2[i])
        {
            return 0;
        }
    }

    // If there are no differences in content, return 1
    return 1;
}

// This method will remove any following tags like \n or \r and repluce them with null
void Crypto::trimMessage(uint8_t* str)
{
    str[strcspn((char*)str, "\r\n")] = '\0';
}


// This will pad a string to be a multiple of 16 bytes
// Returns the size of the new Byte array
uint8_t* Crypto::padString(uint8_t* str)
{
    // Local Variables:
    int originalStrLength = strlen((char*)str); // This keeps track of the incoming string length
    int extraPaddingNum = 16 - (originalStrLength % 16); // This will find the number of paddings we need
    int i = originalStrLength; // This will start us after the last array character
    uint8_t padByte = (uint8_t)(0x5E); // This is the ^ byte that will padded - Change this to an unused character if needed!
    uint8_t totalPadding = (uint8_t)(extraPaddingNum); // This will be placed as the last character of the padding
    int newStrLength = (originalStrLength + extraPaddingNum); // This will later capture the new padded string length
    uint8_t* returnStr = (uint8_t*)malloc((newStrLength) * sizeof(uint8_t) + 1); // New string that str will point to eventually
    if(returnStr == NULL)
    {
        exit(1);
    }

    // Initialize return string with old string data
    int j = 0;
    for(j = 0; j < originalStrLength; j++)
    {   
        returnStr[j] = (uint8_t)str[j];
    }
    returnStr[j] = '\0'; // Adding a null to the end
    

    // Check to make sure we don't go over the bounds of our 128 byte buffer
    if(originalStrLength + extraPaddingNum >= 256)
    {
        exit(1);
    }

    // Extra check to make sure we only pad if we need to
    if(extraPaddingNum != 0)
    {
        // Add ^ bytes until the last byte
        // The last byte will be hex for how much padding we added
        while(extraPaddingNum != 0)
        {
            // If we are on the last padding, add number of pads we added
            if(extraPaddingNum == 1)
            {
                returnStr[i] = (uint8_t)(totalPadding);
            }

            // If it is not the last padding, add a null byte
            else
            {
                returnStr[i] = (uint8_t)padByte;
            }
            
            i++;
            extraPaddingNum--;
        }
        returnStr[i] = (uint8_t)('\0'); // Finish by appending a null byte
    }

    // Return our new padded string
    return returnStr;
}

// This will unpad a string to be a multiple of 16 bytes
// Returns the size of the new Byte array
uint8_t* Crypto::removeStringPadding(uint8_t* str)
{
    // Local Variables:
    int originalStrLength = strlen((char*)str); // This keeps track of the incoming string length
    uint8_t removePaddingNum = str[originalStrLength - 1]; // The hex value of how many Bytes were padded
    int i = originalStrLength - 2; // This will start us at the Byte before the last hex number
    uint8_t padByte = (uint8_t)(0x5E); // Byte initially used to pad string '^'
    int newStrLength = (originalStrLength - (int)removePaddingNum); // This will later capture the new padded string length
    uint8_t* returnStr;

    // If the last character is a number 1-15 in hex do another check
    if((int)removePaddingNum >= 0 && (int)removePaddingNum <= 15)
    {
        // Start at the end digit and keep moving back the amount of spaces the padding number at the end signals
        int j = (int)removePaddingNum;
        int k = originalStrLength - j; // The location of the first padding Byte according to the number provided by the last hex value

        while(i >= k)
        {
            // Check whether this index matches the special padding byte we chose earlier - '^' or hex 0x5E
            if((int)str[i] == (int)padByte)
            {
                // Do nothing;
            }

            // If the special padding byte is not matched then return the string as is - This signals that we did not pad it
            else
            {
                return str;
            }
            
            i--;
        }

        // If we reach this point, we can create a new string and remove the padding
        returnStr = (uint8_t*)malloc((newStrLength) * sizeof(uint8_t) + 1); // New string that str will point to eventually
        if(returnStr == NULL)
        {
            exit(1);
        }

        // Initialize return string with old string data and the removal of the padded Bytes
        for(j = 0; j < newStrLength; j++)
        {   
            returnStr[j] = (uint8_t)str[j];
        }
        returnStr[j] = '\0'; // Adding a null to the end

        return returnStr; // Returns the unpadded string
    }

    // If the last byte is not a number 1-15 then we did not pad it originally - Return the string as is
    else
    {
        return str;
    } 
}
