// Copyright (c) 1999-2014 OPEN CASCADE SAS
//
// This file is part of Open CASCADE Technology software library.
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 2.1 as published
// by the Free Software Foundation, with special exception defined in the file
// OCCT_LGPL_EXCEPTION.txt. Consult the file LICENSE_LGPL_21.txt included in OCCT
// distribution for complete text of the license and disclaimer of any warranty.
//
// Alternatively, this file may be used under the terms of Open CASCADE
// commercial license or contractual agreement.

//:k8 abv 6 Jan 98: using parameter "step.group" for writing assemblies/shapes

#include <DBRep.hxx>
#include <Draw_Appli.hxx>
#include <Draw_Interpretor.hxx>
#include <Draw_ProgressIndicator.hxx>
#include <IFSelect_SessionPilot.hxx>
#include <Interface_Macros.hxx>
#include <Interface_Static.hxx>
#include <Message.hxx>
#include <Message_ProgressScope.hxx>
#include <OSD_OpenFile.hxx>
#include <OSD_Path.hxx>
#include <STEPControl_ActorWrite.hxx>
#include <STEPControl_Controller.hxx>
#include <STEPControl_Reader.hxx>
#include <STEPControl_StepModelType.hxx>
#include <STEPControl_Writer.hxx>
#include <StepData_StepModel.hxx>
#include <StepGeom_Axis2Placement3d.hxx>
#include <StepSelect_Activator.hxx>
#include <STEPSelections_AssemblyExplorer.hxx>
#include <STEPSelections_Counter.hxx>
#include <StepToTopoDS_MakeTransformed.hxx>
#include <TColStd_HSequenceOfTransient.hxx>
#include <TopExp_Explorer.hxx>
#include <Transfer_TransientProcess.hxx>
#include <XSAlgo.hxx>
#include <XSAlgo_AlgoContainer.hxx>
#include <XSControl_Controller.hxx>
#include <XSControl_WorkSession.hxx>
#include <XSDRAW.hxx>
#include <XSDRAWSTEP.hxx>
#include <UnitsMethods.hxx>

#include <stdio.h>
//  Pour le transfert (write)
//  Pour NewModel et Write : definition de produit (temporaire ...)
// steptrans
extern "C" {
static void cleanpilot ()
{
  XSDRAW::Session()->ClearData(1);
}
}


//=======================================================================
//function : Init
//purpose  : 
//=======================================================================

void XSDRAWSTEP::Init ()
{
  Handle(StepSelect_Activator)   stepact = new StepSelect_Activator;
  if (STEPControl_Controller::Init()) // XSDRAW::SetNorm("STEP AP-214"); trop tot
    XSDRAW::SetController(XSControl_Controller::Recorded("STEP"));
  
  atexit (cleanpilot);
}

//  ########  COMMANDE stepread  : teste le Reader  #########

//=======================================================================
//function : stepread
//purpose  : 
//=======================================================================

static Standard_Integer stepread (Draw_Interpretor& di, Standard_Integer argc, const char** argv) 
{
  if (argc < 3) {
    di << "Use: stepread  [file] [f or r (type of model full or reduced)]\n";
    return 1;
  }
  //  On admet le controller AP214 ou une variante
  DeclareAndCast(STEPControl_Controller,ctl,XSDRAW::Controller());
  if (ctl.IsNull()) XSDRAW::SetNorm("STEP");


  // Progress indicator
  Handle(Draw_ProgressIndicator) progress = new Draw_ProgressIndicator ( di, 1 );
  Message_ProgressScope aPSRoot (progress->Start(), "Reading", 100);

  STEPControl_Reader sr (XSDRAW::Session(),Standard_False);
  TCollection_AsciiString fnom,rnom;
  Standard_Boolean modfic = XSDRAW::FileAndVar
    (argv[1],argv[2],"STEP",fnom,rnom);
  if (modfic) di<<" File STEP to read : "<<fnom.ToCString()<<"\n";
  else        di<<" Model taken from the session : "<<fnom.ToCString()<<"\n";
  di<<" -- Names of variables BREP-DRAW prefixed by : "<<rnom.ToCString()<<"\n";
  IFSelect_ReturnStatus readstat = IFSelect_RetVoid;

  aPSRoot.SetName("Loading");
  progress->Show(aPSRoot);

  Standard_Boolean fromtcl = Standard_False;
  Standard_Boolean aFullMode = Standard_False;
  Standard_Integer k = 3;
  if(argc > k )
  {
    if(argv[k][0] == 'f' || argv[3][0] == 'F')
    {
      aFullMode = Standard_True;
      k++;
    }
    else if(argv[k][0] == 'r' || argv[3][0] == 'R')
    {
      aFullMode = Standard_False;
      k++;
    }
    else
      fromtcl = Standard_True;
   
  }
  if(!fromtcl)
    fromtcl = argc > k;
  if(aFullMode)
    std::cout<<"Full model for translation with additional info will be used \n"<<std::flush;
  else
    std::cout<<"Reduced model for translation without additional info will be used \n"<<std::flush;
  
  sr.WS()->SetModeStat(aFullMode);


  if (modfic) readstat = sr.ReadFile (fnom.ToCString());
  else  if (XSDRAW::Session()->NbStartingEntities() > 0) readstat = IFSelect_RetDone;

  aPSRoot.Next(20); // On average loading takes 20% 
  if (aPSRoot.UserBreak())
    return 1;

  if (readstat != IFSelect_RetDone) {
    if (modfic) di<<"Could not read file "<<fnom.ToCString()<<" , abandon\n";
    else di<<"No model loaded\n";
    return 1;
  }

  XSAlgo::AlgoContainer()->PrepareForTransfer(); // update unit info
  sr.SetSystemLengthUnit(UnitsMethods::GetCasCadeLengthUnit());

  //   nom = "." -> fichier deja lu
  Standard_Integer i, num, nbs, modepri = 1;
  if (fromtcl) modepri = 4;
  while (modepri) {
    num = sr.NbRootsForTransfer();
    if (!fromtcl) {
      di<<"NbRootsForTransfer="<<num<<" :\n";
      for (i = 1; i <= num; i ++) {
        di<<"Root."<<i<<", Ent. ";
        Standard_SStream aTmpStream;
        sr.Model()->Print (sr.RootForTransfer(i), aTmpStream);
        di << aTmpStream.str().c_str();
        di<<" Type:"<<sr.RootForTransfer(i)->DynamicType()->Name()<<"\n";
      }

      std::cout<<"Mode (0 End, 1 root n0 1, 2 one root/n0, 3 one entity/n0, 4 Selection) : "<<std::flush;
      std::cin>>modepri;
    }

    if (modepri == 0) { di<<"End Reading STEP\n"; return 0; }
    if (modepri <= 2) {
      num = 1;
      if (modepri == 2) {
        std::cout<<"Root N0 : "<<std::flush;  std::cin>>num;
      }
      aPSRoot.SetName("Translation");
      progress->Show(aPSRoot);

      if (!sr.TransferRoot (num, aPSRoot.Next(80)))
        di<<"Transfer root n0 "<<num<<" : no result\n";
      else {
        nbs = sr.NbShapes();
        char shname[30];  Sprintf (shname,"%s_%d",rnom.ToCString(),nbs);
        di<<"Transfer root n0 "<<num<<" OK  -> DRAW Shape: "<<shname<<"\n";
        di<<"Now, "<<nbs<<" Shapes produced\n";
        TopoDS_Shape sh = sr.Shape(nbs);
        DBRep::Set (shname,sh);
      }
      if (aPSRoot.UserBreak())
        return 1;
    }
    else if (modepri == 3) {
      std::cout<<"Entity : "<<std::flush;  num = XSDRAW::GetEntityNumber();
      if (!sr.TransferOne (num))
        di<<"Transfer entity n0 "<<num<<" : no result\n";
      else {
        nbs = sr.NbShapes();
        char shname[30];  Sprintf (shname,"%s_%d",rnom.ToCString(),num);
        di<<"Transfer entity n0 "<<num<<" OK  -> DRAW Shape: "<<shname<<"\n";
        di<<"Now, "<<nbs<<" Shapes produced\n";
        TopoDS_Shape sh = sr.Shape(nbs);
        DBRep::Set (shname,sh);
      }
    }
    else if (modepri == 4) {
      //      char snm[100];  Standard_Integer answer = 1;
      Handle(TColStd_HSequenceOfTransient)  list;

      //  Selection, nommee ou via tcl. tcl : raccourcis admis
      //   * donne xst-transferrable-roots
      if (fromtcl) {
        modepri = 0;    // d ioffice une seule passe
        if (argv[k][0] == '*' && argv[k][1] == '\0') {
          di<<"Transferrable Roots : ";
          list = XSDRAW::GetList("xst-transferrable-roots");
          //list = new TColStd_HSequenceOfTransient;
          //for(Standard_Integer j=1; j<=num; j++)
          //  list->Append(sr.RootForTransfer(j));
        }
        else {
          di<<"List given by "<<argv[k];
          if (argc > k+1) di<<" "<<argv[k+1];
          di<<" : ";
          list = XSDRAW::GetList (argv[k], ( argc > (k+1) ? argv[k+1] : 0 ) );
        }
        if (list.IsNull()) { di<<"No list defined. Give a selection name or * for all transferrable roots\n"; continue; }
      } else {
        std::cout<<"Name of Selection :"<<std::flush;
        list = XSDRAW::GetList();
        if (list.IsNull()) { di<<"No list defined\n"; continue; }
      }

      Standard_Integer ill, nbl = list->Length();
      di<<"Nb entities selected : "<<nbl<<"\n";
      if (nbl == 0) continue;

      aPSRoot.SetName("Translation");
      progress->Show(aPSRoot);

      Message_ProgressScope aPS(aPSRoot.Next(80), "Root", nbl);
      for (ill = 1; ill <= nbl && aPS.More(); ill++) {
        num = sr.Model()->Number(list->Value(ill));
        if (num == 0) continue;
        if (!sr.TransferOne(num, aPS.Next()))
          di<<"Transfer entity n0 "<<num<<" : no result\n";
        else {
          nbs = sr.NbShapes();
          char shname[30];  Sprintf (shname,"%s_%d",rnom.ToCString(),nbs);
          di<<"Transfer entity n0 "<<num<<" OK  -> DRAW Shape: "<<shname<<"\n";
          di<<"Now, "<<nbs<<" Shapes produced\n";
          TopoDS_Shape sh = sr.Shape(nbs);
          DBRep::Set (shname,sh);
        }
      }
      if (aPSRoot.UserBreak())
        return 1;
    }
    else di<<"Unknown mode n0 "<<modepri<<"\n";
  }
  return 0;
}

//=======================================================================
//function : testreadstep
//purpose  : 
//=======================================================================
static Standard_Integer testreadstep (Draw_Interpretor& di, Standard_Integer argc, const char** argv)
{
  if (argc < 3 || argc > 4)                                                                                      
  {
    di << "ERROR in " << argv[0] << "Wrong Number of Arguments.\n";
    di << " Usage : " << argv[0] << " file_name shape_name [-stream]\n";
    di << " Option -stream forces usage of API accepting stream\n";
    return 1;
  }

  Standard_Boolean useStream = (argc > 3 && ! strcasecmp (argv[3], "-stream"));

  STEPControl_Reader Reader;
  Standard_CString filename = argv[1];
  IFSelect_ReturnStatus readstat;
  if (useStream)
  {
    std::ifstream aStream;
    OSD_OpenStream (aStream, filename, std::ios::in | std::ios::binary);
    TCollection_AsciiString aFolder, aFileNameShort;
    OSD_Path::FolderAndFileFromPath (filename, aFolder, aFileNameShort);
    readstat = Reader.ReadStream (aFileNameShort.ToCString(), aStream);
  }
  else
  {
    readstat = Reader.ReadFile(filename);
  }
  di<<"Status from reading STEP file "<<filename<<" : ";  
  switch(readstat) {                                                              
    case IFSelect_RetVoid  : { di<<"empty file\n"; return 1; }            
    case IFSelect_RetDone  : { di<<"file read\n";    break; }             
    case IFSelect_RetError : { di<<"file not found\n";   return 1; }      
    case IFSelect_RetFail  : { di<<"error during read\n";  return 1; }    
    default  :  { di<<"failure\n";   return 1; }                          
  }
  XSAlgo::AlgoContainer()->PrepareForTransfer(); // update unit info
  Reader.SetSystemLengthUnit(UnitsMethods::GetCasCadeLengthUnit());
  Reader.TransferRoots();
  TopoDS_Shape shape = Reader.OneShape();
  DBRep::Set(argv[2],shape); 
  di<<"Count of shapes produced : "<<Reader.NbShapes()<<"\n";
  return 0;
}
 
//  ########  COMMANDE steptrans  : teste les transformations  #########
//=======================================================================
//function : steptrans
//purpose  : 
//=======================================================================

static Standard_Integer steptrans (Draw_Interpretor& di, Standard_Integer argc, const char** argv) 
{
  if (argc < 5) { di<<"give shape-name new-shape + entity-n0 entity-n0: AXIS2\n";
		  return 1; }
  TopoDS_Shape shape = DBRep::Get(argv[1]);
  if (shape.IsNull()) { di<<"Not a shape : "<<argv[1]<<"\n"; return 1; }
  Handle(StepGeom_Axis2Placement3d) ax1,ax2;
  Standard_Integer n1 = 0, n2 = 0;
  n1 = XSDRAW::GetEntityNumber(argv[3]);
  if (argc > 4) n2 = XSDRAW::GetEntityNumber(argv[4]);
  if (n1 > 0) ax1 = Handle(StepGeom_Axis2Placement3d)::DownCast
    (XSDRAW::Entity(n1));
  if (n2 > 0) ax2 = Handle(StepGeom_Axis2Placement3d)::DownCast
    (XSDRAW::Entity(n2));
  StepToTopoDS_MakeTransformed mktrans;
  if (mktrans.Compute (ax1,ax2)) {
    TopLoc_Location loc (mktrans.Transformation());
    shape.Move (loc);
//    mktrans.Transform (shape);
    DBRep::Set (argv[2],shape);
    di<<"Transformed Shape as "<<argv[2]<<"\n";
  }
  else di<<"No transformation computed\n";
  return 0;
}

//  ########  COMMANDE stepwrite : teste le Writer  #########

//=======================================================================
//function : stepwrite
//purpose  : 
//=======================================================================

static Standard_Integer stepwrite (Draw_Interpretor& di, Standard_Integer argc, const char** argv) 
{
//  On admet le controller AP214 ou une variante
  DeclareAndCast(STEPControl_Controller,ctl,XSDRAW::Controller());
  if (ctl.IsNull()) {
    XSDRAW::SetNorm("STEP");
    //sln 14.01.2002 OCC51: assign new value to ctl in order to avoid exception during using one in the function
    ctl = Handle(STEPControl_Controller)::DownCast(XSDRAW::Controller());
  }

  if (argc < 3) {
    di<<"Give mode[1-4] and Shape name + optional file. Mode possible\n";
    di<<"f ou 1 : FacettedBRep        s ou 2 : ShellBasedSurfaceModel\n"
        <<"m ou 3 : ManifoldSolidBrep   w ou 4 : GeometricCurveSet/WireFrame\n";
    return 1; }
  char modeshape = argv[1][0];
  STEPControl_StepModelType mode;
  switch (modeshape) {
    case 'a' :
    case '0' : mode = STEPControl_AsIs;                    break;
    case 'f' :
    case '1' : mode = STEPControl_FacetedBrep;             break;
    case 's' :
    case '2' : mode = STEPControl_ShellBasedSurfaceModel;  break;
    case 'm' :
    case '3' : mode = STEPControl_ManifoldSolidBrep;       break;
    case 'w' :
    case '4' : mode = STEPControl_GeometricCurveSet;       break;
    default :  di<<"1st arg = mode, incorrect [give fsmw]\n"; return 1;
  }

  //:k8 abv 6 Jan 98: using parameter for writing mode (assemblies/shapes)
  Handle(STEPControl_ActorWrite) ActWrite = 
    Handle(STEPControl_ActorWrite)::DownCast ( ctl->ActorWrite() );
  if ( ! ActWrite.IsNull() ) 
    ActWrite->SetGroupMode (Interface_Static::IVal("write.step.assembly"));

  TopoDS_Shape shape = DBRep::Get(argv[2]);
  STEPControl_Writer sw (XSDRAW::Session(),Standard_False);
  Handle(Interface_InterfaceModel) stepmodel = sw.Model();
  Standard_Integer nbavant = (stepmodel.IsNull() ? 0 : stepmodel->NbEntities());

  Handle(Draw_ProgressIndicator) progress = new Draw_ProgressIndicator ( di, 1 );
  Message_ProgressScope aPSRoot (progress->Start(), "Translating", 100);
  progress->Show(aPSRoot);

  Standard_Integer stat = sw.Transfer (shape, mode, Standard_True, aPSRoot.Next(90));
  if (stat == IFSelect_RetDone)
  {
    di << "Translation: OK\n";
  } 
  else 
  {
    di << "Error: translation failed, status = " << stat << "\n";
  }

  if (aPSRoot.UserBreak())
    return 1;
  aPSRoot.SetName("Writing");
  progress->Show(aPSRoot);

//   Que s est-il passe
  stepmodel = sw.Model();
  Standard_Integer nbapres = (stepmodel.IsNull() ? 0 : stepmodel->NbEntities());
  if (nbavant > 0) di<<"Beware : Model not empty before transferring\n";
  if (nbapres <= nbavant) di<<"Beware : No data produced by this transfer\n";
  if (nbapres == 0) { di<<"No data to write\n"; return 0; }

  if (argc <= 3) {
    di<<" Now, to write a file, command : writeall filename\n";
    return 0;
  }

  const char *nomfic = argv[3];
  stat = sw.Write(nomfic);
  switch (stat) {
  case IFSelect_RetVoid : di<<"Error: No file written\n"; break;
    case IFSelect_RetDone : di<<"File "<<nomfic<<" written\n"; break;
    case IFSelect_RetStop : di<<"Error on writing file: no space on disk or destination is write protected\n"; break;
    default : di<<"Error: File "<<nomfic<<" written with fail messages\n"; break;
  }

  return 0;
}
//=======================================================================
//function : testwritestep
//purpose  : 
//=======================================================================
static Standard_Integer testwrite (Draw_Interpretor& di, Standard_Integer argc, const char** argv) 
{
  TCollection_AsciiString aFilePath;
  TopoDS_Shape aShape;
  bool toTestStream = false;
  for (Standard_Integer anArgIter = 1; anArgIter < argc; ++anArgIter)
  {
    TCollection_AsciiString anArgCase (argv[anArgIter]);
    anArgCase.LowerCase();
    if (anArgCase == "-stream")
    {
      toTestStream = true;
    }
    else if (aFilePath.IsEmpty())
    {
      aFilePath = argv[anArgIter];
    }
    else if (aShape.IsNull())
    {
      aShape = DBRep::Get (argv[anArgIter]);
      if (aShape.IsNull())
      {
        di << "Syntax error: '" << argv[anArgIter] << "' is not a shape";
        return 1;
      }
    }
    else
    {
      di << "Syntax error: unknown argument '" << argv[anArgIter] << "'";
      return 1;
    }
  }
  if (aShape.IsNull())
  {
    di << "Syntax error: wrong number of arguments";
    return 1;
  }

  STEPControl_Writer aWriter;
  IFSelect_ReturnStatus aStat = aWriter.Transfer (aShape, STEPControl_AsIs);
  if (aStat != IFSelect_RetDone)
  {
    di << "Error on transferring shape";
    return 1;
  }

  if (toTestStream)
  {
    std::ofstream aStream;
    OSD_OpenStream (aStream, aFilePath, std::ios::out | std::ios::binary);
    aStat = aWriter.WriteStream (aStream);
    aStream.close();
    if (!aStream.good()
      && aStat == IFSelect_RetDone)
    {
      aStat = IFSelect_RetFail;
    }
  }
  else
  {
    aStat = aWriter.Write (aFilePath.ToCString());
  }
  if (aStat != IFSelect_RetDone)
  {
    di << "Error on writing file";
    return 1; 
  }
  di << "File Is Written";
  return 0;
}

//=======================================================================
//function : countexpected
//purpose  : 
//=======================================================================

static Standard_Integer countexpected
  (Draw_Interpretor& di, Standard_Integer /*argc*/, const char** /*argv*/)
{
  Handle(IFSelect_SessionPilot) pilot = XSDRAW::Pilot();
  Handle(IFSelect_WorkSession) WS = pilot->Session();
   const Interface_Graph& graph = WS->Graph();
  
  Handle(TColStd_HSequenceOfTransient) roots = WS->GiveList("xst-transferrable-roots", "");
  STEPSelections_Counter cnt;
  
  for (Standard_Integer i =1 ; i <= roots->Length(); i++) {
    cnt.Count(graph,roots->Value(i));
  }
  
  di<< "Instances of Faces \t: "<<cnt.NbInstancesOfFaces()<<"\n";
  di<< "Instances of Shells\t: "<<cnt.NbInstancesOfShells()<<"\n";
  di<< "Instances of Solids\t: "<<cnt.NbInstancesOfSolids()<<"\n";
  di<< "Instances of Wires in GS\t: "<<cnt.NbInstancesOfWires()<<"\n";
  di<< "Instances of Edges in GS\t: "<<cnt.NbInstancesOfEdges()<<"\n";
  
  di<< "Source Faces \t: "<<cnt.NbSourceFaces()<<"\n";
  di<< "Source Shells\t: "<<cnt.NbSourceShells()<<"\n";
  di<< "Source Solids\t: "<<cnt.NbSourceSolids()<<"\n";
  di<< "Source Wires in GS\t: "<<cnt.NbSourceWires()<<"\n";
  di<< "Source Edges in GS\t: "<<cnt.NbSourceEdges()<<"\n";
  
  return 1;
}

static Standard_Integer dumpassembly
  (Draw_Interpretor& /*di*/, Standard_Integer /*argc*/, const char** /*argv*/)
{
  Handle(IFSelect_SessionPilot) pilot = XSDRAW::Pilot();
  Handle(IFSelect_WorkSession) WS = pilot->Session();
  const Interface_Graph& graph = WS->Graph();
  
  STEPSelections_AssemblyExplorer exp(graph);
  exp.Dump(std::cout);
  return 0;
}

static Standard_Integer stepfileunits (Draw_Interpretor& di, Standard_Integer argc, const char** argv) 
{

  if(  argc < 2)
  {
    di << "Error: Invalid number of parameters. Should be: getfileunits name_file\n";
    return 1;
  }
  STEPControl_Reader aStepReader;
  
  IFSelect_ReturnStatus readstat = IFSelect_RetVoid;
  readstat = aStepReader.ReadFile (argv[1]);
 
  if (readstat != IFSelect_RetDone) {
    
    di<<"No model loaded\n";
    return 1;
  }

  TColStd_SequenceOfAsciiString anUnitLengthNames;
  TColStd_SequenceOfAsciiString anUnitAngleNames;
  TColStd_SequenceOfAsciiString anUnitSolidAngleNames;
  aStepReader.FileUnits( anUnitLengthNames,anUnitAngleNames,anUnitSolidAngleNames);
                                   
  Standard_Integer i =1;
  di<<"=====================================================\n";
  di<<"LENGTH Unit\n";
  for( ; i <= anUnitLengthNames.Length() ; i++)
    di<<anUnitLengthNames(i).ToCString()<<"\n";
  
  di<<"=====================================================\n";
  di<<"Angle Unit\n";
  for( i =1 ; i <= anUnitAngleNames.Length() ; i++)
    di<<anUnitAngleNames(i).ToCString()<<"\n";

  di<<"=====================================================\n";
  di<<"Solid Angle Unit\n";
  for( i =1 ; i <= anUnitSolidAngleNames.Length() ; i++)
    di<<anUnitSolidAngleNames(i).ToCString()<<"\n";
  
  return 0;
}
//  ########  COMMANDE stepwrite : teste le Writer  #########

void XSDRAWSTEP::InitCommands (Draw_Interpretor& theCommands)
{
  const char* g = "DE: STEP";  // Step transfer file commands
  XSDRAWSTEP::Init();
  XSDRAW::LoadDraw(theCommands);
  theCommands.Add("stepwrite" ,    "stepwrite mode[0-4 afsmw] shape",  __FILE__, stepwrite,     g);
  theCommands.Add("testwritestep", "testwritestep filename.stp shape [-stream]",
                   __FILE__, testwrite, g);
  theCommands.Add("stepread",      "stepread  [file] [f or r (type of model full or reduced)]",__FILE__, stepread,      g);
  theCommands.Add("testreadstep",  "testreadstep file shape [-stream]",__FILE__, testreadstep,  g);
  theCommands.Add("steptrans",     "steptrans shape stepax1 stepax2",  __FILE__, steptrans,     g);
  theCommands.Add("countexpected","TEST",                              __FILE__, countexpected, g);
  theCommands.Add("dumpassembly", "TEST",                              __FILE__, dumpassembly,  g);
  theCommands.Add("stepfileunits" , "stepfileunits name_file", __FILE__, stepfileunits,      g);
}
