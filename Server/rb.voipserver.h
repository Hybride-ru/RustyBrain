#ifndef RB_VOIPSERVER_H
#define RB_VOIPSERVER_H

#include "include.h"

//  ----------------------------------------------------------------------------
//  VOIP SERVER CLASS
//  ----------------------------------------------------------------------------

class RBVoipServer : public QObject
{
        Q_OBJECT

    private:
        RNService *voipService;

    protected slots:
        void slotRecive( char*inBuf, int inSize, RNSocket inSock );
    public:
        operator RNService*();

        RBVoipServer();
        ~RBVoipServer();
};

#endif // RB_VOIPSERVER_H
