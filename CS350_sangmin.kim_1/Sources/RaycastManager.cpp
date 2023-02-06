#include "RaycastManager.h"
#include "Graphic.h"
#include "PositionConverter.h"

RaycastManager::RaycastManager()
{
	
}

Object* RaycastManager::RayToObject(Point mousePosWorld) const
{
	bool is_one_obj_selected = false;
	const std::vector<Object*> objVec = Graphic::objects;
	const size_t objectsSize = objVec.size();
	
	//looping for all objects.1
	for (size_t i = 0; i < objectsSize; ++i)
	{
		Object* obj = objVec[i];
		float radius = obj->Get_Scale().x;
		float radius_squared = radius * radius;

		Point obj_center = obj->Get_Obj_Pos();
		Point e_w = CameraManager::instance->CameraPos();

		Vector C_E = obj_center - e_w;
		Vector C_P = obj_center - mousePosWorld;
		float c_e_dot = dot(C_E, C_E);
		float c_p_dot = dot(C_P, C_P);

		//trivial acceptance
		if (c_e_dot <= radius_squared && c_p_dot <= radius_squared)
		{
			return obj;
		}

		//trivial rejection
		//check if e is outside
		if (c_e_dot > radius_squared)
		{
			Vector q = mousePosWorld - e_w;
			float p = dot(C_E, q);

			if (p > 0)
			{
				float m_under = abs(q);
				float m_upper = p;
				float m = m_upper / m_under;
				float abs_c_e = abs(C_E);
				float n_squared = (abs_c_e * abs_c_e) - (m * m);

				if ((n_squared > radius_squared) && m > 0)
				{
					if (i == objectsSize - 1)
					{
						if (!is_one_obj_selected)
							return nullptr;
					}
				}
				else
				{
					float s = sqrt(radius_squared - n_squared);
					float te = m - s;
					float t = te / m_under;
					obj->Set_T(t);
					obj->Set_Selected(true);
					is_one_obj_selected = true;
				}
			}
			else
			{
				//if checking last object of objects, ray is opposite direction,
				//return nullptr.
				if (i == objectsSize - 1)
				{
					if (!is_one_obj_selected)
						return nullptr;
				}
			}
		}
	}

	if (!is_one_obj_selected)
	{
		return nullptr;
	}
	else
	{
		//Find smallest t object.
		size_t smallest_t_index = 0;

		for (size_t i = 0; i < objectsSize ; ++i)
		{
			Object* obj = objVec[i];

			Object* smallest_t_index_obj = objVec[smallest_t_index];

			if (obj->Get_Selected() == true)
			{
				if (smallest_t_index_obj->Get_Selected() == true)
				{
					if (obj->Get_T() < smallest_t_index_obj->Get_T())
					{
						smallest_t_index = i;
					}
				}
				else
				{
					smallest_t_index_obj = obj;
					smallest_t_index = i;
				}
			}
		}
		//After set return value, set all objects selected false;
		for (size_t i = 0; i < objectsSize; ++i)
		{
			objVec[i]->Set_Selected(false);
		}

		return objVec[smallest_t_index];
	}
}

void RaycastManager::MoveCastedObj(Object* selected)
{
	if (selected != nullptr)
	{
		float speed = 20.f;
		int newx = 0, newy = 0;
		const Point prevMousePos = Graphic::instance->GetPrevMousePos();
		
		SDL_GetMouseState(&newx, &newy);
		const Point curr = PositionConverter::GetMousePosInWorldCoord(static_cast<float>(newx), static_cast<float>(newy));
		const Point prev = PositionConverter::GetMousePosInWorldCoord(prevMousePos.x, prevMousePos.y);
		Point& obj_pos = selected->Get_Obj_Pos();
		obj_pos.x += ((curr.x - prev.x) * speed);
		obj_pos.y += ((curr.y - prev.y) * speed);
		obj_pos.z += ((curr.z - prev.z) * speed);
	}
}
