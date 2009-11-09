// generated by Fast Light User Interface Designer (fluid) version 1.0110

#ifndef UI_h
#define UI_h
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include "GCode.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include <FL/Fl_Button.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Output.H>

class CubeViewUI {
public:
  CubeViewUI();
  Fl_Double_Window *mainWindow;
  GCode *code;
private:
  void cb_Load_i(Fl_Button*, void*);
  static void cb_Load(Fl_Button*, void*);
  void cb_Load1_i(Fl_Button*, void*);
  static void cb_Load1(Fl_Button*, void*);
public:
  Fl_Light_Button *DisplayGCodeButton;
  Fl_Light_Button *DisplaySTLButton;
  Fl_Value_Slider *CuttingPlaneSlider;
private:
  void cb_CuttingPlaneSlider_i(Fl_Value_Slider*, void*);
  static void cb_CuttingPlaneSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *ExamineSlider;
private:
  void cb_ExamineSlider_i(Fl_Value_Slider*, void*);
  static void cb_ExamineSlider(Fl_Value_Slider*, void*);
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
  Fl_Value_Slider *RotationSlider;
private:
  void cb_RotationSlider_i(Fl_Value_Slider*, void*);
  static void cb_RotationSlider(Fl_Value_Slider*, void*);
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
  Fl_Light_Button *DisplayAllLayers;
private:
  void cb_DisplayAllLayers_i(Fl_Light_Button*, void*);
  static void cb_DisplayAllLayers(Fl_Light_Button*, void*);
public:
  Fl_Value_Slider *ShrinkSlider;
private:
  void cb_ShrinkSlider_i(Fl_Value_Slider*, void*);
  static void cb_ShrinkSlider(Fl_Value_Slider*, void*);
public:
  Fl_Light_Button *FixSTLerrorsButton;
private:
  void cb_FixSTLerrorsButton_i(Fl_Light_Button*, void*);
  static void cb_FixSTLerrorsButton(Fl_Light_Button*, void*);
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
  Fl_Output *GCodeLengthText;
  void show(int argc, char **argv);
};
#endif
