/*
 * local.cxx
 *
 * copyright 2005 Derek J Smithies
 *
 *
 * Simple program to report the host name of this machine
 *                          the address of one network interface
 *
 * $Revision: 28484 $
 * $Author: rjongbloed $
 * $Date: 2012-10-05 03:53:11 -0500 (Fri, 05 Oct 2012) $
 */
#include <ptlib.h>
#include <ptlib/sockets.h>

class LocalAddress : public PProcess
{
    PCLASSINFO(LocalAddress, PProcess);
public:
    LocalAddress();
    
    void Main();
};

PCREATE_PROCESS(LocalAddress);

LocalAddress::LocalAddress()
    : PProcess("PwLib Example Factory", "local", 1, 0, ReleaseCode, 0)
{
}

void LocalAddress::Main()
{
    PStringStream progName;
    progName << "Product Name: " << GetName() << endl
             << "Manufacturer: " << GetManufacturer() << endl
             << "Version     : " << GetVersion(true) << endl
             << "System      : " << GetOSName() << '-'
             << GetOSHardware() << ' '
             << GetOSVersion();
    cout << endl <<  progName << endl << endl;
    

    PUDPSocket localSocket;
    PIPSocket::Address addr;
    if(localSocket.GetNetworkInterface(addr)) {
        cout << "local address is    " << addr.AsString() << endl;
        if (addr == 0)
            cout << "sorry, that is a 0.0.0.0 address" << endl;
    } else
      cout << "Sorry, failed to get local address" << endl;
    
    cout << "local host name is  " << localSocket.GetHostName() << endl;
}

// End of local.cxx