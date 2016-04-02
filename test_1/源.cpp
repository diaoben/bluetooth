//#include <Windows.h>
//#include <iostream>
//#include <bluetoothapis.h>
#include <WinSock2.h>
#include <ws2bth.h>
#include <iostream>


using namespace std;
int main(int argc, char *argv[])
{
	bool nneed = false;
	
	if (argc > 1)
	{
		if (strcmp(argv[1], "n")==0)
			nneed = true;
	}
	ULONG result = 0;
	WSADATA wsadata = { 0 };
	
    ULONG flag = 0, ulPQSSize = sizeof(WSAQUERYSET);
	
	result = WSAStartup(MAKEWORD(2, 2), &wsadata);
	WSAQUERYSET* wsaqueryset = (PWSAQUERYSET)HeapAlloc(GetProcessHeap(),
		HEAP_ZERO_MEMORY,
		ulPQSSize);

	
	HANDLE hLookUp = NULL;
	flag = LUP_CONTAINERS | LUP_RETURN_NAME | LUP_RETURN_ADDR | LUP_FLUSHCACHE;

	wsaqueryset->dwSize = ulPQSSize;
	wsaqueryset->dwNameSpace = NS_BTH;

	result = WSALookupServiceBegin(wsaqueryset, flag, &hLookUp);
			
	if(result == NO_ERROR)
	{ 
		
		if (hLookUp != NULL)
		{
			bool continueLookUp = true;
			
			while (continueLookUp)
			{

				if (NO_ERROR == WSALookupServiceNext(hLookUp, flag, &ulPQSSize, wsaqueryset))
				{
					if (nneed)
					{
					
					wprintf(wsaqueryset->lpszServiceInstanceName);
					wprintf(L"\n");
					}

					BTH_ADDR addr;				
					addr = ((SOCKADDR_BTH*)wsaqueryset->lpcsaBuffer->RemoteAddr.lpSockaddr)->btAddr;
					wprintf (L"%04x%08x\n", GET_NAP(addr), GET_SAP(addr));
					
				}
				else {
					result = WSAGetLastError();
					if (WSA_E_NO_MORE == result)
					{
						continueLookUp = FALSE;
					}
					else if (WSAEFAULT == result)
					{
						HeapFree(GetProcessHeap(), 0, wsaqueryset);
						wsaqueryset = (PWSAQUERYSET)HeapAlloc(GetProcessHeap(),
							HEAP_ZERO_MEMORY,
							ulPQSSize);
						if (NULL == wsaqueryset)
						{
							continueLookUp = FALSE;
						}
					}
					else
					{
						continueLookUp = FALSE;
					}
				}
			
			}

			WSALookupServiceEnd(hLookUp);
			WSACleanup();
			}
		}

	return 0;
}

















//#include <windows.h>  
//#include <BluetoothAPIs.h>
//#include <conio.h>
//#include <iostream>
//#include <string>
//
////#include <WinSock2.h>
////#include <ws2bth.h>
//
//#pragma comment(lib, "Bthprops.lib")
//
//using namespace std;
//
//
//int main(void)
//{
//	 wcout.imbue(locale(""));
//	 HBLUETOOTH_RADIO_FIND hbf = NULL;
//    HANDLE hbr = NULL;
//    HBLUETOOTH_DEVICE_FIND hbdf = NULL;
//    BLUETOOTH_FIND_RADIO_PARAMS btfrp = { sizeof(BLUETOOTH_FIND_RADIO_PARAMS) };
//    BLUETOOTH_RADIO_INFO bri = { sizeof(BLUETOOTH_RADIO_INFO) };
//    BLUETOOTH_DEVICE_SEARCH_PARAMS btsp = { sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS) };
//    BLUETOOTH_DEVICE_INFO btdi = { sizeof(BLUETOOTH_DEVICE_INFO) };
//    hbf = BluetoothFindFirstRadio(&btfrp, &hbr);
//    bool brfind = hbf != NULL;
//    while (brfind)
//    {
//        if (BluetoothGetRadioInfo(hbr, &bri) == ERROR_SUCCESS)
//        {
//			
//            cout << "Class of device: 0x" << uppercase << hex << bri.ulClassofDevice << endl;
//            wcout << "Name:" << bri.szName << endl;
//            cout << "Manufacture:0x" << uppercase << hex << bri.manufacturer << endl;
//            cout << "Subversion:0x" << uppercase << hex << bri.lmpSubversion << endl;
//            //  
//            btsp.hRadio = hbr;
//            btsp.fReturnAuthenticated = TRUE;
//           btsp.fReturnConnected = FALSE;
//           btsp.fReturnRemembered = TRUE;
//            btsp.fReturnUnknown = TRUE;
//            btsp.cTimeoutMultiplier = 30;
//            hbdf = BluetoothFindFirstDevice(&btsp, &btdi);
//            bool bfind = hbdf != NULL;
//            while (bfind)
//            {
//                wcout << "[Name]:" << btdi.szName;
//                cout << ",[Address]:0x" << uppercase << hex << btdi.Address.ullLong << endl;
//                bfind = BluetoothFindNextDevice(hbdf, &btdi);
//            }
//            BluetoothFindDeviceClose(hbdf);
//        }
//        CloseHandle(hbr);
//        brfind = BluetoothFindNextRadio(hbf, &hbr);
//    }
//    BluetoothFindRadioClose(hbf);
//    _getch();
//	    return 0;
//}
//
