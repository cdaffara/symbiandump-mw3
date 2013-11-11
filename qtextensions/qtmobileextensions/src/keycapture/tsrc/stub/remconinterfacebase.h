#ifndef REMCONINTERFACEBASE_H
#define REMCONINTERFACEBASE_H

#include <e32base.h>

class CRemConInterfaceBase : public CBase
    { 
public:
    TInt Cancel();
    TAny* GetInterfaceIf(TUid aUid);
    };

#endif // REMCONINTERFACEBASE_H

// end of file
