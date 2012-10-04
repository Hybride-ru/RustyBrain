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
#include "rb.rmdclient.h"
#include "ui_rb.rmdclient.h"

//  ----------------------------------------------------------------------------
//  REMOTE DESKTOP CLIENT CLASS
//  ----------------------------------------------------------------------------


//  PRIVATE
//  ----------------------------------------------------------------------------

void RBRmdClient::toggleWork()
{

    if( ptimer->isActive() )
    {
        ui->startButton->setText("Begin recordong");
        ptimer->stop();
    }
    else
    {
        ui->startButton->setText("Finish recordong");
        ptimer->start(1000/RB_RMD_FPS);
    }
}

void RBRmdClient::updateScreenshotLabel()
{
    ui->showRmd->setPixmap(renderPixmap.scaled(ui->showRmd->size(),
                                                     Qt::KeepAspectRatio,
                                                     Qt::SmoothTransformation));
}

void RBRmdClient::resizeEvent(QResizeEvent * /* event */)
{
    QSize scaledSize = renderPixmap.size();
    scaledSize.scale(ui->showRmd->size(), Qt::KeepAspectRatio);
    if (!ui->showRmd->pixmap()
            || scaledSize != ui->showRmd->pixmap()->size())
        updateScreenshotLabel();
}


//  PROTECTED SLOTS
//  ----------------------------------------------------------------------------

void RBRmdClient::slotRecive(char *inBuf, int inSize, RNSocket inSock)
{
    QByteArray ba( inBuf, inSize );
    renderPixmap.loadFromData( ba, "JPG" );
    updateScreenshotLabel();

    free(inBuf);
}

void RBRmdClient::slotCapture()
{
    QPixmap grabPix = QPixmap::grabWindow(QApplication::desktop()->winId());
//    grabPix = grabPix.scaled( 320, 240,
//                    Qt::KeepAspectRatio );
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::ReadWrite);
    grabPix.save(&buffer, "JPG" );

    rmdService->sendAll( ba.data(), ba.size() );
}

//  PUBLIC
//  ----------------------------------------------------------------------------

RBRmdClient::operator RNService*()
{
    return rmdService;
}

RBRmdClient::RBRmdClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RBRmdClient)
{
    // Ui
    ui->setupUi(this);
    if(parent)
        parent->layout()->addWidget(this);

    ui->showRmd->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->showRmd->setAlignment(Qt::AlignCenter);
    ui->showRmd->setMinimumSize(240, 160);

    ui->startButton->connect(ui->startButton, SIGNAL( clicked()), this, SLOT(toggleWork()) );

    // Service
    rmdService = new RNService(RB_RMD_ID);

    connect( rmdService,
             SIGNAL(recive(char*,int,RNSocket)),
             SLOT(slotRecive(char*,int,RNSocket)) );

    //Other
    ptimer = new QTimer(this);
    connect(ptimer, SIGNAL(timeout()), SLOT(slotCapture()));
}

RBRmdClient::~RBRmdClient()
{
    delete rmdService;
    delete ui;

    ptimer->stop();
}
