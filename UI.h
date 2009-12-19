// generated by Fast Light User Interface Designer (fluid) version 1.0110

#ifndef UI_h
#define UI_h
#include <FL/Fl.H>
class ModelViewController;
#include <FL/Fl_Double_Window.H>
#include "StdAfx.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Text_Editor.H>

class GUI {
public:
  GUI();
  Fl_Double_Window *mainWindow;
  ModelViewController *MVC;
  Fl_Tabs *Tabs;
private:
  void cb_Load_i(Fl_Button*, void*);
  static void cb_Load(Fl_Button*, void*);
public:
  Fl_Light_Button *FixSTLerrorsButton;
private:
  void cb_FixSTLerrorsButton_i(Fl_Light_Button*, void*);
  static void cb_FixSTLerrorsButton(Fl_Light_Button*, void*);
  void cb_Save_i(Fl_Button*, void*);
  static void cb_Save(Fl_Button*, void*);
  void cb_Scale_i(Fl_Button*, void*);
  static void cb_Scale(Fl_Button*, void*);
public:
  Fl_Button *AutoRotateButton;
private:
  void cb_AutoRotateButton_i(Fl_Button*, void*);
  static void cb_AutoRotateButton(Fl_Button*, void*);
public:
  Fl_Button *RotateXButton;
private:
  void cb_RotateXButton_i(Fl_Button*, void*);
  static void cb_RotateXButton(Fl_Button*, void*);
public:
  Fl_Button *RotateYButton;
private:
  void cb_RotateYButton_i(Fl_Button*, void*);
  static void cb_RotateYButton(Fl_Button*, void*);
public:
  Fl_Button *RotateZButton;
private:
  void cb_RotateZButton_i(Fl_Button*, void*);
  static void cb_RotateZButton(Fl_Button*, void*);
public:
  Fl_Value_Slider *PolygonOpasitySlider;
private:
  void cb_PolygonOpasitySlider_i(Fl_Value_Slider*, void*);
  static void cb_PolygonOpasitySlider(Fl_Value_Slider*, void*);
public:
  Fl_Light_Button *DisplayPolygonsButton;
private:
  void cb_DisplayPolygonsButton_i(Fl_Light_Button*, void*);
  static void cb_DisplayPolygonsButton(Fl_Light_Button*, void*);
public:
  Fl_Light_Button *DisplayWireframeButton;
private:
  void cb_DisplayWireframeButton_i(Fl_Light_Button*, void*);
  static void cb_DisplayWireframeButton(Fl_Light_Button*, void*);
public:
  Fl_Light_Button *DisplayNormalsButton;
private:
  void cb_DisplayNormalsButton_i(Fl_Light_Button*, void*);
  static void cb_DisplayNormalsButton(Fl_Light_Button*, void*);
public:
  Fl_Light_Button *DisplayEndpointsButton;
private:
  void cb_DisplayEndpointsButton_i(Fl_Light_Button*, void*);
  static void cb_DisplayEndpointsButton(Fl_Light_Button*, void*);
public:
  Fl_Value_Slider *NumAccelerationStepsSlider;
private:
  void cb_NumAccelerationStepsSlider_i(Fl_Value_Slider*, void*);
  static void cb_NumAccelerationStepsSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *DistancePrAccelerationStep;
private:
  void cb_DistancePrAccelerationStep_i(Fl_Value_Slider*, void*);
  static void cb_DistancePrAccelerationStep(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *MaxPrintSpeedXYSlider;
private:
  void cb_MaxPrintSpeedXYSlider_i(Fl_Value_Slider*, void*);
  static void cb_MaxPrintSpeedXYSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *MinPrintSpeedZSlider;
private:
  void cb_MinPrintSpeedZSlider_i(Fl_Value_Slider*, void*);
  static void cb_MinPrintSpeedZSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *MinPrintSpeedXYSlider;
private:
  void cb_MinPrintSpeedXYSlider_i(Fl_Value_Slider*, void*);
  static void cb_MinPrintSpeedXYSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *MaxPrintSpeedZSlider;
private:
  void cb_MaxPrintSpeedZSlider_i(Fl_Value_Slider*, void*);
  static void cb_MaxPrintSpeedZSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *ShrinkSlider;
private:
  void cb_ShrinkSlider_i(Fl_Value_Slider*, void*);
  static void cb_ShrinkSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *PlaceholderSlider;
private:
  void cb_PlaceholderSlider_i(Fl_Value_Slider*, void*);
  static void cb_PlaceholderSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *RotationSlider;
private:
  void cb_RotationSlider_i(Fl_Value_Slider*, void*);
  static void cb_RotationSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *InfillRotationPrLayerSlider;
private:
  void cb_InfillRotationPrLayerSlider_i(Fl_Value_Slider*, void*);
  static void cb_InfillRotationPrLayerSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *InfillDistanceSlider;
private:
  void cb_InfillDistanceSlider_i(Fl_Value_Slider*, void*);
  static void cb_InfillDistanceSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *LayerThicknessSlider;
private:
  void cb_LayerThicknessSlider_i(Fl_Value_Slider*, void*);
  static void cb_LayerThicknessSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *OptimizationSlider;
private:
  void cb_OptimizationSlider_i(Fl_Value_Slider*, void*);
  static void cb_OptimizationSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *CuttingPlaneSlider;
private:
  void cb_CuttingPlaneSlider_i(Fl_Value_Slider*, void*);
  static void cb_CuttingPlaneSlider(Fl_Value_Slider*, void*);
public:
  Fl_Light_Button *DisplayCuttingPlaneButton;
private:
  void cb_DisplayCuttingPlaneButton_i(Fl_Light_Button*, void*);
  static void cb_DisplayCuttingPlaneButton(Fl_Light_Button*, void*);
public:
  Fl_Light_Button *DisplayinFillButton;
private:
  void cb_DisplayinFillButton_i(Fl_Light_Button*, void*);
  static void cb_DisplayinFillButton(Fl_Light_Button*, void*);
public:
  Fl_Light_Button *DisplayAllLayers;
private:
  void cb_DisplayAllLayers_i(Fl_Light_Button*, void*);
  static void cb_DisplayAllLayers(Fl_Light_Button*, void*);
  void cb_Load1_i(Fl_Button*, void*);
  static void cb_Load1(Fl_Button*, void*);
public:
  Fl_Output *GCodeLengthText;
  Fl_Value_Slider *GCodeDrawStartSlider;
private:
  void cb_GCodeDrawStartSlider_i(Fl_Value_Slider*, void*);
  static void cb_GCodeDrawStartSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *GCodeDrawEndSlider;
private:
  void cb_GCodeDrawEndSlider_i(Fl_Value_Slider*, void*);
  static void cb_GCodeDrawEndSlider(Fl_Value_Slider*, void*);
  void cb_Convert_i(Fl_Button*, void*);
  static void cb_Convert(Fl_Button*, void*);
public:
  Fl_Text_Editor *GCodeStart;
  Fl_Text_Editor *GCodeLayer;
  Fl_Text_Editor *GCodeEnd;
  Fl_Text_Editor *GCodeResult;
private:
  void cb_Save1_i(Fl_Button*, void*);
  static void cb_Save1(Fl_Button*, void*);
public:
  Fl_Value_Slider *ExamineSlider;
private:
  void cb_ExamineSlider_i(Fl_Value_Slider*, void*);
  static void cb_ExamineSlider(Fl_Value_Slider*, void*);
public:
  Fl_Light_Button *DisplayDebuginFillButton;
private:
  void cb_DisplayDebuginFillButton_i(Fl_Light_Button*, void*);
  static void cb_DisplayDebuginFillButton(Fl_Light_Button*, void*);
public:
  Fl_Light_Button *DisplayDebugButton;
private:
  void cb_DisplayDebugButton_i(Fl_Light_Button*, void*);
  static void cb_DisplayDebugButton(Fl_Light_Button*, void*);
public:
  Fl_Light_Button *DisplayGCodeButton;
private:
  void cb_DisplayGCodeButton_i(Fl_Light_Button*, void*);
  static void cb_DisplayGCodeButton(Fl_Light_Button*, void*);
public:
  Fl_Light_Button *DisplaySTLButton;
private:
  void cb_DisplaySTLButton_i(Fl_Light_Button*, void*);
  static void cb_DisplaySTLButton(Fl_Light_Button*, void*);
public:
  Fl_Light_Button *DrawVertexNumbersButton;
private:
  void cb_DrawVertexNumbersButton_i(Fl_Light_Button*, void*);
  static void cb_DrawVertexNumbersButton(Fl_Light_Button*, void*);
public:
  Fl_Light_Button *DrawLineNumbersButton;
private:
  void cb_DrawLineNumbersButton_i(Fl_Light_Button*, void*);
  static void cb_DrawLineNumbersButton(Fl_Light_Button*, void*);
public:
  void show(int argc, char **argv);
};
#endif
