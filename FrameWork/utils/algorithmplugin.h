#ifndef ALGORITHMPLUGIN_H
#define ALGORITHMPLUGIN_H

#include <QRect>
#include <QVector>
#include <QString>
#include <QPair>
#include <QMap>

struct DetectRectInfo_t {
    QRect rect;
    DetectRectInfo_t():rect(QRect(0,0,0,0))
    {

    }
};
struct DetectCutInfo_t {
    QRect rect;
    DetectCutInfo_t() :rect(QRect(0, 0, 0, 0))
    {

    }
    DetectCutInfo_t(QRect _rect) :rect(_rect) {

    }
};

struct DetectCircleInfo_t {
    QPoint center;
    int radius;
    DetectCircleInfo_t():center(QPoint(0,0)),radius(0)
    {

    }
};

struct DetectLineInfo_t{
    QPoint startPoint;
    QPoint endPoint;
    double linewidth;
    double lineheight;
    DetectLineInfo_t():startPoint(QPoint(0,0)),endPoint(QPoint(0,0))
    {

    }
};

struct DetectPointInfo_t{
    QPoint point;
    DetectPointInfo_t():point(QPoint(0,0))
    {

    }
};
struct DetectTextInfo_t{
    QPair<QString,QString> text;
    DetectTextInfo_t():text(QPair<QString,QString>("",""))
    {
    }
};

struct DetectDefectInfo_t{
    QString clsName;   //LTlenth,     //CuPos
    int defectscope;
    bool result;                      //true,false
    float value; 
    bool otherImageDefect;
    DetectTextInfo_t detectAttribute;
    QVector<DetectRectInfo_t> Rects;
    QVector<DetectCircleInfo_t > Circles;
    QVector<DetectLineInfo_t> Lines;
    QVector<DetectPointInfo_t> Point;
    DetectDefectInfo_t():clsName(""), defectscope(0),result(true),value(0),otherImageDefect(false)
    {
    }

    bool operator ==(DetectDefectInfo_t other) {
        if(clsName == other.clsName && result == other.result && otherImageDefect == other.otherImageDefect){
            return true;
        }else{
            return false;
        }
    }
};



struct DetectResult_t {     
     QVector<DetectRectInfo_t> detectRects;     
     QVector<DetectCircleInfo_t > detectCircles;     
     QVector<DetectLineInfo_t> detectLines;     
     QVector<DetectPointInfo_t> detectPoint;
     QVector<DetectDefectInfo_t> detectDefect;
     DetectTextInfo_t detectResult;
     DetectTextInfo_t imageInfo;
     QVector<DetectCutInfo_t> cutInfo;
     DetectCutInfo_t displayCutInfo;
     bool bGood;
     bool bTotolGood;
     DetectResult_t() {
         bGood = true;
         bTotolGood = true;
     }

     DetectResult_t operator +=(DetectResult_t other) {
         bGood = other.bGood && bGood;
         detectRects += other.detectRects;
         detectCircles += other.detectCircles;
         detectLines += other.detectLines;
         detectPoint += other.detectPoint;
         detectDefect += other.detectDefect;
         return *this;
     }

};

struct ImageData_t {
    const char* Data;
    const char* brightImageData;
    int Width;
    int Height;
    int Channel;
    int depth;
    int nViewIndex;
    int nImageIndex;
    QString strQrCode;
    bool b3d;
};

#endif // ALGORITHMPLUGIN_H
