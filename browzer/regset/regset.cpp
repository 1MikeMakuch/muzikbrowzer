// regset.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "regset.h"
#include "../util/Registry.h"
#define LOGOWNER
#include "../util/MyLog.h"
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
//		CString strHello;
//		strHello.LoadString(IDS_HELLO);
//		cout << (LPCTSTR)strHello << endl;

		if (argc < 3) {
			cout << "usage: regset <width> <height>" << endl;
			return 0;
		}

		RegistryKey reg( HKEY_LOCAL_MACHINE, RegKey );
		int x1 = reg.Read(RegWindowX1, 0);
		int y1 = reg.Read(RegWindowY1, 0);
		int x2 = reg.Read(RegWindowX2, 0);
		int y2 = reg.Read(RegWindowX2, 0);
		cout << "TopLeft " << x1 << "," << y1 << endl;
		cout << "BotRght " << x2 << "," << y2 << endl << endl;

		cout << "argv[1,2] = " << argv[1] << "," << argv[2] << endl;
		x2 = x1 + atoi(argv[1]);
		y2 = y1 + atoi(argv[2]);


		reg.Write(RegWindowX2, x2);
		reg.Write(RegWindowY2, y2);

		x1 = reg.Read(RegWindowX1, 0);
		y1 = reg.Read(RegWindowY1, 0);
		x2 = reg.Read(RegWindowX2, 0);
		y2 = reg.Read(RegWindowY2, 0);
		cout << "TopLeft " << x1 << "," << y1 << endl;
		cout << "BotRght " << x2 << "," << y2 << endl;

		cout << "size " << x2-x1 << "," << y2-y1 << endl;
	}

	return nRetCode;
}
