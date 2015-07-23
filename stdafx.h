// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO: reference additional headers your program requires here
#include <GL/glut.h>
#include <gl/glext.h>
#include <string>
#include <fstream>
#include <vector>
#include <stack>
#include <map>
#include <limits>

#define _USE_MATH_DEFINES
#include <math.h>

#include <cstring>

//relative path
#include "Camera.h"
#include "PolyModel.h"
#include "Direction_Light.h"
#include "GTranformation.h"
#include "Light.h"
#include "Mat4.h"
#include "Material.h"
#include "Omni_Light.h"

//mapping
#include "TgaTexture.h"
#include "UVW_Map.h"

#include "Scan_conversion.h"
#include "Vec2.h"
#include "Vec3.h"
#include "Z_Buffer.h"


