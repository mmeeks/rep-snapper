#include "stdafx.h"
#include "RepRapSerial.h"

enum { EOF_Char = 27 };

//From http://www.codeproject.com/KB/system/serial.aspx

int ShowError (LONG lError, LPCTSTR lptszMessage)
{
	// Generate a message text
	TCHAR tszMessage[256];
	wsprintf(tszMessage,_T("%s\n(error code %d)"), lptszMessage, lError);

	// Display message-box and return with an error-code
	::MessageBox(0,tszMessage,_T("Listener"), MB_ICONSTOP|MB_OK);
	return 1;
}


void RepRapSerial::OnEvent (EEvent eEvent, EError eError)
{
	LONG    lLastError = ERROR_SUCCESS;

	// Handle break event
	if (eEvent & CSerial::EEventBreak)
	{
		printf("\n### BREAK received ###\n");
	}

	// Handle CTS event
	if (eEvent & CSerial::EEventCTS)
	{
		printf("\n### Clear to send %s ###\n", GetCTS()?"on":"off");
	}

	// Handle DSR event
	if (eEvent & CSerial::EEventDSR)
	{
		printf("\n### Data set ready %s ###\n", GetDSR()?"on":"off");
	}

	// Handle error event
	if (eEvent & CSerial::EEventError)
	{
		printf("\n### ERROR: ");
		switch (GetError())
		{
		case CSerial::EErrorBreak:		printf("Break condition");			break;
		case CSerial::EErrorFrame:		printf("Framing error");			break;
		case CSerial::EErrorIOE:		printf("IO device error");			break;
		case CSerial::EErrorMode:		printf("Unsupported mode");			break;
		case CSerial::EErrorOverrun:	printf("Buffer overrun");			break;
		case CSerial::EErrorRxOver:		printf("Input buffer overflow");	break;
		case CSerial::EErrorParity:		printf("Input parity error");		break;
		case CSerial::EErrorTxFull:		printf("Output buffer full");		break;
		default:						printf("Unknown");					break;
		}
		printf(" ###\n");
	}

	// Handle ring event
	if (eEvent & CSerial::EEventRing)
	{
		printf("\n### RING ###\n");
	}

	// Handle RLSD/CD event
	if (eEvent & CSerial::EEventRLSD)
	{
		printf("\n### RLSD/CD %s ###\n", GetRLSD()?"on":"off");
	}

	// Handle data receive event
	if (eEvent & CSerial::EEventRecv)
	{
		// Read data, until there is nothing left
		DWORD dwBytesRead = 0;
		char szBuffer[101];
		do{
			// Read data from the COM-port
			lLastError = Read(szBuffer,sizeof(szBuffer)-1,&dwBytesRead);
			if (lLastError != ERROR_SUCCESS)
			{
				ShowError(GetLastError(), _T("Unable to read from COM-port."));
				return;
			}

			if (dwBytesRead > 0)
			{
				// Finalize the data, so it is a valid string
				szBuffer[dwBytesRead] = '\0';

//				gui->CommunationsLogText->insert(szBuffer);

				// Check if EOF (CTRL+'[') has been specified
				if (strchr(szBuffer,EOF_Char))
					return;//fContinue = false;
			}
		}while (dwBytesRead == sizeof(szBuffer)-1);
	

		// Buffer has been read, understand it
		printf("Received:\"%s\" (%d bytes)\n", szBuffer, dwBytesRead);
//		if(string(szBuffer) == "ok")
		{
			if(m_bPrinting)
			{
				SendNextLine();
			}
		}	// if recieved 'ok'
	}
}

void RepRapSerial::StartPrint()
{
	m_bPrinting = true;
	SendNextLine();
	SendNextLine();
	SendNextLine();
	SendNextLine();
}

void RepRapSerial::SendNextLine()
{
	assert(m_bPrinting == true);
	if(m_iLineNr < buffer.size())
		{
		printf("Sending:%s", buffer[m_iLineNr].c_str());
		string a = buffer[m_iLineNr++];
		a+= "\n";
		Write(a.c_str());
		}
	else	// we are done
		{
		m_bPrinting = false;
		buffer.clear();
		}
}

void RepRapSerial::SendNow(string s)
{
	printf("Sending:%s", s.c_str());
	s+= "\n";
	Write(s.c_str());
}