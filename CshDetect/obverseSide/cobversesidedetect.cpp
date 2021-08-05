﻿#include "cobversesidedetect.h"
#include "qdebug.h"

namespace CSHDetect {

#define CHECKAlGERR(err, di) \
    if(err.isWrong()){\
        qDebug()<<"err info:"<< err.msg();\
        di.clsName = Algorithm::getErrDescri(err.code());\
        di.result = false; \
        goto thread_end;   \
}

CObverseSideDetect::CObverseSideDetect()
{
    m_grpRegions = new CGetRegions();
    m_grpCuPos = new CGetCuStrapPos();
    m_grpCircleLen = new CGetCircleLen();
}

CObverseSideDetect::~CObverseSideDetect()
{
    delete m_grpRegions;
    delete m_grpCuPos;
    delete m_grpCircleLen;
}

CError CObverseSideDetect::PamRead(const char* filePath)
{
    CError err;
    err = m_grpRegions->pamRead(filePath);
    CHECKERR(err);

    err = m_grpCuPos->pamRead(filePath);
    CHECKERR(err);

    err = m_grpCircleLen->pamRead(filePath);
    CHECKERR(err);

    return 0;
}

CError CObverseSideDetect::PamRead(const QMap<QString, QMap<QString, QMap<QString, QVariant> > >& pamVal)
{
    CError err;
    auto obversePam = pamVal.value(taskName);
    err = m_grpRegions->pamRead(obversePam);
    CHECKERR(err);

    err = m_grpCuPos->pamRead(obversePam);
    CHECKERR(err);

    err = m_grpCircleLen->pamRead(obversePam);
    CHECKERR(err);

    return 0;
}

DetectResult_t CObverseSideDetect::detect(HObject img)
{
    // total use
    bool finalRes = true;
    QString resString;
    CError err;
    DetectResult_t result;

    //m_grpRegions pam
    RegionS region;

    //m_grpCircleLen pam
    QList<int> errIndex;
    QList<LineInfo> lines;
    QList<DetectLineInfo_t> ngLines, gdLines;
    QList<QRect> ngRects, gdRects;
    DetectRectInfo_t cuRect;
    try {
        DetectDefectInfo_t di;

        //判定图像区域检测
        err = m_grpRegions->detect(img, region);
        CHECKAlGERR(err, di);

        //判定铜片位置
        m_grpCuPos->setImgSize(region.width, region.height);
        err = m_grpCuPos->detect(region, gdRects, ngRects);
        for (auto rect : gdRects) {
            cuRect.rect = rect;
            result.detectRects.push_back(cuRect);
        }

        for (auto rect : ngRects) {
            cuRect.rect = rect;
            di.Rects.push_back(cuRect);
        }
        CHECKAlGERR(err, di);
        result.detectRects.push_back(cuRect);

        if (region.TapeNum == 2)
            goto thread_end;

        err = m_grpCircleLen->detect(region, lines, errIndex);
        if (err.isWrong() && (err.code() != LACKAREA)) {
            CHECKAlGERR(err, di);
        }

        trans2LineInfo(lines, errIndex, gdLines, ngLines);
        bool bres = (err.code() == LACKAREA) ? false : true;
        resString = Algorithm::getErrDescri(err.code());

        for (auto lines : ngLines)
            di.Lines.push_back(lines);

        di.result = bres;
        di.clsName = resString;

thread_end:
        qDebug() << "err info: " << err.code() << '\n' << err.msg();

        finalRes = !err.isWrong();
        result.detectDefect.push_back(di);

        for (auto line : gdLines)
            result.detectLines.push_back(line);

        resString = finalRes ? "OK" : "NG";
        result.imageInfo.text = QPair<QString, QString>("结果:  ", resString);
        result.bGood = finalRes;
        result.bTotolGood = finalRes;
        return result;

    }  catch (...) {
        qDebug() << "CObverseSideDetect::detect crashed!";
        result.bGood = true;
        result.bTotolGood = true;
        return result;
    }
}

QList<DetectLineInfo_t> CObverseSideDetect::transtoLineInfo(const QList<LineInfo>& lineInfo)
{
    DetectLineInfo_t line;
    QList<DetectLineInfo_t> resLine;
    int SX, SY, EX, EY;
    for (auto i : lineInfo) {
        SX = i.startCol[0].D();
        SY = i.startRow[0].D();
        EX = i.endCol[0].D();
        EY = i.endRow[0].D();

        line.startPoint = QPoint(SX, SY);
        line.endPoint = QPoint(EX, EY);
        resLine.push_back(line);

        line.startPoint = QPoint(i.pX, i.pY);
        line.endPoint = QPoint(i.crossX, i.crossY);
        resLine.push_back(line);
    }
    return resLine;
}

void CObverseSideDetect::trans2LineInfo(const QList<LineInfo>& lineInfo, const QList<int> errList,
                                        QList<DetectLineInfo_t>& gdLine, QList<DetectLineInfo_t>& ngLine)
{
    if (lineInfo.isEmpty())
        return;

    gdLine.clear();
    ngLine.clear();

    QList<LineInfo> gdInfo, ngInfo;
    bool isNg = false;

    for (int i = 0 ; i < lineInfo.size() ; i++) {
        for (auto index : errList) {
            if (index == i) isNg = true;
        }

        if (isNg) {
            ngInfo.push_back(lineInfo[i]);
            isNg = false;
        } else {
            gdInfo.push_back(lineInfo[i]);
        }
    }

    ngLine = transtoLineInfo(ngInfo);
    gdLine = transtoLineInfo(gdInfo);
}
}
