#include "rc.log.h"

//  ----------------------------------------------------------------------------
//  LOG PROVIDERS BUILD OPTIONS
//  ----------------------------------------------------------------------------

static const char* const Types[] = { "E", "W", "I", "M" };

#ifdef RC_LOG_CONSOLE
LogConsole RCLog;
#endif

#ifdef RC_LOG_FILE
static const char* const Colors[] =
{
    "b31111",
    "448ddf",
    "42ae1e",
    "777575"
};

LogFile RCLog( RC_LOG_FILENAME );
#endif



//  ----------------------------------------------------------------------------
//  CONSOLE LOG PROVIDER CLASS
//  ----------------------------------------------------------------------------

#ifdef RC_LOG_CONSOLE

//  STRUCTURES PUBLIC
//  ----------------------------------------------------------------------------

LogConsole::State::State( const char *inModule, int inType )
    :module  ( (char*)inModule ),
      type    ( inType )
{
}

//  PRIVATE
//  ----------------------------------------------------------------------------

void LogConsole::writeModule( const char *inModule, int inType )
{
    std::cout << "[" << Types[inType] << "] " << std::setw(15) << inModule << " : ";
}

//  PUBLIC
//  ----------------------------------------------------------------------------

LogConsole& LogConsole::operator <<( LogConsole::State inState )
{
    switch( inState.type )
    {
        case 4:
        {
            std::cout << std::endl;
            break;
        }

        default:
        {
            writeModule(inState.module,inState.type);
            break;
        }
    }

    return (*this);
}

LogConsole& LogConsole::operator <<( int inNumber )
{
    std::cout << inNumber;
    return (*this);
}

LogConsole& LogConsole::operator <<( const char *inText )
{
    std::cout << inText;
    return (*this);
}

LogConsole& LogConsole::operator <<( void *inVoid )
{
    std::cout << inVoid;
    return (*this);
}

LogConsole::State LogConsole::error( const char *inModule )
{
    return State(inModule,0);
}

LogConsole::State LogConsole::warning( const char *inModule )
{
    return State(inModule,1);
}

LogConsole::State LogConsole::info( const char *inModule )
{
    return State(inModule,2);
}

LogConsole::State LogConsole::message( const char *inModule )
{
    return State(inModule,3);
}

LogConsole::State LogConsole::end()
{
    return State(NULL,4);
}

LogConsole::LogConsole()
{
    std::cout << std::setiosflags( std::ios::left );
}

#endif



//  ----------------------------------------------------------------------------
//  HTML LOG PROVIDER CLASS
//  ----------------------------------------------------------------------------

#ifdef RC_LOG_FILE

//  STRUCTURES PUBLIC
//  ----------------------------------------------------------------------------

LogFile::State::State( const char *inModule, int inType )
    : module  ( (char*)inModule ),
      type    ( inType )
{
}

//  PRIVATE
//  ----------------------------------------------------------------------------

void LogFile::writeModule( const char *inModule, int inType )
{
    if( state ) out << "</font>";

    state = true;
    out << "<font color=" << Colors[inType] << ">"
        << "<font color=777575> [" << Types[inType] << "]</font> " << std::setw(15) << inModule << "</font>"
        << "<font color=" << Colors[3] << ">" << " : ";
}

//  PUBLIC
//  ----------------------------------------------------------------------------

LogFile& LogFile::operator <<( LogFile::State inState )
{
    switch( inState.type )
    {
        case 4:
        {
            out << std::endl;
            break;
        }

        default:
        {
            writeModule(inState.module,inState.type);
            break;
        }
    }

    return (*this);
}

LogFile& LogFile::operator <<( int inNumber )
{
    out << inNumber;
    return (*this);
}

LogFile& LogFile::operator <<( const char *inText )
{
    out << inText;
    return (*this);
}

LogFile& LogFile::operator <<( void *inVoid )
{
    out << inVoid;
    return (*this);
}

LogFile::State LogFile::error( const char *inModule )
{
    return State(inModule,0);
}

LogFile::State LogFile::warning( const char *inModule )
{
    return State(inModule,1);
}

LogFile::State LogFile::info( const char *inModule )
{
    return State(inModule,2);
}

LogFile::State LogFile::message( const char *inModule )
{
    return State(inModule,3);
}

LogFile::State LogFile::end()
{
    return State(NULL,4);
}

void LogFile::open( const char *inFilename )
{
    state  = false;

    out.open( inFilename, std::ios_base::out );
    out << setiosflags( std::ios::left );
    out << "<font size=\"2\" face=\"Lucida Console\"><PRE>\n";
}

void LogFile::close()
{
    if( state )
        out << "</font>";
    out << "</font>";

    out.flush();
    out.close();
}

LogFile::LogFile( const char *inFilename )
{
    open( inFilename );
}

LogFile::~LogFile()
{
    close();
}

#endif
