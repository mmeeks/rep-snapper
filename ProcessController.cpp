#include "StdAfx.h"
#include "ProcessController.h"


void ProcessController::Draw()
{
	printer.Draw();
	stl.draw();
	previewCuttingPlane.Draw(0.5f);
	gcode.draw();
}