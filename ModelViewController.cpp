#include "StdAfx.h"
#include "ModelViewController.h"

ModelViewController::ModelViewController(int x,int y,int w,int h,const char *l) : Fl_Gl_Window(x,y,w,h,l)
{
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
	glViewport ((GLsizei)x, (GLsizei)y, (GLsizei)width, (GLsizei)height);				// Reset The Current Viewport
	glMatrixMode (GL_PROJECTION);										// Select The Projection Matrix
	glLoadIdentity ();													// Reset The Projection Matrix
	gluPerspective (45.0f, (GLfloat)(width)/(GLfloat)(height),			// Calculate The Aspect Ratio Of The Window
					1.0f, 1000000.0f);						// Calculate The Aspect Ratio Of The Window
	glMatrixMode (GL_MODELVIEW);										// Select The Modelview Matrix
	glLoadIdentity ();													// Reset The Modelview Matrix

    ArcBall->setBounds((GLfloat)width, (GLfloat)height);                 //*NEW* Update mouse bounds for arcball
    redraw();
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
		gluPerspective (45.0f, w()/h(),1.0f, 1000000.0f);						// Calculate The Aspect Ratio Of The Window
		glMatrixMode (GL_MODELVIEW);										// Select The Modelview Matrix
		glLoadIdentity ();													// Reset The Modelview Matrix
//		return;
		}

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
		default:
			// pass other events to the base class...
			return Fl_Gl_Window::handle(event);
	}
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
	GcodeTxt = GcodeTxt + "\0";
	int length2 = GcodeTxt.length();
	buffer->append( GcodeTxt.c_str() );
	redraw();
}

void ModelViewController::init()
{
	ProcessControl.GCodeStartText = "; GCode generated by RepSnapper by Kulitorum\nG21                              ;metric is good!\nG90                              ;absolute positioning\nT0                                 ;select new extruder\nG28                               ;go home\nG92 E0                          ;set extruder home\nM104 S80.0                   ;set temperature;\n";
	ProcessControl.GCodeLayerText = "M106                            ;cooler on \nG1 X-250 F2000.0       ;horizontal move\nG1 X-249.9                  ;horizontal move\nG1 X-250.0 F50.0        ;horizontal move\nG92 X0                         ;set x 0\nG1 Y-250 F2000.0       ;horizontal move\nG1 Y-249.9                  ;horizontal move\nG1 Y-250.0 F50.0        ;horizontal move\nG92 Y0                         ;set y 0\nG1 X20 E20 F1000       ;Shield\nG1 X0 E20 F1000         ;Shield\nT0                                 ;select new extruder\nG92 E0                         ;zero the extruded length\n";
	ProcessControl.GCodeEndText = "M107                            ;cooler off\nG1 X0 Y0 F2000.0       ;feed for start of next move\nG1 Z50.0  F50              ;z move\nM104 S0.0                    ;set temperature\n";

	Fl_Text_Buffer* buffer = gui->GCodeStart->buffer();
	buffer->append(ProcessControl.GCodeStartText.c_str());
	buffer = gui->GCodeLayer->buffer();
	buffer->append(ProcessControl.GCodeLayerText.c_str());
	buffer = gui->GCodeEnd->buffer();
	buffer->append(ProcessControl.GCodeEndText.c_str());
}

//Make the remaining buttons work
//implement acceleration











