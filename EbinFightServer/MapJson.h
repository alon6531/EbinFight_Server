#pragma once

#include "Globals.h"

class MapJson
{
public:
	MapJson();
	bool CreateJson();
	bool SaveData();
	void RemoveAllDataFromMap(const std::string& item_name);
	void AddTile(const std::string& item_name, json tile);
	void AddObject(const std::string& item_name, json object);
	void AddMap(const std::string& item_name);
	void RemoveMap(const std::string& item_name);

	const json& GetTiles(const std::string& item_name) const;
	const json& GetObjects(const std::string& item_name) const;

	const json& GetMapData(const std::string& item_name) const;

	const json& GetAllMapsData() const;


private:
	std::string m_filePath;
	json m_mapData;
};