#pragma once
#include "./Bifrost/NVB.h"
#include "../aimahead/memory/memory.h"
#include "./offsets.h"

namespace SDK {
	typedef NVB::CBifrostClient** (*_nvbCreateClient)();
	NVB::CBifrostClient* nvbCreateClient();
}