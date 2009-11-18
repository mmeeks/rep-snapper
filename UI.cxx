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

void CubeViewUI::cb_FixSTLerrorsButton_i(Fl_Light_Button*, void*) {
  code->redraw();
}
void CubeViewUI::cb_FixSTLerrorsButton(Fl_Light_Button* o, void* v) {
  ((CubeViewUI*)(o->parent()->user_data()))->cb_FixSTLerrorsButton_i(o,v);
}

void CubeViewUI::cb_AutoRotateButton_i(Fl_Button*, void*) {
  code->OptimizeRotation();
code->CalcBoundingBoxAndZoom(code);
code->redraw();
}
void CubeViewUI::cb_AutoRotateButton(Fl_Button* o, void* v) {
  ((CubeViewUI*)(o->parent()->user_data()))->cb_AutoRotateButton_i(o,v);
}

void CubeViewUI::cb_RotateXButton_i(Fl_Button*, void*) {
  code->RotateObject(1,0,0, M_PI/4);
code->CalcBoundingBoxAndZoom(code);
code->redraw();
}
void CubeViewUI::cb_RotateXButton(Fl_Button* o, void* v) {
  ((CubeViewUI*)(o->parent()->user_data()))->cb_RotateXButton_i(o,v);
}

void CubeViewUI::cb_RotateYButton_i(Fl_Button*, void*) {
  code->RotateObject(0,1,0, M_PI/4);
code->CalcBoundingBoxAndZoom(code);
code->redraw();
}
void CubeViewUI::cb_RotateYButton(Fl_Button* o, void* v) {
  ((CubeViewUI*)(o->parent()->user_data()))->cb_RotateYButton_i(o,v);
}

void CubeViewUI::cb_RotateZButton_i(Fl_Button*, void*) {
  code->RotateObject(0,0,1, M_PI/4);
code->CalcBoundingBoxAndZoom(code);
code->redraw();
}
void CubeViewUI::cb_RotateZButton(Fl_Button* o, void* v) {
  ((CubeViewUI*)(o->parent()->user_data()))->cb_RotateZButton_i(o,v);
}

void CubeViewUI::cb_DrawVertexNumbersButton_i(Fl_Light_Button*, void*) {
  code->redraw();
}
void CubeViewUI::cb_DrawVertexNumbersButton(Fl_Light_Button* o, void* v) {
  ((CubeViewUI*)(o->parent()->user_data()))->cb_DrawVertexNumbersButton_i(o,v);
}

void CubeViewUI::cb_DrawLineNumbersButton_i(Fl_Light_Button*, void*) {
  code->redraw();
}
void CubeViewUI::cb_DrawLineNumbersButton(Fl_Light_Button* o, void* v) {
  ((CubeViewUI*)(o->parent()->user_data()))->cb_DrawLineNumbersButton_i(o,v);
}

void CubeViewUI::cb_OptimizationSlider_i(Fl_Value_Slider*, void*) {
  code->redraw();
}
void CubeViewUI::cb_OptimizationSlider(Fl_Value_Slider* o, void* v) {
  ((CubeViewUI*)(o->parent()->user_data()))->cb_OptimizationSlider_i(o,v);
}

void CubeViewUI::cb_GCodeDrawStartSlider_i(Fl_Value_Slider*, void*) {
  code->redraw();
}
void CubeViewUI::cb_GCodeDrawStartSlider(Fl_Value_Slider* o, void* v) {
  ((CubeViewUI*)(o->parent()->user_data()))->cb_GCodeDrawStartSlider_i(o,v);
}

void CubeViewUI::cb_GCodeDrawEndSlider_i(Fl_Value_Slider*, void*) {
  code->redraw();
}
void CubeViewUI::cb_GCodeDrawEndSlider(Fl_Value_Slider* o, void* v) {
  ((CubeViewUI*)(o->parent()->user_data()))->cb_GCodeDrawEndSlider_i(o,v);
}

CubeViewUI::CubeViewUI() {
  { mainWindow = new Fl_Double_Window(1211, 843, "CubeView");
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
    { Fl_Button* o = new Fl_Button(815, 230, 145, 25, "Load STL");
      o->callback((Fl_Callback*)cb_Load1);
    } // Fl_Button* o
    { DisplayGCodeButton = new Fl_Light_Button(820, 565, 155, 20, "Display GCode");
      DisplayGCodeButton->value(1);
      DisplayGCodeButton->selection_color((Fl_Color)FL_GREEN);
    } // Fl_Light_Button* DisplayGCodeButton
    { DisplaySTLButton = new Fl_Light_Button(820, 595, 155, 20, "Display STL objects");
      DisplaySTLButton->value(1);
      DisplaySTLButton->selection_color((Fl_Color)FL_GREEN);
    } // Fl_Light_Button* DisplaySTLButton
    { CuttingPlaneSlider = new Fl_Value_Slider(820, 500, 375, 20, "CuttingPlane");
      CuttingPlaneSlider->type(1);
      CuttingPlaneSlider->value(0.68);
      CuttingPlaneSlider->textsize(14);
      CuttingPlaneSlider->callback((Fl_Callback*)cb_CuttingPlaneSlider);
      CuttingPlaneSlider->align(FL_ALIGN_TOP_LEFT);
    } // Fl_Value_Slider* CuttingPlaneSlider
    { ExamineSlider = new Fl_Value_Slider(820, 785, 365, 20, "Examine");
      ExamineSlider->type(1);
      ExamineSlider->step(0.001);
      ExamineSlider->value(0.098);
      ExamineSlider->textsize(14);
      ExamineSlider->callback((Fl_Callback*)cb_ExamineSlider);
      ExamineSlider->align(FL_ALIGN_TOP_LEFT);
    } // Fl_Value_Slider* ExamineSlider
    { DisplayPolygonsButton = new Fl_Light_Button(820, 620, 155, 20, "Display Polygons");
      DisplayPolygonsButton->selection_color((Fl_Color)FL_GREEN);
      DisplayPolygonsButton->callback((Fl_Callback*)cb_DisplayPolygonsButton);
    } // Fl_Light_Button* DisplayPolygonsButton
    { DisplayWireframeButton = new Fl_Light_Button(820, 645, 155, 20, "Display Wireframe");
      DisplayWireframeButton->selection_color((Fl_Color)FL_GREEN);
      DisplayWireframeButton->callback((Fl_Callback*)cb_DisplayWireframeButton);
    } // Fl_Light_Button* DisplayWireframeButton
    { DisplayNormalsButton = new Fl_Light_Button(980, 645, 155, 20, "Display Normals");
      DisplayNormalsButton->selection_color((Fl_Color)FL_GREEN);
      DisplayNormalsButton->callback((Fl_Callback*)cb_DisplayNormalsButton);
    } // Fl_Light_Button* DisplayNormalsButton
    { DisplayEndpointsButton = new Fl_Light_Button(980, 670, 155, 20, "Display Endpoints");
      DisplayEndpointsButton->selection_color((Fl_Color)FL_GREEN);
      DisplayEndpointsButton->callback((Fl_Callback*)cb_DisplayEndpointsButton);
    } // Fl_Light_Button* DisplayEndpointsButton
    { DisplayCuttingPlaneButton = new Fl_Light_Button(820, 670, 155, 20, "Display CuttingPlane");
      DisplayCuttingPlaneButton->value(1);
      DisplayCuttingPlaneButton->selection_color((Fl_Color)FL_GREEN);
      DisplayCuttingPlaneButton->callback((Fl_Callback*)cb_DisplayCuttingPlaneButton);
    } // Fl_Light_Button* DisplayCuttingPlaneButton
    { DisplayinFillButton = new Fl_Light_Button(820, 695, 155, 20, "Display inFill");
      DisplayinFillButton->value(1);
      DisplayinFillButton->selection_color((Fl_Color)FL_GREEN);
      DisplayinFillButton->callback((Fl_Callback*)cb_DisplayinFillButton);
    } // Fl_Light_Button* DisplayinFillButton
    { RotationSlider = new Fl_Value_Slider(815, 275, 370, 20, "Rotation");
      RotationSlider->type(1);
      RotationSlider->maximum(360);
      RotationSlider->step(1);
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
    { InfillRotationPrLayerSlider = new Fl_Value_Slider(815, 315, 370, 20, "InFill Rotation pr. Layer");
      InfillRotationPrLayerSlider->type(1);
      InfillRotationPrLayerSlider->maximum(360);
      InfillRotationPrLayerSlider->step(1);
      InfillRotationPrLayerSlider->value(90);
      InfillRotationPrLayerSlider->textsize(14);
      InfillRotationPrLayerSlider->callback((Fl_Callback*)cb_InfillRotationPrLayerSlider);
      InfillRotationPrLayerSlider->align(FL_ALIGN_TOP_LEFT);
    } // Fl_Value_Slider* InfillRotationPrLayerSlider
    { InfillDistanceSlider = new Fl_Value_Slider(815, 355, 370, 20, "Infill Distance");
      InfillDistanceSlider->type(1);
      InfillDistanceSlider->minimum(0.1);
      InfillDistanceSlider->maximum(10);
      InfillDistanceSlider->step(0.1);
      InfillDistanceSlider->value(2);
      InfillDistanceSlider->textsize(14);
      InfillDistanceSlider->callback((Fl_Callback*)cb_InfillDistanceSlider);
      InfillDistanceSlider->align(FL_ALIGN_TOP_LEFT);
    } // Fl_Value_Slider* InfillDistanceSlider
    { LayerThicknessSlider = new Fl_Value_Slider(815, 395, 370, 20, "Layer Thickness");
      LayerThicknessSlider->type(1);
      LayerThicknessSlider->minimum(0.1);
      LayerThicknessSlider->maximum(3);
      LayerThicknessSlider->step(0.1);
      LayerThicknessSlider->value(0.5);
      LayerThicknessSlider->textsize(14);
      LayerThicknessSlider->callback((Fl_Callback*)cb_LayerThicknessSlider);
      LayerThicknessSlider->align(FL_ALIGN_TOP_LEFT);
    } // Fl_Value_Slider* LayerThicknessSlider
    { DisplayAllLayers = new Fl_Light_Button(820, 720, 155, 20, "Display all layers");
      DisplayAllLayers->selection_color((Fl_Color)FL_GREEN);
      DisplayAllLayers->callback((Fl_Callback*)cb_DisplayAllLayers);
    } // Fl_Light_Button* DisplayAllLayers
    { ShrinkSlider = new Fl_Value_Slider(820, 540, 375, 20, "Shrink");
      ShrinkSlider->type(1);
      ShrinkSlider->step(0);
      ShrinkSlider->value(0.5);
      ShrinkSlider->textsize(14);
      ShrinkSlider->callback((Fl_Callback*)cb_ShrinkSlider);
      ShrinkSlider->align(FL_ALIGN_TOP_LEFT);
    } // Fl_Value_Slider* ShrinkSlider
    { FixSTLerrorsButton = new Fl_Light_Button(965, 230, 135, 25, "Fix STL errors");
      FixSTLerrorsButton->value(1);
      FixSTLerrorsButton->selection_color((Fl_Color)FL_GREEN);
      FixSTLerrorsButton->callback((Fl_Callback*)cb_FixSTLerrorsButton);
    } // Fl_Light_Button* FixSTLerrorsButton
    { AutoRotateButton = new Fl_Button(815, 460, 90, 20, "Auto rotate");
      AutoRotateButton->callback((Fl_Callback*)cb_AutoRotateButton);
    } // Fl_Button* AutoRotateButton
    { RotateXButton = new Fl_Button(910, 460, 90, 20, "Rotate X");
      RotateXButton->callback((Fl_Callback*)cb_RotateXButton);
    } // Fl_Button* RotateXButton
    { RotateYButton = new Fl_Button(1005, 460, 90, 20, "Rotate Y");
      RotateYButton->callback((Fl_Callback*)cb_RotateYButton);
    } // Fl_Button* RotateYButton
    { RotateZButton = new Fl_Button(1100, 460, 90, 20, "Rotate Z");
      RotateZButton->callback((Fl_Callback*)cb_RotateZButton);
    } // Fl_Button* RotateZButton
    { GCodeLengthText = new Fl_Output(960, 6, 225, 24);
    } // Fl_Output* GCodeLengthText
    { DrawVertexNumbersButton = new Fl_Light_Button(980, 695, 155, 20, "Draw vertex numbers");
      DrawVertexNumbersButton->selection_color((Fl_Color)FL_GREEN);
      DrawVertexNumbersButton->callback((Fl_Callback*)cb_DrawVertexNumbersButton);
    } // Fl_Light_Button* DrawVertexNumbersButton
    { DrawLineNumbersButton = new Fl_Light_Button(980, 720, 155, 20, "Draw line numbers");
      DrawLineNumbersButton->selection_color((Fl_Color)FL_GREEN);
      DrawLineNumbersButton->callback((Fl_Callback*)cb_DrawLineNumbersButton);
    } // Fl_Light_Button* DrawLineNumbersButton
    { OptimizationSlider = new Fl_Value_Slider(815, 435, 370, 20, "Optimize");
      OptimizationSlider->type(1);
      OptimizationSlider->step(0.001);
      OptimizationSlider->value(0.001);
      OptimizationSlider->textsize(14);
      OptimizationSlider->callback((Fl_Callback*)cb_OptimizationSlider);
      OptimizationSlider->align(FL_ALIGN_TOP_LEFT);
    } // Fl_Value_Slider* OptimizationSlider
    { GCodeDrawStartSlider = new Fl_Value_Slider(810, 55, 370, 20, "GCode draw start");
      GCodeDrawStartSlider->type(1);
      GCodeDrawStartSlider->step(0);
      GCodeDrawStartSlider->textsize(14);
      GCodeDrawStartSlider->callback((Fl_Callback*)cb_GCodeDrawStartSlider);
      GCodeDrawStartSlider->align(FL_ALIGN_TOP_LEFT);
    } // Fl_Value_Slider* GCodeDrawStartSlider
    { GCodeDrawEndSlider = new Fl_Value_Slider(810, 90, 370, 20, "GCode draw end");
      GCodeDrawEndSlider->type(1);
      GCodeDrawEndSlider->step(0);
      GCodeDrawEndSlider->value(1);
      GCodeDrawEndSlider->textsize(14);
      GCodeDrawEndSlider->callback((Fl_Callback*)cb_GCodeDrawEndSlider);
      GCodeDrawEndSlider->align(FL_ALIGN_TOP_LEFT);
    } // Fl_Value_Slider* GCodeDrawEndSlider
    mainWindow->end();
    mainWindow->resizable(mainWindow);
  } // Fl_Double_Window* mainWindow
}

void CubeViewUI::show(int argc, char **argv) {
  mainWindow->show(argc, argv);
//code->ReadStl("C:/box.stl");
code->ReadStl("C:/code/printed-parts/y-bearing-180-outer-right_1off.stl");
//code->ReadStl("C:/code/printed-parts/frame-vertex_6off.stl");
code->redraw();
}
