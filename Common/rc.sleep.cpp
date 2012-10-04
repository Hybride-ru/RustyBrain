#include "rc.sleep.h"

//  ----------------------------------------------------------------------------
//  SLEEP FUNCTION
//  ----------------------------------------------------------------------------

#ifdef SYS_UNIX
    void mSleep( unsigned long inMs )
    {
        while( inMs-- )
            usleep(1000);
    }
#endif

