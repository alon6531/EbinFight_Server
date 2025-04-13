#pragma once

#include "Globals.h"

class PlayersJson
{
public:
	PlayersJson();
	bool CreateJson();
	bool LoadData();
	bool SaveData();
	void AutoSaveLoop();
	bool CheckIfPlayerExists(const std::string& m_player_name) const;
	void AddPlayer(const std::string& m_player_name, json player_data);
	void RemovePlayer(const std::string& m_player_name);
	void UpdatePlayer(const std::string& m_player_name, const json& new_player_data);

	const json& GetAllPlayers(const std::string& m_player_name) const;
	const json& GetPlayer(const std::string& m_player_name) const;


private:
	bool m_dirty;
	std::string m_filePath;
	json m_playerData;
};

