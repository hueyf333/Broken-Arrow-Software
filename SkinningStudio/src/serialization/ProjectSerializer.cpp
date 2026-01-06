#include "serialization/ProjectSerializer.h"
#include "widgets/Button.h"
#include "widgets/Label.h"
#include "widgets/Image.h"
#include "widgets/TextField.h"
#include "widgets/Checkbox.h"
#include "widgets/Slider.h"
#include "widgets/Container.h"
#include <fstream>
#include <filesystem>

bool ProjectSerializer::SaveProject(
    const std::string& filePath,
    const std::vector<std::shared_ptr<UIElement>>& rootElements,
    std::shared_ptr<Theme> theme
) {
    try {
        json projectJson;
        projectJson["version"] = "1.0";
        
        json elementsJson = json::array();
        for (const auto& element : rootElements) {
            elementsJson.push_back(element->Serialize());
        }
        projectJson["elements"] = elementsJson;
        
        projectJson["theme"] = theme->Serialize();
        
        std::ofstream file(filePath);
        if (!file.is_open()) return false;
        
        file << projectJson.dump(2);
        return true;
    } catch (...) {
        return false;
    }
}

bool ProjectSerializer::LoadProject(
    const std::string& filePath,
    std::vector<std::shared_ptr<UIElement>>& outRootElements,
    std::shared_ptr<Theme>& outTheme
) {
    try {
        std::ifstream file(filePath);
        if (!file.is_open()) return false;
        
        json projectJson;
        file >> projectJson;
        
        outRootElements.clear();
        
        if (projectJson.contains("elements")) {
            for (const auto& elementJson : projectJson["elements"]) {
                std::string type = elementJson["type"];
                auto element = CreateElementFromType(type);
                if (element) {
                    element->Deserialize(elementJson);
                    outRootElements.push_back(element);
                    
                    // Recursively load children
                    if (elementJson.contains("children")) {
                        std::function<void(std::shared_ptr<UIElement>, const json&)> loadChildren;
                        loadChildren = [&](std::shared_ptr<UIElement> parent, const json& childrenJson) {
                            for (const auto& childJson : childrenJson) {
                                std::string childType = childJson["type"];
                                auto child = CreateElementFromType(childType);
                                if (child) {
                                    child->Deserialize(childJson);
                                    parent->AddChild(child);
                                    if (childJson.contains("children")) {
                                        loadChildren(child, childJson["children"]);
                                    }
                                }
                            }
                        };
                        loadChildren(element, elementJson["children"]);
                    }
                }
            }
        }
        
        outTheme = std::make_shared<Theme>();
        if (projectJson.contains("theme")) {
            outTheme->Deserialize(projectJson["theme"]);
        } else {
            outTheme = Theme::CreateDefaultDarkTheme();
        }
        
        return true;
    } catch (...) {
        return false;
    }
}

bool ProjectSerializer::ExportTheme(const std::string& filePath, std::shared_ptr<Theme> theme) {
    try {
        json themeJson = theme->Serialize();
        
        std::ofstream file(filePath);
        if (!file.is_open()) return false;
        
        file << themeJson.dump(2);
        return true;
    } catch (...) {
        return false;
    }
}

std::shared_ptr<Theme> ProjectSerializer::ImportTheme(const std::string& filePath) {
    try {
        std::ifstream file(filePath);
        if (!file.is_open()) return nullptr;
        
        json themeJson;
        file >> themeJson;
        
        auto theme = std::make_shared<Theme>();
        theme->Deserialize(themeJson);
        return theme;
    } catch (...) {
        return nullptr;
    }
}

bool ProjectSerializer::ExportRuntimePackage(
    const std::string& outputDir,
    const std::vector<std::shared_ptr<UIElement>>& rootElements,
    std::shared_ptr<Theme> theme
) {
    try {
        std::filesystem::create_directories(outputDir);
        
        // Export theme
        std::string themePath = outputDir + "/theme.json";
        if (!ExportTheme(themePath, theme)) return false;
        
        // Export UI layout
        json layoutJson;
        layoutJson["version"] = "1.0";
        json elementsJson = json::array();
        for (const auto& element : rootElements) {
            elementsJson.push_back(element->Serialize());
        }
        layoutJson["elements"] = elementsJson;
        
        std::ofstream layoutFile(outputDir + "/ui_layout.json");
        if (!layoutFile.is_open()) return false;
        layoutFile << layoutJson.dump(2);
        
        return true;
    } catch (...) {
        return false;
    }
}

std::shared_ptr<UIElement> ProjectSerializer::CreateElementFromType(const std::string& type) {
    if (type == "Button") return std::make_shared<Button>();
    if (type == "Label") return std::make_shared<Label>();
    if (type == "Image") return std::make_shared<Image>();
    if (type == "TextField") return std::make_shared<TextField>();
    if (type == "Checkbox") return std::make_shared<Checkbox>();
    if (type == "Slider") return std::make_shared<Slider>();
    if (type == "ContainerVertical") return std::make_shared<ContainerVertical>();
    if (type == "ContainerHorizontal") return std::make_shared<ContainerHorizontal>();
    return nullptr;
}
