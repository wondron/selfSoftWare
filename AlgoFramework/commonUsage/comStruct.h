#pragma once

#include "halconcpp/HalconCpp.h"
#include "qobject.h"
#include <QRect>
#include <QVector>
#include <QString>
#include <QPair>
#include <QMap>

#ifndef HOBJ
#define HOBJ  HalconCpp::HObject
#endif

struct RegionS{
    int width;
    int height;
    int TapeNum;

    HOBJ m_oriImg;
    HOBJ m_RImg;
    HOBJ m_GImg;
    HOBJ m_BImg;
    HOBJ m_hsvHImg;
    HOBJ m_hsvSImg;
    HOBJ m_hsvVImg;

    HOBJ BlackBangRegion;
    HOBJ DarkRegion;
    HOBJ WhiteRegion;
    HOBJ TapeRegion;

    HOBJ BlueRegion;
    HOBJ MidRect;


    HOBJ MidBlueTape;
    HOBJ WeldRoi;
};

struct ReverRegionS{
    int width;
    int height;
    int batDire;

    HOBJ m_oriImg;

    HOBJ batteryRegion;
    HOBJ midRegion;
    HOBJ blueTapesReg;
    HOBJ dblTapeReg;

    HOBJ hanregion;
};

struct L68Regions
{
    HOBJ m_oriImg;

    HOBJ m_RImg;
    HOBJ m_GImg;
    HOBJ m_BImg;
    HOBJ m_SubImg;

    HOBJ blueRegion;
    HOBJ tapeRegion;
};

struct SDotArray
{
    HOBJ arrayObj;

    int baseRow;
    int baseCol;
    int rowNum;
    int colNum;
    int rowInter;
    int colInter;
    int radius;
    int startRowIndex = 0;
    int startColIndex = 0;
};

struct CCircleInfo{
    double X;
    double Y;
    double Radius;
};

struct LineInfo{
    double crossX;
    double crossY;
    double pX;
    double pY;
    double distance;
    double angle;    // 角度制
    HalconCpp::HTuple startRow;
    HalconCpp::HTuple startCol;
    HalconCpp::HTuple endRow;
    HalconCpp::HTuple endCol;
};

struct RectInfo
{
    int XBia;
    int YBia;

    int len1;
    int len2;
    int row;
    int col;
    double phi = 0;
};

struct MeasureposPam{
    int recLen1;
    int recLen2;
    int transition = 0;
    int pntSelect = 0;
    double sigma = 1;
    int threshold = 20;
};

struct MetrologyPam{
    int numInstance = 1;
    int transition = 0; //0:all, 1:positive,1:negative
    float minScore = 0.2;
    int ptSlect = 0; //0:all, 1:first, 2:last

    int measureLen1 = 100;
    int measureLen2 = 2;
    double sigma = 1;
    int thre = 20;
};
