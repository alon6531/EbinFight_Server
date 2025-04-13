#include "PlayersJson.h"

namespace fs = std::filesystem;

PlayersJson::PlayersJson()
    : m_filePath("..\\Configs\\Players.json")
{
    this->CreateJson();
    this->LoadData();  // נטען את המידע הקיים
}

bool PlayersJson::CreateJson()
{
    if (!fs::exists(m_filePath)) {
        m_playerData = json::object();  // יצירה כאובייקט (map)
        std::ofstream file(m_filePath);
        if (file.is_open()) {
            file << m_playerData.dump(4);
            file.close();
            std::cout << "PlayersJson:JSON file created.\n";
            return true;
        }
        else {
            std::cerr << "PlayersJson:Failed to open file for writing.\n";
            return false;
        }
    }

    return false;
}

bool PlayersJson::LoadData()
{
    std::ifstream file(m_filePath);
    if (file.is_open()) {
        file >> m_playerData;
        file.close();
        return true;
    }
    else {
        std::cerr << "PlayersJson:Failed to open file for reading: " << m_filePath << "\n";
        return false;
    }
}

bool PlayersJson::SaveData()
{

    std::ofstream file(m_filePath);
    if (file.is_open()) {
        file << m_playerData.dump(4);
        file.close();
        std::cout << "PlayersJson:Data saved successfully to " << m_filePath << "\n";
        return true;
    }
    else {
        std::cerr << "PlayersJson:Failed to open file for writing: " << m_filePath << "\n";
        return false;
    }
}

void PlayersJson::AutoSaveLoop()
{
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        if (m_dirty) {
            SaveData();
            m_dirty = false;
        }
    }
}

bool PlayersJson::CheckIfPlayerExists(const std::string& m_player_name) const
{
    
    return m_playerData.contains(m_player_name);
}

void PlayersJson::AddPlayer(const std::string& m_player_name, json player_data)
{
    
    m_playerData[m_player_name] = player_data;

    m_dirty = true; // פלג שמתעדכן
    std::cout << "PlayersJson:New player added: " << m_player_name << "\n";
	
}

void PlayersJson::RemovePlayer(const std::string& m_player_name)
{
    if (CheckIfPlayerExists(m_player_name)) {
        m_playerData.erase(m_player_name);
        m_dirty = true; // פלג שמתעדכן
        std::cout << "PlayersJson:Player removed: " << m_player_name << "\n";
    }
    else {
        std::cerr << "PlayersJson:Player not found.\n";
    }
}

void PlayersJson::UpdatePlayer(const std::string& m_player_name, const json& new_player_data)
{
    if (CheckIfPlayerExists(m_player_name)) {
        json& existing_data = m_playerData[m_player_name];
        bool updated = false;

        for (auto& [key, value] : new_player_data.items()) {
            if (!existing_data.contains(key) || existing_data[key] != value) {
                existing_data[key] = value;
                updated = true;
            }
        }

        if (updated) {
            m_dirty = true; // פלג שמתעדכן
            std::cout << "PlayersJson:Player updated: " << m_player_name << "\n";
        }
    }
}

const json& PlayersJson::GetAllPlayers(const std::string& m_player_name) const
{
    return m_playerData;
}

const json& PlayersJson::GetPlayer(const std::string& m_player_name) const
{
    if (CheckIfPlayerExists(m_player_name)) {
        return m_playerData.at(m_player_name);
    }

    throw std::runtime_error("Player does not exist.");
}