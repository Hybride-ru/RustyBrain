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
#ifndef RB_VOIPCLIENT_H
#define RB_VOIPCLIENT_H

// Options
#define RB_FREQ 22000
#define RB_VOIP_BUFFER_SIZE 2000000
#define RB_VOIP_TIMEOUT 15


#include "include.h"
#include "ui_rb.chatclient.h"

//  ----------------------------------------------------------------------------
//  VOIP CLIENT CLASS
//  ----------------------------------------------------------------------------

class RBVoipClient : public QObject
{
        Q_OBJECT

    private:
        Ui::RBChatClient *ui;
        RNService *voipService;

        QAudioFormat format;
        QAudioDeviceInfo infoInput;
        QAudioDeviceInfo infoOutput;
        QAudioInput *input;
        QAudioOutput *output;
        QIODevice *devInput;
        QIODevice *devOutput;

        char buffer[RB_VOIP_BUFFER_SIZE];

        void createAudioInput();
        void createAudioOutput();
    protected slots:
        void slotInputDeviceChanged(int inIndex);
        void slotOutputDeviceChanged(int inIndex);

        void slotRecive( char *inBuf, int inSize, RNSocket inSock );

        void slotCapture();
    public:
        operator RNService*();

        RBVoipClient(Ui::RBChatClient *inUi);
        ~RBVoipClient();
};

#endif // RB_VOIPCLIENT_H
