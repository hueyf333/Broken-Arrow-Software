#pragma once

#include "core/UIElement.h"
#include <string>
#include <vector>
#include <memory>

struct ValidationIssue {
    enum class Severity { Error, Warning, Info };
    
    Severity severity;
    std::string message;
    UIElement* element;
};

class Validator {
public:
    std::vector<ValidationIssue> ValidateProject(
        const std::vector<std::shared_ptr<UIElement>>& rootElements
    );
    
private:
    void ValidateElement(UIElement* element, std::vector<ValidationIssue>& issues);
    bool CheckCyclicParenting(UIElement* element, UIElement* ancestor);
};
