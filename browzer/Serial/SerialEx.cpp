//	SerialEx.cpp - Implementation of the CSerialEx class
//
//	Copyright (C) 1999-2002 Ramon de Klein (R.de.Klein@iaf.nl)
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


//////////////////////////////////////////////////////////////////////
// Include the standard header files

//#define STRICT
//#include <crtdbg.h>
//#include <tchar.h>
//#include <windows.h>
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////
// Include module headerfile

#include "SerialEx.h"


//////////////////////////////////////////////////////////////////////
// Disable warning C4127: conditional expression is constant, which
// is generated when using the _RPTF and _ASSERTE macros.

#pragma warning(disable: 4127)


//////////////////////////////////////////////////////////////////////
// Enable debug memory manager

#ifdef _DEBUG

#ifdef THIS_FILE
#undef THIS_FILE
#endif

static const char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW

#endif


//////////////////////////////////////////////////////////////////////
// Code

CSerialEx::CSerialEx()
	: m_hevtStop(0)
	, m_hevtCommEvent(0)
	, m_hThread(0)
{
}

CSerialEx::~CSerialEx()
{
	// Check if the thread handle is still there. If so, then we
	// didn't close the serial port. We cannot depend on the
	// CSerial destructor, because if it calls Close then it
	// won't call our overridden Close.
	if (m_hThread)
	{
		// Display a warning
		_RPTF0(_CRT_WARN, "CSerialEx::~CSerialEx - Serial port not closed\n");

		// Close implicitly
		Close();
	}
}

LONG CSerialEx::Open (LPCTSTR lpszDevice, DWORD dwInQueue, DWORD dwOutQueue)
{
	// Call the base class first
	long lLastError = CSerial::Open(lpszDevice,dwInQueue,dwOutQueue);
	if (lLastError != ERROR_SUCCESS)
		return lLastError;

	// Create the event
	m_hevtStop = ::CreateEvent(0,true,false,0);
	m_hevtCommEvent = ::CreateEvent(0,true,false,0);
	if ((m_hevtStop == 0) || (m_hevtCommEvent == 0))
	{
		// Obtain the error code
		long lLastError = ::GetLastError();

		// Display a warning
		_RPTF0(_CRT_WARN, "CSerialEx::Open - Unable to create event\n");

		// Unable to start the watch thread, so we'll close the serial port again
		CSerial::Close();

		// Set the error code and exit
		m_lLastError = lLastError;
		return m_lLastError;
	}

	// Return the error
	return m_lLastError;
}

LONG CSerialEx::Close (void)
{
	// Stop listener thread (wait until it ends)
	StopListener(INFINITE);

	// Close the event handles
	if (m_hevtStop)
	{
		::CloseHandle(m_hevtStop);
		m_hevtStop = 0;

	}
	
	if (m_hevtCommEvent)
	{
		::CloseHandle(m_hevtCommEvent);
		m_hevtCommEvent = 0;

	}
	
	// Call the base class
	return CSerial::Close();
}

LONG CSerialEx::StartListener (void)
{
	// Check if the watcher thread was already running
	if (m_hThread == 0)
	{
		// Start the watcher thread
		DWORD dwThreadId = 0;
		m_hThread = ::CreateThread(0,0,ThreadProc,LPVOID(this),0,&dwThreadId);
		if (m_hThread == 0)
		{
			// Display a warning
			_RPTF0(_CRT_WARN, "CSerialEx::StartListener - Unable to start COMM watcher thread\n");

			// Set the error code and exit
			m_lLastError = ::GetLastError();
			return m_lLastError;
		}
	}

	// Return the error
	m_lLastError = ERROR_SUCCESS;
	return m_lLastError;
}

LONG CSerialEx::StopListener (DWORD dwTimeout)
{
	// Check if the thread is running
	if (m_hThread)
	{
		// Signal the watcher thread that it should stop
		::SetEvent(m_hevtStop);
		
		// Wait until the watcher thread has stopped
		::WaitForSingleObject(m_hThread,dwTimeout);

		// Close handle to the thread
		::CloseHandle(m_hThread);
		m_hThread = 0;
	}

	// Return the error
	m_lLastError = ERROR_SUCCESS;
	return m_lLastError;
}

DWORD WINAPI CSerialEx::ThreadProc (LPVOID lpArg)
{
	// Route the method to the actual object
	CSerialEx* pThis = reinterpret_cast<CSerialEx*>(lpArg);
	return pThis->ThreadProc();
}

DWORD CSerialEx::ThreadProc (void)
{
	// Create an overlapped structure, which we will use
	// to wait for communication events.
	OVERLAPPED overlapped = {0};
	overlapped.hEvent = m_hevtCommEvent;

	// Create the array of handles to wait for
	HANDLE ah[2];
	ah[0] = m_hevtStop;			// Stop event
	ah[1] = overlapped.hEvent;	// COM event (from driver)

	// Start the asynchronous WaitEvent
	if (WaitEvent(&overlapped) != ERROR_SUCCESS)
		return m_lLastError;

	// Keep looping
	bool fStop = false;
	while (!fStop)
	{
		// Wait until one of the events happens
		switch (::WaitForMultipleObjects(sizeof(ah)/sizeof(HANDLE),ah,false,INFINITE))
		{
		case WAIT_OBJECT_0:
			// We need to stop
			fStop = true;
			break;

		case WAIT_OBJECT_0+1:
			{
				// Determine the event
				EEvent eEvent = GetEventType();

				// Obtain the error status during this event
				DWORD dwErrors = 0;
				if (!::ClearCommError(m_hFile,&dwErrors,0))
				{
					// Set the internal error code
					m_lLastError = ::GetLastError();

					// Issue an error and quit
					_RPTF0(_CRT_WARN, "CSerialEx::ThreadProc - Unable to obtain COM status\n");
				}

				// Convert the error
				EError eError = EError(dwErrors);

				// There was a COMM event, which needs handling. We'll call the
				// event handler. We can receive a "zero" event, when the
				// mask or event character has been set. We won't pass this
				// down to the window.
				if (eEvent)
					OnEvent(eEvent,eError);

				// Start the asynchronous WaitComEvent again
				if (WaitEvent(&overlapped) != ERROR_SUCCESS)
					return m_lLastError;
			}
			break;

		default:
			// Something went seriously wrong, which we'll need to report
			fStop = true;
			break;
		}
	}

	// Cancel the pending WaitEvent, but we won't do this using CancelIo. This
	// would break Win95 compatibility and some USB serial dongles cannot handle
	// CancelIo correctly. By setting the event mask again, the call will also
	// be completed before the thread exits. Note that this is only performed when
	// the thread is terminated in a controlled manner. When the thread is killed
	// by other means then the pending WaitEvent cannot be completed and you might
	// still be in trouble. Unfortunately, there is no way this library can help
	// you solve this problem (only the driver vendor can).
	SetMask(GetEventMask());

	// Bye bye
	return 0;
}
