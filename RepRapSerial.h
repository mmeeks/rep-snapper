#pragma once

//From http://www.codeproject.com/KB/system/serial.aspx

#undef WIN32

#ifdef WIN32
#include "Serial/SerialEx.h"
#endif

#include "UI.h"
#include "AsyncSerial.h"
//#include "ftd2xx.h"


//BufferedAsyncSerial serial("/dev/ttyUSB0",115200);
/*
class FooClass
{
public:
	void Print( int a )
	{
		std::cout << "A FooClass, param = "<< a <<" this = " << this << std::endl;
	}
};
void main()
{
	FooClass *myFoo = new FooClass();
	void( FooClass::* oldFunc )(int) = &FooClass::Print; //C style function pointer (myFoo->*oldFunc)( 5 );
	
	boost::function<void(int)> newFunc = boost::bind( &FooClass::Print, myFoo, _1 ); //boost function
	newFunc( 5 );
}
*/

class RepRapSerial : public BufferedAsyncSerial
{
public:
	RepRapSerial(){m_bPrinting = false; m_iLineNr = 0; gui=0;m_bConnected=false; debugMask =  DEBUG_ECHO | DEBUG_INFO | DEBUG_ERRORS;}
	void open(const std::string& devname, unsigned int baud_rate)
	{
		try{
			BufferedAsyncSerial::open(devname, baud_rate);
		} catch (std::exception& e) {
			stringstream oss;
			oss<<"Exception: " << e.what() << ":" << devname << endl;
			debugPrint(oss.str(), true);
		}
	}
	// Event handler
//	virtual void OnEvent (EEvent eEvent, EError eError);

	void OnEvent(char* data, size_t size);

	void AddToBuffer(string s){buffer.push_back(s);}
	void SendNow(string s);
	void Clear() { m_bPrinting = false; m_iLineNr = 0; SetLineNr(-1); buffer.clear();}
	uint Length() { return buffer.size();}
	void StartPrint();
	void SetLineNr(int nr);
	void SetDebugMask(int mask, bool on);
	void SetDebugMask();
	void setGUI(GUI* g){ gui=g;}
	void SendNextLine();
	void SendData(string s, const int lineNr);
	void Connect();
	void DisConnect();
	bool isPrinting(){return m_bPrinting;}

private:
	void debugPrint(string s, bool selectLine = false);
	void echo(string s);
	vector<string> buffer;
	bool m_bPrinting;
	bool m_bConnected;
	uint m_iLineNr;
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
