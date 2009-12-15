// generated by Fast Light User Interface Designer (fluid) version 1.0110

#include "UI.h"

void GUI::cb_Load_i(Fl_Button*, void*) {
  Fl_File_Chooser chooser("C:/code/printed-parts", "*.stl", Fl_File_Chooser::SINGLE, "Choose GCode");
chooser.show();
while (chooser.shown())
	Fl::wait();
if(chooser.value() == 0)
	return;
std::string dir(chooser.value());


if(dir.length())
{
MVC->ReadStl(dir);
MVC->redraw();
};
}
void GUI::cb_Load(Fl_Button* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_Load_i(o,v);
}

void GUI::cb_FixSTLerrorsButton_i(Fl_Light_Button*, void*) {
  MVC->redraw();
}
void GUI::cb_FixSTLerrorsButton(Fl_Light_Button* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_FixSTLerrorsButton_i(o,v);
}

void GUI::cb_AutoRotateButton_i(Fl_Button*, void*) {
  MVC->OptimizeRotation();
MVC->CalcBoundingBoxAndZoom();
MVC->redraw();
}
void GUI::cb_AutoRotateButton(Fl_Button* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_AutoRotateButton_i(o,v);
}

void GUI::cb_RotateXButton_i(Fl_Button*, void*) {
  MVC->RotateObject(1,0,0, M_PI/4);
MVC->CalcBoundingBoxAndZoom();
MVC->redraw();
}
void GUI::cb_RotateXButton(Fl_Button* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_RotateXButton_i(o,v);
}

void GUI::cb_RotateYButton_i(Fl_Button*, void*) {
  MVC->RotateObject(0,1,0, M_PI/4);
MVC->CalcBoundingBoxAndZoom();
MVC->redraw();
}
void GUI::cb_RotateYButton(Fl_Button* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_RotateYButton_i(o,v);
}

void GUI::cb_RotateZButton_i(Fl_Button*, void*) {
  MVC->RotateObject(0,0,1, M_PI/4);
MVC->CalcBoundingBoxAndZoom();
MVC->redraw();
}
void GUI::cb_RotateZButton(Fl_Button* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_RotateZButton_i(o,v);
}

void GUI::cb_ShrinkSlider_i(Fl_Value_Slider*, void*) {
  MVC->redraw();
}
void GUI::cb_ShrinkSlider(Fl_Value_Slider* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_ShrinkSlider_i(o,v);
}

void GUI::cb_CuttingPlaneSlider_i(Fl_Value_Slider*, void*) {
  MVC->redraw();
}
void GUI::cb_CuttingPlaneSlider(Fl_Value_Slider* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_CuttingPlaneSlider_i(o,v);
}

void GUI::cb_RotationSlider_i(Fl_Value_Slider*, void*) {
  MVC->redraw();
}
void GUI::cb_RotationSlider(Fl_Value_Slider* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_RotationSlider_i(o,v);
}

void GUI::cb_InfillRotationPrLayerSlider_i(Fl_Value_Slider*, void*) {
  MVC->redraw();
}
void GUI::cb_InfillRotationPrLayerSlider(Fl_Value_Slider* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_InfillRotationPrLayerSlider_i(o,v);
}

void GUI::cb_InfillDistanceSlider_i(Fl_Value_Slider*, void*) {
  MVC->redraw();
}
void GUI::cb_InfillDistanceSlider(Fl_Value_Slider* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_InfillDistanceSlider_i(o,v);
}

void GUI::cb_LayerThicknessSlider_i(Fl_Value_Slider*, void*) {
  MVC->redraw();
}
void GUI::cb_LayerThicknessSlider(Fl_Value_Slider* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_LayerThicknessSlider_i(o,v);
}

void GUI::cb_OptimizationSlider_i(Fl_Value_Slider*, void*) {
  MVC->redraw();
}
void GUI::cb_OptimizationSlider(Fl_Value_Slider* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_OptimizationSlider_i(o,v);
}

void GUI::cb_Load1_i(Fl_Button*, void*) {
  Fl_File_Chooser chooser("\\", "*.gcode", Fl_File_Chooser::SINGLE, "Choose GCode");
chooser.show();
while (chooser.shown())
	Fl::wait();
if(chooser.value() == 0)
	return;
std::string dir(chooser.value());


if(dir.length())
{
MVC->ReadGCode(dir);
MVC->redraw();
};
}
void GUI::cb_Load1(Fl_Button* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_Load1_i(o,v);
}

void GUI::cb_GCodeDrawStartSlider_i(Fl_Value_Slider*, void*) {
  MVC->redraw();
}
void GUI::cb_GCodeDrawStartSlider(Fl_Value_Slider* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_GCodeDrawStartSlider_i(o,v);
}

void GUI::cb_GCodeDrawEndSlider_i(Fl_Value_Slider*, void*) {
  MVC->redraw();
}
void GUI::cb_GCodeDrawEndSlider(Fl_Value_Slider* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_GCodeDrawEndSlider_i(o,v);
}

void GUI::cb_ExamineSlider_i(Fl_Value_Slider*, void*) {
  MVC->redraw();
}
void GUI::cb_ExamineSlider(Fl_Value_Slider* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_ExamineSlider_i(o,v);
}

void GUI::cb_DisplayDebuginFillButton_i(Fl_Light_Button*, void*) {
  MVC->redraw();
}
void GUI::cb_DisplayDebuginFillButton(Fl_Light_Button* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_DisplayDebuginFillButton_i(o,v);
}

void GUI::cb_DisplayDebugButton_i(Fl_Light_Button* o, void*) {
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


MVC->redraw();
}
void GUI::cb_DisplayDebugButton(Fl_Light_Button* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_DisplayDebugButton_i(o,v);
}

void GUI::cb_DisplayPolygonsButton_i(Fl_Light_Button*, void*) {
  MVC->redraw();
}
void GUI::cb_DisplayPolygonsButton(Fl_Light_Button* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_DisplayPolygonsButton_i(o,v);
}

void GUI::cb_DisplayWireframeButton_i(Fl_Light_Button*, void*) {
  MVC->redraw();
}
void GUI::cb_DisplayWireframeButton(Fl_Light_Button* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_DisplayWireframeButton_i(o,v);
}

void GUI::cb_DisplayNormalsButton_i(Fl_Light_Button*, void*) {
  MVC->redraw();
}
void GUI::cb_DisplayNormalsButton(Fl_Light_Button* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_DisplayNormalsButton_i(o,v);
}

void GUI::cb_DisplayEndpointsButton_i(Fl_Light_Button*, void*) {
  MVC->redraw();
}
void GUI::cb_DisplayEndpointsButton(Fl_Light_Button* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_DisplayEndpointsButton_i(o,v);
}

void GUI::cb_DisplayCuttingPlaneButton_i(Fl_Light_Button*, void*) {
  MVC->redraw();
}
void GUI::cb_DisplayCuttingPlaneButton(Fl_Light_Button* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_DisplayCuttingPlaneButton_i(o,v);
}

void GUI::cb_DisplayinFillButton_i(Fl_Light_Button*, void*) {
  MVC->redraw();
}
void GUI::cb_DisplayinFillButton(Fl_Light_Button* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_DisplayinFillButton_i(o,v);
}

void GUI::cb_DisplayAllLayers_i(Fl_Light_Button*, void*) {
  MVC->redraw();
}
void GUI::cb_DisplayAllLayers(Fl_Light_Button* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_DisplayAllLayers_i(o,v);
}

void GUI::cb_DrawVertexNumbersButton_i(Fl_Light_Button*, void*) {
  MVC->redraw();
}
void GUI::cb_DrawVertexNumbersButton(Fl_Light_Button* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_DrawVertexNumbersButton_i(o,v);
}

void GUI::cb_DrawLineNumbersButton_i(Fl_Light_Button*, void*) {
  MVC->redraw();
}
void GUI::cb_DrawLineNumbersButton(Fl_Light_Button* o, void* v) {
  ((GUI*)(o->parent()->parent()->parent()->user_data()))->cb_DrawLineNumbersButton_i(o,v);
}

GUI::GUI() {
  { mainWindow = new Fl_Double_Window(1354, 809, "CubeView");
    mainWindow->box(FL_UP_BOX);
    mainWindow->labelsize(12);
    mainWindow->user_data((void*)(this));
    { MVC = new ModelViewController(5, 5, 800, 800, "An instace of ModelViewController");
      MVC->box(FL_UP_BOX);
      MVC->color((Fl_Color)FL_BACKGROUND_COLOR);
      MVC->selection_color((Fl_Color)FL_BACKGROUND_COLOR);
      MVC->labeltype(FL_NORMAL_LABEL);
      MVC->labelfont(0);
      MVC->labelsize(14);
      MVC->labelcolor((Fl_Color)FL_FOREGROUND_COLOR);
      MVC->align(FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
      MVC->when(FL_WHEN_RELEASE);
    } // ModelViewController* MVC
    { Tabs = new Fl_Tabs(810, 20, 540, 785, "Process:");
      Tabs->align(FL_ALIGN_TOP_LEFT);
      { Fl_Group* o = new Fl_Group(810, 40, 540, 765, "Input file");
        o->hide();
        { Fl_Button* o = new Fl_Button(815, 55, 145, 25, "Load STL");
          o->callback((Fl_Callback*)cb_Load);
        } // Fl_Button* o
        { FixSTLerrorsButton = new Fl_Light_Button(965, 55, 135, 25, "Fix STL errors");
          FixSTLerrorsButton->value(1);
          FixSTLerrorsButton->selection_color((Fl_Color)FL_GREEN);
          FixSTLerrorsButton->callback((Fl_Callback*)cb_FixSTLerrorsButton);
        } // Fl_Light_Button* FixSTLerrorsButton
        { AutoRotateButton = new Fl_Button(815, 85, 90, 20, "Auto rotate");
          AutoRotateButton->callback((Fl_Callback*)cb_AutoRotateButton);
        } // Fl_Button* AutoRotateButton
        { RotateXButton = new Fl_Button(910, 85, 90, 20, "Rotate X");
          RotateXButton->callback((Fl_Callback*)cb_RotateXButton);
        } // Fl_Button* RotateXButton
        { RotateYButton = new Fl_Button(1005, 85, 90, 20, "Rotate Y");
          RotateYButton->callback((Fl_Callback*)cb_RotateYButton);
        } // Fl_Button* RotateYButton
        { RotateZButton = new Fl_Button(1100, 85, 90, 20, "Rotate Z");
          RotateZButton->callback((Fl_Callback*)cb_RotateZButton);
        } // Fl_Button* RotateZButton
        o->end();
      } // Fl_Group* o
      { Fl_Group* o = new Fl_Group(810, 40, 540, 765, "Printer definition");
        o->hide();
        { ShrinkSlider = new Fl_Value_Slider(825, 270, 375, 20, "Extruded material width");
          ShrinkSlider->type(1);
          ShrinkSlider->value(0.7);
          ShrinkSlider->textsize(14);
          ShrinkSlider->callback((Fl_Callback*)cb_ShrinkSlider);
          ShrinkSlider->align(FL_ALIGN_TOP_LEFT);
        } // Fl_Value_Slider* ShrinkSlider
        { Fl_Group* o = new Fl_Group(820, 60, 165, 35, "Build volume (mm)");
          o->box(FL_UP_BOX);
          { new Fl_Value_Input(840, 66, 35, 24, "X");
          } // Fl_Value_Input* o
          { new Fl_Value_Input(895, 66, 35, 24, "Y");
          } // Fl_Value_Input* o
          { new Fl_Value_Input(950, 66, 25, 24, "Z");
          } // Fl_Value_Input* o
          o->end();
        } // Fl_Group* o
        o->end();
      } // Fl_Group* o
      { Fl_Group* o = new Fl_Group(810, 40, 540, 765, "Slicing");
        o->hide();
        { CuttingPlaneSlider = new Fl_Value_Slider(840, 450, 375, 20, "CuttingPlane");
          CuttingPlaneSlider->type(1);
          CuttingPlaneSlider->value(0.9);
          CuttingPlaneSlider->textsize(14);
          CuttingPlaneSlider->callback((Fl_Callback*)cb_CuttingPlaneSlider);
          CuttingPlaneSlider->align(FL_ALIGN_TOP_LEFT);
        } // Fl_Value_Slider* CuttingPlaneSlider
        { RotationSlider = new Fl_Value_Slider(840, 490, 370, 20, "Rotation");
          RotationSlider->type(1);
          RotationSlider->maximum(360);
          RotationSlider->step(1);
          RotationSlider->value(45);
          RotationSlider->textsize(14);
          RotationSlider->callback((Fl_Callback*)cb_RotationSlider);
          RotationSlider->align(FL_ALIGN_TOP_LEFT);
        } // Fl_Value_Slider* RotationSlider
        { InfillRotationPrLayerSlider = new Fl_Value_Slider(840, 530, 370, 20, "InFill Rotation pr. Layer");
          InfillRotationPrLayerSlider->type(1);
          InfillRotationPrLayerSlider->maximum(360);
          InfillRotationPrLayerSlider->step(1);
          InfillRotationPrLayerSlider->value(90);
          InfillRotationPrLayerSlider->textsize(14);
          InfillRotationPrLayerSlider->callback((Fl_Callback*)cb_InfillRotationPrLayerSlider);
          InfillRotationPrLayerSlider->align(FL_ALIGN_TOP_LEFT);
        } // Fl_Value_Slider* InfillRotationPrLayerSlider
        { InfillDistanceSlider = new Fl_Value_Slider(840, 570, 370, 20, "Infill Distance");
          InfillDistanceSlider->type(1);
          InfillDistanceSlider->minimum(0.1);
          InfillDistanceSlider->maximum(10);
          InfillDistanceSlider->step(0.1);
          InfillDistanceSlider->value(2);
          InfillDistanceSlider->textsize(14);
          InfillDistanceSlider->callback((Fl_Callback*)cb_InfillDistanceSlider);
          InfillDistanceSlider->align(FL_ALIGN_TOP_LEFT);
        } // Fl_Value_Slider* InfillDistanceSlider
        { LayerThicknessSlider = new Fl_Value_Slider(840, 610, 370, 20, "Layer Thickness");
          LayerThicknessSlider->type(1);
          LayerThicknessSlider->minimum(0.1);
          LayerThicknessSlider->maximum(3);
          LayerThicknessSlider->step(0.1);
          LayerThicknessSlider->value(0.4);
          LayerThicknessSlider->textsize(14);
          LayerThicknessSlider->callback((Fl_Callback*)cb_LayerThicknessSlider);
          LayerThicknessSlider->align(FL_ALIGN_TOP_LEFT);
        } // Fl_Value_Slider* LayerThicknessSlider
        { OptimizationSlider = new Fl_Value_Slider(840, 650, 370, 20, "Optimize");
          OptimizationSlider->type(1);
          OptimizationSlider->step(0.001);
          OptimizationSlider->value(0.001);
          OptimizationSlider->textsize(14);
          OptimizationSlider->callback((Fl_Callback*)cb_OptimizationSlider);
          OptimizationSlider->align(FL_ALIGN_TOP_LEFT);
        } // Fl_Value_Slider* OptimizationSlider
        o->end();
      } // Fl_Group* o
      { Fl_Group* o = new Fl_Group(810, 40, 540, 765, "GCode");
        { Fl_Button* o = new Fl_Button(815, 220, 145, 25, "Load Gcode");
          o->callback((Fl_Callback*)cb_Load1);
        } // Fl_Button* o
        { GCodeLengthText = new Fl_Output(965, 221, 225, 24);
        } // Fl_Output* GCodeLengthText
        { GCodeDrawStartSlider = new Fl_Value_Slider(815, 270, 370, 20, "GCode draw start");
          GCodeDrawStartSlider->type(1);
          GCodeDrawStartSlider->step(0);
          GCodeDrawStartSlider->textsize(14);
          GCodeDrawStartSlider->callback((Fl_Callback*)cb_GCodeDrawStartSlider);
          GCodeDrawStartSlider->align(FL_ALIGN_TOP_LEFT);
        } // Fl_Value_Slider* GCodeDrawStartSlider
        { GCodeDrawEndSlider = new Fl_Value_Slider(815, 305, 370, 20, "GCode draw end");
          GCodeDrawEndSlider->type(1);
          GCodeDrawEndSlider->step(0);
          GCodeDrawEndSlider->value(1);
          GCodeDrawEndSlider->textsize(14);
          GCodeDrawEndSlider->callback((Fl_Callback*)cb_GCodeDrawEndSlider);
          GCodeDrawEndSlider->align(FL_ALIGN_TOP_LEFT);
        } // Fl_Value_Slider* GCodeDrawEndSlider
        o->end();
      } // Fl_Group* o
      { Fl_Group* o = new Fl_Group(810, 40, 540, 765, "Debug");
        o->color((Fl_Color)FL_DARK1);
        o->labelfont(1);
        o->labelcolor((Fl_Color)1);
        o->hide();
        { ExamineSlider = new Fl_Value_Slider(880, 760, 365, 20, "Examine");
          ExamineSlider->type(1);
          ExamineSlider->step(0.001);
          ExamineSlider->value(0.098);
          ExamineSlider->textsize(14);
          ExamineSlider->callback((Fl_Callback*)cb_ExamineSlider);
          ExamineSlider->align(FL_ALIGN_TOP_LEFT);
        } // Fl_Value_Slider* ExamineSlider
        { DisplayDebuginFillButton = new Fl_Light_Button(1075, 720, 170, 20, "Display Debug inFill");
          DisplayDebuginFillButton->selection_color((Fl_Color)FL_GREEN);
          DisplayDebuginFillButton->callback((Fl_Callback*)cb_DisplayDebuginFillButton);
        } // Fl_Light_Button* DisplayDebuginFillButton
        { DisplayDebugButton = new Fl_Light_Button(880, 720, 190, 20, "Debug");
          DisplayDebugButton->selection_color((Fl_Color)FL_GREEN);
          DisplayDebugButton->callback((Fl_Callback*)cb_DisplayDebugButton);
        } // Fl_Light_Button* DisplayDebugButton
        { DisplayGCodeButton = new Fl_Light_Button(880, 540, 155, 20, "Display GCode");
          DisplayGCodeButton->value(1);
          DisplayGCodeButton->selection_color((Fl_Color)FL_GREEN);
        } // Fl_Light_Button* DisplayGCodeButton
        { DisplaySTLButton = new Fl_Light_Button(880, 570, 155, 20, "Display STL objects");
          DisplaySTLButton->selection_color((Fl_Color)FL_GREEN);
        } // Fl_Light_Button* DisplaySTLButton
        { DisplayPolygonsButton = new Fl_Light_Button(880, 595, 155, 20, "Display Polygons");
          DisplayPolygonsButton->selection_color((Fl_Color)FL_GREEN);
          DisplayPolygonsButton->callback((Fl_Callback*)cb_DisplayPolygonsButton);
        } // Fl_Light_Button* DisplayPolygonsButton
        { DisplayWireframeButton = new Fl_Light_Button(880, 620, 155, 20, "Display Wireframe");
          DisplayWireframeButton->selection_color((Fl_Color)FL_GREEN);
          DisplayWireframeButton->callback((Fl_Callback*)cb_DisplayWireframeButton);
        } // Fl_Light_Button* DisplayWireframeButton
        { DisplayNormalsButton = new Fl_Light_Button(1040, 620, 155, 20, "Display Normals");
          DisplayNormalsButton->selection_color((Fl_Color)FL_GREEN);
          DisplayNormalsButton->callback((Fl_Callback*)cb_DisplayNormalsButton);
        } // Fl_Light_Button* DisplayNormalsButton
        { DisplayEndpointsButton = new Fl_Light_Button(1040, 645, 155, 20, "Display Endpoints");
          DisplayEndpointsButton->selection_color((Fl_Color)FL_GREEN);
          DisplayEndpointsButton->callback((Fl_Callback*)cb_DisplayEndpointsButton);
        } // Fl_Light_Button* DisplayEndpointsButton
        { DisplayCuttingPlaneButton = new Fl_Light_Button(880, 645, 155, 20, "Display CuttingPlane");
          DisplayCuttingPlaneButton->selection_color((Fl_Color)FL_GREEN);
          DisplayCuttingPlaneButton->callback((Fl_Callback*)cb_DisplayCuttingPlaneButton);
        } // Fl_Light_Button* DisplayCuttingPlaneButton
        { DisplayinFillButton = new Fl_Light_Button(880, 670, 155, 20, "Display inFill");
          DisplayinFillButton->selection_color((Fl_Color)FL_GREEN);
          DisplayinFillButton->callback((Fl_Callback*)cb_DisplayinFillButton);
        } // Fl_Light_Button* DisplayinFillButton
        { DisplayAllLayers = new Fl_Light_Button(880, 695, 155, 20, "Display all layers");
          DisplayAllLayers->selection_color((Fl_Color)FL_GREEN);
          DisplayAllLayers->callback((Fl_Callback*)cb_DisplayAllLayers);
        } // Fl_Light_Button* DisplayAllLayers
        { DrawVertexNumbersButton = new Fl_Light_Button(1040, 670, 155, 20, "Draw vertex numbers");
          DrawVertexNumbersButton->selection_color((Fl_Color)FL_GREEN);
          DrawVertexNumbersButton->callback((Fl_Callback*)cb_DrawVertexNumbersButton);
        } // Fl_Light_Button* DrawVertexNumbersButton
        { DrawLineNumbersButton = new Fl_Light_Button(1040, 695, 155, 20, "Draw line numbers");
          DrawLineNumbersButton->selection_color((Fl_Color)FL_GREEN);
          DrawLineNumbersButton->callback((Fl_Callback*)cb_DrawLineNumbersButton);
        } // Fl_Light_Button* DrawLineNumbersButton
        o->end();
      } // Fl_Group* o
      Tabs->end();
    } // Fl_Tabs* Tabs
    mainWindow->end();
    mainWindow->resizable(mainWindow);
  } // Fl_Double_Window* mainWindow
}

void GUI::show(int argc, char **argv) {
  mainWindow->show(argc, argv);
//MVC->ReadStl("C:/box.stl");
MVC->ReadStl("C:/#Downloads/Reprap Exchange/N_DSL-Stylus.stl");
//MVC->ReadStl("C:/code/printed-parts/frame-vertex_6off.stl");
MVC->redraw();
}
