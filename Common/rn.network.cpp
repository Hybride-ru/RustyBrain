#include "rn.network.h"

//  ----------------------------------------------------------------------------
//  NETWORK CLASS
//  ----------------------------------------------------------------------------

//  STRUCTURES
//  ----------------------------------------------------------------------------

//  PACK PRIVATE STRUCTURE
//  ----------------------------------------------------------------------------

void RNetwork::Pack::reset()
{
    if( buf )
        free( buf );

    buf = NULL;
    size = 0;
    processed = 0;
    idService = RN_S_INVALID_ID;
}

RNetwork::Pack::Pack()
{
    buf = NULL;
    reset();
}

RNetwork::Pack::Pack( char *inBuf, int inSize, char inIdService )
{
    buf = inBuf;
    size = inSize;
    idService = inIdService;
    processed = 0;
}

RNetwork::Pack::~Pack()
{
    reset();
}

//  LINK PRIVATE STRUCTURE
//  ----------------------------------------------------------------------------

void RNetwork::Link::addSendPack( char *inBuf, int inSize, char inIdService )
{
    int *copySize = (int*)malloc( sizeof(int) );
    *copySize = inSize;

    char *copyBuf = (char*)malloc( inSize );
    memcpy( copyBuf, inBuf, inSize );

    sendQueue.push_back( new Pack( (char*)copySize, sizeof(int), inIdService ) );
    sendQueue.push_back( new Pack( copyBuf, inSize, inIdService ) );
}

bool RNetwork::Link::netSend( RNSocket inSock, RNetwork *inNetwork )
{
    if( inSock == RN_INVALID_SOCKET || !inNetwork )
        return false;

    if( sendQueue.size() == 0 )
        return true;

    std::list<char> procSrvs;
    for( std::list<RNetwork::Pack*>::iterator it = sendQueue.begin();
         it != sendQueue.end();
         it++ )
    {
        RNetwork::Pack *pck = *it;

        bool isProcesed = false;

        for( std::list<char>::iterator itSrvs = procSrvs.begin();
             itSrvs != procSrvs.end();
             itSrvs++ )
        {
            if( *itSrvs == pck->idService )
            {
                isProcesed = true;
                break;
            }
        }

        if( isProcesed )
            continue;

        if( send( inSock, &pck->idService, sizeof(char), 0 ) <= 0 )
        {
#           ifdef RN_LOG
            RCLog << RCLog.warning("Network") <<
                   "Error in send data in link with RNSocket = " << (int)inSock <<
                   RCLog.end();
#           endif

            inNetwork->closeRNSocket( inSock );
            reset();
            inNetwork->printErrors();
            return false;
        }

        if( inNetwork->encryptor && pck->processed == 0 )
        {
            int encrSize;
            char *encrBuf = inNetwork->encryptor->encrypt( pck->buf,
                                                           pck->size,
                                                           encrSize );

            if( encrSize >= 1 )
            {
                free( pck->buf );
                pck->buf = encrBuf;
                pck->size = encrSize;
            }
        }

        procSrvs.push_back( pck->idService );

        int sndSize = pck->size - pck->processed;
        if( sndSize > inNetwork->nPackSize )
            sndSize = inNetwork->nPackSize;

        int total = 0;
        while( total < sndSize )
        {
            int bytesSend = send( inSock,
                                  pck->buf + pck->processed + total,
                                  sndSize - total,
                                  0 );
            if( bytesSend <= 0 )
            {
#               ifdef RN_LOG
                RCLog << RCLog.warning("Network") <<
                       "Error in send data in link with RNSocket = " << (int)inSock <<
                       RCLog.end();
#               endif

                inNetwork->closeRNSocket( inSock );
                reset();
                inNetwork->printErrors();
                return false;
            }

            total += bytesSend;
        }

        pck->processed += sndSize;
        if( pck->processed == pck->size )
        {
            delete pck;
            std::list<RNetwork::Pack*>::iterator itToDel = it;
            it--;
            sendQueue.erase( itToDel );
        }
    }
    return true;
}

bool RNetwork::Link::netRecive( RNSocket inSock, RNetwork *inNetwork )
{
    if( inSock == RN_INVALID_SOCKET || !inNetwork )
        return false;

    RNetwork::Pack *pck;
    char idSrvs;

    int retCode = recv( inSock, &idSrvs, sizeof(char), 0 );

    if( retCode == 0 )
    {
#       ifdef RN_LOG
        RCLog << RCLog.message("Network") <<
               "The connection was closed on the other side, RNSocket = " << (int)inSock <<
               RCLog.end();
#       endif

        inNetwork->closeRNSocket( inSock );
        reset();
        return false;
    }

    if( retCode < 0 )
    {
#       ifdef RN_LOG
        RCLog << RCLog.warning("Network") <<
               "Error in recive data in link with RNSocket = " << (int)inSock <<
               RCLog.end();
#       endif

        inNetwork->closeRNSocket( inSock );
        reset();
        inNetwork->printErrors();
        return false;
    }

    int recvSize;
    std::map<char, RNetwork::Pack*>::iterator item = recvPacks.find( idSrvs );
    if( item != recvPacks.end() )
    {
        pck = item->second;

        recvSize = pck->size - pck->processed;
        if( recvSize > inNetwork->nPackSize )
            recvSize = inNetwork->nPackSize;
    }
    else
    {
        pck = new RNetwork::Pack();
        pck->idService = idSrvs;
        pck->buf = (char*) malloc(sizeof(int));
        pck->size = sizeof(int);
        pck->processed = 0;

        recvSize = sizeof(int);
    }

    int total = 0;
    while( total < recvSize )
    {
        int bytesRecv = recv( inSock, pck->buf+pck->processed+total, recvSize-total, 0);

        if( bytesRecv <= 0 )
        {
#           ifdef RN_LOG
            RCLog << RCLog.warning("Network") <<
                   "Error in recive data in link with RNSocket = " << (int)inSock <<
                   RCLog.end();
#           endif

            inNetwork->closeRNSocket( inSock );
            reset();
            inNetwork->printErrors();
            return false;
        }

        total += bytesRecv;
    }

    pck->processed += recvSize;

    if( inNetwork->encryptor && pck->processed == pck->size )
    {
        int decrSize;
        char *decrBuf = inNetwork->encryptor->decrypt( pck->buf,
                                                       pck->size,
                                                       decrSize );

        if( decrSize >= 1 )
        {
            free( pck->buf );
            pck->buf = decrBuf;
            pck->size = decrSize;
        }
    }

    if( item != recvPacks.end() )
    {
        if( pck->processed >= pck->size )
        {
            std::map<char, RNService*>::iterator fSrvs = inNetwork->services.find( idSrvs );

            if( fSrvs != inNetwork->services.end() )
            {
                RNService *srvc = fSrvs->second;

                char *copyBuf = (char*) malloc( pck->size );
                memcpy( copyBuf, pck->buf, pck->size );

                srvc->recive( copyBuf, pck->size, inSock );
            }
            else
            {
#               ifdef RN_LOG
                RCLog << RCLog.warning("Network") <<
                       "Recv packet for unknow service in link with RNSocket = " << (int)inSock <<
                       RCLog.end();
#               endif

                inNetwork->closeRNSocket( inSock );
                reset();
                return false;
            }

            recvPacks.erase( item );
            delete pck;
        }
    }
    else
    {
        if( pck->size <= 0 )
        {
#           ifdef RN_LOG
            RCLog << RCLog.warning("Network") <<
                   "Wrong pack size = " << (int)pck->size <<
                   RCLog.end();
#           endif

            delete pck;

            inNetwork->closeRNSocket( inSock );
            reset();
            return false;
        }

        pck->size = *( (int*)pck->buf );
        pck->processed = 0;

        free( pck->buf );
        pck->buf = (char*) malloc( pck->size );

        recvPacks.insert( std::pair<char, RNetwork::Pack*>(idSrvs, pck) );
    }

    return true;
}

void RNetwork::Link::reset()
{
    for( std::list<Pack*>::iterator it = sendQueue.begin();
         it != sendQueue.end();
         it++ )
        delete *it;
    sendQueue.clear();

    for( std::map<char, Pack*>::iterator it = recvPacks.begin();
         it != recvPacks.end();
         it++ )
        delete it->second;
    recvPacks.clear();
}

RNetwork::Link::Link()
{
    reset();
}

RNetwork::Link::~Link()
{
    reset();
}

//  CLASS
//  ----------------------------------------------------------------------------

//  PRIVATE
//  ----------------------------------------------------------------------------

void RNetwork::closeRNSocket( RNSocket &inSock )
{
    if( inSock != RN_INVALID_SOCKET )
    {
#       ifdef SYS_WINDOWS
            closesocket( inSock );
#       endif

#       ifdef SYS_UNIX
            close( inSock );
#       endif

        inSock = RN_INVALID_SOCKET;
    }
}

void RNetwork::printErrors()
{
#   ifdef RN_LOG

#       ifdef SYS_WINDOWS
            RCLog << RCLog.error("Network") <<
                   strerror( WSAGetLastError() ) <<
                   RCLog.end();
#       endif

#       ifdef SYS_UNIX
            RCLog << RCLog.error("Network") <<
                   strerror( errno ) <<
                   RCLog.end();
#       endif

#   endif
}

void RNetwork::clearSrvs()
{
    for( std::map<char, RNService*>::iterator it = services.begin();
         it != services.end();
         it++ )
        delete it->second;
    services.clear();
}

void RNetwork::clearLinks()
{
    RNSocket tmp;
    for( std::map<RNSocket, RNetwork::Link*>::iterator it = links.begin();
         it != links.end();
         it++ )
    {
        tmp = it->first;
        closeRNSocket( tmp );
        delete it->second;
    }
    links.clear();
}

void RNetwork::threadFunc()
{
#   ifdef RN_LOG
    RCLog << RCLog.info("Network") <<
             "Network thread was started" <<
             RCLog.end();
#   endif

    tWork = true;

    while( tWork )
    {
        startNetwork();
        mSleep( nTimeout );
    }

#   ifdef RN_LOG
    RCLog << RCLog.info("Network") <<
             "Network thread was stopped" <<
             RCLog.end();
#   endif
}

void RNetwork::stopThreadFunc()
{
    stopNetwork();
    tWork = false;
}

void RNetwork::startNetwork()
{
    locker.lock();

#   ifdef SYS_WINDOWS
    //  Winsock init
    WORD wVersion;
    WSADATA wsaData;
    wVersion = MAKEWORD( 2, 0 );

    if( WSAStartup( wVersion, &wsaData ) != 0 )
    {
#       ifdef RN_LOG
        RCLog << RCLog.error("Network") <<
               "Winsock init error" <<
               RCLog.end();
#       endif

        printErrors();
        return;
    }
#   endif

    if( nPort != 0 )
    {
        listener = socket( PF_INET, SOCK_STREAM, 0 );

        if( listener == RN_INVALID_SOCKET )
        {
#           ifdef RN_LOG
            RCLog << RCLog.error("Network") <<
                   "Listner socket was not created" <<
                   RCLog.end();
#           endif

            printErrors();
            return;
        }

        struct sockaddr_in addr;
        memset( &addr, 0, sizeof (struct sockaddr_in) );

        addr.sin_family = PF_INET;
        addr.sin_port = htons( nPort );
        addr.sin_addr.s_addr = htonl( INADDR_ANY );

        if( bind( listener, (struct sockaddr *) &addr, sizeof(addr) ) == RN_SOCKET_ERROR )
        {
#           ifdef RN_LOG
            RCLog << RCLog.error("Network") <<
                   "Function bind() reported error" <<
                   RCLog.end();
#           endif

            closeRNSocket( listener );
            printErrors();
            return;
        }

        if( listen( listener, nBacklog ) == RN_SOCKET_ERROR )
        {
#           ifdef RN_LOG
            RCLog << RCLog.error("Network") <<
                   "Function listen() reported error" <<
                   RCLog.end();
#           endif

            closeRNSocket( listener );
            printErrors();
            return;
        }
    }

    clearLinks();

    nWork = true;
    while( nWork )
    {
        fd_set readset;
        FD_ZERO( &readset );

        fd_set writeset;
        FD_ZERO( &writeset );

        int max = (int)RN_INVALID_SOCKET;

        if( nPort != 0 )
        {
            FD_SET( listener, &readset );
            max = (int)listener;
        }
        else
        {
            while( links.size() == 0  && nWork )
            {
                locker.unlock();
                mSleep( nTimeout );
                locker.lock();
            }
        }

        if( !nWork )
            break;

        for( std::map<RNSocket, Link*>::iterator it = links.begin();
             it != links.end();
             it++ )
        {
            FD_SET( it->first, &readset );

            if( it->second->sendQueue.size() != 0 )
                FD_SET( it->first, &writeset );

            if( (int)it->first > max )
                max = it->first;
        }


        timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = nTimeout*1000;

        locker.unlock();

        if( select( max+1, &readset, &writeset, NULL, &timeout ) < 0 )
        {
#           ifdef RN_LOG
            RCLog << RCLog.error("Network") <<
                   "Function select() reported error" <<
                   RCLog.end();
#           endif

            nWork = false;
            printErrors();

            locker.lock();

            break;
        }

        locker.lock();

        if( nPort != 0 && FD_ISSET( listener, &readset ) && nWork)
        {
            Link *client = new Link();

            RNSocket sock = accept( listener, NULL, NULL );

            if( sock == RN_INVALID_SOCKET )
            {
#               ifdef RN_LOG
                RCLog << RCLog.error("Network") <<
                       "Client have bad socket" <<
                       RCLog.end();
#                   endif

                delete client;
                nWork = false;
                printErrors();
                break;
            }

            links.insert( std::pair<RNSocket, Link*>(sock, client) );

#           ifdef RN_LOG
            RCLog << RCLog.message("Network") <<
                   "New client connected, his RNSocket = " << (int)sock <<
                   RCLog.end();
#           endif
        }

        for( std::map<RNSocket, Link*>::iterator it = links.begin();
             it != links.end();
             it++ )
        {
            if( FD_ISSET( it->first, &readset ) &&
                    !(it->second->netRecive( it->first, this )) )
            {
                links.erase( it );
                break;
            }
            if( FD_ISSET( it->first, &writeset ) &&
                    !(it->second->netSend( it->first, this )) )
            {
                links.erase( it );
                break;
            }
        }
    }

    closeRNSocket( listener );

    clearLinks();

#   ifdef SYS_WINDOWS
    if( WSACleanup() == RN_SOCKET_ERROR )
    {
#       ifdef RN_LOG
        RCLog << RCLog.error("Network") <<
               "Could not perform cleaning up, winsock" <<
               RCLog.end();
#       endif

        printErrors();
    }
#   endif

    locker.unlock();
}

void RNetwork::stopNetwork()
{
    nWork = false;
}

RNetwork::RNetwork()
{
    tWork = false;

    nWork = false;
    nPackSize = 0;
    nTimeout = 5;
    nPort = 0;
    nBacklog = 5;

    listener = RN_INVALID_SOCKET;
    services.clear();
    links.clear();

    encryptor = NULL;

#   ifdef RN_LOG
    RCLog << RCLog.error("Network") <<
           "Use constructor with given parameters" <<
           RCLog.end();
#   endif
}

//  PROTECTED
//  ----------------------------------------------------------------------------

void RNetwork::run()
{
    threadFunc();
}

//  PUBLIC
//  ----------------------------------------------------------------------------

void RNetwork::addService( RNService *inService )
{
    if( !inService || inService->id == RN_S_INVALID_ID )
    {
#       ifdef RN_LOG
        RCLog << RCLog.warning("Network") <<
               "Wrong arguments in function addService()" <<
               RCLog.end();
#       endif

        return;
    }

    if( links.count( inService->id ) != 0 )
    {
#       ifdef RN_LOG
        RCLog << RCLog.warning("Network") <<
               "Service with this id, was previusly added" <<
               RCLog.end();
#       endif

        return;
    }

    locker.lock();

    inService->network = this;
    services.insert( std::pair<char, RNService*>(inService->id, inService) );

    locker.unlock();

    inService->registred();

#   ifdef RN_LOG
    RCLog << RCLog.message("Network") <<
           "Service  with id = " << inService->id << ", has been successfully added in Network" <<
           RCLog.end();
#   endif
}

void RNetwork::delService( RNService *inService )
{
    if( !inService || inService->id == RN_S_INVALID_ID )
    {
#       ifdef RN_LOG
        RCLog << RCLog.warning("Network") <<
               "Wrong arguments in function delService()" <<
               RCLog.end();
#       endif

        return;
    }

    locker.lock();

    std::map<char, RNService*>::iterator itToDel;
    itToDel = services.find( inService->id );

    if( itToDel == services.end() || itToDel->second != inService )
    {
#       ifdef RN_LOG
        RCLog << RCLog.warning("Network") <<
               "Service with this id, doesn't registred in Network" <<
               RCLog.end();
#       endif

        locker.unlock();
        return;
    }

    inService->network = NULL;
    services.erase( itToDel );

    locker.unlock();

    inService->unregistred();

#   ifdef RN_LOG
    RCLog << RCLog.message("Network") <<
           "Service with id = " << inService->id << ", was deleted from Network" <<
           RCLog.end();
#   endif
}

RNSocket RNetwork::addLink( const char *inAddr, ushort inPort )
{
    Link *client = new Link();
    RNSocket sock = socket( PF_INET, SOCK_STREAM, 0 );

    if( sock == RN_INVALID_SOCKET )
    {
#       ifdef RN_LOG
        RCLog << RCLog.warning("Network") <<
               "Error in creating socket for client" <<
               RCLog.end();
#       endif

        printErrors();

        delete client;
        return RN_INVALID_SOCKET;
    }

    struct sockaddr_in addr;
    memset( &addr, 0, sizeof( struct sockaddr_in ) );

    addr.sin_family = PF_INET;
    addr.sin_port = htons( inPort );
    addr.sin_addr.s_addr = inet_addr( inAddr );

    if( ::connect( sock, (struct sockaddr *) &addr, sizeof(addr) ) == RN_SOCKET_ERROR )
    {
#       ifdef RN_LOG
        RCLog << RCLog.warning("Network") <<
               "Can't connect at the specified address " <<
               RCLog.end();
#       endif

        closeRNSocket( sock );
        printErrors();

        delete client;
        return RN_INVALID_SOCKET;
    };

    locker.lock();
    links.insert( std::pair<RNSocket, Link*>(sock, client) );
    locker.unlock();

#   ifdef RN_LOG
    RCLog << RCLog.message("Network") <<
           "Connect done, his Socket " << (int)sock <<
           RCLog.end();
#   endif

    return sock;
}

void RNetwork::delLink( RNSocket inSock )
{
    if( inSock == RN_INVALID_SOCKET )
    {
#       ifdef RN_LOG
        RCLog << RCLog.warning("Network") <<
               "Wrong arguments in function delLink()" <<
               RCLog.end();
#       endif

        return;
    }

    locker.lock();

    std::map<RNSocket, Link*>::iterator itToDel = links.find(inSock);
    if( itToDel != links.end() )
    {
        itToDel->second->reset();
        links.erase( itToDel );

        closeRNSocket( inSock );

#       ifdef RN_LOG
        RCLog << RCLog.message("Network") <<
               "Socket " << (int)inSock << " was disconnected" <<
               RCLog.end();
#       endif
    }
    else
    {
#       ifdef RN_LOG
        RCLog << RCLog.warning("Network") <<
               "Can't find specifed Socket, in function delLink(RNSocket)" <<
               RCLog.end();
#       endif
    }

    locker.unlock();
}

void RNetwork::addEncryptor( RNEncryptor *inEncryptor )
{
    locker.lock();
    encryptor = inEncryptor;
    locker.unlock();

#   ifdef RN_LOG
    RCLog << RCLog.message("Network") <<
           "Encryptor has been successfully added in Network" <<
           RCLog.end();
#   endif
}

void RNetwork::delEncryptor()
{
    locker.lock();
    encryptor = NULL;
    locker.unlock();

#   ifdef RN_LOG
    RCLog << RCLog.message("Network") <<
           "Encryptor was deleted from Network" <<
           RCLog.end();
#   endif
}

RNetwork::RNetwork( int inPackSize, long inTimeout,
                    ushort inPort, int inBacklog )
{
    if( inPackSize < 1 || inTimeout < 1 || inBacklog < 1 )
    {
#       ifdef RN_LOG
        RCLog << RCLog.error("Network") <<
               "Wrong arguments in constructor" <<
               RCLog.end();
#       endif

        return;
    }

    tWork = false;

    nWork = false;
    nPackSize = inPackSize;
    nTimeout = inTimeout;
    nPort = inPort;
    nBacklog = inBacklog;

    listener = RN_INVALID_SOCKET;
    services.clear();
    links.clear();
    encryptor = NULL;

    start();
}

RNetwork::~RNetwork()
{
    stopThreadFunc();
    wait();
}
