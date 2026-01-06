#include "tools/Validator.h"
#include <unordered_set>

std::vector<ValidationIssue> Validator::ValidateProject(
    const std::vector<std::shared_ptr<UIElement>>& rootElements
) {
    std::vector<ValidationIssue> issues;
    std::unordered_set<std::string> usedIds;
    
    for (const auto& root : rootElements) {
        ValidateElement(root.get(), issues);
        
        // Check for duplicate IDs
        std::function<void(UIElement*)> checkIds = [&](UIElement* elem) {
            if (usedIds.count(elem->id)) {
                issues.push_back({
                    ValidationIssue::Severity::Error,
                    "Duplicate ID: " + elem->id,
                    elem
                });
            } else {
                usedIds.insert(elem->id);
            }
            
            for (auto& child : elem->children) {
                checkIds(child.get());
            }
        };
        checkIds(root.get());
    }
    
    return issues;
}

void Validator::ValidateElement(UIElement* element, std::vector<ValidationIssue>& issues) {
    if (!element) return;
    
    // Check for empty ID
    if (element->id.empty()) {
        issues.push_back({
            ValidationIssue::Severity::Error,
            "Element has empty ID",
            element
        });
    }
    
    // Check for cyclic parenting
    if (CheckCyclicParenting(element, element->parent)) {
        issues.push_back({
            ValidationIssue::Severity::Error,
            "Cyclic parent-child relationship detected: " + element->name,
            element
        });
    }
    
    // Validate children
    for (auto& child : element->children) {
        ValidateElement(child.get(), issues);
    }
}

bool Validator::CheckCyclicParenting(UIElement* element, UIElement* ancestor) {
    if (!element || !ancestor) return false;
    
    UIElement* current = ancestor;
    while (current) {
        if (current == element) return true;
        current = current->parent;
    }
    
    return false;
}
