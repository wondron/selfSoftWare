#pragma once

#include "../commonUsage/cerror.h"
#include "../commonUsage/comStruct.h"

namespace CSHDetect{

class CGetCircleLenPrivate;

class CGetCircleLen
{
public:

    CGetCircleLen();
    ~CGetCircleLen();

    explicit CGetCircleLen(const QString name);

    CError detect(const RegionS& testInfo, QList<LineInfo> &lineList, QList<int> &errIndex);

    CError getLineInfo(const RegionS& testInfo, QList<LineInfo> &lineList);

    CError getCircleCenter(const RegionS& testInfo, CCircleInfo &smallPos, CCircleInfo &bigPos,
                           int ringRadius = 210, int dilaLength = 4500, int direction = 1);

    CError getDetectLine(const RegionS& Info, HalconCpp::HObject &showObj, LineInfo &line, int quadrantPos = 0, int direction = 1);

    CError pamRead(const char *xmlfilePath);
    CError pamRead(QMap<QString, QMap<QString, QVariant>>& pamMap);

    void setTaskName(const char* name) {taskName = name;}

    const QString getTaskName() const {return taskName;}

private:
    CError selectRegion(const RegionS& Info, HalconCpp::HObject& region, int quadrantPos = 0);

    CError slctRows(double& top, double& button, const double meanRow, const int quadrantPos = 0);

    CGetCircleLenPrivate* d;

    QString taskName = "GetCircleLen";
};
}
