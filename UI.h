// generated by Fast Light User Interface Designer (fluid) version 1.0110

#ifndef UI_h
#define UI_h
#include <FL/Fl.H>
#pragma warning( disable : 4311 4312 4244 4267 4800)
class ModelViewController;
#include <FL/Fl_Double_Window.H>
#include "stdafx.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Text_Editor.H>
#include "Flu_Tree_Browser.h"
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Multi_Browser.H>
#include <FL/Fl_Progress.H>

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
  void cb_Save1_i(Fl_Button*, void*);
  static void cb_Save1(Fl_Button*, void*);
public:
  Fl_Text_Editor *LuaScriptEditor;
  Fl_Button *RunLuaButton;
private:
  void cb_RunLuaButton_i(Fl_Button*, void*);
  static void cb_RunLuaButton(Fl_Button*, void*);
  void cb_Load1_i(Fl_Button*, void*);
  static void cb_Load1(Fl_Button*, void*);
public:
  Flu_Tree_Browser *RFP_Browser;
private:
  void cb_RFP_Browser_i(Flu_Tree_Browser*, void*);
  static void cb_RFP_Browser(Flu_Tree_Browser*, void*);
public:
  Fl_Value_Input *TranslateX;
private:
  void cb_TranslateX_i(Fl_Value_Input*, void*);
  static void cb_TranslateX(Fl_Value_Input*, void*);
public:
  Fl_Value_Input *TranslateY;
private:
  void cb_TranslateY_i(Fl_Value_Input*, void*);
  static void cb_TranslateY(Fl_Value_Input*, void*);
public:
  Fl_Value_Input *TranslateZ;
private:
  void cb_TranslateZ_i(Fl_Value_Input*, void*);
  static void cb_TranslateZ(Fl_Value_Input*, void*);
public:
  Fl_Value_Input *RotateX;
private:
  void cb_RotateX_i(Fl_Value_Input*, void*);
  static void cb_RotateX(Fl_Value_Input*, void*);
public:
  Fl_Value_Input *RotateY;
private:
  void cb_RotateY_i(Fl_Value_Input*, void*);
  static void cb_RotateY(Fl_Value_Input*, void*);
public:
  Fl_Value_Input *RotateZ;
private:
  void cb_RotateZ_i(Fl_Value_Input*, void*);
  static void cb_RotateZ(Fl_Value_Input*, void*);
public:
  Fl_Value_Input *ScaleX;
private:
  void cb_ScaleX_i(Fl_Value_Input*, void*);
  static void cb_ScaleX(Fl_Value_Input*, void*);
public:
  Fl_Value_Input *ScaleY;
private:
  void cb_ScaleY_i(Fl_Value_Input*, void*);
  static void cb_ScaleY(Fl_Value_Input*, void*);
public:
  Fl_Value_Input *ScaleZ;
private:
  void cb_ScaleZ_i(Fl_Value_Input*, void*);
  static void cb_ScaleZ(Fl_Value_Input*, void*);
public:
  Fl_Light_Button *ScaleAllAxies;
  Fl_Input *FileLocationInput;
private:
  void cb_FileLocationInput_i(Fl_Input*, void*);
  static void cb_FileLocationInput(Fl_Input*, void*);
public:
  Fl_Input *FileTypeInput;
private:
  void cb_FileTypeInput_i(Fl_Input*, void*);
  static void cb_FileTypeInput(Fl_Input*, void*);
public:
  Fl_Input *FileMaterialInput;
private:
  void cb_FileMaterialInput_i(Fl_Input*, void*);
  static void cb_FileMaterialInput(Fl_Input*, void*);
public:
  Fl_Input *ObjectNameInput;
private:
  void cb_ObjectNameInput_i(Fl_Input*, void*);
  static void cb_ObjectNameInput(Fl_Input*, void*);
  void cb_New_i(Fl_Button*, void*);
  static void cb_New(Fl_Button*, void*);
  void cb_Save2_i(Fl_Button*, void*);
  static void cb_Save2(Fl_Button*, void*);
  void cb_Delete_i(Fl_Button*, void*);
  static void cb_Delete(Fl_Button*, void*);
  void cb_Duplicate_i(Fl_Button*, void*);
  static void cb_Duplicate(Fl_Button*, void*);
public:
  Fl_Value_Input *VolumeX;
  Fl_Value_Input *VolumeY;
  Fl_Value_Input *VolumeZ;
  Fl_Value_Input *MarginX;
private:
  void cb_MarginX_i(Fl_Value_Input*, void*);
  static void cb_MarginX(Fl_Value_Input*, void*);
public:
  Fl_Value_Input *MarginY;
private:
  void cb_MarginY_i(Fl_Value_Input*, void*);
  static void cb_MarginY(Fl_Value_Input*, void*);
public:
  Fl_Value_Input *MarginZ;
private:
  void cb_MarginZ_i(Fl_Value_Input*, void*);
  static void cb_MarginZ(Fl_Value_Input*, void*);
public:
  Fl_Value_Slider *distanceToReachFullSpeedSlider;
private:
  void cb_distanceToReachFullSpeedSlider_i(Fl_Value_Slider*, void*);
  static void cb_distanceToReachFullSpeedSlider(Fl_Value_Slider*, void*);
public:
  Fl_Light_Button *EnableAccelerationButton;
private:
  void cb_EnableAccelerationButton_i(Fl_Light_Button*, void*);
  static void cb_EnableAccelerationButton(Fl_Light_Button*, void*);
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
  Fl_Value_Slider *ExtrudedMaterialWidthSlider;
private:
  void cb_ExtrudedMaterialWidthSlider_i(Fl_Value_Slider*, void*);
  static void cb_ExtrudedMaterialWidthSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *extrusionFactorSlider;
private:
  void cb_extrusionFactorSlider_i(Fl_Value_Slider*, void*);
  static void cb_extrusionFactorSlider(Fl_Value_Slider*, void*);
public:
  Fl_Light_Button *UseIncrementalEcodeButton;
private:
  void cb_UseIncrementalEcodeButton_i(Fl_Light_Button*, void*);
  static void cb_UseIncrementalEcodeButton(Fl_Light_Button*, void*);
public:
  Fl_Value_Slider *LayerThicknessSlider;
private:
  void cb_LayerThicknessSlider_i(Fl_Value_Slider*, void*);
  static void cb_LayerThicknessSlider(Fl_Value_Slider*, void*);
public:
  Fl_Light_Button *Use3DGcodeButton;
private:
  void cb_Use3DGcodeButton_i(Fl_Light_Button*, void*);
  static void cb_Use3DGcodeButton(Fl_Light_Button*, void*);
public:
  Fl_Input *portInput;
private:
  void cb_portInput_i(Fl_Input*, void*);
  static void cb_portInput(Fl_Input*, void*);
public:
  Fl_Light_Button *shrinkFastButton;
private:
  void cb_shrinkFastButton_i(Fl_Light_Button*, void*);
  static void cb_shrinkFastButton(Fl_Light_Button*, void*);
public:
  Fl_Light_Button *shrinkNiceButton;
private:
  void cb_shrinkNiceButton_i(Fl_Light_Button*, void*);
  static void cb_shrinkNiceButton(Fl_Light_Button*, void*);
public:
  Fl_Value_Slider *InfillRotationSlider;
private:
  void cb_InfillRotationSlider_i(Fl_Value_Slider*, void*);
  static void cb_InfillRotationSlider(Fl_Value_Slider*, void*);
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
  Fl_Value_Slider *OptimizationSlider;
private:
  void cb_OptimizationSlider_i(Fl_Value_Slider*, void*);
  static void cb_OptimizationSlider(Fl_Value_Slider*, void*);
public:
  Fl_Light_Button *ShellOnlyButton;
private:
  void cb_ShellOnlyButton_i(Fl_Light_Button*, void*);
  static void cb_ShellOnlyButton(Fl_Light_Button*, void*);
public:
  Fl_Value_Slider *ShellCountSlider;
private:
  void cb_ShellCountSlider_i(Fl_Value_Slider*, void*);
  static void cb_ShellCountSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *ApronSizeSlider;
private:
  void cb_ApronSizeSlider_i(Fl_Value_Slider*, void*);
  static void cb_ApronSizeSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *ApronCoverageXSlider;
private:
  void cb_ApronCoverageXSlider_i(Fl_Value_Slider*, void*);
  static void cb_ApronCoverageXSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *ApronHeightSlider;
private:
  void cb_ApronHeightSlider_i(Fl_Value_Slider*, void*);
  static void cb_ApronHeightSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *ApronCoverageYSlider;
private:
  void cb_ApronCoverageYSlider_i(Fl_Value_Slider*, void*);
  static void cb_ApronCoverageYSlider(Fl_Value_Slider*, void*);
public:
  Fl_Light_Button *ApronEnableButton;
private:
  void cb_ApronEnableButton_i(Fl_Light_Button*, void*);
  static void cb_ApronEnableButton(Fl_Light_Button*, void*);
public:
  Fl_Value_Slider *ApronDistanceToObjectSlider;
private:
  void cb_ApronDistanceToObjectSlider_i(Fl_Value_Slider*, void*);
  static void cb_ApronDistanceToObjectSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *ApronInfillDistanceSlider;
private:
  void cb_ApronInfillDistanceSlider_i(Fl_Value_Slider*, void*);
  static void cb_ApronInfillDistanceSlider(Fl_Value_Slider*, void*);
public:
  Fl_Light_Button *ApronPreviewButton;
private:
  void cb_ApronPreviewButton_i(Fl_Light_Button*, void*);
  static void cb_ApronPreviewButton(Fl_Light_Button*, void*);
public:
  Fl_Value_Slider *RaftMaterialPrDistanceRatioSlider;
private:
  void cb_RaftMaterialPrDistanceRatioSlider_i(Fl_Value_Slider*, void*);
  static void cb_RaftMaterialPrDistanceRatioSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *RaftRotationSlider;
private:
  void cb_RaftRotationSlider_i(Fl_Value_Slider*, void*);
  static void cb_RaftRotationSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *RaftBaseDistanceSlider;
private:
  void cb_RaftBaseDistanceSlider_i(Fl_Value_Slider*, void*);
  static void cb_RaftBaseDistanceSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *RaftBaseThicknessSlider;
private:
  void cb_RaftBaseThicknessSlider_i(Fl_Value_Slider*, void*);
  static void cb_RaftBaseThicknessSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *RaftBaseLayerCountSlider;
private:
  void cb_RaftBaseLayerCountSlider_i(Fl_Value_Slider*, void*);
  static void cb_RaftBaseLayerCountSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *RaftBaseTemperatureSlider;
private:
  void cb_RaftBaseTemperatureSlider_i(Fl_Value_Slider*, void*);
  static void cb_RaftBaseTemperatureSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *RaftSizeSlider;
private:
  void cb_RaftSizeSlider_i(Fl_Value_Slider*, void*);
  static void cb_RaftSizeSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *RaftInterfaceMaterialPrDistanceRatioSlider;
private:
  void cb_RaftInterfaceMaterialPrDistanceRatioSlider_i(Fl_Value_Slider*, void*);
  static void cb_RaftInterfaceMaterialPrDistanceRatioSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *RaftRotationPrLayerSlider;
private:
  void cb_RaftRotationPrLayerSlider_i(Fl_Value_Slider*, void*);
  static void cb_RaftRotationPrLayerSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *RaftInterfaceDistanceSlider;
private:
  void cb_RaftInterfaceDistanceSlider_i(Fl_Value_Slider*, void*);
  static void cb_RaftInterfaceDistanceSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *RaftInterfaceThicknessSlider;
private:
  void cb_RaftInterfaceThicknessSlider_i(Fl_Value_Slider*, void*);
  static void cb_RaftInterfaceThicknessSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *RaftInterfaceLayerCountSlider;
private:
  void cb_RaftInterfaceLayerCountSlider_i(Fl_Value_Slider*, void*);
  static void cb_RaftInterfaceLayerCountSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *RaftInterfaceTemperatureSlider;
private:
  void cb_RaftInterfaceTemperatureSlider_i(Fl_Value_Slider*, void*);
  static void cb_RaftInterfaceTemperatureSlider(Fl_Value_Slider*, void*);
public:
  Fl_Light_Button *RaftEnableButton;
private:
  void cb_RaftEnableButton_i(Fl_Light_Button*, void*);
  static void cb_RaftEnableButton(Fl_Light_Button*, void*);
  void cb_Preview_i(Fl_Light_Button*, void*);
  static void cb_Preview(Fl_Light_Button*, void*);
  void cb_Load2_i(Fl_Button*, void*);
  static void cb_Load2(Fl_Button*, void*);
public:
  Fl_Output *GCodeLengthText;
private:
  void cb_Convert_i(Fl_Button*, void*);
  static void cb_Convert(Fl_Button*, void*);
public:
  Fl_Text_Editor *GCodeStart;
  Fl_Text_Editor *GCodeLayer;
  Fl_Text_Editor *GCodeEnd;
  Fl_Text_Editor *GCodeResult;
private:
  void cb_Save3_i(Fl_Button*, void*);
  static void cb_Save3(Fl_Button*, void*);
public:
  Fl_Text_Editor *NotesEditor;
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
  Fl_Value_Slider *PolygonValSlider;
private:
  void cb_PolygonValSlider_i(Fl_Value_Slider*, void*);
  static void cb_PolygonValSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *PolygonSatSlider;
private:
  void cb_PolygonSatSlider_i(Fl_Value_Slider*, void*);
  static void cb_PolygonSatSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *PolygonHueSlider;
private:
  void cb_PolygonHueSlider_i(Fl_Value_Slider*, void*);
  static void cb_PolygonHueSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *WireframeValSlider;
private:
  void cb_WireframeValSlider_i(Fl_Value_Slider*, void*);
  static void cb_WireframeValSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *WireframeSatSlider;
private:
  void cb_WireframeSatSlider_i(Fl_Value_Slider*, void*);
  static void cb_WireframeSatSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *WireframeHueSlider;
private:
  void cb_WireframeHueSlider_i(Fl_Value_Slider*, void*);
  static void cb_WireframeHueSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *NormalValSlider;
private:
  void cb_NormalValSlider_i(Fl_Value_Slider*, void*);
  static void cb_NormalValSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *NormalSatSlider;
private:
  void cb_NormalSatSlider_i(Fl_Value_Slider*, void*);
  static void cb_NormalSatSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *NormalHueSlider;
private:
  void cb_NormalHueSlider_i(Fl_Value_Slider*, void*);
  static void cb_NormalHueSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *EndpointValSlider;
private:
  void cb_EndpointValSlider_i(Fl_Value_Slider*, void*);
  static void cb_EndpointValSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *EndpointSatSlider;
private:
  void cb_EndpointSatSlider_i(Fl_Value_Slider*, void*);
  static void cb_EndpointSatSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *EndpointHueSlider;
private:
  void cb_EndpointHueSlider_i(Fl_Value_Slider*, void*);
  static void cb_EndpointHueSlider(Fl_Value_Slider*, void*);
public:
  Fl_Light_Button *DisplayBBoxButton;
private:
  void cb_DisplayBBoxButton_i(Fl_Light_Button*, void*);
  static void cb_DisplayBBoxButton(Fl_Light_Button*, void*);
public:
  Fl_Value_Slider *HighlightSlider;
private:
  void cb_HighlightSlider_i(Fl_Value_Slider*, void*);
  static void cb_HighlightSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *NormalLengthSlider;
private:
  void cb_NormalLengthSlider_i(Fl_Value_Slider*, void*);
  static void cb_NormalLengthSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *EndpointSizeSlider;
private:
  void cb_EndpointSizeSlider_i(Fl_Value_Slider*, void*);
  static void cb_EndpointSizeSlider(Fl_Value_Slider*, void*);
public:
  Fl_Light_Button *DisplayWireframeShadedButton;
private:
  void cb_DisplayWireframeShadedButton_i(Fl_Light_Button*, void*);
  static void cb_DisplayWireframeShadedButton(Fl_Light_Button*, void*);
public:
  Fl_Value_Slider *PolygonOpasitySlider;
private:
  void cb_PolygonOpasitySlider_i(Fl_Value_Slider*, void*);
  static void cb_PolygonOpasitySlider(Fl_Value_Slider*, void*);
public:
  Fl_Light_Button *DisplayGCodeButton;
private:
  void cb_DisplayGCodeButton_i(Fl_Light_Button*, void*);
  static void cb_DisplayGCodeButton(Fl_Light_Button*, void*);
public:
  Fl_Value_Slider *GCodeExtrudeValSlider;
private:
  void cb_GCodeExtrudeValSlider_i(Fl_Value_Slider*, void*);
  static void cb_GCodeExtrudeValSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *GCodeExtrudeSatSlider;
private:
  void cb_GCodeExtrudeSatSlider_i(Fl_Value_Slider*, void*);
  static void cb_GCodeExtrudeSatSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *GCodeExtrudeHueSlider;
private:
  void cb_GCodeExtrudeHueSlider_i(Fl_Value_Slider*, void*);
  static void cb_GCodeExtrudeHueSlider(Fl_Value_Slider*, void*);
public:
  Fl_Light_Button *LuminanceShowsSpeedButton;
private:
  void cb_LuminanceShowsSpeedButton_i(Fl_Light_Button*, void*);
  static void cb_LuminanceShowsSpeedButton(Fl_Light_Button*, void*);
  void cb_Crop_i(Fl_Button*, void*);
  static void cb_Crop(Fl_Button*, void*);
  void cb_Reset_i(Fl_Button*, void*);
  static void cb_Reset(Fl_Button*, void*);
public:
  Fl_Slider *GCodeDrawStartSlider;
private:
  void cb_GCodeDrawStartSlider_i(Fl_Slider*, void*);
  static void cb_GCodeDrawStartSlider(Fl_Slider*, void*);
public:
  Fl_Slider *GCodeDrawEndSlider;
private:
  void cb_GCodeDrawEndSlider_i(Fl_Slider*, void*);
  static void cb_GCodeDrawEndSlider(Fl_Slider*, void*);
public:
  Fl_Value_Slider *GCodeMoveValSlider;
private:
  void cb_GCodeMoveValSlider_i(Fl_Value_Slider*, void*);
  static void cb_GCodeMoveValSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *GCodeMoveSatSlider;
private:
  void cb_GCodeMoveSatSlider_i(Fl_Value_Slider*, void*);
  static void cb_GCodeMoveSatSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *GCodeMoveHueSlider;
private:
  void cb_GCodeMoveHueSlider_i(Fl_Value_Slider*, void*);
  static void cb_GCodeMoveHueSlider(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *CuttingPlaneValueSlider;
private:
  void cb_CuttingPlaneValueSlider_i(Fl_Value_Slider*, void*);
  static void cb_CuttingPlaneValueSlider(Fl_Value_Slider*, void*);
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
  Fl_Light_Button *DisplayAllLayersButton;
private:
  void cb_DisplayAllLayersButton_i(Fl_Light_Button*, void*);
  static void cb_DisplayAllLayersButton(Fl_Light_Button*, void*);
  void cb_Enable_i(Fl_Light_Button*, void*);
  static void cb_Enable(Fl_Light_Button*, void*);
  void cb_Enable1_i(Fl_Light_Button*, void*);
  static void cb_Enable1(Fl_Light_Button*, void*);
  void cb_Enable2_i(Fl_Light_Button*, void*);
  static void cb_Enable2(Fl_Light_Button*, void*);
  void cb_Enable3_i(Fl_Light_Button*, void*);
  static void cb_Enable3(Fl_Light_Button*, void*);
public:
  Fl_Multi_Browser *CommunationLog;
  Fl_Multi_Browser *ErrorLog;
  Fl_Multi_Browser *Echo;
  Fl_Light_Button *SwitchHeatOnButton;
private:
  void cb_SwitchHeatOnButton_i(Fl_Light_Button*, void*);
  static void cb_SwitchHeatOnButton(Fl_Light_Button*, void*);
public:
  Fl_Value_Input *TargetTempText;
private:
  void cb_TargetTempText_i(Fl_Value_Input*, void*);
  static void cb_TargetTempText(Fl_Value_Input*, void*);
public:
  Fl_Light_Button *RunExtruderButton;
private:
  void cb_RunExtruderButton_i(Fl_Light_Button*, void*);
  static void cb_RunExtruderButton(Fl_Light_Button*, void*);
public:
  Fl_Light_Button *SetExtruderDirectionButton;
private:
  void cb_SetExtruderDirectionButton_i(Fl_Light_Button*, void*);
  static void cb_SetExtruderDirectionButton(Fl_Light_Button*, void*);
  void cb_Home_i(Fl_Button*, void*);
  static void cb_Home(Fl_Button*, void*);
  void cb_Home1_i(Fl_Button*, void*);
  static void cb_Home1(Fl_Button*, void*);
  void cb_Home2_i(Fl_Button*, void*);
  static void cb_Home2(Fl_Button*, void*);
  void cb_Speed_i(Fl_Value_Slider*, void*);
  static void cb_Speed(Fl_Value_Slider*, void*);
  void cb_Length_i(Fl_Value_Slider*, void*);
  static void cb_Length(Fl_Value_Slider*, void*);
public:
  Fl_Input *GCodeInput;
private:
  void cb_Send_i(Fl_Button*, void*);
  static void cb_Send(Fl_Button*, void*);
public:
  Fl_Output *CurrentTempText;
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
  Fl_Light_Button *DrawVertexNumbersButton;
private:
  void cb_DrawVertexNumbersButton_i(Fl_Light_Button*, void*);
  static void cb_DrawVertexNumbersButton(Fl_Light_Button*, void*);
public:
  Fl_Light_Button *DrawLineNumbersButton;
private:
  void cb_DrawLineNumbersButton_i(Fl_Light_Button*, void*);
  static void cb_DrawLineNumbersButton(Fl_Light_Button*, void*);
  void cb_Home3_i(Fl_Button*, void*);
  static void cb_Home3(Fl_Button*, void*);
  void cb_10_i(Fl_Button*, void*);
  static void cb_10(Fl_Button*, void*);
  void cb_1_i(Fl_Button*, void*);
  static void cb_1(Fl_Button*, void*);
  void cb_0_i(Fl_Button*, void*);
  static void cb_0(Fl_Button*, void*);
  void cb_01_i(Fl_Button*, void*);
  static void cb_01(Fl_Button*, void*);
  void cb_11_i(Fl_Button*, void*);
  static void cb_11(Fl_Button*, void*);
  void cb_101_i(Fl_Button*, void*);
  static void cb_101(Fl_Button*, void*);
  void cb_100_i(Fl_Button*, void*);
  static void cb_100(Fl_Button*, void*);
  void cb_1001_i(Fl_Button*, void*);
  static void cb_1001(Fl_Button*, void*);
  void cb_102_i(Fl_Button*, void*);
  static void cb_102(Fl_Button*, void*);
  void cb_12_i(Fl_Button*, void*);
  static void cb_12(Fl_Button*, void*);
  void cb_02_i(Fl_Button*, void*);
  static void cb_02(Fl_Button*, void*);
  void cb_03_i(Fl_Button*, void*);
  static void cb_03(Fl_Button*, void*);
  void cb_13_i(Fl_Button*, void*);
  static void cb_13(Fl_Button*, void*);
  void cb_103_i(Fl_Button*, void*);
  static void cb_103(Fl_Button*, void*);
  void cb_1002_i(Fl_Button*, void*);
  static void cb_1002(Fl_Button*, void*);
  void cb_1003_i(Fl_Button*, void*);
  static void cb_1003(Fl_Button*, void*);
  void cb_104_i(Fl_Button*, void*);
  static void cb_104(Fl_Button*, void*);
  void cb_14_i(Fl_Button*, void*);
  static void cb_14(Fl_Button*, void*);
  void cb_04_i(Fl_Button*, void*);
  static void cb_04(Fl_Button*, void*);
  void cb_05_i(Fl_Button*, void*);
  static void cb_05(Fl_Button*, void*);
  void cb_15_i(Fl_Button*, void*);
  static void cb_15(Fl_Button*, void*);
  void cb_105_i(Fl_Button*, void*);
  static void cb_105(Fl_Button*, void*);
  void cb_1004_i(Fl_Button*, void*);
  static void cb_1004(Fl_Button*, void*);
  void cb_1005_i(Fl_Button*, void*);
  static void cb_1005(Fl_Button*, void*);
public:
  Fl_Value_Input *XposText;
private:
  void cb_XposText_i(Fl_Value_Input*, void*);
  static void cb_XposText(Fl_Value_Input*, void*);
public:
  Fl_Value_Input *YposText;
private:
  void cb_YposText_i(Fl_Value_Input*, void*);
  static void cb_YposText(Fl_Value_Input*, void*);
public:
  Fl_Value_Input *ZposText;
private:
  void cb_ZposText_i(Fl_Value_Input*, void*);
  static void cb_ZposText(Fl_Value_Input*, void*);
public:
  Fl_Value_Slider *DownstreamMultiplierSlider;
  Fl_Value_Slider *TempUpdateSpeedSlider;
private:
  void cb_TempUpdateSpeedSlider_i(Fl_Value_Slider*, void*);
  static void cb_TempUpdateSpeedSlider(Fl_Value_Slider*, void*);
public:
  Fl_Light_Button *AutoscrollButton;
  Fl_Value_Slider *LinesToKeepSlider;
private:
  void cb_LinesToKeepSlider_i(Fl_Value_Slider*, void*);
  static void cb_LinesToKeepSlider(Fl_Value_Slider*, void*);
public:
  Fl_Light_Button *SetFileLoggingButton;
private:
  void cb_SetFileLoggingButton_i(Fl_Light_Button*, void*);
  static void cb_SetFileLoggingButton(Fl_Light_Button*, void*);
  void cb_Enable4_i(Fl_Light_Button*, void*);
  static void cb_Enable4(Fl_Light_Button*, void*);
  void cb_Clear_i(Fl_Light_Button*, void*);
  static void cb_Clear(Fl_Light_Button*, void*);
  void cb_Clear1_i(Fl_Button*, void*);
  static void cb_Clear1(Fl_Button*, void*);
public:
  Fl_Light_Button *ConnectToPrinterButton;
private:
  void cb_ConnectToPrinterButton_i(Fl_Light_Button*, void*);
  static void cb_ConnectToPrinterButton(Fl_Light_Button*, void*);
public:
  Fl_Light_Button *PrintButton;
private:
  void cb_PrintButton_i(Fl_Light_Button*, void*);
  static void cb_PrintButton(Fl_Light_Button*, void*);
public:
  Fl_Button *ContinueButton;
private:
  void cb_ContinueButton_i(Fl_Button*, void*);
  static void cb_ContinueButton(Fl_Button*, void*);
  void cb_Power_i(Fl_Light_Button*, void*);
  static void cb_Power(Fl_Light_Button*, void*);
  void cb_Fan_i(Fl_Light_Button*, void*);
  static void cb_Fan(Fl_Light_Button*, void*);
  void cb_Errors_i(Fl_Light_Button*, void*);
  static void cb_Errors(Fl_Light_Button*, void*);
  void cb_Info_i(Fl_Light_Button*, void*);
  static void cb_Info(Fl_Light_Button*, void*);
  void cb_Echo_i(Fl_Light_Button*, void*);
  static void cb_Echo(Fl_Light_Button*, void*);
public:
  Fl_Progress *ProgressBar;
  void show(int argc, char **argv);
};
void TempReadTimer(void *);
#endif
