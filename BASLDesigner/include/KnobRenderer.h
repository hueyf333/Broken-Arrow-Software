#ifndef KNOBRENDERER_H
#define KNOBRENDERER_H

#include "ProjectModel.h"
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QList>

class KnobRenderer {
public:
    KnobRenderer();
    
    // Render a single frame
    QImage renderFrame(const KnobParameters& params, int frameIndex);
    
    // Render all frames
    QList<QImage> renderAllFrames(const KnobParameters& params);
    
    // Export functions
    bool exportFilmstrip(const KnobParameters& params, const QString& filePath, 
                        bool horizontal = true, int scale = 1);
    bool exportFrameSequence(const KnobParameters& params, const QString& dirPath,
                           const QString& prefix = "frame_", int scale = 1);
    bool exportAtlas(const KnobParameters& params, const QString& pngPath,
                    const QString& jsonPath, int scale = 1);
    
private:
    void renderBackground(QPainter& painter, const KnobParameters& params);
    void renderTicks(QPainter& painter, const KnobParameters& params);
    void renderPointer(QPainter& painter, const KnobParameters& params, double angle);
    void renderLayers(QPainter& painter, const KnobParameters& params);
    void applyShadow(QImage& image, const KnobParameters& params);
    
    double angleForFrame(const KnobParameters& params, int frameIndex);
};

#endif // KNOBRENDERER_H
