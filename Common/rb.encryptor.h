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
#ifndef RB_ENCRYPTOR_H
#define RB_ENCRYPTOR_H

#include "include.h"

//  ----------------------------------------------------------------------------
//  RB ENCRYPTOR CLASS
//  ----------------------------------------------------------------------------

class RBEncryptor : public RNEncryptor
{
    private:
        char* encrypt( char *inBuf, int inSize, int &outSize );
        char* decrypt( char *inBuf, int inSize, int &outSize );

};

#endif // RB_ENCRYPTOR_H




