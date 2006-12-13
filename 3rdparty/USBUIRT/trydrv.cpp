// trydrv.cpp : Defines the entry point for the console application.
//

#include <windows.h>
#include <stdio.h>
#include "uuirtdrv.h"
#include "conio.h"

HINSTANCE		hinstLib = NULL; 

// Driver handle for UUIRT device
HUUHANDLE		hDrvHandle;

unsigned int	drvVersion;

// Globals to hold last-learned IR-Code and its format...
char	gIRCode[2048] = "0000 0071 0000 0032 0080 0040 0010 0010 0010 0030 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0030 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0030 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0030 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0030 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0030 0010 0aad";
int		gIRCodeFormat = UUIRTDRV_IRFMT_PRONTO;

// Globals used during Learning...
char	gLearnBuffer[2048];
int		gLearnFormat;

// UUIRT .dll funtion pointers. These will be assigned when calling LoadDLL()
pfn_UUIRTOpen				fnUUIRTOpen;
pfn_UUIRTClose				fnUUIRTClose;
pfn_UUIRTGetDrvInfo			fn_UUIRTGetDrvInfo;
pfn_UUIRTGetUUIRTInfo		fn_UUIRTGetUUIRTInfo;
pfn_UUIRTGetUUIRTConfig		fn_UUIRTGetUUIRTConfig;
pfn_UUIRTSetUUIRTConfig		fn_UUIRTSetUUIRTConfig;
pfn_UUIRTSetReceiveCallback	fn_UUIRTSetReceiveCallback;
pfn_UUIRTTransmitIR			fn_UUIRTTransmitIR;
pfn_UUIRTLearnIR			fn_UUIRTLearnIR;

/*****************************************************************************/
/* unLoadDLL: Disconnects from .DLL and unloads it from memory				 */
/*																			 */
/* returns: none															 */
/*																			 */
/*****************************************************************************/
void unLoadDLL(void)
{
	if (hinstLib)
		FreeLibrary(hinstLib);
	hinstLib = NULL;
}

/*****************************************************************************/
/* loadDLL: Establish contact with the UUIRTDRV dll and assign function      */
/*			entry points													 */
/*																			 */
/* returns: TRUE on success, FALSE on failure								 */
/*																			 */
/*****************************************************************************/
BOOL loadDLL(void)
{
    // Get a handle to the DLL module.
 
    hinstLib = LoadLibrary("uuirtdrv"); 
 
    // If the handle is valid, try to get the function address.
 
    if (hinstLib != NULL) 
    { 
        fnUUIRTOpen = (pfn_UUIRTOpen) GetProcAddress(hinstLib, "UUIRTOpen");
        fnUUIRTClose = (pfn_UUIRTClose) GetProcAddress(hinstLib, "UUIRTClose");
		fn_UUIRTGetDrvInfo  = (pfn_UUIRTGetDrvInfo) GetProcAddress(hinstLib, "UUIRTGetDrvInfo");
		fn_UUIRTGetUUIRTInfo = (pfn_UUIRTGetUUIRTInfo) GetProcAddress(hinstLib, "UUIRTGetUUIRTInfo");
		fn_UUIRTGetUUIRTConfig = (pfn_UUIRTGetUUIRTConfig) GetProcAddress(hinstLib, "UUIRTGetUUIRTConfig");
		fn_UUIRTSetUUIRTConfig = (pfn_UUIRTSetUUIRTConfig) GetProcAddress(hinstLib, "UUIRTSetUUIRTConfig");
		fn_UUIRTSetReceiveCallback = (pfn_UUIRTSetReceiveCallback) GetProcAddress(hinstLib, "UUIRTSetReceiveCallback");
		fn_UUIRTTransmitIR = (pfn_UUIRTTransmitIR) GetProcAddress(hinstLib, "UUIRTTransmitIR");
		fn_UUIRTLearnIR = (pfn_UUIRTLearnIR) GetProcAddress(hinstLib, "UUIRTLearnIR");

		if (!fnUUIRTOpen || 
			!fnUUIRTClose || 
			!fn_UUIRTGetDrvInfo || 
			!fn_UUIRTGetUUIRTInfo || 
			!fn_UUIRTGetUUIRTConfig || 
			!fn_UUIRTSetUUIRTConfig || 
			!fn_UUIRTSetReceiveCallback || 
			!fn_UUIRTTransmitIR || 
			!fn_UUIRTLearnIR)
		{
			unLoadDLL();
			return FALSE;
		}

		return TRUE;
	}
	return FALSE;
}

/*****************************************************************************/
/* IRReceiveCallback: Receive IR Callback Procedure						     */
/*																			 */
/* This procedure is called by the UUIRT .dll whenever an IRcode is received */
/* The IRcode is passed to the callback in UIR format.						 */
/*																			 */
/*****************************************************************************/
void WINAPI IRReceiveCallback (char *IREventStr, void *userData)
{
	COORD coord;
	HANDLE hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO conInfo;

	// Move the cursor to the bottom of the screen
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE); 
	GetConsoleScreenBufferInfo(hStdOut, &conInfo);
	coord.X = conInfo.srWindow.Left;
	coord.Y = conInfo.srWindow.Bottom-1;
	SetConsoleCursorPosition(hStdOut, coord);

	printf("<IR Receive: Code = %s, UserData = %08x!!!\n", IREventStr, (UINT32)userData);

	// Move the cursor back to its original position
	SetConsoleCursorPosition(hStdOut, conInfo.dwCursorPosition);
}

/*****************************************************************************/
/* IRLearnCallback: Learn IR Callback Procedure						         */
/*																			 */
/* This procedure is called by the UUIRT .dll during the LEARN process		 */
/* to allow user feedback on Learn progress, signal quality and (if needed)  */
/* carrier frequency.														 */
/*																			 */
/*****************************************************************************/
void WINAPI IRLearnCallback (unsigned int progress, unsigned int sigQuality, unsigned long carrierFreq, void *userData)
{
	COORD coord;
	HANDLE hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO conInfo;

	// Move the cursor to the bottom of the screen
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE); 
	GetConsoleScreenBufferInfo(hStdOut, &conInfo);
	coord.X = conInfo.srWindow.Left;
	coord.Y = conInfo.srWindow.Bottom-2;
	SetConsoleCursorPosition(hStdOut, coord);

	printf("<Learn Progress: %d%%, Signal = %d%%, Freq = %ld, UserData = %08x!!!\n", progress, sigQuality & 0xff, carrierFreq, (UINT32)userData);

	// Move the cursor back to its original position
	SetConsoleCursorPosition(hStdOut, conInfo.dwCursorPosition);
}


/*****************************************************************************/
/* LearnThread: Learn IR Thread function									 */
/*																			 */
/* This function executes as a separate thread which calls the UUIRTLearnIR  */
/* function.  In this example, the UUIRTLearnIR function is called from this */
/* separate thread to allow the main console thread to continue monitoring   */
/* the keyboard so that the user may abort the learn process. Depending on   */
/* the application, the UUIRTLearnIR may be called from the main thread if   */
/* an asynchronous method (such as a timer) is available to monitor user     */
/* input.																	 */
/*																			 */
/*****************************************************************************/
DWORD WINAPI LearnThread( LPVOID lpParameter )
{
	BOOL *pbAbortLearn = (BOOL *)lpParameter;

	printf("\nCalling LearnIR...");
	if (!fn_UUIRTLearnIR(hDrvHandle, gLearnFormat, gLearnBuffer, IRLearnCallback, (void *)0x5a5a5a5a, pbAbortLearn, 0, NULL, NULL))
	{
		printf("\n\t*** ERROR calling UUIRTLearnIR! ***\n");
	}
	else
	{
		if (!*pbAbortLearn)
		{
			printf("...Done...IRCode = %s\n",gLearnBuffer);
			strcpy(gIRCode, gLearnBuffer);
			gIRCodeFormat = gLearnFormat & 0xff;
		}
		else
		{
			printf("...*** LEARN ABORTED ***\n");
		}
	}
	return 0;
}

int main(int argc, char* argv[])
{
	UUINFO UsbUirtInfo;
	UINT32 uConfig;
	char ch;

	printf("UUIRTDRV Example Program...\n");
	printf("===========================\n\n");

	if (!loadDLL())
	{
		printf("ERROR: Unable to load uuirtdrv,dll!\n");
		Sleep(1000);
		return 0;
	}

	if (!fn_UUIRTGetDrvInfo(&drvVersion))
	{
		printf("ERROR: Unable to retrieve uuirtdrv version!\n");
		Sleep(1000);
		unLoadDLL();
		return 0;
	}

	if (drvVersion != 0x0100)
	{
		printf("ERROR: Invalid uuirtdrv version!\n");
		Sleep(1000);
		unLoadDLL();
		return 0;
	}

	hDrvHandle = fnUUIRTOpen();
	if (hDrvHandle == INVALID_HANDLE_VALUE)
	{
		DWORD err;

		err = GetLastError();

		if (err == UUIRTDRV_ERR_NO_DLL)
		{
			printf("ERROR: Unable to find USB-UIRT Driver. Please make sure driver is Installed!\n");
		}
		else if (err == UUIRTDRV_ERR_NO_DEVICE)
		{
			printf("ERROR: Unable to connect to USB-UIRT device!  Please ensure device is connected to the computer!\n");
		}
		else if (err == UUIRTDRV_ERR_NO_RESP)
		{
			printf("ERROR: Unable to communicate with USB-UIRT device!  Please check connections and try again.  If you still have problems, try unplugging and reconnecting your USB-UIRT.  If problem persists, contact Technical Support!\n");
		}
		else if (err == UUIRTDRV_ERR_VERSION)
		{
			printf("ERROR: Your USB-UIRT's firmware is not compatible with this API DLL. Please verify you are running the latest API DLL and that you're using the latest version of USB-UIRT firmware!  If problem persists, contact Technical Support!\n");
		}
		else
		{
			printf("ERROR: Unable to initialize USB-UIRT (unknown error)!\n");
		}

		unLoadDLL();

		printf("Press any key to exit...");
		getch();
		printf("\n");
		
		return 0;
	}

	printf("\n");

	// Register a callback function for IR receive...
	fn_UUIRTSetReceiveCallback(hDrvHandle, &IRReceiveCallback, (void *)0xA5A5A5A5);

	while (1)
	{
		HANDLE hStdOut;
		CONSOLE_SCREEN_BUFFER_INFO conInfo;
		COORD coord;
		DWORD cWritten;

		// Move cursor to top-left of screen
		hStdOut = GetStdHandle(STD_OUTPUT_HANDLE); 
		GetConsoleScreenBufferInfo(hStdOut, &conInfo);
		coord.X = conInfo.srWindow.Left;
		coord.Y = conInfo.srWindow.Top;
		SetConsoleCursorPosition(hStdOut, coord);

		// Clear screen
		FillConsoleOutputCharacter( 
			hStdOut,          // screen buffer handle 
			' ',       // fill with spaces 
			(conInfo.srWindow.Right - conInfo.srWindow.Left + 1)*(conInfo.srWindow.Bottom-conInfo.srWindow.Top+1),            // number of cells to fill 
			coord,            // first cell to write to 
			&cWritten);       // actual number written 
		

		// Retrieve information about the USB-UIRT device...
		if (!fn_UUIRTGetUUIRTInfo(hDrvHandle, &UsbUirtInfo))
		{
			printf("\t*** ERROR Retrieving USB-UIRT Information! ***\n");
		}
		else
		{
			printf("\tUSB-UIRT Info: FirmwareVer=%d.%d ProtVer=%d.%d FirmwareDate=%d/%d/%d\n",
					UsbUirtInfo.fwVersion>>8,
					UsbUirtInfo.fwVersion&0xff,
					UsbUirtInfo.protVersion>>8,
					UsbUirtInfo.protVersion&0xff,
					UsbUirtInfo.fwDateMonth,
					UsbUirtInfo.fwDateDay,
					UsbUirtInfo.fwDateYear+2000);
			printf("\tuuirtdrv: %u\n", drvVersion);
		}

		printf("\n");

		// Retrieve USB-UIRT feature config...
		if (!fn_UUIRTGetUUIRTConfig(hDrvHandle, &uConfig))
		{
			printf("\t*** ERROR Retrieving USB-UIRT Configuration Info! ***\n");
		}

		printf("TRYDRV Test Menu v0.5:\n");
		printf("-----------------------------\n");
		printf("(1) Transmit IR Code (blocking)\n");
		printf("(2) Transmit IR Code (non-blocking)\n");
		printf("(3) Learn IR Code (Pronto Format)\n");
		printf("(4) Learn IR Code (UIRT-Raw Format)\n");
		printf("(5) Learn IR Code (UIRT-Struct Format)\n");
		printf("(6) Change Config, currently = %08X (LED_RX=%d, LED_TX=%d, LEGACY_RX=%d)\n",
					uConfig,
					uConfig & UUIRTDRV_CFG_LEDRX ? 1 : 0,
					uConfig & UUIRTDRV_CFG_LEDTX ? 1 : 0,
					uConfig & UUIRTDRV_CFG_LEGACYRX ? 1 : 0);
		printf("(ESC) Exit\n");
		printf("-----------------------------\n");

		printf("Press a key...");
		ch = getch();
		printf("%c\n",ch<' '?' ':ch);

		if (ch == 27)	// ESC key
			break;

		switch (ch)
		{
			case '1':
				printf("\nTransmitting IR Code (blocking)...");
				if (!fn_UUIRTTransmitIR(hDrvHandle,
										gIRCode /* IRCode */,
										gIRCodeFormat /* codeFormat */,
										10 /* repeatCount */,
										0 /* inactivityWaitTime */,
										NULL /* hEvent */,
										NULL /* reserved1 */,
										NULL /* reserved2 */
										))
				{
					printf("\n\t*** ERROR calling UUIRTTransmitIR! ***\n");
				}
				else
				{
					printf("...Returned from call (Done)!\n");
				}
				printf("Press any key to continue...");
				getch();
				break;
			case '2':
				{
					HANDLE hIrDoneEvent = NULL;
					
					hIrDoneEvent = CreateEvent(NULL, FALSE, FALSE, "hUSBUIRTXAckEvent");
					if (!hIrDoneEvent)
					{
						printf("\n\t*** ERROR: unable to create Windows Event! ***\n");
						break;
					}

					printf("\nTransmitting IR Code (non-blocking)...");
					if (!fn_UUIRTTransmitIR(hDrvHandle,
											gIRCode /* IRCode */,
											gIRCodeFormat /* codeFormat */,
											10 /* repeatCount */,
											0 /* inactivityWaitTime */,
											hIrDoneEvent /* hEvent */,
											NULL /* reserved1 */,
											NULL /* reserved2 */
											))
					{
						printf("\n\t*** ERROR calling UUIRTTransmitIR! ***\n");
					}
					else
					{
						printf("...Returned from call...");
						if (WaitForSingleObject(hIrDoneEvent, 5000) == WAIT_OBJECT_0)
						{
							printf("...IR Transmission Complete!\n");
						}
						else
						{
							printf("\n\t*** ERROR: Timeout error waiting for IR to finish!\n");
						}
					}
					if (hIrDoneEvent)
						CloseHandle(hIrDoneEvent);
					printf("Press any key to continue...");
					getch();
				}
				break;
			case '3':
			case '4':
			case '5':
				{
					DWORD dwThreadId;
					BOOL bLrnAbort = FALSE;
					HANDLE LearnThreadHandle;

					// Launch Learning thread. We use a thread here so that we can continuously monitor user keyboard
					// input and abort the learn process if the user presses ESC. For a Windows GUI app, a thread
					// may not be necessary if another asynchronous method is available for the user to abort the learn
					// process.
					if (ch == '3')
						gLearnFormat = UUIRTDRV_IRFMT_PRONTO;
					else if (ch == '4')
						gLearnFormat = UUIRTDRV_IRFMT_UUIRT;
					else 
						gLearnFormat = UUIRTDRV_IRFMT_UUIRT | UUIRTDRV_IRFMT_LEARN_FORCESTRUC;

					LearnThreadHandle = CreateThread(NULL,0,&LearnThread,(void *)&bLrnAbort,0,&dwThreadId);

					printf("<Press ESC to abort Learn>");

					while (WaitForSingleObject(LearnThreadHandle, 100) == WAIT_TIMEOUT)
					{
						if (kbhit())
						{
							ch = getch();
							if (ch == 27)	// ESC key
								bLrnAbort = TRUE;
						}
					}
					CloseHandle(LearnThreadHandle);

					printf("Press any key to continue...");
					getch();
				}
				break;
			case '6':
				{
					long newConfig = 0;

					printf("\nEnter new config value: ");
					scanf("%ld",&newConfig);
					uConfig = newConfig;
					printf("Programming new config value=%08X...",uConfig);
					if (!fn_UUIRTSetUUIRTConfig(hDrvHandle, uConfig))
					{
						printf("\n\t*** ERROR calling UUIRTSetUUIRTConfig! ***\n");
					}
					else
					{
						printf("...Done\n");
					}
					printf("Press any key to continue...");
					getch();
				}
				break;
			default:
				break;
		}
	}

	fnUUIRTClose(hDrvHandle);

	unLoadDLL();

	printf("\n");

	return 0;
}

