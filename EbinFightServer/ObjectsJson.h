#pragma once

#include "Globals.h"

class ObjectsJson
{
public:
	ObjectsJson();
	bool CreateJson();
	bool SaveData();
	void AddObject(json object);
	const json GetAllObjects() const;


private:
	std::string m_filePath;
	json m_objectsData;
};

