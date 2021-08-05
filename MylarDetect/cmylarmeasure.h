#ifndef CMYLARMEASURE_H
#define CMYLARMEASURE_H

#include "halconcpp/HalconCpp.h"
#include "commonUsage/commonDef.h"
#include "cmylarmatch.h"
#include "cerror.h"

namespace MylarDetect {

class CMylarMeasurePrivate;
class CMylarMeasure
{
public:
    CMylarMeasure();

    /**
     * @brief pamRead: 参数读取
     * @param pamMap：传入的检测参数
     */
    CError pamRead(QMap<QString, QMap<QString, QVariant>>& pamMap);

    inline void setTaskName(const QString& task) { m_taskName = task;}

public:
    CError detect(CONSTIMG dst, const SMylarMatchData matchData);

private:

    QString m_taskName = "MylarMeasure";      //参数任务名
    CMylarMeasurePrivate *d;
};
}
#endif // CMYLARMEASURE_H
