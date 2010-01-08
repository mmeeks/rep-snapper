/* -------------------------------------------------------- *
*
* ModelViewController.cpp
*
* Copyright 2009+ Michael Holm - www.kulitorum.com
*
* This file is part of RepSnapper and is made available under
* the terms of the GNU General Public License, version 2, or at your
* option, any later version, incorporated herein by reference.
*
* ------------------------------------------------------------------------- */
#include "StdAfx.h"
#include "ModelViewController.h"

ModelViewController::~ModelViewController()
{
}

ModelViewController::ModelViewController(int x,int y,int w,int h,const char *l) : Fl_Gl_Window(x,y,w,h,l)
{
	gui = 0;
	Min = Vector3f(0, 0, 0);
	Max = Vector3f(200,200,200);
	Center.x = Center.y = 100.0f;
	Center.z = 0.0f;
	zoom = 100.0f;

	glClearColor (0.0f, 0.0f, 0.0f, 0.5f);							// Black Background
	glClearDepth (1.0f);											// Depth Buffer Setup
	glDepthFunc (GL_LEQUAL);										// The Type Of Depth Testing (Less Or Equal)
	glEnable (GL_DEPTH_TEST);										// Enable Depth Testing
	glShadeModel (GL_FLAT);											// Select Flat Shading (Nice Definition Of Objects)
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);				// Set Perspective Calculations To Most Accurate

	ArcBall = new ArcBallT((GLfloat)w, (GLfloat)h);				                // NEW: ArcBall Instance

	Transform.M[0] = 1.0f;	Transform.M[1] = 0.0f; Transform.M[2] = 0.0f;Transform.M[3] = 0.0f,				// NEW: Final Transform
	Transform.M[4] = 0.0f;	Transform.M[5] = 1.0f; Transform.M[6] = 0.0f;Transform.M[7] = 0.0f,				// NEW: Final Transform
	Transform.M[8] = 0.0f;	Transform.M[9] = 0.0f; Transform.M[10] = 1.0f;Transform.M[11] = 0.0f,				// NEW: Final Transform
	Transform.M[12] = 0.0f;	Transform.M[13] = 0.0f; Transform.M[14] = 0.0f;Transform.M[15] = 1.0f,				// NEW: Final Transform

	LastRot.M[0]=1.0f;LastRot.M[1]=0.0f;LastRot.M[2]=0.0f;					// NEW: Last Rotation
	LastRot.M[3]=0.0f;LastRot.M[4]=1.0f;LastRot.M[5]=0.0f;					// NEW: Last Rotation
	LastRot.M[6]=0.0f;LastRot.M[7]=0.0f;LastRot.M[8]=1.0f;					// NEW: Last Rotation
	ThisRot.M[0]=1.0f;ThisRot.M[1]=0.0f;ThisRot.M[2]=0.0f;					// NEW: Last Rotation
	ThisRot.M[3]=0.0f;ThisRot.M[4]=1.0f;ThisRot.M[5]=0.0f;					// NEW: Last Rotation
	ThisRot.M[6]=0.0f;ThisRot.M[7]=0.0f;ThisRot.M[8]=1.0f;					// NEW: Last Rotation

	quadratic=gluNewQuadric();										// Create A Pointer To The Quadric Object
	gluQuadricNormals(quadratic, GLU_SMOOTH);						// Create Smooth Normals
	gluQuadricTexture(quadratic, GL_TRUE);							// Create Texture Coords

	ProcessControl.LoadXML();
	CopySettingsToGUI();

	m_bConnected = false;
	m_bExtruderDirection = true;
}


void ModelViewController::CalcBoundingBoxAndZoom()
{
	ProcessControl.stl.CalcBoundingBoxAndZoom();
	
	Max = ProcessControl.stl.Max;
	Min = ProcessControl.stl.Min;
	Center = ProcessControl.stl.Center;

	if((Max-Min).length() > 0)
		{
		// Find zoom
		float L=0;
		if(Max.x - Min.x > L)	L = Max.x - Min.x;
		if(Max.y - Min.y > L)	L = Max.y - Min.y;
		if(Max.z - Min.z > L)	L = Max.z - Min.z;

		zoom= L;
		}
	else
		zoom = 100.0f;
}

void ModelViewController::resize(int x,int y, int width, int height)					// Reshape The Window When It's Moved Or Resized
{
	Fl_Gl_Window::resize(x,y,width,height);
}

void ModelViewController::CenterView()
{
	glTranslatef(-Center.x, -Center.y, -Center.z);
}

void ModelViewController::draw()
{
    if (!valid())
		{
		glLoadIdentity();
		glViewport (0, 0, w(),h());											// Reset The Current Viewport
		glMatrixMode (GL_PROJECTION);										// Select The Projection Matrix
		glLoadIdentity ();													// Reset The Projection Matrix
		gluPerspective (45.0f, (float)w()/(float)h(),1.0f, 1000000.0f);						// Calculate The Aspect Ratio Of The Window
		glMatrixMode (GL_MODELVIEW);										// Select The Modelview Matrix
		glLoadIdentity ();													// Reset The Modelview Matrix
		ArcBall->setBounds((GLfloat)w(), (GLfloat)h());                 //*NEW* Update mouse bounds for arcball
		glEnable(GL_LIGHTING);
		for(int i=0;i<4;i++)
		{
			lights[i].Init ( (GLenum)(GL_LIGHT0+i) );
			//		lights[i].SetPosition ( 0, 0, 0, 0 );
			lights[i].SetAmbientColor ( 0.2f, 0.2f, 0.2f, 1.0f );
			lights[i].SetDiffuseColor ( 1.0f, 1.0f, 1.0f, 1.0f );
			lights[i].SetSpecular ( 1.0f, 1.0f, 1.0f, 1.0f );
			lights[i].SetValues ( );
			lights[i].TurnOff ( );
		}

		lights[0].SetPosition ( -100, 100, 200, 0 );
		lights[1].SetPosition ( 100, 100, 200, 0 );
		lights[2].SetPosition ( 100, -100, 200, 0 );
		lights[3].SetPosition ( 100, -100, 200, 0 );

		for(int i=0;i<4;i++)
			lights[i].SetValues ( );

		lights[0].TurnOn();
		lights[3].TurnOn();

/*
		float params[4];

		for(int i=0;i<8;i++)
		{
			GLenum lightNr = (GLenum)(GL_LIGHT0+i);
			cout << " Light" << i << "\n";
			glGetLightfv( (GLenum)lightNr, GL_DIFFUSE, &params[0] );
			cout << "GL_DIFFUSE " << params[0] << " " << params[1] << " " << params[2];
			glGetLightfv( (GLenum)lightNr, GL_POSITION, &params[0] );
			cout << "GL_POSITION " << params[0] << " " << params[1] << " " << params[2];
			cout << "\n";
		}
*/

		// enable lighting
		glDisable ( GL_LIGHTING);
	}

//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// Clear Screen And Depth Buffer
	glLoadIdentity();												// Reset The Current Modelview Matrix
	glTranslatef(0.0f,0.0f,-zoom*2);								// Move Left 1.5 Units And Into The Screen 6.0
	
	glMultMatrixf(Transform.M);										// NEW: Apply Dynamic Transform
	CenterView();

    glPushMatrix();													// NEW: Prepare Dynamic Transform
	glColor3f(0.75f,0.75f,1.0f);


	/*--------------- Draw Grid and Axis ---------------*/

	DrawGridAndAxis();

	/*--------------- Draw models ------------------*/

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

	ProcessControl.Draw();

	/*--------------- Exit -----------------*/
	glPopMatrix();													// NEW: Unapply Dynamic Transform
	glFlush ();														// Flush The GL Rendering Pipeline
//	swap_buffers();
}


int ModelViewController::handle(int event) 
{
	Vector2f    Clickpoint;												// NEW: Current Mouse Point
	static float Click_y;
	static float old_zoom;
	
	Clickpoint.x =  (GLfloat)Fl::event_x();;
	Clickpoint.y =  (GLfloat)Fl::event_y();;

	switch(event) {
		case FL_PUSH:	//mouse down event position in Fl::event_x() and Fl::event_y()
			{
				switch(Fl::event_button())	{
				case FL_LEFT_MOUSE:
					MousePt.T[0] = (GLfloat)Clickpoint.x;
					MousePt.T[1] = (GLfloat)Clickpoint.y;
					ArcBall->click(&MousePt);								// Update Start Vector And Prepare For Dragging
					break;
				case FL_MIDDLE_MOUSE:
					Matrix3fSetIdentity(&LastRot);								// Reset Rotation
					Matrix3fSetIdentity(&ThisRot);								// Reset Rotation
					Matrix4fSetRotationFromMatrix3f(&Transform, &ThisRot);		// Reset Rotation
					break;
				case FL_RIGHT_MOUSE:
					Click_y = Clickpoint.y;
					old_zoom = zoom;
					break;
				}
				LastRot = ThisRot;										// Set Last Static Rotation To Last Dynamic One
				redraw();
				return 1;
			}
		case FL_DRAG:	//mouse moved while down event ...
			switch(Fl::event_button())
				{
				case FL_LEFT_MOUSE:
					Quat4fT     ThisQuat;

					MousePt.T[0] = Clickpoint.x;
					MousePt.T[1] = Clickpoint.y;

					ArcBall->drag(&MousePt, &ThisQuat);						// Update End Vector And Get Rotation As Quaternion
					Matrix3fSetRotationFromQuat4f(&ThisRot, &ThisQuat);		// Convert Quaternion Into Matrix3fT
					Matrix3fMulMatrix3f(&ThisRot, &LastRot);				// Accumulate Last Rotation Into This One
					Matrix4fSetRotationFromMatrix3f(&Transform, &ThisRot);	// Set Our Final Transform's Rotation From This One
					redraw();
					break;
				case FL_MIDDLE_MOUSE:
					break;				
				case FL_RIGHT_MOUSE:
					float y = 	Click_y - Clickpoint.y;
					zoom = old_zoom + y*0.1;
					redraw();
					break;				
				}
			return 1;
		case FL_RELEASE: //mouse up event ...
			MousePt.T[0] = (GLfloat)Fl::event_x();
			MousePt.T[1] = (GLfloat)Fl::event_y();
			redraw();
			return 1;
		case FL_FOCUS :
		case FL_UNFOCUS : // Return 1 if you want keyboard events, 0 otherwise
			return 0;
		case FL_KEYBOARD: //keypress, key is in Fl::event_key(), ascii in Fl::event_text() .. Return 1 if you understand/use the keyboard event, 0 otherwise...
			return 1;
		case FL_SHORTCUT: // shortcut, key is in Fl::event_key(), ascii in Fl::event_text() ... Return 1 if you understand/use the shortcut event, 0 otherwise...
			return 1;
		case FL_CLOSE:
			if(fl_ask("Save settings?"))
			{
				int a=0;
//				ProcessControl.SaveXML();
			}
			break;
		default:
			break;
	}
	// pass other events to the base class...
	return Fl_Gl_Window::handle(event);
}

void ModelViewController::DrawGridAndAxis()
{
	//Grid
}

void ModelViewController::ConvertToGCode()
{
	string GcodeTxt;
	
	Fl_Text_Buffer* buffer = gui->GCodeResult->buffer();
	buffer->remove(0, buffer->length());

	buffer = gui->GCodeStart->buffer();
	char* pText = buffer->text();
	string GCodeStart(pText);
	buffer = gui->GCodeLayer->buffer();
	pText = buffer->text();
	string GCodeLayer(pText);
	buffer = gui->GCodeEnd->buffer();
	pText = buffer->text();
	string GCodeEnd(pText);

	ProcessControl.ConvertToGCode(GcodeTxt, GCodeStart, GCodeLayer, GCodeEnd);
	buffer = gui->GCodeResult->buffer();
	
	int length = GcodeTxt.length();
	GcodeTxt += "\0";
	int length2 = GcodeTxt.length();
	buffer->append( GcodeTxt.c_str() );
	redraw();
}

void ModelViewController::init()
{

	Fl_Text_Buffer* buffer = gui->GCodeStart->buffer();
	buffer->text(ProcessControl.GCodeStartText.c_str());
	buffer = gui->GCodeLayer->buffer();
	buffer->text(ProcessControl.GCodeLayerText.c_str());
	buffer = gui->GCodeEnd->buffer();
	buffer->text(ProcessControl.GCodeEndText.c_str());
	buffer = gui->NotesEditor->buffer();
	buffer->text(ProcessControl.Notes.c_str());

//	buffer = gui->CommunationsLogText->buffer();
//	buffer->text("Dump");
}

//Make the remaining buttons work
//implement acceleration

void ModelViewController::CopySettingsToGUI()
{
	if(gui == 0)
		return;
	Fl_Text_Buffer* buffer = gui->GCodeStart->buffer();
	buffer->text(ProcessControl.GCodeStartText.c_str());
	buffer = gui->GCodeLayer->buffer();
	buffer->text(ProcessControl.GCodeLayerText.c_str());
	buffer = gui->GCodeEnd->buffer();
	buffer->text(ProcessControl.GCodeEndText.c_str());
	buffer = gui->NotesEditor->buffer();
	buffer->text(ProcessControl.Notes.c_str());

	gui->RaftEnableButton->value(ProcessControl.RaftEnable);
	gui->RaftSizeSlider->value(ProcessControl.RaftSize);
	gui->RaftBaseLayerCountSlider->value(ProcessControl.RaftBaseLayerCount);
	gui->RaftMaterialPrDistanceRatioSlider->value(ProcessControl.RaftMaterialPrDistanceRatio);
	gui->RaftRotationSlider->value(ProcessControl.RaftRotation);
	gui->RaftBaseDistanceSlider->value(ProcessControl.RaftBaseDistance);
	gui->RaftBaseThicknessSlider->value(ProcessControl.RaftBaseThickness);
	gui->RaftBaseTemperatureSlider->value(ProcessControl.RaftBaseTemperature);
	gui->RaftInterfaceLayerCountSlider->value(ProcessControl.RaftInterfaceLayerCount);
	gui->RaftInterfaceMaterialPrDistanceRatioSlider->value(ProcessControl.RaftInterfaceMaterialPrDistanceRatio);
	gui->RaftRotationPrLayerSlider->value(ProcessControl.RaftRotationPrLayer);
	gui->RaftInterfaceDistanceSlider->value(ProcessControl.RaftInterfaceDistance);
	gui->RaftInterfaceThicknessSlider->value(ProcessControl.RaftInterfaceThickness);
	gui->RaftInterfaceTemperatureSlider->value(ProcessControl.RaftInterfaceTemperature);

	// GCode
	gui->GCodeDrawStartSlider->value(ProcessControl.GCodeDrawStart);
	gui->GCodeDrawEndSlider->value(ProcessControl.GCodeDrawEnd);
	gui->MinPrintSpeedXYSlider->value(ProcessControl.MinPrintSpeedXY);
	gui->MaxPrintSpeedXYSlider->value(ProcessControl.MaxPrintSpeedXY);
	gui->MinPrintSpeedZSlider->value(ProcessControl.MinPrintSpeedZ);
	gui->MaxPrintSpeedZSlider->value(ProcessControl.MaxPrintSpeedZ);

	gui->accelerationStepsSlider->value(ProcessControl.accelerationSteps);
	gui->distanceBetweenSpeedStepsSlider->value(ProcessControl.distanceBetweenSpeedSteps);
	gui->UseFirmwareAccelerationButton->value(ProcessControl.UseFirmwareAcceleration);
	gui->extrusionFactorSlider->value(ProcessControl.extrusionFactor);
	gui->UseIncrementalEcodeButton->value(ProcessControl.UseIncrementalEcode);

	// Printer
	gui->VolumeX->value(ProcessControl.m_fVolume.x);
	gui->VolumeY->value(ProcessControl.m_fVolume.y);
	gui->VolumeZ->value(ProcessControl.m_fVolume.z);
	gui->MarginX->value(ProcessControl.PrintMargin.x);
	gui->MarginY->value(ProcessControl.PrintMargin.y);

	gui->ExtrudedMaterialWidthSlider->value(ProcessControl.ExtrudedMaterialWidth);

	// STL 
	gui->LayerThicknessSlider->value(ProcessControl.LayerThickness);
	gui->CuttingPlaneValueSlider->value(ProcessControl.CuttingPlaneValue);
//	gui->PolygonOpasitySlider->value(ProcessControl.PolygonOpasity);
	// CuttingPlane
	gui->InfillDistanceSlider->value(ProcessControl.InfillDistance);
	gui->InfillRotationSlider->value(ProcessControl.InfillRotation);
	gui->InfillRotationPrLayerSlider->value(ProcessControl.InfillRotationPrLayer);
	gui->OptimizationSlider->value(ProcessControl.Optimization);
	gui->ExamineSlider->value(ProcessControl.Examine);

	gui->ShellOnlyButton->value(ProcessControl.ShellOnly);
	gui->ShellCountSlider->value(ProcessControl.ShellCount);

	gui->EnableAccelerationButton->value(ProcessControl.EnableAcceleration);

	// GUI GUI
	gui->DisplayEndpointsButton->value(ProcessControl.DisplayEndpoints);
	gui->DisplayNormalsButton->value(ProcessControl.DisplayNormals);
	gui->DisplayWireframeButton->value(ProcessControl.DisplayWireframe);
	gui->DisplayWireframeShadedButton->value(ProcessControl.DisplayWireframeShaded);
	gui->DisplayPolygonsButton->value(ProcessControl.DisplayPolygons);
	gui->DisplayAllLayersButton->value(ProcessControl.DisplayAllLayers);
	gui->DisplayinFillButton->value(ProcessControl.DisplayinFill);
	gui->DisplayDebuginFillButton->value(ProcessControl.DisplayDebuginFill);
	gui->DisplayDebugButton->value(ProcessControl.DisplayDebug);
	gui->DisplayCuttingPlaneButton->value(ProcessControl.DisplayCuttingPlane);
	gui->DrawVertexNumbersButton->value(ProcessControl.DrawVertexNumbers);
	gui->DrawLineNumbersButton->value(ProcessControl.DrawLineNumbers);

	// Rendering
	gui->PolygonValSlider->value(ProcessControl.PolygonVal);
	gui->PolygonSatSlider->value(ProcessControl.PolygonSat);
	gui->PolygonHueSlider->value(ProcessControl.PolygonHue);
	gui->WireframeValSlider->value(ProcessControl.WireframeVal);
	gui->WireframeSatSlider->value(ProcessControl.WireframeSat);
	gui->WireframeHueSlider->value(ProcessControl.WireframeHue);
	gui->NormalSatSlider->value(ProcessControl.NormalsSat);
	gui->NormalValSlider->value(ProcessControl.NormalsVal);
	gui->NormalHueSlider->value(ProcessControl.NormalsHue);
	gui->EndpointSatSlider->value(ProcessControl.EndpointsSat);
	gui->EndpointValSlider->value(ProcessControl.EndpointsVal);
	gui->EndpointHueSlider->value(ProcessControl.EndpointsHue);
	gui->GCodeExtrudeHueSlider->value(ProcessControl.GCodeExtrudeHue);
	gui->GCodeExtrudeSatSlider->value(ProcessControl.GCodeExtrudeSat);
	gui->GCodeExtrudeValSlider->value(ProcessControl.GCodeExtrudeVal);
	gui->GCodeMoveHueSlider->value(ProcessControl.GCodeMoveHue);
	gui->GCodeMoveSatSlider->value(ProcessControl.GCodeMoveSat);
	gui->GCodeMoveValSlider->value(ProcessControl.GCodeMoveVal);
	gui->HighlightSlider->value(ProcessControl.Highlight);

	gui->NormalLengthSlider->value(ProcessControl.NormalsLength);
	gui->EndpointSizeSlider->value(ProcessControl.EndPointSize);

	gui->DisplayGCodeButton->value(ProcessControl.DisplayGCode);
	gui->LuminanceShowsSpeedButton->value(ProcessControl.LuminanceShowsSpeed);

	gui->ApronEnableButton->value(ProcessControl.ApronEnable);
	gui->ApronPreviewButton->value(ProcessControl.ApronPreview);
	gui->ApronSizeSlider->value(ProcessControl.ApronSize);
	gui->ApronHeightSlider->value(ProcessControl.ApronHeight);
	gui->ApronCoverageXSlider->value(ProcessControl.ApronCoverageX);
	gui->ApronCoverageYSlider->value(ProcessControl.ApronCoverageY);
	gui->ApronDistanceToObjectSlider->value(ProcessControl.ApronDistanceToObject);
	gui->ApronInfillDistanceSlider->value(ProcessControl.ApronInfillDistance);

}

void ModelViewController::Continue()
{
	serial.SendNextLine();
}

void ModelViewController::TestSerial()
{
	serial.test();
}

void ModelViewController::Print()
{
	// Snack one line at a time from the Gcode view, and buffer it.

	if(gui->PrintButton->value() == 0)	// Turned off print, cancel buffer and flush
	{
		serial.Clear();
		return;
	}

	gui->CommunationLog->clear();
	Fl_Text_Buffer* buffer = gui->GCodeResult->buffer();
	char* pText = buffer->text();
	UINT length = buffer->length();

	UINT pos = 2;
	while(pos < length)
		{
		char* line = buffer->line_text(pos);
		if(line[0] == ';')
			{
			pos = buffer->line_end(pos)+1;	// skip newline
			continue;
			}
		serial.AddToBuffer(line);
		pos = buffer->line_end(pos)+1;	// find end of line
		}

	gui->ProgressBar->maximum(serial.Length());
	gui->ProgressBar->label("Printing");
	gui->ProgressBar->value(0);

	serial.StartPrint();
}


void ModelViewController::SwitchHeat(bool on, float temp)
{
	if(on)
		serial.SendNow("M104 S64");
	else
		serial.SendNow("M104 S0");
}
void ModelViewController::SetTargetTemp(float temp)
{
}
void ModelViewController::RunExtruder()
{
	serial.SendNow("G1 F3000.0");
	serial.SendNow("G92 E0");	// set extruder zero

	if(m_bExtruderDirection)	// Forwards
		serial.SendNow("G1 E250.0");
	else
		serial.SendNow("G1 E-250.0");
	serial.SendNow("G1 F1500.0");
}
void ModelViewController::SetExtruderDirection(bool reverse)
{
	m_bExtruderDirection = !reverse;
}
