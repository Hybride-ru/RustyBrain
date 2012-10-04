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
#ifndef INCLUDE_H
#define INCLUDE_H


//  Options
//#define SYS_WINDOWS
#define SYS_UNIX


#define RB_PORT      23960
#define RB_PACK_SIZE 100
#define RB_TIMEOUT   5
#define RB_CHAT_ID   'c'
#define RB_VOIP_ID   'v'
#define RB_RMD_ID    'r'

#include <iostream>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <list>
#include <map>

#include <QtCore>
#include <QtGui>
#include <QtMultimedia>

#ifdef SYS_WINDOWS
#   include <winsock2.h>
#endif

#ifdef SYS_UNIX
#   include <cerrno>
#   include <arpa/inet.h>
#   include <fcntl.h>
#   include <netinet/in.h>
#   include <sys/socket.h>
#   include <sys/types.h>
#endif

typedef unsigned short ushort;

#include "rc.log.h"
#include "rc.sleep.h"

class RNEncryptor;
class RNetwork;
class RNService;
#include "rn.socket.h"
#include "rn.encryptor.h"
#include "rn.network.h"
#include "rn.service.h"

#include "rb.encryptor.h"

#endif // INCLUDE_H
