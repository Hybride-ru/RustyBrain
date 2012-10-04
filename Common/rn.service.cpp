#include "rn.service.h"

//  ----------------------------------------------------------------------------
//  NETWORK SERVICE CLASS
//  ----------------------------------------------------------------------------

//  PRIVATE
//  ----------------------------------------------------------------------------

bool RNService::isRegistered()
{
    if( !network || id == RN_S_INVALID_ID )
    {
#       ifdef RN_S_LOG
        RCLog << RCLog.warning("RNService") <<
                 "Service is not registered in the RNetwork" <<
                 RCLog.end();
#       endif

        return false;
    }

    return true;
}

//  PUBLIC
//  ----------------------------------------------------------------------------

void RNService::sendTo( char *inBuf, int inSize, RNSocket inSock )
{
    if( !isRegistered() )
        return;

    if(  !inBuf || inSize <= 0 || inSock == RN_INVALID_SOCKET )
    {
#       ifdef RN_S_LOG
        RCLog << RCLog.warning("RNService") <<
               "Wrong arguments in function sendTo()" <<
               RCLog.end();
#       endif

        return;
    }

    network->locker.lock();

    std::map<RNSocket, RNetwork::Link*>::iterator it = network->links.find( inSock );
    if( it == network->links.end() )
    {
#       ifdef RN_S_LOG
        RCLog << RCLog.warning("RNService") <<
               "Can't find specifed RNSocket" <<
               RCLog.end();
#       endif

        network->locker.unlock();
        return;
    }

    it->second->addSendPack( inBuf, inSize, id );

    network->locker.unlock();
}

void RNService::sendToList( char *inBuf, int inSize, std::list<RNSocket> &inList )
{
    if( !isRegistered() )
        return;

    if(  !inBuf || inSize <= 0 )
    {
#       ifdef RN_S_LOG
        RCLog << RCLog.warning("RNService") <<
               "Wrong arguments in function sendToList()" <<
               RCLog.end();
#       endif

        return;
    }

    network->locker.lock();

    for( std::list<RNSocket>::iterator itList = inList.begin();
         itList != inList.end();
         itList++)
    {
        std::map<RNSocket, RNetwork::Link*>::iterator itLink =
                network->links.find( *itList );
        if( itLink == network->links.end() )
        {
#           ifdef RN_S_LOG
            RCLog << RCLog.warning("RNService") <<
                     "Can't find specifed RNSocket" <<
                     RCLog.end();
#           endif

            continue;
        }

        itLink->second->addSendPack( inBuf, inSize, id );
    }
    network->locker.unlock();
}

void RNService::sendUnless( char *inBuf, int inSize, RNSocket inSock )
{
    if( !isRegistered() )
        return;

    if(  !inBuf || inSize <= 0 )
    {
#       ifdef RN_S_LOG
        RCLog << RCLog.warning("RNService") <<
               "Wrong arguments in function sendUnless()" <<
               RCLog.end();
#       endif

        return;
    }

    network->locker.lock();

    for( std::map<RNSocket, RNetwork::Link*>::iterator it = network->links.begin();
         it != network->links.end();
         it++ )
    {
        if( it->first != inSock )
            it->second->addSendPack( inBuf, inSize, id );
    }

    network->locker.unlock();
}

void RNService::sendAll( char *inBuf, int inSize )
{
    if( !isRegistered() )
        return;

    if(  !inBuf || inSize <= 0 )
    {
#       ifdef RN_S_LOG
        RCLog << RCLog.warning("RNService") <<
               "Wrong arguments in function sendAll()" <<
               RCLog.end();
#       endif

        return;
    }

    network->locker.lock();

    for( std::map<RNSocket, RNetwork::Link*>::iterator it = network->links.begin();
         it != network->links.end();
         it++ )
        it->second->addSendPack( inBuf, inSize, id );


    network->locker.unlock();
}

RNService::RNService()
{
    id = RN_S_INVALID_ID;
    network = NULL;
}

RNService::RNService( char inId )
{
    id = inId;
    network = NULL;
}

RNService::~RNService()
{
    if( isRegistered() )
        network->delService( this );

    id = RN_S_INVALID_ID;
    network = NULL;
}
