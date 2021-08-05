#ifndef CSINGLEDTCT_H
#define CSINGLEDTCT_H

#include "commonUsage/commonDef.h"
#include "commonUsage/comStruct.h"
#include "algorithmplugin.h"

namespace CSH68L {
class CSingleDtctPrivate;

class CSingleDtct
{
public:
    CSingleDtct();
    ~CSingleDtct();

    /**
     * @brief setCuPos: 设定是否是铜片位置
     * @param isCu： true，为铜片， false，不是铜片
     */
    void setCuPos(const bool isCu);

    /**
    * @brief setImageIndex: 设定图像序号
    */
    void setImageIndex(cint index) { m_index = index; }

    void setTaskName(const QString name) {taskName = name;}
    CError PamRead(const QMap<QString, QMap<QString, QMap<QString, QVariant>>>& pamVal);
    DetectResult_t detect(CONSTIMG dst);

private:
    CSingleDtctPrivate* d;
    int m_index = 0;
    QString taskName = "CuTopDtct";
};
}

#endif // CSINGLEDTCT_H
