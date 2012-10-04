#include "rb.chatserver.h"

//  ----------------------------------------------------------------------------
//  CHAT SERVER CLASS
//  ----------------------------------------------------------------------------

//  PROTECTED SLOTS
//  ----------------------------------------------------------------------------

void RBChatServer::slotRecive(char *inBuf, int inSize, RNSocket inSock)
{
    chatService->sendAll(inBuf, inSize);
    free(inBuf);
}

//  PUBLIC
//  ----------------------------------------------------------------------------

RBChatServer::operator RNService*()
{
    return chatService;
}

RBChatServer::RBChatServer()
{
    chatService = new RNService(RB_CHAT_ID);

    connect( chatService,
             SIGNAL(recive(char*,int,RNSocket)),
             SLOT(slotRecive(char*,int,RNSocket)) );
}

RBChatServer::~RBChatServer()
{
    delete chatService;
}
