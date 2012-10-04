#include "rb.encryptor.h"

//  ----------------------------------------------------------------------------
//  RB ENCRYPTOR CLASS
//  ----------------------------------------------------------------------------

//  PRIVATE
//  ----------------------------------------------------------------------------

char* RBEncryptor::encrypt(char *inBuf, int inSize, int &outSize)
{
    outSize = inSize;
    char *outBuf = (char*)malloc( inSize );

    for( int i = 0; i < inSize; i++ )
        outBuf[i] = inBuf[i] ^ RB_CHAT_ID;

    return outBuf;
}

char* RBEncryptor::decrypt(char *inBuf, int inSize, int &outSize)
{
    outSize = inSize;
    char* outBuf = (char*)malloc( inSize );

    for( int i = 0; i < inSize; i++ )
        outBuf[i] = inBuf[i] ^ RB_CHAT_ID;

    return outBuf;
}
