#pragma once

//From http://www.codeproject.com/KB/system/serial.aspx

#include "Serial/SerialEx.h"

class RepRapSerial : public CSerialEx
{
public:
	RepRapSerial(){m_bPrinting = false; m_iLineNr = 0;}
	// Event handler
	virtual void OnEvent (EEvent eEvent, EError eError);

	void AddToBuffer(string s){s+="\n"; buffer.push_back(s);}
	void SendNow(string s);
	void Clear() { buffer.clear();}
	UINT Length() { return buffer.size();}
	void StartPrint();
private:
	void SendNextLine();
	vector<string> buffer;
	bool m_bPrinting;
	UINT m_iLineNr;
};
