#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#include <iostream>
#include <stdio.h>
#include <memory>
#include <string>
#include <vector>
#include <iterator>
#include <map>
#include <algorithm>
#include <functional>
#include <stdlib.h>

#define __WINDOWS

#ifdef __WINDOWS
	#include <crtdbg.h>
	#include <windows.h>
#endif
using namespace std;