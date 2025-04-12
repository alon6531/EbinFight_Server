#pragma once
class UsersJson
{
public:
	UsersJson();
	bool CreateJson();
	bool SaveData();
	void RemoveAllData(const std::string& item_name);
	void AddUser(const std::string& item_name, json tile);
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

