
Remote controlled PCs
Gavin Smyth 

--------------------------------------------------------------------------------

This is an article which appeared in Dr Dobb's Journal in May 2000 under the slightly modified title of 'Infra-red control of your PC.'


--------------------------------------------------------------------------------

Recently I've started to notice PCs as media devices - it has been possible to play games on them practically since they first appeared, but nowadays, with ever more powerful hardware and clever compression algorithms, it is almost trivial to configure one to store and play back your favourite music tracks, and a number of manufacturers are telling us that video is coming soon. However, there are two small flies in the ointment: first, most PCs are still ugly large boxes that you probably do not want near your hi-fi kit (for a start, PCs are beige and hi-fi kit is black, as everyone knows!); second, keyboards and mice, even cordless ones, are not really suitable control devices.

I will ignore the first of those for the time being, but an expensive and very cool solution to that latter problem would be to control your PC from your whizzy PDA via IrDA. On the other hand, a much cheaper, and almost as cool, solution is to use a standard TV remote unit and a low cost (or even home made) IR receiver attached to your PC - and that is what this article is about. (Incidentally, lest there be any confusion, IrDA is not compatible with the IR transmissions used by consumer electronics.)

First, I'll briefly describe the IR hardware I am using, then present a thin C++ class to map IR signals on to key codes, with a slight detour through Win32 overlapped I/O. Having got this far, I will suggest a few ways to attach such control to applications, showing one in more detail.

IR hardware
Virtually every consumer IR remote in existence uses a very similar protocol: in fact, that is why universal or learning remotes are available - they all speak the same low level language and all that distinguishes one from another is the precise sequence of bits that are transmitted. There are a few web sites that show you how to make IR receivers to attach to your PC's serial or parallel port, such as the Universal Infra Red Receiver, but being electronically incompetent, I chose to let someone else do the hard work for me by selecting the relatively inexpensive Irman device produced by Evation.

This small device attaches to a serial port and translates each remote keypress to a sequence of six bytes sent to the serial port. It is powered from the serial port and can be placed at some convenient point near your PC. The Evation web site contains a few instructions for using the Irman from your software and a list of applications and libraries that use it. Those pieces of code tend to be distributed either without source or for unix/Linux. One exception is a Winamp plugin that does virtually what I am describing here (in fact, more than I am describing), and I fully acknowledge how useful that code was when I had problems getting the Irman to respond, so you can compare some tight C code with my C++ class, Irman, defined as shown in Listing 1.

Listing 1: Irman class definition 
class Irman
{
public:
// Initialise from registry (or take defaults)
Irman( const TCHAR* regKey,       // Registry key under which to find the values
int numKeys );             // Number of keys on the Remote

// Shutdown, and rewrite configuration to the registry
~Irman();

// Get and set the port name (COMx)
const TCHAR* Port() const { return comPortName; }
void Port( const TCHAR* comPort );

// Get and set the inter-key delay (milliseconds)
unsigned long Delay() const { return interKeyDelay; }
void Delay( unsigned long d ) { interKeyDelay = d; }

// Wait for a data packet to be received and return the index into
// the vector that represents it (or -1 if not recognised)
int Key();

// Trigger the next received key sequence to be stored for the
// indicated key code
void SetKey( int key );

// Interrupt the current read - used from a separate task
void Interrupt();

private:
// Each Irman key is decoded to a sequence of 6 bytes
struct KeyCode
{
unsigned char code[ 6 ];

bool operator==( const KeyCode& key )
{
for( int i = 0; i < sizeof( code ); ++i )
if( key.code[ i ] != code[ i ] )
return false;
return true;
}
};

TCHAR comPortName[ 5 ];  // contains COMx\0
volatile HANDLE comPort; // Handle to the opened COM port
HANDLE ioCompletion;     // Handle used for overlapped I/O

// Where to read/write values - passed to constructor
const TCHAR* regKey;

// How many keys on the remote - passed to constructor
int numKeys;

// Codes corresponding to each key to be recognised (numKeys long)
KeyCode* keyCodes;

// Open and close the COM port
bool Open();
void Close();

// Read a complete Irman sequence
bool Read( KeyCode& );

// Time (ms) last Irman sequence read
unsigned long keyTime;

// Key code to which to set next read key
volatile int setKey;

// Low level (blocking, but interruptable via Interrupt() above)
// COM port read and write
bool ReadWait( void* data, unsigned long size );
bool WriteWait( const void* data, unsigned long size );

// Waggle the control lines to power up or down the Irman
void PowerOn() const;
void PowerOff() const;

// Discard any characters in the COM port buffers
void Flush();

// Time to wait from reading one key ro the next
volatile unsigned long interKeyDelay;

// Disable copying
Irman( const Irman& );
Irman& operator=( const Irman& );  
};


The sequence of six bytes is fairly arbitrary, but there is a one to one correspondance between remote keys and the sequence received. There are occasional synchronisation errors, but these generally recover quickly (i.e., by the next key press). I have also found that, for the three different remote units I have used, that an individual key press results in three sequences sent to the PC: in two cases, the three sequences are identical, and in the other some bits are inverted in each successive sequence. This does mean that the receiver program should ignore sequences received quickly in sequence and should be able to deal with inverted codes - by deal with, I really mean ignore, since at least one of the three will be non-inverted. All that really matters is that each remote key press results in the six bytes somewhere in the total sequence that will match previously received and stored ones.

Irman driver software
Given that the Irman connects to the PC's serial port, it should come as no surprise that the driver code is composed of two parts: a COM port interface and a sequence matcher.

COM port programming is fairly straightforward on Win32 - open the device called COM1 (or some other port number) and read and write characters. You can use the Win32 functions CreateFile, ReadFile and WriteFile, or the C stream fopen, fread and fwrite, or even C++ iostreams if you want. However, there is one major advantage of the first group, as I will show shortly. After opening the COM port, various RS232 parameters must be set - baud rate, number of bits per character, etc. This is achieved using the Win32 DCB structure: the program retrieves one of these containing the current settings, changes the appropriate ones, and sends it back to the COM port, as shown in the Open method in Listing 2. Now that the COM port is set up correctly, the Irman itself is powered up and initialised: it takes power from the RTS and DTR lines, so these are set. As far as initialisation is concerned, all that is necessary is sending the two bytes "IR" to the Irman and if everything is working fine, the program receives "OK" in return. (There are a few timing requirements, as described in the Irman specification and as can be seen in the program listings.)

Listing 2: Irman class source 
Irman::Irman( const TCHAR* regKey_,  int numKeys_ ) :
regKey( regKey_ ), numKeys( numKeys_ ),
keyCodes( new KeyCode[ numKeys_ ] ),
comPort( INVALID_HANDLE_VALUE ),
keyTime( GetTickCount() ), setKey( -1 ),
ioCompletion( CreateEvent( NULL, TRUE, FALSE, NULL ) )
{
// Read the port name, inter key delay and all the key codes from
// the registry, and then fire up the device

RegistryKey reg( HKEY_LOCAL_MACHINE, regKey );
reg.Read( RegPort, comPortName, sizeof( comPortName ) / sizeof( TCHAR ), _T("COM2") );
interKeyDelay = reg.Read( RegDelay, 500 );
for( int i = 0; i < numKeys; ++i )
{
static KeyCode blankKeyCode;
TCHAR num[ 16 ];
wsprintf( num, _T("%03d"), i );
reg.Read( num, keyCodes[ i ].code, sizeof( KeyCode ), &blankKeyCode );
}

// Don't open the port yet in case it hangs...
}

Irman::~Irman()
{
// Close everything down, and write back to the registry

Close();
if( ioCompletion != INVALID_HANDLE_VALUE )
{
CloseHandle( ioCompletion );
ioCompletion = INVALID_HANDLE_VALUE;
}

RegistryKey reg( HKEY_LOCAL_MACHINE, regKey );
reg.Write( RegPort, comPortName );
reg.Write( RegDelay, interKeyDelay );
for( int i = 0; i < numKeys; ++i )
{
TCHAR num[ 16 ];
wsprintf( num, _T("%03d"), i );
reg.Write( num, keyCodes[ i ].code, sizeof( KeyCode ) );
}

delete [] keyCodes;
}

bool Irman::Open()
{
const TCHAR* error = _T("Error opening device");
comPort = CreateFile( comPortName, GENERIC_READ | GENERIC_WRITE,
0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL );
if( comPort == INVALID_HANDLE_VALUE )
MessageBox( NULL, _T("Could not open COM port - maybe something else is using it"), error, MB_ICONEXCLAMATION | MB_OK );
else
{
DCB dcb;
if( Verify( GetCommState( comPort, &dcb ) ) )
{
dcb.BaudRate    = CBR_9600;
dcb.fParity     = 0;
dcb.Parity      = NOPARITY;
dcb.ByteSize    = 8;
dcb.StopBits    = ONESTOPBIT;
dcb.fDtrControl = DTR_CONTROL_DISABLE;
dcb.fRtsControl = RTS_CONTROL_DISABLE;
if( Verify( SetCommState( comPort, &dcb ) ) )
{
PowerOff();           // Just in case it was already on
Sleep( 200 );
PowerOn();
Sleep( 100 );         // Time for the output to settle
Flush();              // Remove power up garbage
WriteWait( "I", 1 );  // These strings must be ASCII, not Unicode
Sleep( 2 );           // Need to have >500us between the 'I' & the 'R'
WriteWait( "R", 1 );

 char data[ 2 ];
		 if( ReadWait( data, 2 ) && data[ 0 ] == 'O' && data[ 1 ] == 'K' )
					   return true;
							   else
											 MessageBox( NULL, _T("Irman not responding"), error, MB_ICONEXCLAMATION | MB_OK );
												   }
													   }
														 }

														   // To get this far, something must have gone wrong
															 Close();
															   return false;
}

void Irman::Close()
{
if( comPort != INVALID_HANDLE_VALUE )
{
Verify( CloseHandle( comPort ) );
comPort = INVALID_HANDLE_VALUE;
}
}

void Irman::Port( const TCHAR* comPort )
{
_tcsnccpy( comPortName, comPort, sizeof( comPortName ) / sizeof( TCHAR ) );
comPortName[ sizeof( comPortName ) / sizeof( TCHAR ) - 1 ] = 0;

// Reopen the port if the name changed - I could have checked the new
// and old names and if they were the same, skip the reopen. However,
// this way, I can force a recover from a "stuck" I/O port...

Close();
}

void Irman::SetKey( int key )
{
if( comPort == INVALID_HANDLE_VALUE )
{
MessageBox( NULL, _T("COM port not valid - can't configure"), _T("IR Configuration Error"), MB_ICONEXCLAMATION | MB_OK );
return;
}

if( key < 0 || key > numKeys )
{
return;
}

// Just indicate to the reading function that it should store the next
// sequence instead of matching it

setKey = key;
}

int Irman::Key()
{
if( comPort == INVALID_HANDLE_VALUE && !Open() )
return -1;

// The Irman reports a number of sequences for each key - chuck away
// old ones before reading the next key.
Flush();

KeyCode key;

unsigned long startTime = keyTime;
do
{
if( !Read( key ) )
return -1;
} while( keyTime - startTime < interKeyDelay );

		   // Now, key contains a valid code sequence, so do something with it

			 if( setKey != -1 )
				   {
						   // If we're in record mode, just use this sequence for the relevant
							   // entry in keyCodes[]
								   keyCodes[ setKey ] = key;
									   int retVal = setKey;
										   setKey = -1;
											   return retVal;
												 }
												   else
														 {
																 // If we're not in record mode, scan the list to find a match,
																	 // and repeat for up to the inter key period before giving up and
																		 // admitting it's unrecognised - the reason for the loop is to
																			 // catch any inversions along the way
																				 startTime = keyTime;
																					 do
																							 {
																									   for( int i = 0; i < numKeys; ++i )
																												   if( key == keyCodes[ i ] )
																																 return i;
																																	   if( !Read( key ) )
																																				   return -1;
																																					   } while( GetTickCount() - startTime < interKeyDelay );
																																						   return -1; // No key found
																																							 }
}

bool Irman::Read( KeyCode& key )
{
bool success = ReadWait( key.code, sizeof( key.code ) );
keyTime = GetTickCount();
return success;
}

void Irman::Interrupt()
{
SetEvent( ioCompletion );
}

bool Irman::ReadWait( void* data, unsigned long size )
{
if( comPort == INVALID_HANDLE_VALUE )
return false;

OVERLAPPED ov;
ZeroMemory( &ov, sizeof( ov ) );
ov.hEvent = ioCompletion;

// If ReadFile doesn't succeed, but the error is ERROR_IO_PENDING, we
// just wait for the operation to complete.

DWORD length;
if( !ReadFile( comPort, data, size, &length, &ov ) )
{
if( GetLastError() == ERROR_IO_PENDING )
	   {
				 if( Verify( GetOverlappedResult( comPort, &ov, &length, TRUE ) ) == FALSE )
							 return false;
								 }
									 else
											 {
													   Verify( FALSE ); // Read failed
															 return false;
																 }
																   }

																	 // At this point, the buffer contains the expected data
																	   return size == length;
}

bool Irman::WriteWait( const void* data, unsigned long size )
{
if( comPort == INVALID_HANDLE_VALUE )
return false;

OVERLAPPED ov;
ZeroMemory( &ov, sizeof( ov ) );
ov.hEvent = ioCompletion;

DWORD length;
if( !WriteFile( comPort, data, size, &length, &ov ) )
{
if( GetLastError() == ERROR_IO_PENDING )
   {
			 if( Verify( GetOverlappedResult( comPort, &ov, &length, TRUE ) ) == FALSE )
						 return false;
							 }
								 else
										 {
												   Verify( FALSE ); // Write failed
														 return false;
															 }
															   }

																 return size == length;
}

// Power the Irman device by twiddling the control lines

void Irman::PowerOff() const
{
Verify( EscapeCommFunction( comPort, CLRDTR ) );
Verify( EscapeCommFunction( comPort, CLRRTS ) );
}

void Irman::PowerOn() const
{
Verify( EscapeCommFunction( comPort, SETDTR ) );
Verify( EscapeCommFunction( comPort, SETRTS ) );
}

// Throw away anything in the COM port buffers, and set the ignore time
// to start from now.

void Irman::Flush()
{
Verify( PurgeComm( comPort, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ) );
keyTime = GetTickCount();
}


Apart from issuing the "IR" sequence, all of the serial interactions in this program are input operations, and all of these bar the "OK" at the start are six byte sequences. However, what if something goes wrong? What if the Irman is removed in mid sequence or, more likely, the user wants to exit the program while it is waiting for some characters from the serial port? I need to be able to interrupt a read, and that is the root of the only significant problem with COM port I/O. By default, the COM port is opened in blocking mode, and there is no timeout, so the program will wait indefinitely for the character sequences. There are a number of ways round this: you could, for example, open in non-blocking mode or have short timeouts and poll the device, but that is fairly wasteful of resources. All other solutions involve separate threads to read the COM port and to service user interactions. In the unix world, the select function or similar would let you handle events from different sources asynchronously; or you could set up the COM port reader thread to be signalled (i.e., interrupt the read) when some user event occurred. In the Win32 world, a more attractive option is asynchronous I/O.

In normal (synchronous) I/O, the program prepares buffers to transmit or receive, calls the appropriate I/O function, and by the time the function returns, the buffers have been dealt with - completely transmitted or completely filled (or some error has occurred). With asynchronous I/O on the other hand, the program merely initiates the transfer and then continues while Windows, in the background, completes the operation and notifies the application some time in the future when the operation has completed. In this case, the application is not permitted to reuse the buffer space until Windows has told it that the I/O transaction has completed: Windows is handling the asynchronous nature of the I/O transfer at the cost of more complex buffer management. Windows NT (and 2000) support asynchronous I/O on virtually every flavour of I/O stream, while Windows 9x restricts it to not much more than COM port I/O - which is fairly convenient here!

This is what you need to do to manage asynchronous I/O. First, open the device with the extra flag FILE_FLAG_OVERLAPPED. Note that when a file is opened for asynchronous I/O, all I/O must be asynchronous - you cannot mix synchronous and asynchronous I/O.

After this, a read is initiated with a ReadFile, as normal, but with an extra parameter, a pointer to an OVERLAPPED structure. This contains a number of fields relating to the I/O operation, but the only important one here is hEvent, the handle of a Win32 manual reset event created specifically for indicating completion of the asynchronous I/O operation.

When the ReadFile returns, there are two possibilities (ignoring errors): the read may have completed already (for example, there might already have been data in the device's buffers), or it might not! In the first case, just continue as before; in the second, the program must wait for the operation to finish, either by polling or suspending on the completion event flag: both these options cane be exercised via the GetOverlappedResult function - its final argument is a flag which determines whether the function waits until the event is signalled. This implies that to interrupt an I/O operation, this event flag could be set from elsewhere - it is just a standard Win32 event - to wake up the waiting thread. The Microsoft documentation includes a CancelIo function which it is suggested should be invoked to cut short an asynchronous operation: however, that does not exist on Windows 95 so I have no option but to signal the event and keep my fingers crossed...

This has dealt with low level I/O operations - I now have an interruptible mechanism for retrieving byte sequences from the Irman. The next level handles error conditions. First, as I mentioned earlier, the Irman returns a number of byte sequences for each key press, at least with the remotes I have used. I make the assumption that the user is not going to hit keys more rapidly than a couple a second (though this parameter is settable in the registry - see later), so after receiving a sequence, I ignore all others for the next half second. (I have also found that hitting Winamp buttons too rapidly can sometimes result in it crashing, so a bit of throttling is useful in preventing that.) An additional mechanism for getting rid of spurious data is to flush the COM device's buffers (via PurgeComm) when I suspect there might be garbage.

The top level of the driver is to match Irman byte sequences with index numbers: this is a trivial search through a preloaded array of sequences. (If there were a lot of key codes, a hash table would be a better choice for data structure, but a simple vector will do here.) The array, along with any other operational parameters, is stored in the registry via the trivial C++ class, Registry. There is not space in this article to discuss this class, but well commented source files are available electronically.

There is one final layer to place on this before it is suitable for controlling applications on the PC: this is just a set of inline blocking functions - to be able to interrupt a I/O operation, I still need an extra thread to be triggered by user operations. I did contemplate wrapping this in a C++ class, but then decided that the options were too varied - direct invocation of functions (callbacks, in other words), sending Windows messages, COM invocations or socket messages, to name just a few) to make such a class tidy and efficient. Instead, I will just present one simple mechanism for controlling another application: a big switch statement! A reader thread (see Listing 3) loops reading key indexes from the Irman class and either invokes the appropriate operation via a switch statement or programs the Irman's byte sequence to key mapping. This thread terminates when the user interface thread sets the going flag to false: the foreground thread also invokes the Irman's Interrupt method to cancel any outstanding I/O operation, to avoid the reader thread blocking indefinitely.

Listing 3: Main code and asynchronous reader function 
#define UID 8877                   // Fairly arbitrary id for the tray icon
#define NOT_MSG ( WM_USER + 33 )   // Tray message id (arbitrary value)

static HMENU menu;                 // Popup menu

// There are two icons in the system tray, and iconNumber indicates
// which to display next

static HICON icon[ 2 ];
static int iconNumber;

// Names of the remote keys recognised - should be in the same order as 
// the configure dialog box buttons, and the same order as the Irman key
// codes.

static const TCHAR* keyStrings[] =
{
_T("Next Track"),
_T("Previous Track"),
_T("Increase Volume"),
_T("Decrease Volume"),
_T("Shutdown PC")
};
static const int numKeys = sizeof( keyStrings ) / sizeof( keyStrings[ 0 ] );

// Singleton Winamp controller and Irman interface

WinampManager mgr;
Irman ir( _T("Software\\BeesKnees\\Irman"), numKeys );

volatile HWND mainWnd;       // Handle to the (invisible) application window
volatile HWND configureWnd;  // Handle to the configuration dialog box: if
// this is non-null, the dialog is visible
volatile bool recording;     // True if we're waiting for a key to be recorded
volatile bool testing;       // True if we're testing instead of acting on keys

volatile bool going = true;  // The reader loop runs while this is set

static void Reader( void* )
{
while( going )
{
// Rather sloppily, this does not bother to guard against changes in the
// COM port (eg, a different one selected by the code below), but it seems
// to work OK...
int key = ir.Key();

// Toggle the tray icon
NOTIFYICONDATA not;
not.cbSize = sizeof( not );
not.hWnd = mainWnd;
not.uID = UID;
not.uFlags = NIF_ICON;
not.hIcon = icon[ iconNumber = !iconNumber ];
Shell_NotifyIcon( NIM_MODIFY, &not );

if( configureWnd )    // ie, if we're running the configure dialog
{
if( recording )     // ie, we need to respond to a completed record
{
SetDlgItemText( configureWnd, IDC_STATUS, key != -1 ? _T("OK") : _T("Failed") );
recording = false;
}
else if( testing )  // ie, echo, don't act
{
		 TCHAR buff[ 64 ];
				 wsprintf( buff, _T("Read %d (%s)"), key, key != -1 ? keyStrings[ key ] : _T("Undefined") );
						 SetDlgItemText( configureWnd, IDC_STATUS, buff );
							   }
								   }
									   else
											   {
														 // Just perform the appropriate action
															   switch( key )
																		 {
																					 case 0:
																							   mgr.Skip( true );
																										 break;
																												 case 1:
																														   mgr.Skip( false );
																																	 break;
																																			 case 2:
																																					   mgr.Volume( true );
																																								 break;
																																										 case 3:
																																												   mgr.Volume( false );
																																															 break;
																																																	 case 4:
																																																			   mgr.Exit();
																																																						 break;
																																																							   }
																																																								   }
																																																									 } // end while going
}

BOOL CALLBACK ConfigProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM /*lParam*/ )
{
switch( msg )
{
case WM_INITDIALOG:
{
configureWnd = hwnd;    // Setting this indicates that configuration is active
const TCHAR* port = ir.Port();
if( _tcslen( port ) == 4 && port[ 3 ] >= _T('1') && port[ 3 ] <= _T('4') )
CheckRadioButton( hwnd, IDC_COM1, IDC_COM4, IDC_COM1 + port[ 3 ] - _T('1') );
SetDlgItemInt( hwnd, IDC_DELAY, ir.Delay(), FALSE );
}
return TRUE;

case WM_COMMAND:
switch( LOWORD( wParam ) )
{
case IDOK:
{
	 BOOL success;
				 UINT d = GetDlgItemInt( hwnd, IDC_DELAY, &success, FALSE );
							 if( success )
											   ir.Delay( d );
														   EndDialog( hwnd, 0 );
																	   configureWnd = NULL;
																				 }
																						   return TRUE;

																								   case IDC_COM1:
																										   case IDC_COM2:
																												   case IDC_COM3:
																														   case IDC_COM4:
																																	 {
																																					 // I could check that the radio button hasn't already been
																																								 // selected, but not doing this has the side-effect of
																																											 // reinitialising the port if necessary...
																																														 TCHAR buff[ 8 ];
																																																	 wsprintf( buff, _T("COM%d"), LOWORD( wParam ) - IDC_COM1 + 1 );
																																																				 ir.Port( buff );
																																																						   }
																																																									 return TRUE;

																																																											 case IDC_NEXT_TRACK:
																																																													 case IDC_PREV_TRACK:
																																																															 case IDC_VOL_UP:
																																																																	 case IDC_VOL_DOWN:
																																																																			 case IDC_SHUTDOWN:
																																																																					   {
																																																																									   int code = LOWORD( wParam ) - IDC_NEXT_TRACK;
																																																																												   TCHAR buff[ 64 ];
																																																																															   wsprintf( buff, _T("Press remote key for %s"), keyStrings[ code ] );
																																																																																		   SetDlgItemText( hwnd, IDC_STATUS, buff );
																																																																																					   recording = true;         // Initiate a key record
																																																																																								   ir.SetKey( code );
																																																																																											 }
																																																																																													   return TRUE;

																																																																																															   case IDC_CHECK_TEST:
																																																																																																		 testing = IsDlgButtonChecked( hwnd, IDC_CHECK_TEST ) == BST_CHECKED;
																																																																																																				   return TRUE;
																																																																																																						 }
																																																																																																							   break;
																																																																																																								 }
																																																																																																								   return FALSE;
}

/*****************************************************************************

Main (invisible) window procedure. Just has to respond to create (spawn off
sub process), destroy and tray commands (toggle visibility).

This is only defined if we're not building a Winamp plugin.

*****************************************************************************/

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{

switch( msg )
{
case WM_CREATE:
_beginthread( Reader, 0, NULL );
break;

case WM_DESTROY:
going = false;  // This will kill off the reader
ir.Interrupt();
PostQuitMessage( 0 );
return 0;

case WM_COMMAND:
switch( LOWORD( wParam ) )
{
case ID_CONFIGURE:
DialogBox( reinterpret_cast< HINSTANCE >( GetWindowLong( hwnd, GWL_HINSTANCE ) ),
MAKEINTRESOURCE( IDD_CONFIGURE ), hwnd, ConfigProc );
break;

case ID_QUIT:
SendMessage( hwnd, WM_CLOSE, 0, 0 );
	 return 0;
		   }
				 break;

					 case NOT_MSG:
						   // The only message I expect from the system tray is a mouse click,
								 // in which case I pop up a menu at that cursor position
									   switch( lParam )
												 {
															 case WM_LBUTTONDOWN:
																	 case WM_RBUTTONDOWN:
																			   {
																							   POINT p;
																										   GetCursorPos( &p );
																													   SetForegroundWindow( hwnd );
																																   TrackPopupMenu( menu, TPM_RIGHTALIGN | TPM_RIGHTBUTTON, p.x, p.y, 0, hwnd, NULL) ;
																																			 }
																																					   break;
																																							 }
																																								   return 0;
																																									 }
																																									   return DefWindowProc( hwnd, msg, wParam, lParam );
}

int WINAPI _tWinMain( HINSTANCE hInstance, HINSTANCE /*prev*/, LPSTR /*cmdline*/, int /*cmdShow*/ )
{
icon[ 0 ] = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_ICON ) );
icon[ 1 ] = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_ICON_INV ) );

WNDCLASS wc;
wc.style         = 0;
wc.lpfnWndProc   = WndProc;
wc.cbClsExtra    = 0;
wc.cbWndExtra    = 0;
wc.hInstance     = hInstance;
wc.hIcon         = icon[ 0 ];
wc.hCursor       = LoadCursor( NULL, IDC_ARROW );
wc.hbrBackground = (HBRUSH)( COLOR_WINDOW + 1 );
wc.lpszMenuName  = NULL;
wc.lpszClassName = _T("WinampIR");

if( !RegisterClass( &wc ) )
								 return 0;

								   mainWnd = CreateWindow( wc.lpszClassName, wc.lpszClassName,
															 WS_POPUP,
																					   CW_USEDEFAULT, 0,
																												 CW_USEDEFAULT, 0,
																																		   NULL, NULL,
																																									 hInstance, NULL );
									 if( !mainWnd )
											 return 0;

											   menu = LoadMenu( hInstance, MAKEINTRESOURCE( IDR_MENU ) );
												 menu = GetSubMenu( menu, 0 );

												   // Populate the system tray
													 NOTIFYICONDATA not;
													   not.cbSize = sizeof( not );
														 not.hWnd = mainWnd;
														   not.uID = UID;
															 not.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
															   not.uCallbackMessage = NOT_MSG;
																 not.hIcon = wc.hIcon;
																   _tcsncpy( not.szTip, _T("Winamp IR controller"), 20 );
																	 Shell_NotifyIcon( NIM_ADD, &not );

																	   MSG msg;
																		 while( GetMessage( &msg, NULL, 0, 0 ) )
																			   {
																					   TranslateMessage( &msg );
																						   DispatchMessage( &msg );
																							 }

																							   // Release all Windows resources grabbed at startup
																								 Shell_NotifyIcon( NIM_DELETE, &not );
																								   DestroyIcon( icon[ 0 ] );
																									 DestroyIcon( icon[ 1 ] );

																									   return msg.wParam;
}


Winamp control
As a concrete example of application control, I chose Winamp: this was mainly because the application is so commonly used, but also because it presents a very nice plugin interface which would allow my code to mesh into it.

Winamp is very easy to control via a well defined set of Windows messages, described in the header file in the SDK available on its web site. All you need to do is locate the Winamp window and send the appropriate message. There are two ways to couple extra code to Winamp: keep the code in a totally separate executable and use something like the Win32 function FindWindow to locate the Winamp window; or make it a DLL with a few specific entry points and enable it to be loaded automatically by Winamp. Although the second technique has the advantages that the Winamp window handle is available immediately so there is no need to search for it, that the code is smaller since it is merely a DLL to be loaded, and that the code is more tightly integrated with Winamp, I chose the former technique because I wanted to use the same approach with other programs that are not quite as plugin friendly (and, I will be honest, it is easier to debug code in a separate application and my code is less likely to break Winamp this way!).

I wrapped the small number of Winamp message sends in a simple WinampManager class, defined in Listings 4 and 5, which maps member functions on to the specific message sends. There is one extra member, Exit, which closes Winamp and shuts down Windows - I take care to close Winamp first and wait for a bit since I have found that shutting down Windows while Winamp is active quite often does not succeed, possibly because of some of the other Winamp plugins I tend to have loaded.

Listing 4: WinampManager class definition 
class WinampManager
{
public:
WinampManager();
  ~WinampManager();

	// Step forward or back a small number of seconds
	  bool Step( bool forward = true );

		// Skip forward or backward a track
		  bool Skip( bool forward = true );

			// Bump the volumen up or down
			  bool Volume( bool up );

				// Exit Winamp and shutdown Windows
				  bool Exit();

				  private:
					// Send an arbitrary message to Winamp
					  bool SendWinamp( UINT message, WPARAM wparam, LPARAM lparam );

						// Slightly specialised for to send a "button press" message
						  bool SendButton( WPARAM button );

							// Locate the Winamp window, to which to send the messages
							  void FindWinamp();

								// Cached Winamp window handle
								  HWND winampWindow;

									// Disallow copying
									  WinampManager( const WinampManager& );
										WinampManager& operator=( const WinampManager& );
};


Listing 5: WinampManagerclass source 
WinampManager::WinampManager() : winampWindow( NULL )
{
}

WinampManager::~WinampManager()
{
}

bool WinampManager::Exit()
{
 // I've found that occasionally shutting down Windows with Winamp still
   // active is bad news - it's better to exit Winamp, and then kill Windows
	 SendWinamp( WM_CLOSE, 0, 0 );

	   // It's of considerable assistance to debugging NOT to exit windows!
# ifndef _DEBUG
		   Sleep( 1000 );                    // Give Winamp time to die
			   ExitWindowsEx( EWX_POWEROFF | EWX_SHUTDOWN, 0 );
# endif

				 return true;
}

// The action functions map fairly closely to the messages described in
// winamp.h, supplied by the Winamp folks

bool WinampManager::Volume( bool up )
{
 SendButton( up ? WINAMP_VOLUMEUP : WINAMP_VOLUMEDOWN );
   return true;
}

bool WinampManager::Skip( bool forward )
{
 SendButton( forward ? WINAMP_BUTTON5 : WINAMP_BUTTON1 );
   return true;
}

bool WinampManager::Step( bool forward )
{
 SendButton( forward ? WINAMP_FFWD5S : WINAMP_REW5S );
   return true;
}

void WinampManager::FindWinamp()
{
 winampWindow = FindWindow( _T("Winamp v1.x"), NULL );
}

bool WinampManager::SendButton( WPARAM button )
{
 return SendWinamp( WM_COMMAND, button, 0 );
}

bool WinampManager::SendWinamp( UINT message, WPARAM wparam, LPARAM lparam )
{
 // Although I have a Winamp handle member, and had intended to cache it,
   // I don't! If Winamp is closed and reopened, the handle would be invalid,
	 // and I have no easy way to tell that - at least, no cheaper than just
	   // searching for the window in the first place. Sooo... skip caching!
		 FindWinamp();
		   if( winampWindow )
				 {
						 SendMessage( winampWindow, message, wparam, lparam );
							 return true;
							   }
								 else
										 return false;
}



Tying it all together
If you examine the source code in Listing 3, you will see a few extra twiddles.

Although the code is C++ and does involve some string manipulation, I deliberately steered clear of the STL - some STL implementations, notably that supplied with the Microsoft compiler, do not have thread safe string classes, and working around that was more effort than it was worth for such a short piece of code. (There are some patches to the STL which could make it thread safe, at quite a computational cost.)

The application does not have a main window itself, but just occupies a space in the system tray. (If I had made this a Winamp plugin, I could even have avoided the extravagance of this screen space.) A click on the icon pops up a menu which offers the configuration dialog box or quit.

When each key sequence is processed, the tray icon toggles between normal and inverted, just to confirm that something is happening since I prefer not to trust hardware unless I can see an immediate result.

The configuration dialog (below) lets you test the Irman key definitions by just printing the name of the key pressed instead of carrying out the intended action - very useful for debugging.



Finally, the code has been written to be unicode compatible - all strings appear inside the _T macro, and all string related functions use the macro names in tchar.h. Defining UNICODE when compiling will result in all these mapping to unicode variants which should be more efficient on NT and omitting the definition results in ASCII code, more efficient on Win9x.

Conclusion
The starting point of this exercise for me was the desire to make a PC easier to use as an audio playing device. I found that there were a few cost effective ways to use remote control, one of the easiest being the Irman device. However, making use of it in a program was not completely trivial, but the C++ class and associated code I have shown here makes it fairly straightforward to use the device to drive another application. I now have a PC (still big, beige and ugly, but never mind) which to all intents an purposes replaces my CD player (in fact, I am using its remote!) with the added benefit that I do not have to get up to change the disc every hour or so.


--------------------------------------------------------------------------------

Gavin's home page | BeesKnees home page
Last modified on 30th December 2000 
Remote controlled PCs
Gavin Smyth 

--------------------------------------------------------------------------------

This is an article which appeared in Dr Dobb's Journal in May 2000 under the slightly modified title of 'Infra-red control of your PC.'


--------------------------------------------------------------------------------

Recently I've started to notice PCs as media devices - it has been possible to play games on them practically since they first appeared, but nowadays, with ever more powerful hardware and clever compression algorithms, it is almost trivial to configure one to store and play back your favourite music tracks, and a number of manufacturers are telling us that video is coming soon. However, there are two small flies in the ointment: first, most PCs are still ugly large boxes that you probably do not want near your hi-fi kit (for a start, PCs are beige and hi-fi kit is black, as everyone knows!); second, keyboards and mice, even cordless ones, are not really suitable control devices.

I will ignore the first of those for the time being, but an expensive and very cool solution to that latter problem would be to control your PC from your whizzy PDA via IrDA. On the other hand, a much cheaper, and almost as cool, solution is to use a standard TV remote unit and a low cost (or even home made) IR receiver attached to your PC - and that is what this article is about. (Incidentally, lest there be any confusion, IrDA is not compatible with the IR transmissions used by consumer electronics.)

First, I'll briefly describe the IR hardware I am using, then present a thin C++ class to map IR signals on to key codes, with a slight detour through Win32 overlapped I/O. Having got this far, I will suggest a few ways to attach such control to applications, showing one in more detail.

IR hardware
Virtually every consumer IR remote in existence uses a very similar protocol: in fact, that is why universal or learning remotes are available - they all speak the same low level language and all that distinguishes one from another is the precise sequence of bits that are transmitted. There are a few web sites that show you how to make IR receivers to attach to your PC's serial or parallel port, such as the Universal Infra Red Receiver, but being electronically incompetent, I chose to let someone else do the hard work for me by selecting the relatively inexpensive Irman device produced by Evation.

This small device attaches to a serial port and translates each remote keypress to a sequence of six bytes sent to the serial port. It is powered from the serial port and can be placed at some convenient point near your PC. The Evation web site contains a few instructions for using the Irman from your software and a list of applications and libraries that use it. Those pieces of code tend to be distributed either without source or for unix/Linux. One exception is a Winamp plugin that does virtually what I am describing here (in fact, more than I am describing), and I fully acknowledge how useful that code was when I had problems getting the Irman to respond, so you can compare some tight C code with my C++ class, Irman, defined as shown in Listing 1.

Listing 1: Irman class definition 
class Irman
{
  public:
	// Initialise from registry (or take defaults)
	  Irman( const TCHAR* regKey,       // Registry key under which to find the values
			   int numKeys );             // Number of keys on the Remote

				 // Shutdown, and rewrite configuration to the registry
				   ~Irman();

					 // Get and set the port name (COMx)
					   const TCHAR* Port() const { return comPortName; }
						 void Port( const TCHAR* comPort );

						   // Get and set the inter-key delay (milliseconds)
							 unsigned long Delay() const { return interKeyDelay; }
							   void Delay( unsigned long d ) { interKeyDelay = d; }

								 // Wait for a data packet to be received and return the index into
								   // the vector that represents it (or -1 if not recognised)
									 int Key();

									   // Trigger the next received key sequence to be stored for the
										 // indicated key code
										   void SetKey( int key );

											 // Interrupt the current read - used from a separate task
											   void Interrupt();

											   private:
												 // Each Irman key is decoded to a sequence of 6 bytes
												   struct KeyCode
													 {
															 unsigned char code[ 6 ];

																 bool operator==( const KeyCode& key )
																	 {
																			   for( int i = 0; i < sizeof( code ); ++i )
																						   if( key.code[ i ] != code[ i ] )
return false;
return true;
}
};

TCHAR comPortName[ 5 ];  // contains COMx\0
volatile HANDLE comPort; // Handle to the opened COM port
HANDLE ioCompletion;     // Handle used for overlapped I/O

// Where to read/write values - passed to constructor
const TCHAR* regKey;

// How many keys on the remote - passed to constructor
int numKeys;

// Codes corresponding to each key to be recognised (numKeys long)
KeyCode* keyCodes;

// Open and close the COM port
bool Open();
void Close();

// Read a complete Irman sequence
bool Read( KeyCode& );

// Time (ms) last Irman sequence read
unsigned long keyTime;

// Key code to which to set next read key
volatile int setKey;

// Low level (blocking, but interruptable via Interrupt() above)
// COM port read and write
bool ReadWait( void* data, unsigned long size );
bool WriteWait( const void* data, unsigned long size );

// Waggle the control lines to power up or down the Irman
void PowerOn() const;
void PowerOff() const;

// Discard any characters in the COM port buffers
void Flush();

// Time to wait from reading one key ro the next
volatile unsigned long interKeyDelay;

// Disable copying
Irman( const Irman& );
Irman& operator=( const Irman& );  
};


The sequence of six bytes is fairly arbitrary, but there is a one to one correspondance between remote keys and the sequence received. There are occasional synchronisation errors, but these generally recover quickly (i.e., by the next key press). I have also found that, for the three different remote units I have used, that an individual key press results in three sequences sent to the PC: in two cases, the three sequences are identical, and in the other some bits are inverted in each successive sequence. This does mean that the receiver program should ignore sequences received quickly in sequence and should be able to deal with inverted codes - by deal with, I really mean ignore, since at least one of the three will be non-inverted. All that really matters is that each remote key press results in the six bytes somewhere in the total sequence that will match previously received and stored ones.

Irman driver software
Given that the Irman connects to the PC's serial port, it should come as no surprise that the driver code is composed of two parts: a COM port interface and a sequence matcher.

COM port programming is fairly straightforward on Win32 - open the device called COM1 (or some other port number) and read and write characters. You can use the Win32 functions CreateFile, ReadFile and WriteFile, or the C stream fopen, fread and fwrite, or even C++ iostreams if you want. However, there is one major advantage of the first group, as I will show shortly. After opening the COM port, various RS232 parameters must be set - baud rate, number of bits per character, etc. This is achieved using the Win32 DCB structure: the program retrieves one of these containing the current settings, changes the appropriate ones, and sends it back to the COM port, as shown in the Open method in Listing 2. Now that the COM port is set up correctly, the Irman itself is powered up and initialised: it takes power from the RTS and DTR lines, so these are set. As far as initialisation is concerned, all that is necessary is sending the two bytes "IR" to the Irman and if everything is working fine, the program receives "OK" in return. (There are a few timing requirements, as described in the Irman specification and as can be seen in the program listings.)

Listing 2: Irman class source 
Irman::Irman( const TCHAR* regKey_,  int numKeys_ ) :
regKey( regKey_ ), numKeys( numKeys_ ),
keyCodes( new KeyCode[ numKeys_ ] ),
comPort( INVALID_HANDLE_VALUE ),
keyTime( GetTickCount() ), setKey( -1 ),
ioCompletion( CreateEvent( NULL, TRUE, FALSE, NULL ) )
{
// Read the port name, inter key delay and all the key codes from
// the registry, and then fire up the device

RegistryKey reg( HKEY_LOCAL_MACHINE, regKey );
reg.Read( RegPort, comPortName, sizeof( comPortName ) / sizeof( TCHAR ), _T("COM2") );
interKeyDelay = reg.Read( RegDelay, 500 );
for( int i = 0; i < numKeys; ++i )
{
static KeyCode blankKeyCode;
TCHAR num[ 16 ];
wsprintf( num, _T("%03d"), i );
reg.Read( num, keyCodes[ i ].code, sizeof( KeyCode ), &blankKeyCode );
}

// Don't open the port yet in case it hangs...
}

Irman::~Irman()
{
// Close everything down, and write back to the registry

Close();
if( ioCompletion != INVALID_HANDLE_VALUE )
{
CloseHandle( ioCompletion );
ioCompletion = INVALID_HANDLE_VALUE;
}

RegistryKey reg( HKEY_LOCAL_MACHINE, regKey );
reg.Write( RegPort, comPortName );
reg.Write( RegDelay, interKeyDelay );
for( int i = 0; i < numKeys; ++i )
{
TCHAR num[ 16 ];
wsprintf( num, _T("%03d"), i );
reg.Write( num, keyCodes[ i ].code, sizeof( KeyCode ) );
}

delete [] keyCodes;
}

bool Irman::Open()
{
const TCHAR* error = _T("Error opening device");
comPort = CreateFile( comPortName, GENERIC_READ | GENERIC_WRITE,
0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL );
if( comPort == INVALID_HANDLE_VALUE )
MessageBox( NULL, _T("Could not open COM port - maybe something else is using it"), error, MB_ICONEXCLAMATION | MB_OK );
else
{
DCB dcb;
if( Verify( GetCommState( comPort, &dcb ) ) )
{
dcb.BaudRate    = CBR_9600;
dcb.fParity     = 0;
dcb.Parity      = NOPARITY;
dcb.ByteSize    = 8;
dcb.StopBits    = ONESTOPBIT;
dcb.fDtrControl = DTR_CONTROL_DISABLE;
dcb.fRtsControl = RTS_CONTROL_DISABLE;
if( Verify( SetCommState( comPort, &dcb ) ) )
{
PowerOff();           // Just in case it was already on
Sleep( 200 );
PowerOn();
Sleep( 100 );         // Time for the output to settle
Flush();              // Remove power up garbage
WriteWait( "I", 1 );  // These strings must be ASCII, not Unicode
Sleep( 2 );           // Need to have >500us between the 'I' & the 'R'
WriteWait( "R", 1 );

char data[ 2 ];
if( ReadWait( data, 2 ) && data[ 0 ] == 'O' && data[ 1 ] == 'K' )
return true;
else
MessageBox( NULL, _T("Irman not responding"), error, MB_ICONEXCLAMATION | MB_OK );
}
 }
   }

	 // To get this far, something must have gone wrong
	   Close();
		 return false;
}

void Irman::Close()
{
if( comPort != INVALID_HANDLE_VALUE )
{
Verify( CloseHandle( comPort ) );
comPort = INVALID_HANDLE_VALUE;
}
}

void Irman::Port( const TCHAR* comPort )
{
_tcsnccpy( comPortName, comPort, sizeof( comPortName ) / sizeof( TCHAR ) );
comPortName[ sizeof( comPortName ) / sizeof( TCHAR ) - 1 ] = 0;

// Reopen the port if the name changed - I could have checked the new
// and old names and if they were the same, skip the reopen. However,
// this way, I can force a recover from a "stuck" I/O port...

Close();
}

void Irman::SetKey( int key )
{
if( comPort == INVALID_HANDLE_VALUE )
{
MessageBox( NULL, _T("COM port not valid - can't configure"), _T("IR Configuration Error"), MB_ICONEXCLAMATION | MB_OK );
return;
}

if( key < 0 || key > numKeys )
{
return;
}

// Just indicate to the reading function that it should store the next
// sequence instead of matching it

setKey = key;
}

int Irman::Key()
{
if( comPort == INVALID_HANDLE_VALUE && !Open() )
return -1;

// The Irman reports a number of sequences for each key - chuck away
// old ones before reading the next key.
Flush();

KeyCode key;

unsigned long startTime = keyTime;
do
{
if( !Read( key ) )
return -1;
} while( keyTime - startTime < interKeyDelay );

// Now, key contains a valid code sequence, so do something with it

if( setKey != -1 )
{
// If we're in record mode, just use this sequence for the relevant
// entry in keyCodes[]
keyCodes[ setKey ] = key;
int retVal = setKey;
setKey = -1;
																				 return retVal;
																				   }
																					 else
																						   {
																								   // If we're not in record mode, scan the list to find a match,
																									   // and repeat for up to the inter key period before giving up and
																										   // admitting it's unrecognised - the reason for the loop is to
																											   // catch any inversions along the way
																												   startTime = keyTime;
																													   do
																															   {
																																		 for( int i = 0; i < numKeys; ++i )
																																					 if( key == keyCodes[ i ] )
																																								   return i;
																																										 if( !Read( key ) )
																																													 return -1;
																																														 } while( GetTickCount() - startTime < interKeyDelay );
																																															 return -1; // No key found
																																															   }
}

bool Irman::Read( KeyCode& key )
{
bool success = ReadWait( key.code, sizeof( key.code ) );
keyTime = GetTickCount();
 return success;
}

void Irman::Interrupt()
{
SetEvent( ioCompletion );
}

bool Irman::ReadWait( void* data, unsigned long size )
{
if( comPort == INVALID_HANDLE_VALUE )
return false;

OVERLAPPED ov;
ZeroMemory( &ov, sizeof( ov ) );
ov.hEvent = ioCompletion;

// If ReadFile doesn't succeed, but the error is ERROR_IO_PENDING, we
// just wait for the operation to complete.

DWORD length;
if( !ReadFile( comPort, data, size, &length, &ov ) )
{
if( GetLastError() == ERROR_IO_PENDING )
{
if( Verify( GetOverlappedResult( comPort, &ov, &length, TRUE ) ) == FALSE )
return false;
}
else
{
Verify( FALSE ); // Read failed
return false;
}
}

// At this point, the buffer contains the expected data
return size == length;
}

bool Irman::WriteWait( const void* data, unsigned long size )
{
if( comPort == INVALID_HANDLE_VALUE )
return false;

OVERLAPPED ov;
ZeroMemory( &ov, sizeof( ov ) );
ov.hEvent = ioCompletion;

DWORD length;
if( !WriteFile( comPort, data, size, &length, &ov ) )
{
if( GetLastError() == ERROR_IO_PENDING )
{
if( Verify( GetOverlappedResult( comPort, &ov, &length, TRUE ) ) == FALSE )
return false;
}
else
{
Verify( FALSE ); // Write failed
return false;
}
}

return size == length;
}

// Power the Irman device by twiddling the control lines

void Irman::PowerOff() const
{
Verify( EscapeCommFunction( comPort, CLRDTR ) );
Verify( EscapeCommFunction( comPort, CLRRTS ) );
}

void Irman::PowerOn() const
{
Verify( EscapeCommFunction( comPort, SETDTR ) );
Verify( EscapeCommFunction( comPort, SETRTS ) );
}

// Throw away anything in the COM port buffers, and set the ignore time
// to start from now.

void Irman::Flush()
{
Verify( PurgeComm( comPort, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ) );
keyTime = GetTickCount();
}


Apart from issuing the "IR" sequence, all of the serial interactions in this program are input operations, and all of these bar the "OK" at the start are six byte sequences. However, what if something goes wrong? What if the Irman is removed in mid sequence or, more likely, the user wants to exit the program while it is waiting for some characters from the serial port? I need to be able to interrupt a read, and that is the root of the only significant problem with COM port I/O. By default, the COM port is opened in blocking mode, and there is no timeout, so the program will wait indefinitely for the character sequences. There are a number of ways round this: you could, for example, open in non-blocking mode or have short timeouts and poll the device, but that is fairly wasteful of resources. All other solutions involve separate threads to read the COM port and to service user interactions. In the unix world, the select function or similar would let you handle events from different sources asynchronously; or you could set up the COM port reader thread to be signalled (i.e., interrupt the read) when some user event occurred. In the Win32 world, a more attractive option is asynchronous I/O.

In normal (synchronous) I/O, the program prepares buffers to transmit or receive, calls the appropriate I/O function, and by the time the function returns, the buffers have been dealt with - completely transmitted or completely filled (or some error has occurred). With asynchronous I/O on the other hand, the program merely initiates the transfer and then continues while Windows, in the background, completes the operation and notifies the application some time in the future when the operation has completed. In this case, the application is not permitted to reuse the buffer space until Windows has told it that the I/O transaction has completed: Windows is handling the asynchronous nature of the I/O transfer at the cost of more complex buffer management. Windows NT (and 2000) support asynchronous I/O on virtually every flavour of I/O stream, while Windows 9x restricts it to not much more than COM port I/O - which is fairly convenient here!

This is what you need to do to manage asynchronous I/O. First, open the device with the extra flag FILE_FLAG_OVERLAPPED. Note that when a file is opened for asynchronous I/O, all I/O must be asynchronous - you cannot mix synchronous and asynchronous I/O.

After this, a read is initiated with a ReadFile, as normal, but with an extra parameter, a pointer to an OVERLAPPED structure. This contains a number of fields relating to the I/O operation, but the only important one here is hEvent, the handle of a Win32 manual reset event created specifically for indicating completion of the asynchronous I/O operation.

When the ReadFile returns, there are two possibilities (ignoring errors): the read may have completed already (for example, there might already have been data in the device's buffers), or it might not! In the first case, just continue as before; in the second, the program must wait for the operation to finish, either by polling or suspending on the completion event flag: both these options cane be exercised via the GetOverlappedResult function - its final argument is a flag which determines whether the function waits until the event is signalled. This implies that to interrupt an I/O operation, this event flag could be set from elsewhere - it is just a standard Win32 event - to wake up the waiting thread. The Microsoft documentation includes a CancelIo function which it is suggested should be invoked to cut short an asynchronous operation: however, that does not exist on Windows 95 so I have no option but to signal the event and keep my fingers crossed...

This has dealt with low level I/O operations - I now have an interruptible mechanism for retrieving byte sequences from the Irman. The next level handles error conditions. First, as I mentioned earlier, the Irman returns a number of byte sequences for each key press, at least with the remotes I have used. I make the assumption that the user is not going to hit keys more rapidly than a couple a second (though this parameter is settable in the registry - see later), so after receiving a sequence, I ignore all others for the next half second. (I have also found that hitting Winamp buttons too rapidly can sometimes result in it crashing, so a bit of throttling is useful in preventing that.) An additional mechanism for getting rid of spurious data is to flush the COM device's buffers (via PurgeComm) when I suspect there might be garbage.

The top level of the driver is to match Irman byte sequences with index numbers: this is a trivial search through a preloaded array of sequences. (If there were a lot of key codes, a hash table would be a better choice for data structure, but a simple vector will do here.) The array, along with any other operational parameters, is stored in the registry via the trivial C++ class, Registry. There is not space in this article to discuss this class, but well commented source files are available electronically.

There is one final layer to place on this before it is suitable for controlling applications on the PC: this is just a set of inline blocking functions - to be able to interrupt a I/O operation, I still need an extra thread to be triggered by user operations. I did contemplate wrapping this in a C++ class, but then decided that the options were too varied - direct invocation of functions (callbacks, in other words), sending Windows messages, COM invocations or socket messages, to name just a few) to make such a class tidy and efficient. Instead, I will just present one simple mechanism for controlling another application: a big switch statement! A reader thread (see Listing 3) loops reading key indexes from the Irman class and either invokes the appropriate operation via a switch statement or programs the Irman's byte sequence to key mapping. This thread terminates when the user interface thread sets the going flag to false: the foreground thread also invokes the Irman's Interrupt method to cancel any outstanding I/O operation, to avoid the reader thread blocking indefinitely.

Listing 3: Main code and asynchronous reader function 
#define UID 8877                   // Fairly arbitrary id for the tray icon
#define NOT_MSG ( WM_USER + 33 )   // Tray message id (arbitrary value)

static HMENU menu;                 // Popup menu

// There are two icons in the system tray, and iconNumber indicates
// which to display next

static HICON icon[ 2 ];
static int iconNumber;

// Names of the remote keys recognised - should be in the same order as 
// the configure dialog box buttons, and the same order as the Irman key
// codes.

static const TCHAR* keyStrings[] =
{
_T("Next Track"),
_T("Previous Track"),
_T("Increase Volume"),
_T("Decrease Volume"),
_T("Shutdown PC")
};
static const int numKeys = sizeof( keyStrings ) / sizeof( keyStrings[ 0 ] );

// Singleton Winamp controller and Irman interface

WinampManager mgr;
Irman ir( _T("Software\\BeesKnees\\Irman"), numKeys );

volatile HWND mainWnd;       // Handle to the (invisible) application window
volatile HWND configureWnd;  // Handle to the configuration dialog box: if
// this is non-null, the dialog is visible
volatile bool recording;     // True if we're waiting for a key to be recorded
volatile bool testing;       // True if we're testing instead of acting on keys

volatile bool going = true;  // The reader loop runs while this is set

static void Reader( void* )
{
while( going )
{
// Rather sloppily, this does not bother to guard against changes in the
// COM port (eg, a different one selected by the code below), but it seems
// to work OK...
int key = ir.Key();

// Toggle the tray icon
NOTIFYICONDATA not;
not.cbSize = sizeof( not );
not.hWnd = mainWnd;
not.uID = UID;
not.uFlags = NIF_ICON;
not.hIcon = icon[ iconNumber = !iconNumber ];
Shell_NotifyIcon( NIM_MODIFY, &not );

if( configureWnd )    // ie, if we're running the configure dialog
{
if( recording )     // ie, we need to respond to a completed record
{
SetDlgItemText( configureWnd, IDC_STATUS, key != -1 ? _T("OK") : _T("Failed") );
recording = false;
}
else if( testing )  // ie, echo, don't act
{
TCHAR buff[ 64 ];
wsprintf( buff, _T("Read %d (%s)"), key, key != -1 ? keyStrings[ key ] : _T("Undefined") );
SetDlgItemText( configureWnd, IDC_STATUS, buff );
}
}
else
{
// Just perform the appropriate action
switch( key )
{
	   case 0:
				 mgr.Skip( true );
						   break;
								   case 1:
											 mgr.Skip( false );
													   break;
															   case 2:
																		 mgr.Volume( true );
																				   break;
																						   case 3:
																									 mgr.Volume( false );
																											   break;
																													   case 4:
																																 mgr.Exit();
																																		   break;
																																				 }
																																					 }
																																					   } // end while going
}

BOOL CALLBACK ConfigProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM /*lParam*/ )
{
switch( msg )
{
case WM_INITDIALOG:
{
configureWnd = hwnd;    // Setting this indicates that configuration is active
const TCHAR* port = ir.Port();
if( _tcslen( port ) == 4 && port[ 3 ] >= _T('1') && port[ 3 ] <= _T('4') )
CheckRadioButton( hwnd, IDC_COM1, IDC_COM4, IDC_COM1 + port[ 3 ] - _T('1') );
SetDlgItemInt( hwnd, IDC_DELAY, ir.Delay(), FALSE );
}
return TRUE;

case WM_COMMAND:
switch( LOWORD( wParam ) )
{
case IDOK:
{
BOOL success;
UINT d = GetDlgItemInt( hwnd, IDC_DELAY, &success, FALSE );
if( success )
ir.Delay( d );
EndDialog( hwnd, 0 );
configureWnd = NULL;
   }
			 return TRUE;

					 case IDC_COM1:
							 case IDC_COM2:
									 case IDC_COM3:
											 case IDC_COM4:
													   {
																	   // I could check that the radio button hasn't already been
																				   // selected, but not doing this has the side-effect of
																							   // reinitialising the port if necessary...
																										   TCHAR buff[ 8 ];
																													   wsprintf( buff, _T("COM%d"), LOWORD( wParam ) - IDC_COM1 + 1 );
																																   ir.Port( buff );
																																			 }
																																					   return TRUE;

																																							   case IDC_NEXT_TRACK:
																																									   case IDC_PREV_TRACK:
																																											   case IDC_VOL_UP:
																																													   case IDC_VOL_DOWN:
																																															   case IDC_SHUTDOWN:
																																																		 {
																																																						 int code = LOWORD( wParam ) - IDC_NEXT_TRACK;
																																																									 TCHAR buff[ 64 ];
																																																												 wsprintf( buff, _T("Press remote key for %s"), keyStrings[ code ] );
																																																															 SetDlgItemText( hwnd, IDC_STATUS, buff );
																																																																		 recording = true;         // Initiate a key record
																																																																					 ir.SetKey( code );
																																																																							   }
																																																																										 return TRUE;

																																																																												 case IDC_CHECK_TEST:
																																																																														   testing = IsDlgButtonChecked( hwnd, IDC_CHECK_TEST ) == BST_CHECKED;
																																																																																	 return TRUE;
																																																																																		   }
																																																																																				 break;
																																																																																				   }
																																																																																					 return FALSE;
}

/*****************************************************************************

Main (invisible) window procedure. Just has to respond to create (spawn off
sub process), destroy and tray commands (toggle visibility).

This is only defined if we're not building a Winamp plugin.

*****************************************************************************/

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{

switch( msg )
{
case WM_CREATE:
_beginthread( Reader, 0, NULL );
break;

case WM_DESTROY:
going = false;  // This will kill off the reader
ir.Interrupt();
PostQuitMessage( 0 );
return 0;

case WM_COMMAND:
switch( LOWORD( wParam ) )
{
case ID_CONFIGURE:
DialogBox( reinterpret_cast< HINSTANCE >( GetWindowLong( hwnd, GWL_HINSTANCE ) ),
MAKEINTRESOURCE( IDD_CONFIGURE ), hwnd, ConfigProc );
break;

case ID_QUIT:
SendMessage( hwnd, WM_CLOSE, 0, 0 );
return 0;
}
break;

case NOT_MSG:
// The only message I expect from the system tray is a mouse click,
// in which case I pop up a menu at that cursor position
switch( lParam )
{
case WM_LBUTTONDOWN:
case WM_RBUTTONDOWN:
{
POINT p;
GetCursorPos( &p );
SetForegroundWindow( hwnd );
TrackPopupMenu( menu, TPM_RIGHTALIGN | TPM_RIGHTBUTTON, p.x, p.y, 0, hwnd, NULL) ;
}
break;
}
return 0;
}
return DefWindowProc( hwnd, msg, wParam, lParam );
}

int WINAPI _tWinMain( HINSTANCE hInstance, HINSTANCE /*prev*/, LPSTR /*cmdline*/, int /*cmdShow*/ )
{
icon[ 0 ] = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_ICON ) );
icon[ 1 ] = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_ICON_INV ) );

WNDCLASS wc;
wc.style         = 0;
wc.lpfnWndProc   = WndProc;
wc.cbClsExtra    = 0;
wc.cbWndExtra    = 0;
wc.hInstance     = hInstance;
wc.hIcon         = icon[ 0 ];
wc.hCursor       = LoadCursor( NULL, IDC_ARROW );
wc.hbrBackground = (HBRUSH)( COLOR_WINDOW + 1 );
wc.lpszMenuName  = NULL;
wc.lpszClassName = _T("WinampIR");

if( !RegisterClass( &wc ) )
return 0;

mainWnd = CreateWindow( wc.lpszClassName, wc.lpszClassName,
WS_POPUP,
CW_USEDEFAULT, 0,
CW_USEDEFAULT, 0,
NULL, NULL,
hInstance, NULL );
if( !mainWnd )
return 0;

menu = LoadMenu( hInstance, MAKEINTRESOURCE( IDR_MENU ) );
menu = GetSubMenu( menu, 0 );

// Populate the system tray
NOTIFYICONDATA not;
not.cbSize = sizeof( not );
not.hWnd = mainWnd;
not.uID = UID;
not.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
not.uCallbackMessage = NOT_MSG;
not.hIcon = wc.hIcon;
_tcsncpy( not.szTip, _T("Winamp IR controller"), 20 );
Shell_NotifyIcon( NIM_ADD, &not );

MSG msg;
while( GetMessage( &msg, NULL, 0, 0 ) )
{
TranslateMessage( &msg );
DispatchMessage( &msg );
}

// Release all Windows resources grabbed at startup
Shell_NotifyIcon( NIM_DELETE, &not );
DestroyIcon( icon[ 0 ] );
DestroyIcon( icon[ 1 ] );

return msg.wParam;
}


Winamp control
As a concrete example of application control, I chose Winamp: this was mainly because the application is so commonly used, but also because it presents a very nice plugin interface which would allow my code to mesh into it.

Winamp is very easy to control via a well defined set of Windows messages, described in the header file in the SDK available on its web site. All you need to do is locate the Winamp window and send the appropriate message. There are two ways to couple extra code to Winamp: keep the code in a totally separate executable and use something like the Win32 function FindWindow to locate the Winamp window; or make it a DLL with a few specific entry points and enable it to be loaded automatically by Winamp. Although the second technique has the advantages that the Winamp window handle is available immediately so there is no need to search for it, that the code is smaller since it is merely a DLL to be loaded, and that the code is more tightly integrated with Winamp, I chose the former technique because I wanted to use the same approach with other programs that are not quite as plugin friendly (and, I will be honest, it is easier to debug code in a separate application and my code is less likely to break Winamp this way!).

I wrapped the small number of Winamp message sends in a simple WinampManager class, defined in Listings 4 and 5, which maps member functions on to the specific message sends. There is one extra member, Exit, which closes Winamp and shuts down Windows - I take care to close Winamp first and wait for a bit since I have found that shutting down Windows while Winamp is active quite often does not succeed, possibly because of some of the other Winamp plugins I tend to have loaded.

Listing 4: WinampManager class definition 
class WinampManager
{
public:
WinampManager();
~WinampManager();

// Step forward or back a small number of seconds
bool Step( bool forward = true );

// Skip forward or backward a track
bool Skip( bool forward = true );

// Bump the volumen up or down
bool Volume( bool up );

// Exit Winamp and shutdown Windows
bool Exit();

private:
// Send an arbitrary message to Winamp
bool SendWinamp( UINT message, WPARAM wparam, LPARAM lparam );

// Slightly specialised for to send a "button press" message
bool SendButton( WPARAM button );

// Locate the Winamp window, to which to send the messages
void FindWinamp();

// Cached Winamp window handle
HWND winampWindow;

// Disallow copying
WinampManager( const WinampManager& );
WinampManager& operator=( const WinampManager& );
};


Listing 5: WinampManagerclass source 
WinampManager::WinampManager() : winampWindow( NULL )
{
}

WinampManager::~WinampManager()
{
}

bool WinampManager::Exit()
{
// I've found that occasionally shutting down Windows with Winamp still
// active is bad news - it's better to exit Winamp, and then kill Windows
SendWinamp( WM_CLOSE, 0, 0 );

// It's of considerable assistance to debugging NOT to exit windows!
# ifndef _DEBUG
Sleep( 1000 );                    // Give Winamp time to die
ExitWindowsEx( EWX_POWEROFF | EWX_SHUTDOWN, 0 );
# endif

return true;
}

// The action functions map fairly closely to the messages described in
// winamp.h, supplied by the Winamp folks

bool WinampManager::Volume( bool up )
{
SendButton( up ? WINAMP_VOLUMEUP : WINAMP_VOLUMEDOWN );
return true;
}

bool WinampManager::Skip( bool forward )
{
SendButton( forward ? WINAMP_BUTTON5 : WINAMP_BUTTON1 );
return true;
}

bool WinampManager::Step( bool forward )
{
SendButton( forward ? WINAMP_FFWD5S : WINAMP_REW5S );
return true;
}

void WinampManager::FindWinamp()
{
winampWindow = FindWindow( _T("Winamp v1.x"), NULL );
}

bool WinampManager::SendButton( WPARAM button )
{
return SendWinamp( WM_COMMAND, button, 0 );
}

bool WinampManager::SendWinamp( UINT message, WPARAM wparam, LPARAM lparam )
{
// Although I have a Winamp handle member, and had intended to cache it,
// I don't! If Winamp is closed and reopened, the handle would be invalid,
// and I have no easy way to tell that - at least, no cheaper than just
// searching for the window in the first place. Sooo... skip caching!
FindWinamp();
if( winampWindow )
{
SendMessage( winampWindow, message, wparam, lparam );
return true;
}
else
return false;
}



Tying it all together
If you examine the source code in Listing 3, you will see a few extra twiddles.

Although the code is C++ and does involve some string manipulation, I deliberately steered clear of the STL - some STL implementations, notably that supplied with the Microsoft compiler, do not have thread safe string classes, and working around that was more effort than it was worth for such a short piece of code. (There are some patches to the STL which could make it thread safe, at quite a computational cost.)

The application does not have a main window itself, but just occupies a space in the system tray. (If I had made this a Winamp plugin, I could even have avoided the extravagance of this screen space.) A click on the icon pops up a menu which offers the configuration dialog box or quit.

When each key sequence is processed, the tray icon toggles between normal and inverted, just to confirm that something is happening since I prefer not to trust hardware unless I can see an immediate result.

The configuration dialog (below) lets you test the Irman key definitions by just printing the name of the key pressed instead of carrying out the intended action - very useful for debugging.



Finally, the code has been written to be unicode compatible - all strings appear inside the _T macro, and all string related functions use the macro names in tchar.h. Defining UNICODE when compiling will result in all these mapping to unicode variants which should be more efficient on NT and omitting the definition results in ASCII code, more efficient on Win9x.

Conclusion
The starting point of this exercise for me was the desire to make a PC easier to use as an audio playing device. I found that there were a few cost effective ways to use remote control, one of the easiest being the Irman device. However, making use of it in a program was not completely trivial, but the C++ class and associated code I have shown here makes it fairly straightforward to use the device to drive another application. I now have a PC (still big, beige and ugly, but never mind) which to all intents an purposes replaces my CD player (in fact, I am using its remote!) with the added benefit that I do not have to get up to change the disc every hour or so.


--------------------------------------------------------------------------------

Gavin's home page | BeesKnees home page
Last modified on 30th December 2000 

