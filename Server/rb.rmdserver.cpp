#include "rb.rmdserver.h"

//  ----------------------------------------------------------------------------
//  VOIP SERVER CLASS
//  ----------------------------------------------------------------------------

//  PROTECTED SLOTS
//  ----------------------------------------------------------------------------

void RBRmdServer::slotRecive(char *inBuf, int inSize, RNSocket inSock)
{
    rmdService->sendUnless( inBuf, inSize, inSock );
    free(inBuf);
}

//  PUBLIC
//  ----------------------------------------------------------------------------

RBRmdServer::operator RNService*()
{
    return rmdService;
}

RBRmdServer::RBRmdServer()
{
    rmdService = new RNService(RB_RMD_ID);

    connect( rmdService,
             SIGNAL(recive(char*,int,RNSocket)),
             SLOT(slotRecive(char*,int,RNSocket)) );
}

RBRmdServer::~RBRmdServer()
{
    delete rmdService;
}
