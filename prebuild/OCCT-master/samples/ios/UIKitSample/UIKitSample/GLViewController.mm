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

#import <Foundation/Foundation.h>

#import "GLViewController.h"
#import "GLView.h"

@implementation GLViewController

// =======================================================================
// function : init
// purpose  :
// =======================================================================
- (id) init
{
  self = [super init];
  
  if (self) {
    myOcctViewer = new OcctViewer();
  }
  
  return self;
}

// =======================================================================
// function : Draw
// purpose  :
// =======================================================================
- (void) Draw
{
}

// =======================================================================
// function : Setup
// purpose  :
// =======================================================================
- (void) Setup {
  if (!myOcctViewer->InitViewer(self.view)) {
    NSLog(@"Failed to init viewer");
  }
  else {
    [self importScrew:nullptr];
  }
}

// =======================================================================
// function : loadView
// purpose  :
// =======================================================================
- (void) loadView
{
  GLView* aGLView = [[GLView alloc] init];
  aGLView->myController = self;
  self.view = aGLView;
}

// =======================================================================
// function : touchesBegan
// purpose  :
// =======================================================================
- (void)touchesBegan:(NSSet *)theTouches withEvent:(UIEvent *)theEvent
{
  [super touchesBegan:theTouches withEvent:theEvent];
  
  UITouch *aTouch = [theTouches anyObject];
  if (aTouch != NULL) {
    CGPoint aTouchPoint = [aTouch locationInView:self.view];
    myOcctViewer->StartRotation((int)aTouchPoint.x, (int)aTouchPoint.y);
  }
}

// =======================================================================
// function : touchesMoved
// purpose  :
// =======================================================================
- (void)touchesMoved:(NSSet *)theTouches withEvent:(UIEvent *)theEvent
{
  [super touchesMoved:theTouches withEvent:theEvent];
  
  UITouch *aTouch = [theTouches anyObject];
  if (aTouch != NULL) {
    CGPoint aTouchPoint = [aTouch locationInView:self.view];
    myOcctViewer->Rotation((int)aTouchPoint.x, (int)aTouchPoint.y);
  }
  
  return;
}

// =======================================================================
// function : viewDidLoad
// purpose  :
// =======================================================================
-(void)viewDidLoad
{
  // add zoom recognizer
  UIPinchGestureRecognizer *aZoomRecognizer = [[UIPinchGestureRecognizer alloc]
                                               initWithTarget:self
                                               action:@selector(zoomHandler:)];
  
  [[self view] addGestureRecognizer:aZoomRecognizer];
  
  // add pan recognizer
  UIPanGestureRecognizer *aPanRecognizer = [[UIPanGestureRecognizer alloc]
                                            initWithTarget:self
                                            action:@selector(panHandler:)];
  
  aPanRecognizer.maximumNumberOfTouches = 2;
  aPanRecognizer.minimumNumberOfTouches = 2;
  
  [[self view] addGestureRecognizer:aPanRecognizer];
  
  UITapGestureRecognizer *aTapRecognizer = [[UITapGestureRecognizer alloc]
                                            initWithTarget:self
                                            action:@selector(tapHandler:)];
  
  [[self view] addGestureRecognizer:aTapRecognizer];
  
  
  // add import buttons
  UIBarButtonItem *importScrewBtn = [[UIBarButtonItem alloc]
                                     initWithTitle:@"Sample 1"
                                     style:UIBarButtonItemStylePlain
                                     target:self
                                     action:@selector(importScrew:)];
  
  UIBarButtonItem *importLinkrodsBtn = [[UIBarButtonItem alloc]
                                        initWithTitle:@"Sample 2"
                                        style:UIBarButtonItemStylePlain
                                        target:self
                                        action:@selector(importLinkrods:)];
  
  UIBarButtonItem *displayAboutDlgBtn = [[UIBarButtonItem alloc]
                                         initWithTitle:@"About"
                                         style:UIBarButtonItemStylePlain
                                         target:self
                                         action:@selector(displayAboutDlg:)];
  
  [self.navigationItem setLeftBarButtonItems:[NSArray arrayWithObjects:importScrewBtn, importLinkrodsBtn, nil]];
  [self.navigationItem setRightBarButtonItem: displayAboutDlgBtn];
}

// =======================================================================
// function : zoomHandler
// purpose  :
// =======================================================================
- (void)zoomHandler:(UIPinchGestureRecognizer *)pinchRecognizer
{
  if ([pinchRecognizer numberOfTouches] > 1)
  {
    UIGestureRecognizerState aState = [pinchRecognizer state];
    if (aState == UIGestureRecognizerStateBegan)
    {
      myFirstTouch[0] = [pinchRecognizer locationOfTouch:0 inView:self.view];
      myFirstTouch[1] = [pinchRecognizer locationOfTouch:1 inView:self.view];
    }
    else if (aState == UIGestureRecognizerStateChanged) {
      CGPoint aLastTouch[2] = {
        [pinchRecognizer locationOfTouch:0 inView:self.view],
        [pinchRecognizer locationOfTouch:1 inView:self.view]
      };
      
      double aPinchCenterXStart = ( myFirstTouch[0].x + myFirstTouch[1].x ) / 2.0;
      double aPinchCenterYStart = ( myFirstTouch[0].y + myFirstTouch[1].y ) / 2.0;
      
      double aStartDist = Sqrt( ( myFirstTouch[0].x - myFirstTouch[1].x ) * ( myFirstTouch[0].x - myFirstTouch[1].x ) +
                              ( myFirstTouch[0].y - myFirstTouch[1].y ) * ( myFirstTouch[0].y - myFirstTouch[1].y ) );
      double anEndDist = Sqrt( ( aLastTouch[0].x - aLastTouch[1].x ) * ( aLastTouch[0].x - aLastTouch[1].x ) +
                            ( aLastTouch[0].y - aLastTouch[1].y ) * ( aLastTouch[0].y - aLastTouch[1].y ) );
      
      double aDeltaDist = anEndDist - aStartDist;
      
      myOcctViewer->Zoom(aPinchCenterXStart, aPinchCenterYStart, aDeltaDist);
      
      myFirstTouch[0] = aLastTouch[0];
      myFirstTouch[1] = aLastTouch[1];
    }
  }
}

// =======================================================================
// function : panHandler
// purpose  :
// =======================================================================
- (void)panHandler:(UIPanGestureRecognizer *)panRecognizer
{
  if ([panRecognizer numberOfTouches] > 1)
  {
    UIGestureRecognizerState aState = [panRecognizer state];
    if (aState == UIGestureRecognizerStateBegan)
    {
      myFirstTouch[0] = [panRecognizer locationOfTouch:0 inView:self.view];
      myFirstTouch[1] = [panRecognizer locationOfTouch:1 inView:self.view];
    }
    else if (aState == UIGestureRecognizerStateChanged) {
      CGPoint aLastTouch[2] = {
        [panRecognizer locationOfTouch:0 inView:self.view],
        [panRecognizer locationOfTouch:1 inView:self.view]
      };
      
      double aPinchCenterXStart = ( myFirstTouch[0].x + myFirstTouch[1].x ) / 2.0;
      double aPinchCenterYStart = ( myFirstTouch[0].y + myFirstTouch[1].y ) / 2.0;
      
      double aPinchCenterXEnd = ( aLastTouch[0].x + aLastTouch[1].x ) / 2.0;
      double aPinchCenterYEnd = ( aLastTouch[0].y + aLastTouch[1].y ) / 2.0;
      
      double aPinchCenterXDev = aPinchCenterXEnd - aPinchCenterXStart;
      double aPinchCenterYDev = aPinchCenterYEnd - aPinchCenterYStart;
      
      myOcctViewer->Pan((int)aPinchCenterXDev, (int)-aPinchCenterYDev);
    }
  }
}

// =======================================================================
// function : tapHandler
// purpose  :
// =======================================================================
- (void)tapHandler:(UITapGestureRecognizer *)tapRecognizer
{
  CGPoint aTapPoint = [tapRecognizer locationInView:self.view];
  myOcctViewer->Select(aTapPoint.x, aTapPoint.y);
}

// =======================================================================
// function : importScrew
// purpose  :
// =======================================================================
- (void)importScrew:(UIBarButtonItem *)theSender
{
  NSString* aNsPath = [[NSBundle mainBundle] pathForResource:@"screw"
                                                      ofType:@"step"];
  std::string aPath = std::string([aNsPath UTF8String]);
  
  myOcctViewer->ImportSTEP(aPath);
  myOcctViewer->FitAll();
}

// =======================================================================
// function : importLinkrods
// purpose  :
// =======================================================================
- (void)importLinkrods:(UIBarButtonItem *)theSender
{
  NSString* aNsPath = [[NSBundle mainBundle] pathForResource:@"linkrods"
                                                      ofType:@"step"];
  std::string aPath = std::string([aNsPath UTF8String]);
  
  myOcctViewer->ImportSTEP(aPath);
  myOcctViewer->FitAll();
}

// =======================================================================
// function : displayAboutDlg
// purpose  :
// =======================================================================
- (void)displayAboutDlg:(UIBarButtonItem *)theSender
{
  UIAlertController* anAbout = [UIAlertController alertControllerWithTitle:@"About"
                                message:@"UIKit based application for tutorial to Open CASCADE Technology.\n\n"
                                      @"Copyright (c) 2017 OPEN CASCADE SAS"
                                preferredStyle:UIAlertControllerStyleAlert];
  
  UIAlertAction* aDefaultAction = [UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault
                                                         handler:^(UIAlertAction * action) {}];
  
  [anAbout addAction:aDefaultAction];
  [self presentViewController:anAbout animated:YES completion:nil];
}

@end
