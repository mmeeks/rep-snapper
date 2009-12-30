#pragma once


#include "Serial/SerialEx.h"

class RepRapSerial : public CSerialEx
{
public:

	// Event handler
	virtual void OnEvent (EEvent eEvent, EError eError);

};