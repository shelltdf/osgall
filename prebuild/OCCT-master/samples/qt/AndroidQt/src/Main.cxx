// Copyright (c) 2014 OPEN CASCADE SAS
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

#include <Standard_WarningsDisable.hxx>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <Standard_WarningsRestore.hxx>

#include "AndroidQt.h"

#include <OSD_Environment.hxx>

int main(int argc, char *argv[])
{
#if defined(_WIN32) && (QT_VERSION > 0x050000)
  TCollection_AsciiString aPlugindsDirName = OSD_Environment ("QTDIR").Value();
  if (!aPlugindsDirName.IsEmpty())
    QApplication::addLibraryPath (QString (aPlugindsDirName.ToCString()) + "/plugins");
#endif

  QApplication app(argc, argv);

  qmlRegisterType<AndroidQt>("AndroidQt", 1, 0, "AndroidQt");

  QQmlApplicationEngine engine;
  engine.load (QUrl (QStringLiteral ("qrc:/main.qml")));

  return app.exec();
}
