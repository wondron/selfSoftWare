#ifndef CCHECKTAPEANGLE_H
#define CCHECKTAPEANGLE_H

#include "commonUsage/commonDef.h"
#include "commonUsage/comStruct.h"

namespace CSH68L {
class CCheckTapeAnglePrivate;

class CCheckTapeAngle
{
public:
    CCheckTapeAngle();
    ~CCheckTapeAngle();

    /**
     * @brief pamRead:参数读取
     */
    CError pamRead(QMap<QString, QMap<QString, QVariant>>& pamMap);
    CError detect(const L68Regions& region, QRect &tapeRect, LineInfo& res, bool& isFinal);

public:
    CError checkTapeSize(const L68Regions& region, QRect& tapeRect);
    CError getLineAngle(const L68Regions& region, LineInfo& res, RHOBJ detectRegion);
private:
    CCheckTapeAnglePrivate* d;
    QString taskName = "L68CheckTapeAngle";
};
}
#endif // CCHECKTAPEANGLE_H
