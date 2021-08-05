﻿#include "creversesidedetect.h"
#include "qdebug.h"
#include "QLine"

namespace CSHDetect {

#define CHECKAlGERR(a, di) \
    if(err.isWrong()){\
        qDebug()<<"err info:"<< err.msg();\
        di.clsName = Algorithm::getErrDescri(err.code());\
        di.result = false; \
        goto thread_end;   \
}

CReverseSideDetect::CReverseSideDetect()
{
    m_grpRegions = new CGetReRegions();
    m_grpHanreg = new CCheckHanReg();
    m_grpLineAngle = new CGetTapeAngle();
    m_includeGrp = new CCheckTapeIncldHan();
}

CReverseSideDetect::~CReverseSideDetect()
{
    delete m_grpRegions;
    delete m_grpHanreg;
    delete m_grpLineAngle;
    delete m_includeGrp;
}

CError CReverseSideDetect::PamRead(const char* filePath)
{
    CError err;
    err = m_grpRegions->pamRead(filePath);
    CHECKERR(err);

    err = m_grpHanreg->pamRead(filePath);
    CHECKERR(err);

    err = m_grpLineAngle->pamRead(filePath);
    CHECKERR(err);

    err = m_includeGrp->pamRead(filePath);
    CHECKERR(err);
    return 0;
}

CError CReverseSideDetect::PamRead(const QMap<QString, QMap<QString, QMap<QString, QVariant>>>& pamVal)
{
    CError err;
    auto reversePam = pamVal.value(taskName);
    err = m_grpRegions->pamRead(reversePam);
    CHECKERR(err);

    err = m_grpHanreg->pamRead(reversePam);
    CHECKERR(err);

    err = m_grpLineAngle->pamRead(reversePam);
    CHECKERR(err);

    err = m_includeGrp->pamRead(reversePam);
    CHECKERR(err);
    return 0;
}

DetectResult_t CReverseSideDetect::detect(HalconCpp::HObject img)
{
    CError err;
    bool finalRes = true;
    QString resString;
    ReverRegionS region;
    DetectResult_t result;

    QList<QRect> rects, ngIncldRect, gdIncldRect;
    QList<QLine> gLine, bLine;
    DetectRectInfo_t resRects;

    try {
        DetectDefectInfo_t di;
        region.m_oriImg = img;

        //图像区域划分
        err = m_grpRegions->detect(img, region);
        CHECKAlGERR(err, di);

        //焊印数量确定
        err = m_grpHanreg->detect(img, region, rects);
        if (err.code() == HANSIZE)
            di.Rects = changetoRect(rects);
        if (err.code() == 0)
            result.detectRects.append(changetoRect(rects));
        CHECKAlGERR(err, di);

        //胶带是否覆盖焊印区域
        ngIncldRect.clear();
        gdIncldRect.clear();
        err = m_includeGrp->detect(img, rects, gdIncldRect, ngIncldRect);
        if (ngIncldRect.size()) {
            for (QRect rec : ngIncldRect) {
                resRects.rect = rec;
                di.Rects.push_back(resRects);
            }
        }
        CHECKAlGERR(err, di);

        //胶带偏角检测
        err = m_grpLineAngle->detect(img, region, gLine, bLine);
        result.detectLines = changetoLine(gLine);
        di.Lines = changetoLine(bLine);
        CHECKAlGERR(err, di);

thread_end:
        finalRes = !err.isWrong();
        resString = finalRes ? "OK" : "NG";

        result.imageInfo.text = QPair<QString, QString>("结果:  ", resString);
        result.bGood = finalRes;
        result.bTotolGood = finalRes;
        result.detectDefect.push_back(di);
        return result;
    }  catch (...) {
        qDebug() << "CObverseSideDetect::detect crashed!";
        result.bGood = true;
        result.bTotolGood = true;
        return result;
    }
}

QVector<DetectRectInfo_t> CReverseSideDetect::changetoRect(const QList<QRect>& rects)
{
    QVector<DetectRectInfo_t> res;
    res.clear();

    DetectRectInfo_t single;
    for (auto i : rects) {
        single.rect = i;
        res.push_back(single);
    }
    return res;
}

QVector<DetectLineInfo_t> CReverseSideDetect::changetoLine(const QList<QLine>& lines)
{
    DetectLineInfo_t line;
    QVector<DetectLineInfo_t> resLine;
    for (auto i : lines) {
        line.startPoint = i.p1();
        line.endPoint = i.p2();
        resLine.push_back(line);
    }
    return resLine;
}

}
