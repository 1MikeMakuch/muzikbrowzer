// $Header: E:\CVS/Software/APIDLL/TiraHost.cpp,v 1.9 2005/10/25 19:35:45 Administrator Exp $
// ---------------------------------------------------------------------------
//=============================================================================
//  Copyright (C) 2004, Home Electronics ( http://www.home-electro.com )
//  support@home-electro.com
//
// This software is a part of Tira API. Permission is granted to use and modify
// as long as this notice is preserved, and the software remains a part of Tira API
// 
// No warranty express or implied.
//=============================================================================

#include <string.h>

#define USE_CAPTURE

#ifdef _WIN32_
	#include <windows.h>
	#include <conio.h>
	#include <stdio.h>
	using namespace std;

	#define STR_W(S) S

	#define TIRA_LIBRARY	"tira2.dll"
//	#define TIRA_LIBRARY	"tira3.dll"
//#define TIRA_LIBRARY	"Tira2eagle.dll"
//#define TIRA_LIBRARY	"..\\HIDLib\\HidDLL.dll"
	typedef char SCHAR;
   #define STRCPY  strcpy
#elif defined (_WIN32_WCE)
	int kbhit() { return 1; };

	int getche() { return getchar(); }

	#define STR_W(S) L##S

	#define TIRA_LIBRARY	L"tira_ce"

	typedef wchar_t SCHAR;
   #define STRCPY   wcscpy
#elif defined (__linux__)

	#include <errno.h>
	#include <dlfcn.h>
	#include <termios.h>
	#include <unistd.h>
	#include <fcntl.h>

	#include <stdio.h>
	

	static int lastKeyRead = 0;
	static char ReadLine[2];
	int kbhit() { 
		
		int res = read (0, ReadLine, 1);
		if (res)		
			return 1; 
		else
			return 0;
	};
	int getche() { 
		return ReadLine[0]; 
	}
	#define STR_W(S) S
	#define TIRA_LIBRARY	"obj/TiraAPI.so"
	typedef	char SCHAR;
   #define STRCPY   strcpy
	#define __stdcall	
	typedef void* HMODULE;
	
	int GetLastError() {
		return errno;	
	};   
   
	void* LoadLibrary(const char* LibName) {
		void* res = dlopen(LibName, RTLD_NOW);
		if ( res == 0 )
			printf(dlerror());
		return res;
	};
	bool FreeLibrary(void* handle) {
		return dlclose(handle) == 0;	
	};
	void* GetProcAddress(void* handle, const char* Name){
		return dlsym(handle,Name);
	};
	void Sleep(int n) {
	   usleep(1000 * n);   
	};

#endif

	static SCHAR LibName[200];
//---------------------------------------------------------------------------

int Error() {

   printf("Last Error returned : %d\n", GetLastError() );
   return 1;
};

int EmptyFunc() {
	printf("API Library not loaded\n");
	return 1;
};
typedef int (__stdcall * tira_six_byte_cb) (const char * eventstring);

typedef int (__stdcall * t_tira_init) ();
typedef int (__stdcall * t_tira_cleanup) ();
typedef int (__stdcall * t_tira_set_handler) (tira_six_byte_cb cb);
typedef int (__stdcall * t_tira_start) ( int PortID);
typedef int (__stdcall * t_tira_stop) ();
typedef int (__stdcall * t_tira_start_capture) ();
typedef int (__stdcall * t_tira_cancel_capture) ();
typedef int (__stdcall * t_tira_get_captured_data)
                   (const unsigned char** data, int* size );;
typedef int (__stdcall * t_tira_transmit)
       (int repeat, int frequency, const unsigned char* data, const int dataSize );
typedef int (__stdcall * t_tira_delete)(const unsigned char* ptr);

typedef int (__stdcall * t_tira_set_dword)(unsigned int addr, unsigned int val);
typedef const char* (__stdcall * t_tira_get_version)(int component);

typedef int (__stdcall * t_tira_access_feature)(
                                          unsigned int FeatureID,
                                          bool Write,
                                          unsigned int* Value,
                                          unsigned int Mask);

t_tira_init               p_tira_init = (t_tira_init)EmptyFunc;
t_tira_cleanup            p_tira_cleanup = (t_tira_cleanup)EmptyFunc;;
t_tira_set_handler        p_tira_set_handler = (t_tira_set_handler)EmptyFunc;;
t_tira_start              p_tira_start = (t_tira_start)EmptyFunc;;
t_tira_stop               p_tira_stop = (t_tira_stop)EmptyFunc;;
t_tira_start_capture      p_tira_start_capture = (t_tira_start_capture)EmptyFunc;;
t_tira_cancel_capture     p_tira_cancel_capture = (t_tira_cancel_capture)EmptyFunc;;
t_tira_get_captured_data  p_tira_get_captured_data = (t_tira_get_captured_data)EmptyFunc;;
t_tira_transmit           p_tira_transmit = (t_tira_transmit)EmptyFunc;;
t_tira_delete             p_tira_delete = (t_tira_delete)EmptyFunc;;
t_tira_set_dword          p_tira_set_dword = (t_tira_set_dword) EmptyFunc;
t_tira_get_version        p_tira_get_version = (t_tira_get_version) EmptyFunc;
t_tira_access_feature     p_tira_access_feature = (t_tira_access_feature) EmptyFunc;


int __stdcall OurCalback(const char * eventstring) {
   printf("IR Data %s\n", eventstring);
   return 0;
};
int Help();

static HMODULE handle = 0;
int LoadTiraDLL();
int UnloadTiraDLL();

int main(int argc, char* argv[]) {

#ifdef __linux
	
	// unblock stdio
	struct termios tty;
	struct termios savetty;
	tcgetattr(0, &savetty);
	tcgetattr(0, &tty);
	tty.c_lflag &= ~(ICANON|IEXTEN);
	//tty.c_lflag &= ~(ECHO|ECHONL|ICANON|IEXTEN);
	tty.c_cc[VTIME] = 0;
	tty.c_cc[VMIN] = 0;
	tcsetattr(0, TCSADRAIN, &tty);
	
#endif

	if ( argc > 1 )
		STRCPY(LibName, argv[1]);
	else
		STRCPY(LibName, TIRA_LIBRARY);
	
	printf("Using api library: %s\n", LibName);
   Help();
   bool CaptureActive = false;
   unsigned char EnteredData[500];
   const unsigned char* Data = 0;
   int         DataSize = 0;

   printf("\n");
	
   while (1) {

      int res = -1;

      Sleep(40);

      if ( CaptureActive ) {
         res = p_tira_get_captured_data(&Data, &DataSize);
         if ( Data != 0 && DataSize != 0 ){
            printf("IR Code captured!\n");
            CaptureActive = false;
         }; // else
            // cout << "No Data so far...\n";
      };
      char c = 0;
      if ( kbhit() ) {
         c = getche();
         if (c == -1 )
         	continue;
   	}else
         continue;
		
	   printf("\n");

      switch (c) {
         case 'L':
         case 'l':
				//printf("Enter Library Name:");
				//scanf("%ls", LibName);
#ifndef _WIN32_WCE
				printf("Loading Library: %s\n", LibName);
#else
				printf("Loading Library: %ls\n", LibName);
#endif
				res = LoadTiraDLL();
               if ( res == 0 ) 
					printf("Library Loaded\n");
			   else
					break;

               p_tira_init();
               CaptureActive = false;
               break;
         case 'U':
         case 'u':
               res = UnloadTiraDLL();
               if ( res == 0 ) printf("Library unloaded\n");
               CaptureActive = false;
               break;
         case  '0':      
         case  '1':    case '2':   case '3':    case '4':
         case  '5':    case '6':   case  '7':   case '8':
         case  '9':

               {
                  int p = c - '1';
		#if  defined(__linux) 
			p++;	// On linux ports
			// printf ("Opening Port %d \n", p);
		#endif
                  
                  if ( c == '9' ) p = 1024;

                  res = p_tira_start(p);
                  if ( res == 0 ) printf("Tira activated\n");
                  break;
               };
         case 'S':
         case 's':
               res = p_tira_stop();
               if ( res == 0 ) printf("Tira disactivated\n");
               CaptureActive = false;
               break;
         case 'B':
         case 'b':
               res = p_tira_set_handler(OurCalback);
               if ( res == 0 ) printf("Callback activated\n");
               break;
         case 'F':
         case 'f':
               {
               char InputStr[200];
               unsigned int FeatureID, FeatureVal;
               printf("Access feature...\n\n");
               printf("Enter Feature ID :");

               fgets(InputStr, 200, stdin);
               sscanf(InputStr, "%x", &FeatureID);

               printf("Enter Feature Value :");
               fgets(InputStr, 200, stdin);
               sscanf(InputStr, "%x", &FeatureVal);

               res = p_tira_access_feature(FeatureID, 1, &FeatureVal, 0);
               if ( res == 0 )
                   printf("Feature Value updated");
               break;
               }

         case 'C':
         case 'c':
               if ( Data != 0 ) {
                  printf("Disposing captured data...\n");
                  res = p_tira_delete(Data);
                  if ( res == 0 ) printf("Memory freed\n");
                  Data = 0;
               };
               res = p_tira_start_capture();
               if ( res == 0 ) printf("Capture activated\n");
               CaptureActive = (res == 0 );
               break;
         case 'A':
         case 'a':
               res = p_tira_cancel_capture();
               if ( res == 0 ) printf("Capture disactivated\n");
               CaptureActive = false;
               break;

         case 'T':
         case 't':
               if ( Data == 0 ) {
                  printf("There is nothing to transmit\n");
                  break;
              };
               res = p_tira_transmit(2, /* repeat 3 times*/
                                     -1, /* Use embedded frequency value*/
                                     Data,
                                     DataSize);

               if ( res == 0 ) printf("IR code transmitted\n");

               CaptureActive = false;
               break;

         case 'D':
         case 'd':
               res = p_tira_delete(Data);
               if ( res == 0 ) printf("Memory freed\n");
               Data = 0;
               break;

         case 'P':
         case 'p':
				if ( DataSize == 0 )
					printf("There is no IR code stored\n");
				else {
					for ( int i = 0; i < DataSize; i++) {
						int t = Data[i];
						printf("%02x ", t);
						if (i % 8 == 7 )
							printf("\n");
					};
					printf("\n");
				};
				break;
         case 'E':
         case 'e':
			 {
				printf("Enter the IR code data (hex format)\n");
				printf("You can split it over several lines. Enter empty line to finish\n");
				int CurrByte = 0;
				char HexString[300];

		#if  defined(__linux) 
			tcsetattr(0, TCSADRAIN, &savetty); //Need to enable blocking mode for line input
		#endif
		#if  defined(_WIN32_WCE) 
			fgets(HexString, 200, stdin); // we need to discard the CR
		#endif
				while(1) {
					
					memset(HexString, 0, 200);
					char * res = fgets(HexString, 200, stdin);
					
					if ( res == 0 ) {
						Sleep(100);
						continue;		
					};
					
					
					if ( HexString[0] == 0 )
						break;
					//parse the enterd string
					int CurrStart = 0;
					bool EmptyString = true;
					while( HexString[CurrStart] != 0 ) {
						int HexNum = 0;
						int BytesRead = 0;
						int N = sscanf(HexString + CurrStart, " %X%n",&HexNum, &BytesRead);
						//printf("wwww %d\n", N);
						if ( N <= 0 )
							break;
						EmptyString = false;
						EnteredData[CurrByte++] = HexNum;
						CurrStart += BytesRead;
					};
					if ( EmptyString )
						break;
	
				};
				Data = EnteredData;
				DataSize = CurrByte;
		#if  defined(__linux) 
				tcsetattr(0, TCSADRAIN, &tty);
      #endif
				break;
	
			 };
         case EOF:
         case '\n':
               break;

         case 'R':
         case 'r':
               { int Threshold = 0;
                  printf("Enter new threshold (2 - 31) :");
                  scanf("%d", &Threshold);

                  if ( Threshold > 1 && Threshold < 31) {
                     int res = p_tira_set_dword(2, Threshold);
                     if ( res == 0 ) {
                        printf("Assigned new threshold value\n");
                     };
                  };
               };
               break;
         case 'H':
         case 'h':
            printf( "\t The functions must be called in a specific order\n"
                     "\t 1. Execute 'LoadLibrary'. This also executes 'tira_init'\n"
                     "\t 2. Open Tira on the appropriate COM port\n"
                     "\t 3. Attach callback\n\n"
                     "\t At this point you'll be able to receive 6 byte IR codes \n"
                     "\t    from your remote\n"
                     "\t point the remote to Tira and press some button\n"
                     "\t Note that '6 byte' codes are not sutable for transmission\n"
                     "\t In order to be able to transmit do the following:\n"
                     "\t 1. call 'tira_start_capture'\n"
                     "\t 2. periodically call 'tira_get_captured_data' to see if IR data is available\n"
                     "\t 3. Note that in 'capture' mode Tira uses short range receiver: \n"

                     "\t    The remote must be within one inch from Tira and it's emitter\n"
                     "\t    must point directly to the center of Tira\n"
                     "\t 4. If you want to cancel 'capture' mode, call 'tira_cancel_capture'\n"
                     "\t 5. Each successful capture cycle Tira2.dll will allocate a new memory block\n"
                     "\t 6. Your application is responsible for proper disposal of that memory\n"
                     "\t    use tira_delete to free the memory block returned from tira_get_captured_data\n"
                     "\t 7. Call 'tira_transmit' to transmit previously captured data\n"

					 );
                     break;
         case 'Q':
         case 'q':
               printf("Exiting...\n");
               goto L_EXIT;
         case 'V':
         case 'v':
               {
               const char* Version = p_tira_get_version(0);
               printf("DLL Version: %s\n", Version);
               Version = p_tira_get_version(1);
               printf("Firmware Version: %s\n", Version);
               unsigned int SerNum = 0;
               int res  =
                  p_tira_access_feature(
                  0x20000000, 0, &SerNum, 0);


               if ( res == 0 )
                  printf("Serial Number: %x\n", SerNum);
               else
                  printf("Serial Number Not Available\n");

               break;
               }
         default:
               Help();
      }
      if ( res != 0 && res != -1 )
         printf("Last function call failed!\n");

      printf(">\n");

   };
L_EXIT:   
#if  defined(__linux)
	tcsetattr(0, TCSADRAIN, &savetty);
#endif
   return 0;
}
//==========================================================
#define FUNC_VAR_LOAD(FF)  \
  last = (void*) p_##FF = (t_##FF) GetProcAddress(handle, #FF ); \
  if ( last == 0 ) return Error();

#define FUNC_VAR_ERASE(FF) \
   p_##FF = (t_##FF) EmptyFunc;
//==========================================================
int LoadTiraDLL() {
   handle = LoadLibrary( LibName );
   if ( handle == 0 ) return Error();

   printf("library loaded, retrieving entry points\n");
   void* last;
   FUNC_VAR_LOAD(tira_init);
	FUNC_VAR_LOAD(tira_cleanup);
	FUNC_VAR_LOAD(tira_set_handler);
	FUNC_VAR_LOAD(tira_start);
	FUNC_VAR_LOAD(tira_stop);
#ifdef USE_CAPTURE
	FUNC_VAR_LOAD(tira_start_capture);
	FUNC_VAR_LOAD(tira_cancel_capture);
	FUNC_VAR_LOAD(tira_get_captured_data);
	FUNC_VAR_LOAD(tira_set_dword);
#endif
	FUNC_VAR_LOAD(tira_transmit);
	FUNC_VAR_LOAD(tira_delete);

	FUNC_VAR_LOAD(tira_get_version);
	FUNC_VAR_LOAD(tira_access_feature);

   return 0;
};

int UnloadTiraDLL() {
   p_tira_cleanup();
   int res = FreeLibrary(handle);

   FUNC_VAR_ERASE(tira_init);
	FUNC_VAR_ERASE(tira_cleanup);
	FUNC_VAR_ERASE(tira_set_handler);
	FUNC_VAR_ERASE(tira_start);
	FUNC_VAR_ERASE(tira_stop);
	FUNC_VAR_ERASE(tira_start_capture);
	FUNC_VAR_ERASE(tira_cancel_capture);
	FUNC_VAR_ERASE(tira_get_captured_data);
	FUNC_VAR_ERASE(tira_set_dword);
	FUNC_VAR_ERASE(tira_transmit);
	FUNC_VAR_ERASE(tira_delete);
	FUNC_VAR_ERASE(tira_get_version);
	FUNC_VAR_ERASE(tira_access_feature);
   return 0;
};

int Help() {
   printf(
            "L\t Load Tira.dll\n"
            "U\t Unload Tira.dll\n"
            "1-8\t Open Tira on corresponding COM port\n"
            "\t for example, '3' opens Tira on COM3\n"
            "S\t Stops Tira\n"
            "B\t attach Callback. Application will be able to receive IR signals\n"
            "C\t activates capture mode\n"
            "A\t cancels capture mode\n"
            "T\t transmit the most recently captured IR code\n"
            "P\t print captured IR code\n"
            "E\t manual entry of the IR code\n"
            "R\t set difference threshold\n"
            "D\t dispose data allocated for IR code\n"
            "V\t Display version of the DLL and device\n"

            "H\t Explain functions\n"
            "Q\t Quit\n" );

   return 0;
};

// $Log: TiraHost.cpp,v $
// Revision 1.9  2005/10/25 19:35:45  Administrator
// added querying for version number
//
// Revision 1.8  2005/09/19 15:31:25  Administrator
// access_feature and get_version added
//
// Revision 1.7  2005/08/30 07:15:29  szilber
// Linux compatibility
//// Revision 1.6  2004/07/01 00:32:40  szilber// Copyright notice added//// Revision 1.5  2004/06/30 15:14:27  szilber// Linux support added//