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
#ifndef RN_SOCKET_H
#define RN_SOCKET_H

#include "include.h"

//  ----------------------------------------------------------------------------
//  NETWORK SOCKET DESCRIPTION
//  ----------------------------------------------------------------------------

#ifdef SYS_WINDOWS
#   define RN_INVALID_SOCKET INVALID_SOCKET
#   define RN_SOCKET_ERROR   SOCKET_ERROR

    typedef SOCKET RNSocket;
#endif

#ifdef SYS_UNIX
#   define RN_INVALID_SOCKET -1
#   define RN_SOCKET_ERROR   -1

    typedef int RNSocket;
#endif

#endif // RN_SOCKET_H
