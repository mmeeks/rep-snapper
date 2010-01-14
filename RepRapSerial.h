#pragma once

//From http://www.codeproject.com/KB/system/serial.aspx

#include "Serial/SerialEx.h"
#include "UI.h"
//#include "ftd2xx.h"

class RepRapSerial : public CSerialEx
{
public:
	RepRapSerial(){m_bPrinting = false; m_iLineNr = 0; gui=0;m_bConnected=false; debugMask =  DEBUG_ECHO | DEBUG_INFO | DEBUG_ERRORS;}
	// Event handler
	virtual void OnEvent (EEvent eEvent, EError eError);

	void AddToBuffer(string s){buffer.push_back(s);}
	void SendNow(string s);
	void Clear() { m_bPrinting = false; m_iLineNr = 0; SetLineNr(-1); buffer.clear();}
	UINT Length() { return buffer.size();}
	void StartPrint();
	void SetLineNr(int nr);
	void SetDebugMask(int mask, bool on);
	void SetDebugMask();
	void setGUI(GUI* g){ gui=g;}
	void SendNextLine();
	void test();
	void SendData(const string &s, const int lineNr);
	void Connect();
	void DisConnect();
	bool isPrinting(){return m_bPrinting;}

private:
	void debugPrint(string s, bool selectLine = false);
	void echo(string s);
	vector<string> buffer;
	bool m_bPrinting;
	bool m_bConnected;
	UINT m_iLineNr;
	string InBuffer;
	short debugMask;

	GUI* gui;
/*
	// Very private :P
	FT_HANDLE fthandle;
	FT_STATUS res;
	LONG COMPORT;

	char COMx[5];
	int n;

	DCB dcb;
	HANDLE hCommPort;
	BOOL fSuccess;	*/


};
