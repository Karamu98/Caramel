#ifndef _DEFINES_H__
#define _DEFINES_H__

extern int* g_ScreenWidth;
extern int* g_ScreenHeight;

#include "ComponentList.h"
#include <fstream>
#include "Log.h"

enum Flags : uint8_t
{
	ENTITYLIST_START,
	COMPONENTLIST_START,


	CAMERA_START,
	DIRLIGHT_START,
	PNTLIGHT_START,
	SPTLIGHT_START,
	MODEL_START
};


struct FileHeader
{
	Flags flag;
	size_t size;

};

//Component* CreateComponent(Entity* a_owner, Flags a_componentFlag)
//{
//	Component* newComp = nullptr;
//	switch (a_componentFlag)
//	{
//	case CAMERA_START:
//	{
//		Camera* newCam = new Camera(a_owner);
//		newComp = newCam;
//		break;
//	}
//	case DIRLIGHT_START:
//	{
//		DirectionalLight* newCam = new DirectionalLight(a_owner);
//		newComp = newCam;
//		break;
//	}
//	case PNTLIGHT_START:
//	{
//		PointLight* newCam = new PointLight(a_owner);
//		newComp = newCam;
//		break;
//	}
//	case SPTLIGHT_START:
//	{
//		SpotLight* newCam = new SpotLight(a_owner);
//		newComp = newCam;
//		break;
//	}
//	case MODEL_START:
//	{
//		MeshFilter* newCam = new MeshFilter(a_owner);
//		newComp = newCam;
//		break;
//	}
//	default:
//	{
//		CL_CORE_ERROR("Unknown component tag {0}.", a_componentFlag);
//		break;
//	}
//	}
//
//	return newComp;
//}

// Helper template to save typing!
template<typename T> 
void SaveToFile(std::ostream* a_outStream, T a_toSave)
{
	a_outStream->write((char*)& a_toSave, sizeof(T));
}

template<typename T>
void LoadFromFile(std::ifstream* a_inStream, T* a_toRead)
{
	a_inStream->read((char*)a_toRead, sizeof(T));
}




#endif