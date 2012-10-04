#include "include.h"
#include "rb.chatserver.h"
#include "rb.voipserver.h"
#include "rb.rmdserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qRegisterMetaType<RNSocket>("RNSocket");

    RNetwork testNetwork( RB_PACK_SIZE, RB_TIMEOUT, RB_PORT );
    RBEncryptor myEncr;
    RBChatServer s;
    RBVoipServer v;
    RBRmdServer  r;

    testNetwork.addEncryptor( &myEncr );
    testNetwork.addService( s );
    testNetwork.addService( v );
    testNetwork.addService( r );

    return a.exec();
}
