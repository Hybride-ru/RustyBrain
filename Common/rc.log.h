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
#ifndef RC_LOG_H
#define RC_LOG_H


//  Options
#define RC_LOG_CONSOLE
//#define RC_LOG_FILE
#define RC_LOG_FILENAME "log.html"


#include "include.h"

//  ----------------------------------------------------------------------------
//  CONSOLE LOG PROVIDER CLASS
//  ----------------------------------------------------------------------------

#ifdef RC_LOG_CONSOLE
class LogConsole
{
    //  Structures
    private:
        struct State
        {
                char *module;
                int   type;
                State( const char *inModule, int inType );
        };

    //  Class
    private:
        void writeModule( const char *inModule, int inType );
    public:
        LogConsole& operator <<( State inState );
        LogConsole& operator <<( const char *inText );
        LogConsole& operator <<( int   inNumber );
        LogConsole& operator <<( void *inVoid );

        State error  ( const char *inModule );
        State warning( const char *inModule );
        State info   ( const char *inModule );
        State message( const char *inModule );
        State end();

        LogConsole();
};
#endif



//  ----------------------------------------------------------------------------
//  HTML FILE LOG PROVIDER CLASS
//  ----------------------------------------------------------------------------

#ifdef RC_LOG_FILE
class LogFile
{
    //  Structures
    private:
        struct State
        {
                char *module;
                int   type;
                State( const char *inModule, int inType );
        };

    //  Class
    private:
        std::ofstream out;
        bool state;

        void writeModule( const char *inModule, int inType );
    public:
        LogFile& operator <<( State inState );
        LogFile& operator <<( const char *inText );
        LogFile& operator <<( int   inNumber );
        LogFile& operator <<( void *inVoid );

        State error  ( const char *inModule );
        State warning( const char *inModule );
        State info   ( const char *inModule );
        State message( const char *inModule );
        State end();

        void open( const char *inFilename );
        void close();

        LogFile() {}
        LogFile( const char *inFilename );
        ~LogFile();
};
#endif



#ifdef RC_LOG_CONSOLE
extern LogConsole RCLog;
#endif

#ifdef RC_LOG_FILE
extern LogFile RCLog;
#endif

#endif // RC_LOG_H
