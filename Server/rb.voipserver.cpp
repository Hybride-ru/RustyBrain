#include "rb.voipserver.h"

//  ----------------------------------------------------------------------------
//  VOIP SERVER CLASS
//  ----------------------------------------------------------------------------

//  PROTECTED SLOTS
//  ----------------------------------------------------------------------------

void RBVoipServer::slotRecive(char *inBuf, int inSize, RNSocket inSock)
{
    voipService->sendUnless( inBuf, inSize, inSock );
    free(inBuf);
}

//  PUBLIC
//  ----------------------------------------------------------------------------

RBVoipServer::operator RNService*()
{
    return voipService;
}

RBVoipServer::RBVoipServer()
{
    voipService = new RNService(RB_VOIP_ID);

    connect( voipService,
             SIGNAL(recive(char*,int,RNSocket)),
             SLOT(slotRecive(char*,int,RNSocket)) );
}

RBVoipServer::~RBVoipServer()
{
    delete voipService;
}

