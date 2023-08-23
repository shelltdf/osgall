// Copyright (c) 2017 OPEN CASCADE SAS
//
// This file is part of the examples of the Open CASCADE Technology software library.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE

#include "OcctViewer.h"
#include "OcctDocument.h"

#include <OpenGl_GraphicDriver.hxx>

#include <AIS_ConnectedInteractive.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <BRep_Builder.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRepTools.hxx>
#include <Cocoa_Window.hxx>
#include <Message.hxx>
#include <Message_Messenger.hxx>
#include <Prs3d_Drawer.hxx>
#include <StdPrs_ToolTriangulatedShape.hxx>
#include <STEPControl_Reader.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <TDF_Tool.hxx>
#include <TDF_ChildIterator.hxx>
#include <Transfer_TransientProcess.hxx>
#include <XSControl_TransferReader.hxx>
#include <XCAFDoc_DocumentTool.hxx>

// =======================================================================
// function : OcctViewer
// purpose  :
// =======================================================================
OcctViewer::OcctViewer()
{
  myDoc = new OcctDocument();
}

// =======================================================================
// function : ~OcctViewer
// purpose  :
// =======================================================================
OcctViewer::~OcctViewer()
{
  //
}

// =======================================================================
// function : release
// purpose  :
// =======================================================================
void OcctViewer::release()
{
  myContext.Nullify();
  if (!myView.IsNull())
  {
    myView->Remove();
  }
  myView.Nullify();
  myViewer.Nullify();
  
  myDoc.Nullify();
}

// =======================================================================
// function : InitViewer
// purpose  :
// =======================================================================
bool OcctViewer::InitViewer (UIView* theWin)
{
  EAGLContext* aRendCtx = [EAGLContext currentContext];
  if (theWin == NULL || aRendCtx   == NULL)
  {
    NSLog(@"Error: No active EAGL context!");
    release();
    return false;
  }
  if (!myView.IsNull())
  {
    myView->MustBeResized();
    myView->Invalidate();
    return true;
  }

  Handle(Aspect_DisplayConnection) aDisplayConnection = new Aspect_DisplayConnection();
  Handle(Graphic3d_GraphicDriver) aGraphicDriver = new OpenGl_GraphicDriver (aDisplayConnection);

  // Create Viewer
  myViewer = new V3d_Viewer (aGraphicDriver);
  myViewer->SetDefaultLights();
  myViewer->SetLightOn();

  // Create AIS context
  myContext = new AIS_InteractiveContext (myViewer);
  myContext->SetDisplayMode ((int )AIS_DisplayMode::AIS_Shaded, false);

  myView = myViewer->CreateView();
  myView->TriedronDisplay (Aspect_TOTP_LEFT_LOWER, Quantity_NOC_WHITE, 0.20, V3d_ZBUFFER);

  Handle(Cocoa_Window) aCocoaWindow = new Cocoa_Window (theWin);
  myView->SetWindow (aCocoaWindow, aRendCtx);
  if (!aCocoaWindow->IsMapped())
  {
    aCocoaWindow->Map();
  }

  myView->Redraw();
  myView->MustBeResized();
  return true;
}

// =======================================================================
// function : FitAll
// purpose  :
// =======================================================================
void OcctViewer::FitAll()
{
  if (!myView.IsNull())
  {
    myView->FitAll();
    myView->ZFitAll();
  }
}

// =======================================================================
// function : StartRotation
// purpose  :
// =======================================================================
void OcctViewer::StartRotation(int theX, int theY)
{
  if (!myView.IsNull())
  {
    myView->StartRotation(theX, theY);
  }
}

// =======================================================================
// function : Rotation
// purpose  :
// =======================================================================
void OcctViewer::Rotation(int theX, int theY)
{
  if (!myView.IsNull())
  {
    myView->Rotation(theX, theY);
  }
}

// =======================================================================
// function : Pan
// purpose  :
// =======================================================================
void OcctViewer::Pan(int theX, int theY)
{
  if (!myView.IsNull())
  {
    myView->Pan(theX, theY, 1, Standard_False);
  }
}

// =======================================================================
// function : Zoom
// purpose  :
// =======================================================================
void OcctViewer::Zoom(int theX, int theY, double theDelta)
{
  if (!myView.IsNull())
  {
    if (theX >=0 && theY >=0)
    {
      myView->StartZoomAtPoint(theX, theY);
      myView->ZoomAtPoint(0, 0, (int) theDelta, (int) theDelta);
    }
    else
    {
      double aCoeff = Abs(theDelta) / 100.0 + 1.0;
      aCoeff = theDelta > 0.0 ? aCoeff : 1.0 / aCoeff;
      myView->SetZoom(aCoeff, Standard_True);
    }
  }
}

// =======================================================================
// function : Select
// purpose  :
// =======================================================================
void OcctViewer::Select(int theX, int theY)
{
  if (!myContext.IsNull())
  {
    myContext->ClearSelected(Standard_False);
    myContext->MoveTo(theX, theY, myView, Standard_False);
    myContext->Select(Standard_False);
  }
}

// =======================================================================
// function : ImportSTEP
// purpose  :
// =======================================================================
bool OcctViewer::ImportSTEP(std::string theFilename)
{
  // create a new document
  myDoc->InitDoc();

  STEPCAFControl_Reader aReader;
  Handle(XSControl_WorkSession) aSession = aReader.Reader().WS();

  try
  {
    if (!aReader.ReadFile (theFilename.c_str()))
    {
      clearSession (aSession);
      return false;
    }
    
    if (!aReader.Transfer (myDoc->ChangeDocument()))
    {
      clearSession (aSession);
      return false;
    }

    clearSession(aSession);
  }
  catch (const Standard_Failure& theFailure)
  {
    Message::SendFail (TCollection_AsciiString ("Exception raised during STEP import\n[")
                     + theFailure.GetMessageString() + "]\n" + theFilename.c_str());
    return false;
  }

  Handle(XCAFDoc_ShapeTool) aShapeTool = XCAFDoc_DocumentTool::ShapeTool (myDoc->Document()->Main());
  Handle(XCAFDoc_ColorTool) aColorTool = XCAFDoc_DocumentTool::ColorTool (myDoc->Document()->Main());

  TDF_LabelSequence aLabels;
  aShapeTool->GetFreeShapes (aLabels);

  // perform meshing explicitly
  TopoDS_Compound aCompound;
  BRep_Builder    aBuildTool;
  aBuildTool.MakeCompound (aCompound);
  for (Standard_Integer aLabIter = 1; aLabIter <= aLabels.Length(); ++aLabIter)
  {
    TopoDS_Shape     aShape;
    const TDF_Label& aLabel = aLabels.Value (aLabIter);
    if (XCAFDoc_ShapeTool::GetShape (aLabel, aShape))
    {
      aBuildTool.Add (aCompound, aShape);
    }
  }

  Handle(Prs3d_Drawer) aDrawer = myContext->DefaultDrawer();
  Standard_Real aDeflection = StdPrs_ToolTriangulatedShape::GetDeflection (aCompound, aDrawer);
  if (!BRepTools::Triangulation (aCompound, aDeflection))
  {
    BRepMesh_IncrementalMesh anAlgo;
    anAlgo.ChangeParameters().Deflection = aDeflection;
    anAlgo.ChangeParameters().Angle = aDrawer->DeviationAngle();
    anAlgo.ChangeParameters().InParallel = Standard_True;
    anAlgo.SetShape (aCompound);
    anAlgo.Perform();
  }

  // clear presentations
  clearContext();

  // create presentations
  MapOfPrsForShapes aMapOfShapes;
  XCAFPrs_Style aDefStyle;
  aDefStyle.SetColorSurf (Quantity_NOC_GRAY65);
  aDefStyle.SetColorCurv (Quantity_NOC_GRAY65);
  for (Standard_Integer aLabIter = 1; aLabIter <= aLabels.Length(); ++aLabIter)
  {
    const TDF_Label& aLabel = aLabels.Value (aLabIter);
    displayWithChildren (*aShapeTool, *aColorTool, aLabel, TopLoc_Location(), aDefStyle, "", aMapOfShapes);
  }
  
  return true;
}

// =======================================================================
// function : displayWithChildren
// purpose  :
// =======================================================================
void OcctViewer::displayWithChildren (XCAFDoc_ShapeTool&             theShapeTool,
                                      XCAFDoc_ColorTool&             theColorTool,
                                      const TDF_Label&               theLabel,
                                      const TopLoc_Location&         theParentTrsf,
                                      const XCAFPrs_Style&           theParentStyle,
                                      const TCollection_AsciiString& theParentId,
                                      MapOfPrsForShapes&             theMapOfShapes)
{
  TDF_Label aRefLabel = theLabel;
  if (theShapeTool.IsReference (theLabel))
  {
    theShapeTool.GetReferredShape (theLabel, aRefLabel);
  }

  TCollection_AsciiString anEntry;
  TDF_Tool::Entry (theLabel, anEntry);
  if (!theParentId.IsEmpty())
  {
    anEntry = theParentId + "\n" + anEntry;
  }
  anEntry += ".";

  if (!theShapeTool.IsAssembly (aRefLabel))
  {
    Handle(AIS_InteractiveObject) anAis;
    if (!theMapOfShapes.Find (aRefLabel, anAis))
    {
      anAis = new CafShapePrs (aRefLabel, theParentStyle, Graphic3d_NameOfMaterial_ShinyPlastified);
      theMapOfShapes.Bind (aRefLabel, anAis);
    }

    Handle(TCollection_HAsciiString) anId       = new TCollection_HAsciiString (anEntry);
    Handle(AIS_ConnectedInteractive) aConnected = new AIS_ConnectedInteractive();
    aConnected->Connect (anAis, theParentTrsf.Transformation());
    aConnected->SetOwner (anId);
    aConnected->SetLocalTransformation (theParentTrsf.Transformation());
    aConnected->SetHilightMode(1);
    myContext->Display  (aConnected, Standard_False);
    return;
  }

  XCAFPrs_Style aDefStyle = theParentStyle;
  Quantity_Color aColor;
  if (theColorTool.GetColor (aRefLabel, XCAFDoc_ColorGen, aColor))
  {
    aDefStyle.SetColorCurv (aColor);
    aDefStyle.SetColorSurf (aColor);
  }
  if (theColorTool.GetColor (aRefLabel, XCAFDoc_ColorSurf, aColor))
  {
    aDefStyle.SetColorSurf (aColor);
  }
  if (theColorTool.GetColor (aRefLabel, XCAFDoc_ColorCurv, aColor))
  {
    aDefStyle.SetColorCurv (aColor);
  }

  for (TDF_ChildIterator childIter (aRefLabel); childIter.More(); childIter.Next())
  {
    TDF_Label aLabel = childIter.Value();
    if (!aLabel.IsNull()
        && (aLabel.HasAttribute() || aLabel.HasChild()))
    {
      TopLoc_Location aTrsf = theParentTrsf * theShapeTool.GetLocation (aLabel);
      displayWithChildren (theShapeTool, theColorTool, aLabel, aTrsf, aDefStyle, anEntry, theMapOfShapes);
    }
  }
}

// =======================================================================
// function : clearSession
// purpose  :
// =======================================================================
void OcctViewer::clearSession (const Handle(XSControl_WorkSession)& theSession)
{
  if (theSession.IsNull())
  {
    return;
  }

  Handle(Transfer_TransientProcess) aMapReader = theSession->TransferReader()->TransientProcess();
  if (!aMapReader.IsNull())
  {
    aMapReader->Clear();
  }

  Handle(XSControl_TransferReader) aTransferReader = theSession->TransferReader();
  if (!aTransferReader.IsNull())
  {
    aTransferReader->Clear(1);
  }
}

// =======================================================================
// function : clearContext
// purpose  :
// =======================================================================
void OcctViewer::clearContext()
{
  if (!myContext.IsNull())
  {
    myContext->ClearSelected(Standard_False);
    myContext->RemoveAll(Standard_False);
  }
}
