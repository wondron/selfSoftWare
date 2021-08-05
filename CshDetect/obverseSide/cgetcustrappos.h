#pragma once
#include "../commonUsage/cerror.h"
#include "halconcpp/HalconCpp.h"
#include "../commonUsage/commonDef.h"

namespace CSHDetect {

#ifndef CONSTIMG
#define CONSTIMG const HalconCpp::HObject&
#endif

#ifndef RHOBJ
#define RHOBJ HalconCpp::HObject&
#endif

class CGetCuStrapPosPrivate;
/**
 * @brief The CGetCuStrapPos class:检测铜片位置
 */
class CGetCuStrapPos
{
public:
    explicit CGetCuStrapPos(const int wid = 5472, const int height = 3648);

    explicit CGetCuStrapPos(const QString taskName);

    explicit CGetCuStrapPos(const RegionS& region);

    ~CGetCuStrapPos();

    void setImgSize(const int width, const int height);

    void setRegions(const RegionS& regions);

    CError detect(const RegionS& regions, QList<QRect>& gdRect, QList<QRect>& ngRect);

    CError pamRead(const char* xmlfilePath);
    CError pamRead(QMap<QString, QMap<QString, QVariant>>& pamMap);

    void setTaskName(const char* name) {taskName = name;}

    const QString getTaskName() const {return taskName;}

public:
    CError getCuPos(const RegionS& regions, RHOBJ resCuRegion, int& CuPos, cint minThre, cint connectDirect);

    CError checkTapeNum(const RegionS& region, CONSTIMG cuRegion, QList<RectInfo> testInfo,
                        QList<QRect>& gdInfo, QList<QRect>& ngInfo,
                        cint minThre, cint maxThre, const qreal areaRatoi, const QSize size);

public:
    CGetCuStrapPosPrivate* d;
    QString taskName = "GetCuStrapPos";
};
}
