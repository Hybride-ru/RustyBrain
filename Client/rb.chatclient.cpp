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
#include "rb.chatclient.h"
#include "ui_rb.chatclient.h"

//  ----------------------------------------------------------------------------
//  CHAT CLIENT CLASS
//  ----------------------------------------------------------------------------

//  PROTECTED SLOTS
//  ----------------------------------------------------------------------------

void RBChatClient::slotRecive(char *inBuf, int inSize, RNSocket inSock)
{
    QString temp( (QChar*)inBuf );

    ui->listWidget->addItem( temp );

    free(inBuf);
}

void RBChatClient::slotSendMsg()
{
    if( ui->lineEdit->text().isEmpty() )
        return;

    QString sndStr = chatNick;
    sndStr += ui->lineEdit->text();
    chatService->sendAll( (char*)sndStr.data(), (int)sndStr.size()*sizeof(QChar)+sizeof(QChar)  );
    ui->lineEdit->clear();
}

//  PUBLIC
//  ----------------------------------------------------------------------------

RBChatClient::operator RNService*()
{
    return chatService;
}

RBChatClient::RBChatClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RBChatClient)
{
    ui->setupUi(this);

    chatService = new RNService(RB_CHAT_ID);

    connect( ui->lineEdit,
             SIGNAL(returnPressed()),
             SLOT(slotSendMsg()) );

    connect( chatService,
             SIGNAL(recive(char*,int,RNSocket)),
             SLOT(slotRecive(char*,int,RNSocket)) );

    connect( ui->listWidget->model(),
             SIGNAL(rowsInserted ( const QModelIndex &, int, int ) ),
             ui->listWidget,
             SLOT(scrollToBottom ()) );

    bool bOk;
    chatNick = QInputDialog::getText( 0,
                                  "Input",
                                  "Nickname:",
                                  QLineEdit::Normal,
                                  QString::fromLocal8Bit(RB_CHAT_NICKNAME),
                                  &bOk );
    if( !bOk )
        chatNick = QString::fromLocal8Bit(RB_CHAT_NICKNAME);

    chatNick += ": ";
}

RBChatClient::~RBChatClient()
{
    delete chatService;
    delete ui;
}
