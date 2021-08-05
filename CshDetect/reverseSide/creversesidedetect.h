#pragma once
#include "../commonUsage/cerror.h"
#include "halconcpp/HalconCpp.h"
#include "../commonUsage/commonDef.h"
#include "algorithmplugin.h"

#include "ccheckhanreg.h"
#include "cgetreregions.h"
#include "cgettapeangle.h"
#include "cchecktapeincldhan.h"

namespace CSHDetect {

class CReverseSideDetect
{
public:
    CReverseSideDetect();
    ~CReverseSideDetect();

    CError PamRead(const char* filePath);
    CError PamRead(const QMap<QString, QMap<QString, QMap<QString, QVariant>>>& pamVal);
    DetectResult_t detect(HalconCpp::HObject img);

private:
    QVector<DetectRectInfo_t> changetoRect(const QList<QRect> &rects);
    QVector<DetectLineInfo_t> changetoLine(const QList<QLine> &lines);

public:  //检测参数添加
    CGetReRegions* m_grpRegions;
    CCheckHanReg* m_grpHanreg;
    CGetTapeAngle* m_grpLineAngle;
    CCheckTapeIncldHan* m_includeGrp;

private:
    QString taskName = "ReverseSide";
};

}
