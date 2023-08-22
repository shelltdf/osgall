// ISession_Direction.cpp: implementation of the ISession_Direction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\\GeometryApp.h"
#include "ISession_Direction.h"
#include <Prs3d_ArrowAspect.hxx>
#include <DsgPrs_LengthPresentation.hxx>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

IMPLEMENT_STANDARD_RTTIEXT(ISession_Direction,AIS_InteractiveObject)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ISession_Direction::ISession_Direction()
{

}

ISession_Direction::ISession_Direction (const gp_Pnt& aPnt,
                                        const gp_Dir& aDir,
                                        Standard_Real aLength,
                                        Standard_Real anArrowLength)
: myPnt (aPnt),
  myDir (aDir),
  myLength (aLength),
  myArrowLength (anArrowLength)
{}

ISession_Direction::ISession_Direction (const gp_Pnt& aPnt,
                                        const gp_Vec& aVec,
                                        Standard_Real anArrowLength)
: myPnt (aPnt),
  myDir (aVec),
  myArrowLength (anArrowLength)
{
  myLength = aVec.Magnitude();
}

ISession_Direction::ISession_Direction (const gp_Pnt2d& aPnt2d,
                                        const gp_Dir2d& aDir2d,
                                        Standard_Real aLength)
: myPnt (gp_Pnt(aPnt2d.X(),aPnt2d.Y(),0.0)),
  myDir (gp_Dir(aDir2d.X(),aDir2d.Y(),0.0)),
  myLength (aLength)
{
  myArrowLength = 1.0;
}

ISession_Direction::ISession_Direction (const gp_Pnt2d& aPnt2d,
                                        const gp_Vec2d& aVec2d)
: myPnt (gp_Pnt (aPnt2d.X(), aPnt2d.Y(), 0.0)),
  myDir (gp_Dir(aVec2d.X(), aVec2d.Y(), 0.0))
{
  myLength = aVec2d.Magnitude();
  myArrowLength = 1.0;
}


ISession_Direction::~ISession_Direction()
{

}

void ISession_Direction::Compute (const Handle(PrsMgr_PresentationManager)& /*aPresentationManager*/,
                                  const Handle(Prs3d_Presentation)& aPresentation,
                                  const Standard_Integer /*aMode*/)
{
  // Set style for arrow
  Handle(Prs3d_ArrowAspect) anArrowAspect = myDrawer->ArrowAspect();
  anArrowAspect->SetLength (myArrowLength);

  gp_Pnt aLastPoint = myPnt;
  aLastPoint.Translate (myLength*gp_Vec(myDir));

  // Draw Line
  Handle(Graphic3d_ArrayOfSegments) aPrims = new Graphic3d_ArrayOfSegments (2);
  aPrims->AddVertex (myPnt);
  aPrims->AddVertex (aLastPoint);
  aPresentation->CurrentGroup()->SetPrimitivesAspect (myDrawer->LineAspect()->Aspect());
  aPresentation->CurrentGroup()->AddPrimitiveArray (aPrims);
  // Draw arrow
  Prs3d_Arrow::Draw (aPresentation->CurrentGroup(),
                     aLastPoint,
                     myDir,
                     anArrowAspect->Angle(),
                     anArrowAspect->Length());

  // Draw text
  if (myText.Length() != 0)
  {
    gp_Pnt aTextPosition = aLastPoint;
    Prs3d_Text::Draw (aPresentation->CurrentGroup(),
                      myDrawer->TextAspect(),
                      myText,
                      aTextPosition);
  }
}

void ISession_Direction::ComputeSelection (const Handle(SelectMgr_Selection)& /*aSelection*/,
                                           const Standard_Integer /*aMode*/) 
{
}

void ISession_Direction::SetText (TCollection_ExtendedString & theText)
{
  myText = theText;
}

void ISession_Direction::SetText (Standard_CString theText)
{
  myText = theText;
}

void ISession_Direction::SetLineAspect (const Handle(Prs3d_LineAspect)& theAspect)
{
  myDrawer->SetLineAspect (theAspect);
}
