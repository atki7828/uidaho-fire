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

// Encrypt the plaintext using AES 128 bit encryption - CBC Mode
uint8_t* Crypto::encrypt_cbc(uint8_t *plaintext)
{
    printf("--------------------------------------\n");
    printf("Encrypting the plaintext!\n");
    printf("--------------------------------------\n\n");

    // Local Variables:
    struct AES_ctx ctx;
    int cipherTextLength = 0, plainLength = 0, i =0;
    uint8_t* ciphertext;

    // Remove any extra characters like newlines, padBytes, etc. - Leaves a null byte at the end
    trimMessage(plaintext);

    int plaintextLength = strlen((char*)plaintext); // Return the plaintext string length without counting null Byte

    // Now check to see if we need to pad the message
    // The input text must be an interval of 16 bytes - I cap it at 256 Bytes - This can be changed
    // Mod the length by 16 to check if it is not a multiple
    if(plaintextLength % 16 != 0)
    {
        // Pad the string if it is not a multiple
        ciphertext = padString(plaintext);
        cipherTextLength = strlen((char*)ciphertext);
        printf("New String Length = %d\n", cipherTextLength);
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

    printf("New String Lengthsss = %d\n", strlen((char*)(ciphertext)));

    return ciphertext; // Returns the hex version of our ciphertext
}

// Decrypt the given ciphertext
uint8_t* Crypto::decrypt_cbc(uint8_t *ciphertext)
{
    printf("--------------------------------------\n");
    printf("Decrypting the ciphertext!\n");
    printf("--------------------------------------\n\n");

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
        printf("Memory allocation failed. Aborting Encryption.\n\n");
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
        printf("The padded string will not be in the boundary of 128 bytes for the plaintext message. Aborting.\n");
        exit(1);
    }

    printf("Original String Length = %d\n", originalStrLength);
    printf("Extra padding Num = %d\n", extraPaddingNum);

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
                //printf("returnStr[%d] = 0x%02X\n", i, (uint8_t)(totalPadding));
                returnStr[i] = (uint8_t)(totalPadding);
            }

            // If it is not the last padding, add a null byte
            else
            {
                //printf("returnStr[%d] = 0x%02X\n", i, (uint8_t)(padByte));
                returnStr[i] = (uint8_t)padByte;
            }
            
            i++;
            extraPaddingNum--;
        }
        returnStr[i] = (uint8_t)('\0'); // Finish by appending a null byte
    }

    for(i = 0; i < strlen((char*)returnStr); i++)
    {
        printf("0x%02X, ", returnStr[i]);
    }
    printf("\n");

    printf("New String Length = %d\n", strlen((char*)returnStr));

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
            printf("Memory allocation failed. Aborting Encryption.\n\n");
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