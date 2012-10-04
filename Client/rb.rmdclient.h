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
#ifndef RB_RMDCLIENT_H
#define RB_RMDCLIENT_H

// Options
#define RB_RMD_FPS 7

#include "include.h"

//  ----------------------------------------------------------------------------
//  REMOTE DESKTOP CLIENT CLASS
//  ----------------------------------------------------------------------------

namespace Ui
{
    class RBRmdClient;
}

class RBRmdClient : public QWidget
{
        Q_OBJECT

    private:
        Ui::RBRmdClient  *ui;

        RNService *rmdService;

        QPixmap renderPixmap;
        QTimer* ptimer;

        void resizeEvent(QResizeEvent *event);
        void updateScreenshotLabel();
    protected slots:
        void toggleWork();
        void slotRecive( char*inBuf, int inSize, RNSocket inSock );
        void slotCapture();
    public:
        operator RNService*();

        explicit RBRmdClient(QWidget *parent = 0);
        ~RBRmdClient();
};

#endif // RB_RMDCLIENT_H
