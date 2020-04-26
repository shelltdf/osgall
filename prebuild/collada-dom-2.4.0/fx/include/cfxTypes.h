/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
#ifndef _CFX_TYPES_H
#define _CFX_TYPES_H

#ifdef WIN32
#define NORMAL_OS	1
#endif

#ifdef LINUX
#undef NORMAL_OS	
#endif

#ifdef __APPLE__
#undef NORMAL_OS
#endif


typedef int cfxBool;
typedef unsigned int cfxUint;
typedef int cfxInt;
typedef int cfxError;
typedef float cfxFloat;
typedef double cfxDouble;

typedef struct {
  cfxBool b0;
} cfxBool1;

typedef struct {
  cfxBool b0;
  cfxBool b1;
} cfxBool2;

typedef struct {
  cfxBool b0;
  cfxBool b1;
  cfxBool b2;
} cfxBool3;

typedef struct {
  cfxBool b0;
  cfxBool b1;
  cfxBool b2;
  cfxBool b3;
} cfxBool4;

typedef struct {
  cfxInt i0;
} cfxInt1;

typedef struct {
  cfxInt i0;
  cfxInt i1;
} cfxInt2;

typedef struct {
  cfxInt i0;
  cfxInt i1;
  cfxInt i2;
} cfxInt3;

typedef struct {
  cfxInt i0;
  cfxInt i1;
  cfxInt i2;
  cfxInt i3;
} cfxInt4;

typedef struct {
  cfxFloat f0;
} cfxFloat1;

typedef struct {
  cfxFloat f0;
  cfxFloat f1;
} cfxFloat2;

typedef struct {
  cfxFloat f0;
  cfxFloat f1;
  cfxFloat f2;
} cfxFloat3;

typedef struct {
  cfxFloat f0;
  cfxFloat f1;
  cfxFloat f2;
  cfxFloat f3;
} cfxFloat4;


typedef struct {
  cfxFloat f00;
  cfxFloat f01;
  cfxFloat f10;
  cfxFloat f11;
} cfxFloat2x2;

typedef struct {
  cfxFloat f00;
  cfxFloat f01;
  cfxFloat f02;
  cfxFloat f10;
  cfxFloat f11;
  cfxFloat f12;
  cfxFloat f20;
  cfxFloat f21;
  cfxFloat f22;
} cfxFloat3x3;

typedef struct {
  cfxFloat f00;
  cfxFloat f01;
  cfxFloat f02;
  cfxFloat f03;
  cfxFloat f10;
  cfxFloat f11;
  cfxFloat f12;
  cfxFloat f13;
  cfxFloat f20;
  cfxFloat f21;
  cfxFloat f22;
  cfxFloat f23;
  cfxFloat f30;
  cfxFloat f31;
  cfxFloat f32;
  cfxFloat f33;
} cfxFloat4x4;


#endif // _CFX_TYPES_H
