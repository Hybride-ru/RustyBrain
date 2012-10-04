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
#ifndef RN_SERVICE_H
#define RN_SERVICE_H


//  Options
#define RN_S_LOG
#define RN_S_INVALID_ID -1


#include "include.h"

//  ----------------------------------------------------------------------------
//  NETWORK SERVICE CLASS
//  ----------------------------------------------------------------------------

class RNService : public QObject
{
        Q_OBJECT

        friend class RNetwork;
    signals:
        void registred();
        void unregistred();

        void recive( char *inBuf, int inSize, RNSocket inSock );
    private:
        char id;
        RNetwork *network;

        bool isRegistered();
    public:
        void sendTo    ( char *inBuf, int inSize, RNSocket inSock );
        void sendToList( char *inBuf, int inSize, std::list<RNSocket> &inList );
        void sendUnless( char *inBuf, int inSize, RNSocket inSock );
        void sendAll   ( char *inBuf, int inSize );

        char getId();
        RNetwork* getNetwork();

        RNService();
        RNService( char inId );
        ~RNService();
};

#endif // RN_SERVICE_H
