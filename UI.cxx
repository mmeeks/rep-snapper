// generated by Fast Light User Interface Designer (fluid) version 1.0110

#include "UI.h"

void CubeViewUI::cb_Load_i(Fl_Button*, void*) {
  Fl_File_Chooser chooser("\\", "*.gcode", Fl_File_Chooser::SINGLE, "Choose GCode");
chooser.show();
while (chooser.shown())
	Fl::wait();
if(chooser.value() == 0)
	return;
std::string dir(chooser.value());


if(dir.length())
{
code->Read(dir);
code->redraw();
};
}
void CubeViewUI::cb_Load(Fl_Button* o, void* v) {
  ((CubeViewUI*)(o->parent()->user_data()))->cb_Load_i(o,v);
}

void CubeViewUI::cb_Load1_i(Fl_Button*, void*) {
  Fl_File_Chooser chooser("C:/code/printed-parts", "*.stl", Fl_File_Chooser::SINGLE, "Choose GCode");
chooser.show();
while (chooser.shown())
	Fl::wait();
if(chooser.value() == 0)
	return;
std::string dir(chooser.value());


if(dir.length())
{
code->ReadStl(dir);
code->redraw();
};
}
void CubeViewUI::cb_Load1(Fl_Button* o, void* v) {
  ((CubeViewUI*)(o->parent()->user_data()))->cb_Load1_i(o,v);
}

void CubeViewUI::cb_CuttingPlaneSlider_i(Fl_Value_Slider*, void*) {
  code->redraw();
}
void CubeViewUI::cb_CuttingPlaneSlider(Fl_Value_Slider* o, void* v) {
  ((CubeViewUI*)(o->parent()->user_data()))->cb_CuttingPlaneSlider_i(o,v);
}

void CubeViewUI::cb_ExamineSlider_i(Fl_Value_Slider*, void*) {
  code->redraw();
}
void CubeViewUI::cb_ExamineSlider(Fl_Value_Slider* o, void* v) {
  ((CubeViewUI*)(o->parent()->user_data()))->cb_ExamineSlider_i(o,v);
}

void CubeViewUI::cb_DisplayPolygonsButton_i(Fl_Light_Button*, void*) {
  code->redraw();
}
void CubeViewUI::cb_DisplayPolygonsButton(Fl_Light_Button* o, void* v) {
  ((CubeViewUI*)(o->parent()->user_data()))->cb_DisplayPolygonsButton_i(o,v);
}

void CubeViewUI::cb_DisplayWireframeButton_i(Fl_Light_Button*, void*) {
  code->redraw();
}
void CubeViewUI::cb_DisplayWireframeButton(Fl_Light_Button* o, void* v) {
  ((CubeViewUI*)(o->parent()->user_data()))->cb_DisplayWireframeButton_i(o,v);
}

void CubeViewUI::cb_DisplayNormalsButton_i(Fl_Light_Button*, void*) {
  code->redraw();
}
void CubeViewUI::cb_DisplayNormalsButton(Fl_Light_Button* o, void* v) {
  ((CubeViewUI*)(o->parent()->user_data()))->cb_DisplayNormalsButton_i(o,v);
}

void CubeViewUI::cb_DisplayEndpointsButton_i(Fl_Light_Button*, void*) {
  code->redraw();
}
void CubeViewUI::cb_DisplayEndpointsButton(Fl_Light_Button* o, void* v) {
  ((CubeViewUI*)(o->parent()->user_data()))->cb_DisplayEndpointsButton_i(o,v);
}

void CubeViewUI::cb_DisplayCuttingPlaneButton_i(Fl_Light_Button*, void*) {
  code->redraw();
}
void CubeViewUI::cb_DisplayCuttingPlaneButton(Fl_Light_Button* o, void* v) {
  ((CubeViewUI*)(o->parent()->user_data()))->cb_DisplayCuttingPlaneButton_i(o,v);
}

void CubeViewUI::cb_DisplayinFillButton_i(Fl_Light_Button*, void*) {
  code->redraw();
}
void CubeViewUI::cb_DisplayinFillButton(Fl_Light_Button* o, void* v) {
  ((CubeViewUI*)(o->parent()->user_data()))->cb_DisplayinFillButton_i(o,v);
}

void CubeViewUI::cb_RotationSlider_i(Fl_Value_Slider*, void*) {
  code->redraw();
}
void CubeViewUI::cb_RotationSlider(Fl_Value_Slider* o, void* v) {
  ((CubeViewUI*)(o->parent()->user_data()))->cb_RotationSlider_i(o,v);
}

void CubeViewUI::cb_DisplayDebuginFillButton_i(Fl_Light_Button*, void*) {
  code->redraw();
}
void CubeViewUI::cb_DisplayDebuginFillButton(Fl_Light_Button* o, void* v) {
  ((CubeViewUI*)(o->parent()->user_data()))->cb_DisplayDebuginFillButton_i(o,v);
}

void CubeViewUI::cb_DisplayDebugButton_i(Fl_Light_Button* o, void*) {
  if(o->value())
{
DisplayDebuginFillButton->show();
ExamineSlider->show();
}
else
{
DisplayDebuginFillButton->hide();
ExamineSlider->hide();
}


code->redraw();
}
void CubeViewUI::cb_DisplayDebugButton(Fl_Light_Button* o, void* v) {
  ((CubeViewUI*)(o->parent()->user_data()))->cb_DisplayDebugButton_i(o,v);
}

void CubeViewUI::cb_InfillRotationPrLayerSlider_i(Fl_Value_Slider*, void*) {
  code->redraw();
}
void CubeViewUI::cb_InfillRotationPrLayerSlider(Fl_Value_Slider* o, void* v) {
  ((CubeViewUI*)(o->parent()->user_data()))->cb_InfillRotationPrLayerSlider_i(o,v);
}

void CubeViewUI::cb_InfillDistanceSlider_i(Fl_Value_Slider*, void*) {
  code->redraw();
}
void CubeViewUI::cb_InfillDistanceSlider(Fl_Value_Slider* o, void* v) {
  ((CubeViewUI*)(o->parent()->user_data()))->cb_InfillDistanceSlider_i(o,v);
}

void CubeViewUI::cb_LayerThicknessSlider_i(Fl_Value_Slider*, void*) {
  code->redraw();
}
void CubeViewUI::cb_LayerThicknessSlider(Fl_Value_Slider* o, void* v) {
  ((CubeViewUI*)(o->parent()->user_data()))->cb_LayerThicknessSlider_i(o,v);
}

void CubeViewUI::cb_DisplayAllLayers_i(Fl_Light_Button*, void*) {
  code->redraw();
}
void CubeViewUI::cb_DisplayAllLayers(Fl_Light_Button* o, void* v) {
  ((CubeViewUI*)(o->parent()->user_data()))->cb_DisplayAllLayers_i(o,v);
}

void CubeViewUI::cb_ShrinkSlider_i(Fl_Value_Slider*, void*) {
  code->redraw();
}
void CubeViewUI::cb_ShrinkSlider(Fl_Value_Slider* o, void* v) {
  ((CubeViewUI*)(o->parent()->user_data()))->cb_ShrinkSlider_i(o,v);
}

CubeViewUI::CubeViewUI() {
  { mainWindow = new Fl_Double_Window(1267, 843, "CubeView");
    mainWindow->box(FL_UP_BOX);
    mainWindow->labelsize(12);
    mainWindow->user_data((void*)(this));
    { code = new GCode(5, 6, 800, 800, "This is the cube_view");
      code->box(FL_UP_BOX);
      code->color((Fl_Color)FL_BACKGROUND_COLOR);
      code->selection_color((Fl_Color)FL_BACKGROUND_COLOR);
      code->labeltype(FL_NORMAL_LABEL);
      code->labelfont(0);
      code->labelsize(14);
      code->labelcolor((Fl_Color)FL_FOREGROUND_COLOR);
      code->align(FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
      code->when(FL_WHEN_RELEASE);
    } // GCode* code
    { Fl_Button* o = new Fl_Button(810, 5, 145, 25, "Load Gcode");
      o->callback((Fl_Callback*)cb_Load);
    } // Fl_Button* o
    { Fl_Button* o = new Fl_Button(810, 40, 145, 25, "Load STL");
      o->callback((Fl_Callback*)cb_Load1);
    } // Fl_Button* o
    { DisplayGCodeButton = new Fl_Light_Button(820, 430, 190, 20, "Display GCode");
      DisplayGCodeButton->value(1);
      DisplayGCodeButton->selection_color((Fl_Color)FL_GREEN);
    } // Fl_Light_Button* DisplayGCodeButton
    { DisplaySTLButton = new Fl_Light_Button(820, 460, 190, 20, "Display STL objects");
      DisplaySTLButton->value(1);
      DisplaySTLButton->selection_color((Fl_Color)FL_GREEN);
    } // Fl_Light_Button* DisplaySTLButton
    { CuttingPlaneSlider = new Fl_Value_Slider(820, 360, 370, 20, "CuttingPlane");
      CuttingPlaneSlider->type(1);
      CuttingPlaneSlider->value(0.5);
      CuttingPlaneSlider->textsize(14);
      CuttingPlaneSlider->callback((Fl_Callback*)cb_CuttingPlaneSlider);
      CuttingPlaneSlider->align(FL_ALIGN_TOP_LEFT);
    } // Fl_Value_Slider* CuttingPlaneSlider
    { ExamineSlider = new Fl_Value_Slider(820, 785, 370, 20, "Examine");
      ExamineSlider->type(1);
      ExamineSlider->value(0.24);
      ExamineSlider->textsize(14);
      ExamineSlider->callback((Fl_Callback*)cb_ExamineSlider);
      ExamineSlider->align(FL_ALIGN_TOP_LEFT);
    } // Fl_Value_Slider* ExamineSlider
    { DisplayPolygonsButton = new Fl_Light_Button(820, 505, 190, 20, "Display Polygons");
      DisplayPolygonsButton->selection_color((Fl_Color)FL_GREEN);
      DisplayPolygonsButton->callback((Fl_Callback*)cb_DisplayPolygonsButton);
    } // Fl_Light_Button* DisplayPolygonsButton
    { DisplayWireframeButton = new Fl_Light_Button(820, 530, 190, 20, "Display Wireframe");
      DisplayWireframeButton->selection_color((Fl_Color)FL_GREEN);
      DisplayWireframeButton->callback((Fl_Callback*)cb_DisplayWireframeButton);
    } // Fl_Light_Button* DisplayWireframeButton
    { DisplayNormalsButton = new Fl_Light_Button(820, 555, 190, 20, "Display Normals");
      DisplayNormalsButton->selection_color((Fl_Color)FL_GREEN);
      DisplayNormalsButton->callback((Fl_Callback*)cb_DisplayNormalsButton);
    } // Fl_Light_Button* DisplayNormalsButton
    { DisplayEndpointsButton = new Fl_Light_Button(820, 580, 190, 20, "Display Endpoints");
      DisplayEndpointsButton->value(1);
      DisplayEndpointsButton->selection_color((Fl_Color)FL_GREEN);
      DisplayEndpointsButton->callback((Fl_Callback*)cb_DisplayEndpointsButton);
    } // Fl_Light_Button* DisplayEndpointsButton
    { DisplayCuttingPlaneButton = new Fl_Light_Button(820, 605, 190, 20, "Display CuttingPlane");
      DisplayCuttingPlaneButton->value(1);
      DisplayCuttingPlaneButton->selection_color((Fl_Color)FL_GREEN);
      DisplayCuttingPlaneButton->callback((Fl_Callback*)cb_DisplayCuttingPlaneButton);
    } // Fl_Light_Button* DisplayCuttingPlaneButton
    { DisplayinFillButton = new Fl_Light_Button(820, 630, 190, 20, "Display inFill");
      DisplayinFillButton->value(1);
      DisplayinFillButton->selection_color((Fl_Color)FL_GREEN);
      DisplayinFillButton->callback((Fl_Callback*)cb_DisplayinFillButton);
    } // Fl_Light_Button* DisplayinFillButton
    { RotationSlider = new Fl_Value_Slider(810, 90, 370, 20, "Rotation");
      RotationSlider->type(1);
      RotationSlider->maximum(360);
      RotationSlider->value(45);
      RotationSlider->textsize(14);
      RotationSlider->callback((Fl_Callback*)cb_RotationSlider);
      RotationSlider->align(FL_ALIGN_TOP_LEFT);
    } // Fl_Value_Slider* RotationSlider
    { DisplayDebuginFillButton = new Fl_Light_Button(1015, 745, 170, 20, "Display Debug inFill");
      DisplayDebuginFillButton->selection_color((Fl_Color)FL_GREEN);
      DisplayDebuginFillButton->callback((Fl_Callback*)cb_DisplayDebuginFillButton);
    } // Fl_Light_Button* DisplayDebuginFillButton
    { DisplayDebugButton = new Fl_Light_Button(820, 745, 190, 20, "Debug");
      DisplayDebugButton->selection_color((Fl_Color)FL_GREEN);
      DisplayDebugButton->callback((Fl_Callback*)cb_DisplayDebugButton);
    } // Fl_Light_Button* DisplayDebugButton
    { InfillRotationPrLayerSlider = new Fl_Value_Slider(810, 130, 370, 20, "InFill Rotation pr. Layer");
      InfillRotationPrLayerSlider->type(1);
      InfillRotationPrLayerSlider->maximum(360);
      InfillRotationPrLayerSlider->value(60);
      InfillRotationPrLayerSlider->textsize(14);
      InfillRotationPrLayerSlider->callback((Fl_Callback*)cb_InfillRotationPrLayerSlider);
      InfillRotationPrLayerSlider->align(FL_ALIGN_TOP_LEFT);
    } // Fl_Value_Slider* InfillRotationPrLayerSlider
    { InfillDistanceSlider = new Fl_Value_Slider(810, 170, 370, 20, "Infill Distance");
      InfillDistanceSlider->type(1);
      InfillDistanceSlider->minimum(0.1);
      InfillDistanceSlider->maximum(10);
      InfillDistanceSlider->value(2);
      InfillDistanceSlider->textsize(14);
      InfillDistanceSlider->callback((Fl_Callback*)cb_InfillDistanceSlider);
      InfillDistanceSlider->align(FL_ALIGN_TOP_LEFT);
    } // Fl_Value_Slider* InfillDistanceSlider
    { LayerThicknessSlider = new Fl_Value_Slider(810, 210, 370, 20, "Layer Thickness");
      LayerThicknessSlider->type(1);
      LayerThicknessSlider->maximum(3);
      LayerThicknessSlider->value(0.5);
      LayerThicknessSlider->textsize(14);
      LayerThicknessSlider->callback((Fl_Callback*)cb_LayerThicknessSlider);
      LayerThicknessSlider->align(FL_ALIGN_TOP_LEFT);
    } // Fl_Value_Slider* LayerThicknessSlider
    { DisplayAllLayers = new Fl_Light_Button(820, 655, 190, 20, "Display all layers");
      DisplayAllLayers->selection_color((Fl_Color)FL_GREEN);
      DisplayAllLayers->callback((Fl_Callback*)cb_DisplayAllLayers);
    } // Fl_Light_Button* DisplayAllLayers
    { ShrinkSlider = new Fl_Value_Slider(820, 400, 370, 20, "Shrink");
      ShrinkSlider->type(1);
      ShrinkSlider->value(0.5);
      ShrinkSlider->textsize(14);
      ShrinkSlider->callback((Fl_Callback*)cb_ShrinkSlider);
      ShrinkSlider->align(FL_ALIGN_TOP_LEFT);
    } // Fl_Value_Slider* ShrinkSlider
    mainWindow->end();
    mainWindow->resizable(mainWindow);
  } // Fl_Double_Window* mainWindow
}

void CubeViewUI::show(int argc, char **argv) {
  mainWindow->show(argc, argv);
code->ReadStl("C:/bendbox.stl");
}
