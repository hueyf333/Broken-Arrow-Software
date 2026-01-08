#ifndef PROJECTMODEL_H
#define PROJECTMODEL_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QList>
#include <QColor>
#include <QRectF>
#include <memory>

// Forward declarations
struct Layer;
struct KnobParameters;
struct ControlDefinition;

// Layer definition for knob rendering
struct Layer {
    QString name;
    QColor color;
    QColor gradientColor;
    bool useGradient = false;
    double opacity = 1.0;
    bool visible = true;
    int order = 0;
    
    QJsonObject toJson() const;
    static Layer fromJson(const QJsonObject& json);
};

// Knob rendering parameters
struct KnobParameters {
    int canvasWidth = 256;
    int canvasHeight = 256;
    int frameCount = 64;
    double startAngle = -135.0;
    double endAngle = 135.0;
    int innerRadius = 30;
    int outerRadius = 100;
    int tickCount = 11;
    QString tickStyle = "Line"; // Line, Dot, None
    QString pointerStyle = "Line"; // Line, Arrow, Triangle
    bool enableShadow = true;
    int shadowBlur = 10;
    double shadowOpacity = 0.5;
    
    QList<Layer> layers;
    
    QJsonObject toJson() const;
    static KnobParameters fromJson(const QJsonObject& json);
};

// Control types in layout
enum class ControlType {
    Knob,
    SliderHorizontal,
    SliderVertical,
    ButtonToggle,
    ButtonMomentary,
    SwitchMultiState,
    MeterSegmented
};

// Control definition in layout
struct ControlDefinition {
    QString id;
    QString name;
    ControlType type;
    QRectF bounds;
    QString imageSource;
    bool isFilmstrip = true;
    int frameCount = 1;
    QString hitTestShape = "Rectangle"; // Rectangle, Ellipse, Path
    QJsonObject metadata;
    
    QJsonObject toJson() const;
    static ControlDefinition fromJson(const QJsonObject& json);
};

// Layout parameters
struct LayoutParameters {
    int width = 800;
    int height = 600;
    int gridSize = 10;
    bool snapToGrid = true;
    QList<ControlDefinition> controls;
    
    QJsonObject toJson() const;
    static LayoutParameters fromJson(const QJsonObject& json);
};

// Main project model
class ProjectModel {
public:
    ProjectModel();
    
    // Project metadata
    QString projectName;
    QString projectPath;
    int version = 1;
    
    // Design data
    KnobParameters knobParams;
    LayoutParameters layoutParams;
    
    // Persistence
    bool loadFromFile(const QString& filePath);
    bool saveToFile(const QString& filePath);
    QJsonObject toJson() const;
    bool fromJson(const QJsonObject& json);
    
    // State management
    bool isModified() const { return m_modified; }
    void setModified(bool modified) { m_modified = modified; }
    
private:
    bool m_modified = false;
};

#endif // PROJECTMODEL_H
