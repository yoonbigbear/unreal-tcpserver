#ifndef _PCH_H_
#define _PCH_H_


#pragma comment(lib, "net.lib")

#include <net_pch.h>
#include "server/server_enum.h"

#include <DbgHelp.h>
#pragma comment(lib, "dbghelp.lib")

#include <box2d/box2d.h>
#pragma comment(lib, "box2d.lib")


#pragma comment(lib, "Detour-d.lib")
#pragma comment(lib, "DetourCrowd-d.lib")
#pragma comment(lib, "DetourTileCache-d.lib")

#include <protocol_generated.h>
#include <random>

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