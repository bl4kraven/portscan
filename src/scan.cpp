// port scan
// Usage:
//     portscan ipaddress portbegin [portend]

#include <iostream>
#include <cstdlib>
#include "SimpleNet/Src/snet.h"
#include "lib-linux/src/utility.h"
using namespace std;
using namespace snet;

void Help()
{
    cout<<"Usage:"<<endl
        <<"    portscan ipaddress portbegin [portend]"<<endl;
}

class ScanSession:public ISession
{
    protected:
        void OnConnect(SessionManager *pSessionManager)
        {
            cout<<get_peer_addr().get_port()<<" Opened"<<endl;
            //TRACE("Current session num: %d", pSessionManager->GetSessionCount());
            pSessionManager->Delete(this);
        }

        void OnDisconnect(SessionManager *pSessionManager)
        {
        }

        void OnRead(SessionManager *pSessionManager)
        {
        }

        void OnError(SessionManager *pSessionManager, int nErrorCode, const char *pStr)
        {
            //Socket::OnError(nErrorCode, pStr);
        }
};

int main(int argc, const char *argv[])
{
    // linux默认每个进程文件描述符为1024个
    int nMaxfd = 500;
    int nPortBegin = 0;
    int nPortEnd = 0;
    int nPortCurrent = 0;
    const char *pstrIPAddress = NULL;

    switch (argc)
    {
        case 3:
            pstrIPAddress = argv[1];
            nPortCurrent = nPortBegin = nPortEnd = atoi(argv[2]);
            break;
        case 4:
            pstrIPAddress = argv[1];
            nPortCurrent = nPortBegin = atoi(argv[2]);
            nPortEnd = atoi(argv[3]);
            break;
        default:
            Help();
            exit(1);
    }

    if (nPortBegin <= 0 || nPortEnd >= 65535 ||
        nPortEnd <= 0 || nPortEnd >= 65535 ||
        nPortBegin > nPortEnd)
    {
        Help();
        exit(1);
    }

    SessionFactoryImp<ScanSession> sm;
    SessionManager manager(nMaxfd);

    while (true)
    {
        int sessions_num = manager.GetSessionCount() + manager.GetWaitingNBSessionCount();
        if (sessions_num == 0 && nPortCurrent > nPortEnd)
        {
            break;
        }
        else if (nPortCurrent <= nPortEnd)
        {
            // add remain scan port
            int fd_remain = nMaxfd - sessions_num;
            for (int i=0; i<fd_remain && nPortCurrent <= nPortEnd; i++,nPortCurrent++)
            {
                ISession *pSession = manager.Connect(sm, Address(pstrIPAddress, nPortCurrent), false);
                assert(pSession);
                //TRACE("scan %s %d", pstrIPAddress, nPortCurrent);
            }
        }

        timeval timeout = { 1, 0 };
        manager.Poll(timeout);
        Utility::Sleep(10);
    }

    return 0;
}
