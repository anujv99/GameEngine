
#ifndef PREV_GAME_ENGINE
#error "This pch is only used by PrevGameEngine. If using this in engine please define or else don't use this pch in project other than PrevGameEngine"
#endif

#if !defined(ENGINE_OPENGL) && !defined(ENGINE_DIRECTX)
#error "Atleast one api is required to build the engine. User either OpenGL or DirectX"
#endif

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstdint>

#include <unordered_map>
#include <vector>
#include <array>
#include <list>

#include "common/types.h"
#include "common/log.h"
#include "common/assert.h"
#include "common/singleton.h"

#include "debug/instrumentor.h"