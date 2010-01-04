#pragma once

//From http://www.codeproject.com/KB/system/serial.aspx

#include "Serial/SerialEx.h"
#include "UI.h"

class RepRapSerial : public CSerialEx
{
public:
	RepRapSerial(){m_bPrinting = false; m_iLineNr = 0; gui=0;}
	// Event handler
	virtual void OnEvent (EEvent eEvent, EError eError);

	void AddToBuffer(string s){buffer.push_back(s);}
	void SendNow(string s);
	void Clear() { m_bPrinting = false; m_iLineNr = 0; buffer.clear();}
	UINT Length() { return buffer.size();}
	void StartPrint();
	void setGUI(GUI* g){ gui=g;}
	void SendNextLine();
	void test();
	void SendData(const string &s, const int lineNr);
private:
	void debugPrint(string s, bool selectLine = false);
	void echo(string s);
	vector<string> buffer;
	bool m_bPrinting;
	UINT m_iLineNr;
	string InBuffer;
	
	GUI* gui;

};
