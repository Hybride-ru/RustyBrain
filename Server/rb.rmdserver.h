#ifndef RB_RMDSERVER_H
#define RB_RMDSERVER_H

#include "include.h"

//  ----------------------------------------------------------------------------
//  REMOTE DESKTOP SERVER CLASS
//  ----------------------------------------------------------------------------

class RBRmdServer : public QObject
{
        Q_OBJECT

    private:
        RNService *rmdService;

    protected slots:
        void slotRecive( char*inBuf, int inSize, RNSocket inSock );
    public:
        operator RNService*();

        RBRmdServer();
        ~RBRmdServer();
};

#endif // RB_RMDSERVER_H
