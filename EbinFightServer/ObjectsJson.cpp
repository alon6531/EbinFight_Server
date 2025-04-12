#include "ObjectsJson.h"

namespace fs = std::filesystem;

ObjectsJson::ObjectsJson()
    : m_filePath("..\\Configs\\Objects.json")
{
  
    this->CreateJson();

}

bool ObjectsJson::CreateJson()
{
    if (!fs::exists(m_filePath)) {
        // Create the JSON data

        m_objectsData["game_objects"] = {
            {
                {"name", "exmple"},
                {"texture", "a.png"},
                {"position", {500, 60}},
                {"scale", {50, 50}},
                {"hitBoxComponent", {true, {0, 0}, {0, 0}}}  // Adjusted this part to be valid
            }
        };

        // Save to the file
        std::ofstream file(m_filePath);
        if (file.is_open()) {
            // Use dump(4) for pretty print with indentation
            file << m_objectsData.dump(4);  // 4 spaces of indentation for pretty print
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
                file >> m_objectsData;
                file.close();
                std::cout << "Objects data loaded successfully.\n";
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

bool ObjectsJson::SaveData()
{
    std::ofstream file(m_filePath);
    if (file.is_open()) {
        file << m_objectsData.dump(4);  // Pretty print with 4 spaces of indentation
        file.close();
        std::cout << "Data saved successfully to " << m_filePath << "\n";
        return true;
    }
    else {
        std::cerr << "Failed to open file for writing: " << m_filePath << "\n";
        return false;
    }
}
void ObjectsJson::AddObject(json object)
{
    // Check if the "game_objects" array exists, if not, create it
    if (m_objectsData.find("game_objects") == m_objectsData.end()) {
        this->CreateJson();
    }

    // Add the new object to the "game_objects" array
    m_objectsData["game_objects"].push_back(object);

    // Optionally, save the updated data to the file
    this->SaveData();

    std::cout << "New object added.\n";
}

const json ObjectsJson::GetAllObjects() const
{
    // Return the "game_objects" array from the JSON data
   
    return m_objectsData;
 
}
