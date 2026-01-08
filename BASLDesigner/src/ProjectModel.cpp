#include "ProjectModel.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>

// Layer implementation
QJsonObject Layer::toJson() const {
    QJsonObject obj;
    obj["name"] = name;
    obj["color"] = color.name();
    obj["gradientColor"] = gradientColor.name();
    obj["useGradient"] = useGradient;
    obj["opacity"] = opacity;
    obj["visible"] = visible;
    obj["order"] = order;
    return obj;
}

Layer Layer::fromJson(const QJsonObject& json) {
    Layer layer;
    layer.name = json["name"].toString();
    layer.color = QColor(json["color"].toString());
    layer.gradientColor = QColor(json["gradientColor"].toString());
    layer.useGradient = json["useGradient"].toBool();
    layer.opacity = json["opacity"].toDouble();
    layer.visible = json["visible"].toBool();
    layer.order = json["order"].toInt();
    return layer;
}

// KnobParameters implementation
QJsonObject KnobParameters::toJson() const {
    QJsonObject obj;
    obj["canvasWidth"] = canvasWidth;
    obj["canvasHeight"] = canvasHeight;
    obj["frameCount"] = frameCount;
    obj["startAngle"] = startAngle;
    obj["endAngle"] = endAngle;
    obj["innerRadius"] = innerRadius;
    obj["outerRadius"] = outerRadius;
    obj["tickCount"] = tickCount;
    obj["tickStyle"] = tickStyle;
    obj["pointerStyle"] = pointerStyle;
    obj["enableShadow"] = enableShadow;
    obj["shadowBlur"] = shadowBlur;
    obj["shadowOpacity"] = shadowOpacity;
    
    QJsonArray layersArray;
    for (const auto& layer : layers) {
        layersArray.append(layer.toJson());
    }
    obj["layers"] = layersArray;
    
    return obj;
}

KnobParameters KnobParameters::fromJson(const QJsonObject& json) {
    KnobParameters params;
    params.canvasWidth = json["canvasWidth"].toInt();
    params.canvasHeight = json["canvasHeight"].toInt();
    params.frameCount = json["frameCount"].toInt();
    params.startAngle = json["startAngle"].toDouble();
    params.endAngle = json["endAngle"].toDouble();
    params.innerRadius = json["innerRadius"].toInt();
    params.outerRadius = json["outerRadius"].toInt();
    params.tickCount = json["tickCount"].toInt();
    params.tickStyle = json["tickStyle"].toString();
    params.pointerStyle = json["pointerStyle"].toString();
    params.enableShadow = json["enableShadow"].toBool();
    params.shadowBlur = json["shadowBlur"].toInt();
    params.shadowOpacity = json["shadowOpacity"].toDouble();
    
    QJsonArray layersArray = json["layers"].toArray();
    for (const auto& layerValue : layersArray) {
        params.layers.append(Layer::fromJson(layerValue.toObject()));
    }
    
    return params;
}

// ControlDefinition implementation
QJsonObject ControlDefinition::toJson() const {
    QJsonObject obj;
    obj["id"] = id;
    obj["name"] = name;
    obj["type"] = static_cast<int>(type);
    obj["x"] = bounds.x();
    obj["y"] = bounds.y();
    obj["width"] = bounds.width();
    obj["height"] = bounds.height();
    obj["imageSource"] = imageSource;
    obj["isFilmstrip"] = isFilmstrip;
    obj["frameCount"] = frameCount;
    obj["hitTestShape"] = hitTestShape;
    obj["metadata"] = metadata;
    return obj;
}

ControlDefinition ControlDefinition::fromJson(const QJsonObject& json) {
    ControlDefinition def;
    def.id = json["id"].toString();
    def.name = json["name"].toString();
    def.type = static_cast<ControlType>(json["type"].toInt());
    def.bounds = QRectF(
        json["x"].toDouble(),
        json["y"].toDouble(),
        json["width"].toDouble(),
        json["height"].toDouble()
    );
    def.imageSource = json["imageSource"].toString();
    def.isFilmstrip = json["isFilmstrip"].toBool();
    def.frameCount = json["frameCount"].toInt();
    def.hitTestShape = json["hitTestShape"].toString();
    def.metadata = json["metadata"].toObject();
    return def;
}

// LayoutParameters implementation
QJsonObject LayoutParameters::toJson() const {
    QJsonObject obj;
    obj["width"] = width;
    obj["height"] = height;
    obj["gridSize"] = gridSize;
    obj["snapToGrid"] = snapToGrid;
    
    QJsonArray controlsArray;
    for (const auto& control : controls) {
        controlsArray.append(control.toJson());
    }
    obj["controls"] = controlsArray;
    
    return obj;
}

LayoutParameters LayoutParameters::fromJson(const QJsonObject& json) {
    LayoutParameters params;
    params.width = json["width"].toInt();
    params.height = json["height"].toInt();
    params.gridSize = json["gridSize"].toInt();
    params.snapToGrid = json["snapToGrid"].toBool();
    
    QJsonArray controlsArray = json["controls"].toArray();
    for (const auto& controlValue : controlsArray) {
        params.controls.append(ControlDefinition::fromJson(controlValue.toObject()));
    }
    
    return params;
}

// ProjectModel implementation
ProjectModel::ProjectModel() {
    // Initialize with default parameters
    Layer defaultLayer;
    defaultLayer.name = "Base";
    defaultLayer.color = QColor(100, 150, 200);
    defaultLayer.visible = true;
    knobParams.layers.append(defaultLayer);
}

bool ProjectModel::loadFromFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    
    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    
    if (!doc.isObject()) {
        return false;
    }
    
    return fromJson(doc.object());
}

bool ProjectModel::saveToFile(const QString& filePath) {
    QJsonDocument doc(toJson());
    
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    
    m_modified = false;
    return true;
}

QJsonObject ProjectModel::toJson() const {
    QJsonObject obj;
    obj["version"] = version;
    obj["projectName"] = projectName;
    obj["knobParameters"] = knobParams.toJson();
    obj["layoutParameters"] = layoutParams.toJson();
    return obj;
}

bool ProjectModel::fromJson(const QJsonObject& json) {
    version = json["version"].toInt();
    projectName = json["projectName"].toString();
    knobParams = KnobParameters::fromJson(json["knobParameters"].toObject());
    layoutParams = LayoutParameters::fromJson(json["layoutParameters"].toObject());
    m_modified = false;
    return true;
}
