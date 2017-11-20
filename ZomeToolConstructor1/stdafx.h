#ifndef STDAFX
#define STDAFX

#include <GL/glew.h>
//#include <GL/freeglut.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <iostream>
#include <math.h>
#include "MathHelper.h"	
#include <time.h>
#include <stdlib.h>


#define GRID_SCALE 5
#define GRID_THICKNESS 2
#define M_PI 3.14159265358979323846
#define SOCKET_RADIUS 12.0
#define SMALL_NUMBER 0.0001
#define TRUSS_RADIUS 3.0
#define DISTANCE_RELAX 2000.0
#define SURFACE_THICKNESS 3
#define CONE_ENGAGED_DISTANCE 2//結構距離表面多遠
#define SEGMENT_WEIGHT 30.0
#define YOUNG_MODULUS 1.	// PVC-U的彈性模數3040MPa，換算成 310204.0816 g/mm2 (乘1000/9.8)
									// 把模樹改成 310.2040816 看看

#define REARRANGE_TIMES 3

#define MAKE_CONE 0
#define MAKE_OUTERCONE 1//還會有受力分析
#define MAKE_BRANCH 2
#define MAKE_BRANCH_END 3
#define MAKE_SEGMENT 4
#define SELFDEFINE_STRUCTURE 9

#define CONSTRUCTOR_MODE 9

#define STRUCTURAL_ANALYSIS true
#define FIX_FAILURE_POINT true
#define SOLID_CONE false

#define FORCE_SCALE 1. //輸出受力的顯示比例

#endif