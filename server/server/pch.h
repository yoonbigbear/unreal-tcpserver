#ifndef _PCH_H_
#define _PCH_H_


#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#pragma comment(lib, "net.lib")

#include <net_pch.h>
#include "server/server_enum.h"

#include <DbgHelp.h>
#pragma comment(lib, "dbghelp.lib")

#include <box2d/box2d.h>
#pragma comment(lib, "box2d.lib")

#include <protocol_generated.h>

using flatbuffer = flatbuffers::FlatBufferBuilder;

class AI;
using AIPtr = std::shared_ptr<AI>;
class Character;
using CharacterPtr = std::shared_ptr<Character>;
class Transform;
using TransformPtr = std::shared_ptr<Transform>;
class GameObject;
using GameObjectPtr = std::shared_ptr<GameObject>;
class Npc;
using NpcPtr = std::shared_ptr<Npc>;



#endif // !_PCH_H_