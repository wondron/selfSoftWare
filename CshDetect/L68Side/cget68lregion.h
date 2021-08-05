#ifndef CGET68LREGION_H
#define CGET68LREGION_H

#include "commonUsage/commonDef.h"
#include "commonUsage/comStruct.h"

namespace CSH68L {

class CGet68LRegionPrivate;
class CGet68LRegion
{
public:
    CGet68LRegion();
    ~CGet68LRegion();

    CError pamRead(QMap<QString, QMap<QString, QVariant>>& pamMap);
    CError detect(CONSTIMG img, L68Regions& regions, LineInfo& line);

public:
    CError getBlueRegion(CONSTIMG img, L68Regions& regions, LineInfo &line);
    CError getTapeRegion(CONSTIMG img, L68Regions& regions, RHOBJ showReg);

private:
    QString taskName = "L68GetRegion";
    CGet68LRegionPrivate* d;
};


}


#endif // CGET68LREGION_H
