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
#ifndef RB_CHATCLIENT_H
#define RB_CHATCLIENT_H


// Options
#define RB_CHAT_IP "127.0.0.1"
#define RB_CHAT_NICKNAME "Raymondo"


#include "include.h"

//  ----------------------------------------------------------------------------
//  CHAT CLIENT CLASS
//  ----------------------------------------------------------------------------

namespace Ui
{
    class RBChatClient;
}

class RBChatClient : public QWidget
{
        Q_OBJECT
           
    private:
        RNService *chatService;
        QString    chatNick;
    protected slots:
        void slotRecive( char *inBuf, int inSize, RNSocket inSock );
        void slotSendMsg();
    public:
        Ui::RBChatClient *ui;
        operator RNService*();

        explicit RBChatClient(QWidget *parent = 0);
        ~RBChatClient();
};

#endif // RB_CHATCLIENT_H
