#ifndef RB_CHATSERVER_H
#define RB_CHATSERVER_H

#include "include.h"

//  ----------------------------------------------------------------------------
//  CHAT SERVER CLASS
//  ----------------------------------------------------------------------------

class RBChatServer : public QObject
{
        Q_OBJECT

    private:
        RNService *chatService;

    protected slots:
        void slotRecive( char*inBuf, int inSize, RNSocket inSock );
    public:
        operator RNService*();

        RBChatServer();
        ~RBChatServer();
};

#endif // RB_CHATSERVER_H
