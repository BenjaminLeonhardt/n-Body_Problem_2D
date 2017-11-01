// stdafx.h: Includedatei f�r Standardsystem-Includedateien
// oder h�ufig verwendete projektspezifische Includedateien,
// die nur in unregelm��igen Abst�nden ge�ndert werden.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Selten verwendete Komponenten aus Windows-Headern ausschlie�en
// Windows-Headerdateien:
#include <windows.h>

// C RunTime-Headerdateien
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include "Graphics.h"
#include "dreieck.h"
#include <vector>
#include <chrono>
#include <thread>
#include "Body.h"

#include <d2d1.h>
#include <iostream>
#include <exception>
#include <Dwrite.h>
#include <mutex>



// TODO: Hier auf zus�tzliche Header, die das Programm erfordert, verweisen.
