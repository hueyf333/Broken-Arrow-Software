#include "KnobRenderer.h"
#include <QPainter>
#include <QPainterPath>
#include <QRadialGradient>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QtMath>

KnobRenderer::KnobRenderer() {
}

double KnobRenderer::angleForFrame(const KnobParameters& params, int frameIndex) {
    if (params.frameCount <= 1) {
        return params.startAngle;
    }
    double range = params.endAngle - params.startAngle;
    return params.startAngle + (range * frameIndex / (params.frameCount - 1));
}

QImage KnobRenderer::renderFrame(const KnobParameters& params, int frameIndex) {
    QImage image(params.canvasWidth, params.canvasHeight, QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    
    // Render layers
    renderLayers(painter, params);
    
    // Render ticks
    renderTicks(painter, params);
    
    // Render pointer at the angle for this frame
    double angle = angleForFrame(params, frameIndex);
    renderPointer(painter, params, angle);
    
    painter.end();
    
    // Apply shadow if enabled
    if (params.enableShadow) {
        applyShadow(image, params);
    }
    
    return image;
}

QList<QImage> KnobRenderer::renderAllFrames(const KnobParameters& params) {
    QList<QImage> frames;
    for (int i = 0; i < params.frameCount; ++i) {
        frames.append(renderFrame(params, i));
    }
    return frames;
}

void KnobRenderer::renderLayers(QPainter& painter, const KnobParameters& params) {
    QPointF center(params.canvasWidth / 2.0, params.canvasHeight / 2.0);
    
    // Sort layers by order
    QList<Layer> sortedLayers = params.layers;
    std::sort(sortedLayers.begin(), sortedLayers.end(), 
              [](const Layer& a, const Layer& b) { return a.order < b.order; });
    
    for (const auto& layer : sortedLayers) {
        if (!layer.visible) continue;
        
        painter.save();
        painter.setOpacity(layer.opacity);
        
        if (layer.useGradient) {
            QRadialGradient gradient(center, params.outerRadius);
            gradient.setColorAt(0, layer.color);
            gradient.setColorAt(1, layer.gradientColor);
            painter.setBrush(gradient);
        } else {
            painter.setBrush(layer.color);
        }
        
        painter.setPen(Qt::NoPen);
        
        // Draw annulus (ring) from inner to outer radius
        QPainterPath path;
        path.addEllipse(center, params.outerRadius, params.outerRadius);
        path.addEllipse(center, params.innerRadius, params.innerRadius);
        painter.fillPath(path, painter.brush());
        
        painter.restore();
    }
}

void KnobRenderer::renderTicks(QPainter& painter, const KnobParameters& params) {
    if (params.tickCount <= 0 || params.tickStyle == "None") {
        return;
    }
    
    QPointF center(params.canvasWidth / 2.0, params.canvasHeight / 2.0);
    painter.setPen(QPen(Qt::black, 2));
    
    double angleRange = params.endAngle - params.startAngle;
    
    for (int i = 0; i < params.tickCount; ++i) {
        double angle = params.startAngle + (angleRange * i / (params.tickCount - 1));
        double rad = qDegreesToRadians(angle);
        
        double innerX = center.x() + (params.innerRadius - 5) * std::cos(rad);
        double innerY = center.y() + (params.innerRadius - 5) * std::sin(rad);
        double outerX = center.x() + (params.outerRadius + 5) * std::cos(rad);
        double outerY = center.y() + (params.outerRadius + 5) * std::sin(rad);
        
        if (params.tickStyle == "Line") {
            painter.drawLine(QPointF(innerX, innerY), QPointF(outerX, outerY));
        } else if (params.tickStyle == "Dot") {
            painter.setBrush(Qt::black);
            painter.drawEllipse(QPointF(outerX, outerY), 3, 3);
        }
    }
}

void KnobRenderer::renderPointer(QPainter& painter, const KnobParameters& params, double angle) {
    QPointF center(params.canvasWidth / 2.0, params.canvasHeight / 2.0);
    double rad = qDegreesToRadians(angle);
    
    painter.setPen(QPen(Qt::white, 3));
    painter.setBrush(Qt::white);
    
    if (params.pointerStyle == "Line") {
        double endX = center.x() + params.outerRadius * std::cos(rad);
        double endY = center.y() + params.outerRadius * std::sin(rad);
        painter.drawLine(center, QPointF(endX, endY));
    } else if (params.pointerStyle == "Arrow" || params.pointerStyle == "Triangle") {
        double length = params.outerRadius;
        double endX = center.x() + length * std::cos(rad);
        double endY = center.y() + length * std::sin(rad);
        
        // Create triangle
        QPolygonF triangle;
        triangle << QPointF(endX, endY);
        
        double perpRad1 = rad + M_PI / 2;
        double perpRad2 = rad - M_PI / 2;
        triangle << QPointF(center.x() + 5 * std::cos(perpRad1), 
                           center.y() + 5 * std::sin(perpRad1));
        triangle << QPointF(center.x() + 5 * std::cos(perpRad2), 
                           center.y() + 5 * std::sin(perpRad2));
        
        painter.drawPolygon(triangle);
    }
}

void KnobRenderer::applyShadow(QImage& image, const KnobParameters& params) {
    // Simple shadow effect - create a blurred copy and composite
    // This is a simplified version; full implementation would use QGraphicsBlurEffect
    // For now, just darken the edges
}

bool KnobRenderer::exportFilmstrip(const KnobParameters& params, const QString& filePath, 
                                  bool horizontal, int scale) {
    QList<QImage> frames = renderAllFrames(params);
    
    if (frames.isEmpty()) {
        return false;
    }
    
    int frameWidth = params.canvasWidth * scale;
    int frameHeight = params.canvasHeight * scale;
    
    QImage filmstrip;
    if (horizontal) {
        filmstrip = QImage(frameWidth * frames.size(), frameHeight, QImage::Format_ARGB32);
    } else {
        filmstrip = QImage(frameWidth, frameHeight * frames.size(), QImage::Format_ARGB32);
    }
    filmstrip.fill(Qt::transparent);
    
    QPainter painter(&filmstrip);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    
    for (int i = 0; i < frames.size(); ++i) {
        QImage scaledFrame = frames[i].scaled(frameWidth, frameHeight, 
                                             Qt::IgnoreAspectRatio, 
                                             Qt::SmoothTransformation);
        if (horizontal) {
            painter.drawImage(i * frameWidth, 0, scaledFrame);
        } else {
            painter.drawImage(0, i * frameHeight, scaledFrame);
        }
    }
    
    return filmstrip.save(filePath);
}

bool KnobRenderer::exportFrameSequence(const KnobParameters& params, const QString& dirPath,
                                      const QString& prefix, int scale) {
    QList<QImage> frames = renderAllFrames(params);
    
    for (int i = 0; i < frames.size(); ++i) {
        QString fileName = QString("%1/%2%3.png")
            .arg(dirPath)
            .arg(prefix)
            .arg(i, 4, 10, QChar('0'));
        
        QImage scaledFrame = frames[i].scaled(
            params.canvasWidth * scale, 
            params.canvasHeight * scale,
            Qt::IgnoreAspectRatio,
            Qt::SmoothTransformation
        );
        
        if (!scaledFrame.save(fileName)) {
            return false;
        }
    }
    
    return true;
}

bool KnobRenderer::exportAtlas(const KnobParameters& params, const QString& pngPath,
                              const QString& jsonPath, int scale) {
    QList<QImage> frames = renderAllFrames(params);
    
    if (frames.isEmpty()) {
        return false;
    }
    
    // Simple atlas layout: grid packing
    int framesPerRow = static_cast<int>(std::ceil(std::sqrt(frames.size())));
    int rows = static_cast<int>(std::ceil(static_cast<double>(frames.size()) / framesPerRow));
    
    int frameWidth = params.canvasWidth * scale;
    int frameHeight = params.canvasHeight * scale;
    
    QImage atlas(frameWidth * framesPerRow, frameHeight * rows, QImage::Format_ARGB32);
    atlas.fill(Qt::transparent);
    
    QPainter painter(&atlas);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    
    QJsonArray framesArray;
    
    for (int i = 0; i < frames.size(); ++i) {
        int col = i % framesPerRow;
        int row = i / framesPerRow;
        
        QImage scaledFrame = frames[i].scaled(frameWidth, frameHeight,
                                             Qt::IgnoreAspectRatio,
                                             Qt::SmoothTransformation);
        painter.drawImage(col * frameWidth, row * frameHeight, scaledFrame);
        
        // Add to JSON map
        QJsonObject frameObj;
        frameObj["frame"] = i;
        frameObj["x"] = col * frameWidth;
        frameObj["y"] = row * frameHeight;
        frameObj["width"] = frameWidth;
        frameObj["height"] = frameHeight;
        framesArray.append(frameObj);
    }
    
    painter.end();
    
    // Save atlas image
    if (!atlas.save(pngPath)) {
        return false;
    }
    
    // Save JSON map
    QJsonObject root;
    root["frames"] = framesArray;
    root["meta"] = QJsonObject{
        {"image", QFileInfo(pngPath).fileName()},
        {"size", QJsonObject{{"w", atlas.width()}, {"h", atlas.height()}}},
        {"scale", scale}
    };
    
    QJsonDocument doc(root);
    QFile jsonFile(jsonPath);
    if (!jsonFile.open(QIODevice::WriteOnly)) {
        return false;
    }
    
    jsonFile.write(doc.toJson(QJsonDocument::Indented));
    jsonFile.close();
    
    return true;
}
