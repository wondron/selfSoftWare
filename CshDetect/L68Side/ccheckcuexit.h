#ifndef CCHECKCUEXIT_H
#define CCHECKCUEXIT_H

#include "commonUsage/commonDef.h"
#include "commonUsage/comStruct.h"

namespace CSH68L {

class CCheckCuExitPrivate;

class CCheckCuExit
{
public:
    CCheckCuExit();
    ~CCheckCuExit();

    /**
     * @brief pamRead: 参数读取
     * @param pamMap：传入的检测参数
     */
    CError pamRead(QMap<QString, QMap<QString, QVariant>>& pamMap);
    CError detect(const L68Regions& region, bool& isCu, QRect& rect);

public:
    /**
     * @brief getCuRect:: 获取大蓝胶下面，铜片微微露出的区域。
     * @return
     */
    CError getCuRect(const L68Regions& region, QRect& rect);

private:
    CCheckCuExitPrivate* d;
    QString taskName = "L68CheckCuExit";
};

}

#endif // CCHECKCUEXIT_H
