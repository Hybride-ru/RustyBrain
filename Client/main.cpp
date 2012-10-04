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
#include "include.h"
#include "rb.chatclient.h"
#include "ui_rb.chatclient.h"
#include "rb.voipclient.h"
#include "rb.rmdclient.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<RNSocket>("RNSocket");

    RNetwork testNetwork( RB_PACK_SIZE, RB_TIMEOUT );
    RBEncryptor myEncr;
    //  Создавать воип после чата, и передавать гуй!
    RBChatClient w;
    RBVoipClient v(w.ui);
    RBRmdClient  r(w.ui->tab_3);

    testNetwork.addEncryptor( &myEncr );
    testNetwork.addService( w );
    testNetwork.addService( v );
    testNetwork.addService( r );
    testNetwork.addLink( RB_CHAT_IP, RB_PORT );

    w.show();

    return a.exec();
}
