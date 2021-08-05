﻿#ifndef WGRAPHICSSCENE_H
#define WGRAPHICSSCENE_H

#include "qgraphicsscene.h"
#include "framework_global.h"

namespace Graphics {

enum ZVALUE{MIN = 0, IMAGZVALUE = 800, MAX = 1500};

class WImageItem;
class WScenePrivate;

class FRAMEWORK_EXPORT WGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    WGraphicsScene(QObject* parent = nullptr);
    virtual ~WGraphicsScene();

    /**
     * @brief 获取图像item
     */
    WImageItem* centerImageItem() const;

    /**
     * @brief 获取显示图像
     */
    QImage getImage() const;

    /**
     * @brief 辅助线使能
     */
    void  setCenterImageAimVisible(bool visible);

public slots:
    void setCenterImage(const QImage& image);

signals:
    void centerImageChanged();
    void imageSizeChange();
    void fpsChanged(int);

private:
    WScenePrivate* d;
};

}

#endif // WGRAPHICSSCENE_H
