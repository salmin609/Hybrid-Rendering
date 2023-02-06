#include "PositionConverter.h"


#include "Graphic.h"
#include "Projection.h"

namespace PositionConverter
{
	Point GetMousePosInWorldCoord(float x, float y)
	{
		Hcoord windowWH = Graphic::instance->GetWindowWH();
		int windowW = static_cast<int>(windowWH.x);
		int windowH = static_cast<int>(windowWH.y);

		Point curr{ static_cast<float>(x), static_cast<float>(y), 1 };
		curr = DeviceToNdc(windowW, windowH) * curr;
		curr = NDCToCamera(*CameraManager::instance->GetCamera()) * curr;
		curr = CameraToWorld(*CameraManager::instance->GetCamera()) * curr;
		return curr;
	}
}
