﻿#pragma once

#include <QGraphicsView>
#include "QTimer"
#include "QLabel"
#include "framework_global.h"

namespace Graphics {

class FRAMEWORK_EXPORT WGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    WGraphicsView(QWidget *parent = nullptr);

public slots:
    void zoomIn(qreal level = 1.05);
    void zoomOut(qreal level = 0.95);
    void zoom(qreal scale);
    void fitScene();
    void movetoCenter();
    void setAntialiase(bool enabel);

protected:
    void wheelEvent(QWheelEvent *) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    bool event(QEvent *e) override;

private:

signals:
    void scaleChanged(qreal);
    void dragChanged();

public slots:


private:
    QTimer *m_timer;
    bool dragFlag = false;
};

}

