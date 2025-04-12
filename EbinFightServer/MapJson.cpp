#include "MapJson.h"

namespace fs = std::filesystem;

MapJson::MapJson() 
    : m_filePath("..\\Configs\\Maps.json")

{
	this->CreateJson();
}

bool MapJson::CreateJson()
{
    if (!fs::exists(m_filePath)) {
        // Create the JSON data

        m_mapData["map1"] = {
         {
             { "tiles" , {

                 {
                  {"texture_pos", {16, 16}},
                  {"position", {500, 60}},
                  {"scale", {0.5, 0.5}}
                 }

                }
             },
             { "objects" , {

                 {
                  {"name" , "player"},
                  {"texture", "a.png"},
                  {"position", {500, 60}},
                  {"scale", {50, 50}},
                  {"hitBoxComponent", {true,{50, 50}, {50, 50}} }

                 }


                }
              }




         }
        };

        // Save to the file
        std::ofstream file(m_filePath);
        if (file.is_open()) {
            // Use dump(4) for pretty print with indentation
            file << m_mapData.dump(4);  // 4 spaces of indentation for pretty print
            file.close();
            std::cout << "JSON file created.\n";
            return true;
        }
        else {
            std::cerr << "Failed to open file for writing.\n";
        }
    }
    else {
        std::ifstream file(m_filePath);
        if (file.is_open()) {
            // Parse the file into m_objectsData
            try {
                file >> m_mapData;
                file.close();
                return true;
            }
            catch (const std::exception& e) {
                std::cerr << "Error parsing the file: " << e.what() << "\n";
            }
        }
        else {
            std::cerr << "Failed to open the file for reading: " << m_filePath << "\n";
        }
    }
    return false;
}

bool MapJson::SaveData()
{
    std::ofstream file(m_filePath);
    if (file.is_open()) {
        file << m_mapData.dump(4);  // Pretty print with 4 spaces of indentation
        file.close();
        std::cout << "Data saved successfully to " << m_filePath << "\n";
        return true;
    }
    else {
        std::cerr << "Failed to open file for writing: " << m_filePath << "\n";
        return false;
    }
}

void MapJson::RemoveAllDataFromMap(const std::string& item_name)
{
    // Ensure the map entry exists and is an array with at least one object
    if (!m_mapData.contains(item_name) || m_mapData[item_name].empty()) {
        std::cerr << "Invalid map data for: " << item_name << std::endl;
        return;
    }

    // Get reference to the first map entry (usually index 0)
    auto& mapEntry = m_mapData[item_name];

    // Ensure "tiles" exists and is an array, then clear it
    if (mapEntry.contains("tiles") && mapEntry["tiles"].is_array() && mapEntry.contains("objects") && mapEntry["objects"].is_array()) {
        mapEntry["tiles"].clear();
		mapEntry["objects"].clear(); // Clear the objects array as well
        std::cout << "All tiles removed from " << item_name << ".\n";
    }
    else {
        std::cerr << "No tiles found in " << item_name << ".\n";
    }

    // Save the updated data
    this->SaveData();
}

void MapJson::AddTile(const std::string& item_name, json tile)
{
    // Check if the map entry exists and is an array with at least one object
    if (!m_mapData.contains(item_name) || m_mapData[item_name].empty()) {
        std::cerr << "Invalid map data for: " << item_name << std::endl;
        return;
    }

    // Get reference to the first map entry (usually index 0)
    auto& mapEntry = m_mapData[item_name];

    // Ensure "tiles" exists and is an array
    if (!mapEntry.contains("tiles") || !mapEntry["tiles"].is_array()) {
        mapEntry["tiles"] = json::array(); // Create the array if missing
    }

    // Add the new tile
    mapEntry["tiles"].push_back(tile);

    // Save the updated data
    this->SaveData();

    std::cout << "New tile added to " << item_name << ".\n";
}

void MapJson::AddObject(const std::string& item_name, json object)
{
    // בדיקה שה־map קיים וכולל מערך כלשהו
    if (!m_mapData.contains(item_name) || m_mapData[item_name].empty()) {
        std::cerr << "Invalid map data for: " << item_name << std::endl;
        return;
    }

    // קבלת התייחסות לאובייקט הראשון של המפה
    auto& mapEntry = m_mapData[item_name];

    // ודא שקיים שדה "objects" והוא מערך
    if (!mapEntry.contains("objects") || !mapEntry["objects"].is_array()) {
        mapEntry["objects"] = json::array();  // צור מערך חדש אם חסר
    }

    // הוסף את האובייקט למערך
    mapEntry["objects"].push_back(object);

    // שמירה לקובץ
    this->SaveData();

    std::cout << "New object added to " << item_name << ".\n";
}

void MapJson::AddMap(const std::string& item_name)
{
    // Check if the map already exists
    if (m_mapData.contains(item_name)) {
        std::cerr << "Map " << item_name << " already exists.\n";
        return;
    }

    // Create a default structure for the new map with "tiles" and "objects"
    m_mapData[item_name] = {
        
        {"tiles", json::array()},
        {"objects", json::array()}
        
    };

    std::cout << "Map " << item_name << " has been added with default tiles and objects.\n";

    // Optionally, save the updated data to the file
    this->SaveData();
}

void MapJson::RemoveMap(const std::string& item_name)
{
    // Check if the map entry exists
    if (m_mapData.contains(item_name)) {
        // Remove the map entry
        m_mapData.erase(item_name);
        std::cout << "Map " << item_name << " has been removed.\n";

        // Optionally, save the updated data to the file
        this->SaveData();
    }
    else {
        std::cerr << "Map " << item_name << " not found.\n";
    }
}

const json& MapJson::GetTiles(const std::string& item_name) const
{
    const auto& mapEntry = m_mapData[item_name]; // First item in the array
    if (mapEntry.contains("tiles")) {
        return mapEntry["tiles"];
    }
    return json::array();
}

const json& MapJson::GetObjects(const std::string& item_name) const
{
    const auto& mapEntry = m_mapData[item_name]; // First item in the array
    if (mapEntry.contains("objects")) {
        return mapEntry["objects"];
    }
    return json::array();
}

const json& MapJson::GetMapData(const std::string& item_name) const
{
    return m_mapData[item_name];
}

const json& MapJson::GetAllMapsData() const
{
    return m_mapData;
}


