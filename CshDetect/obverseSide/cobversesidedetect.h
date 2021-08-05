#pragma once
#include "cgetregions.h"
#include "cgetcircleLen.h"
#include "cgetcustrappos.h"
#include "algorithmplugin.h"
#include "../commonUsage/commonDef.h"

namespace CSHDetect{

class CObverseSideDetect
{
public:
    CObverseSideDetect();
    ~CObverseSideDetect();

    CError PamRead(const char* filePath);
    DetectResult_t detect(HalconCpp::HObject img);
    CError PamRead(const QMap<QString, QMap<QString, QMap<QString, QVariant>>>& pamVal);

private:
    QList<DetectLineInfo_t> transtoLineInfo(const QList<LineInfo>& lineInfo);
    void trans2LineInfo(const QList<LineInfo>& lineInfo, const QList<int> errList, QList<DetectLineInfo_t>& gdLine, QList<DetectLineInfo_t>& ngLine);

public:
    CGetRegions* m_grpRegions;
    CGetCuStrapPos* m_grpCuPos;
    CGetCircleLen* m_grpCircleLen;

private:
    QString taskName = "ObverseSide";
};
}

