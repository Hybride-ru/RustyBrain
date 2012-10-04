/****************************************************************************
**
** Copyright (C) 2012 Eremin Yakov and Putintsev Roman.
** All rights reserved.
**
**
** This file is part of the RustyBrain project.
**
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
**
**     RustyBrain is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY;
**
**
****************************************************************************/
#ifndef RN_NETWORK_H
#define RN_NETWORK_H


//  Options
#define RN_LOG


#include "include.h"

//  ----------------------------------------------------------------------------
//  NETWORK CLASS
//  ----------------------------------------------------------------------------

class RNetwork : public QThread
{
        Q_OBJECT

        friend class RNService;
        //  Structures
    private:
        struct Pack
        {
            public:
                char *buf;
                int   size;
                int   processed;
                char  idService;

                void reset();

                Pack();
                Pack( char *inBuf, int inSize, char inIdService );
                ~Pack();
        };

        struct Link
        {
            public:
                std::list<Pack*> sendQueue;
                std::map<char, Pack*> recvPacks;

                void addSendPack( char *inBuf, int inSize, char inIdService );
                bool netSend( RNSocket inSock, RNetwork *inNetwork );
                bool netRecive( RNSocket inSock, RNetwork *inNetwork );
                void reset();

                Link();
                ~Link();
        };

        //  Class
    signals:
        void linkFound( RNSocket inSock );
        void linkLost( RNSocket inSock );
    private:
        bool tWork;

        bool nWork;
        int  nPackSize;
        long nTimeout;
        ushort nPort;
        int  nBacklog;

        RNSocket listener;

        QMutex locker;
        std::map<char, RNService*> services;
        std::map<RNSocket, Link*> links;
        RNEncryptor *encryptor;

        void closeRNSocket( RNSocket &inSock );

        void printErrors();

        void clearSrvs();
        void clearLinks();

        void threadFunc();
        void stopThreadFunc();

        void startNetwork();
        void stopNetwork();

        RNetwork();
    protected:
        void run();
    public:
        void addService( RNService *inService );
        void delService( RNService *inService );

        RNSocket addLink( const char *inAddr, ushort inPort );
        void     delLink( RNSocket inSock );

        void addEncryptor( RNEncryptor *iRNEncryptor );
        void delEncryptor();

        RNetwork( int inPackSize = 200, long inTimeout = 5,
                 ushort inPort = 0, int inBacklog = 5 );
        ~RNetwork();
};

#endif // RN_NETWORK_H
