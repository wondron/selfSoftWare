#ifndef CCHECKTAPEINCLDHAN_H
#define CCHECKTAPEINCLDHAN_H

#include "../commonUsage/cerror.h"
#include "../commonUsage/comStruct.h"

namespace CSHDetect {

#ifndef CONSTIMG
#define CONSTIMG const HalconCpp::HObject&
#endif

#ifndef RHOBJ
#define RHOBJ HalconCpp::HObject&
#endif

typedef const int cint;

class CCheckTapeIncldHanPrivate;

/*!
 * \brief The CCheckTapeIncldHan class: 确认蓝胶包含住焊印
 */
class CCheckTapeIncldHan
{
public:
    CCheckTapeIncldHan();
    ~CCheckTapeIncldHan();
    CError detect(CONSTIMG regions, const QList<QRect>& rects, QList<QRect>& gdRects, QList<QRect>& ngRects);

    /**
     * @brief pamRead: 读取检测参数
     * @return
     */
    CError pamRead(const char* xmlfilePath);
    CError pamRead(QMap<QString, QMap<QString, QVariant>>& pamMap);

public:
    /**
     * @brief checkRegionShape：针对蓝胶是否覆盖焊印检测（数量可以是多个，也可以是单个）
     * @param dst： 检测原图
     * @param hanRect： 输入的所有焊印的矩形区域
     * @param detectInfo：检测的偏移值，主要用到 xbia 和ybia
     * @param gdRect：好的矩形区域
     * @param ngRect：ng的矩形区域
     * @param minThre：检测区域的灰度下限
     * @param maxThre：检测区域的灰度上限
     * @return
     */
    CError checkRegionShape(CONSTIMG dst, const QList<QRect>& hanRect, const QList<RectInfo>& detectInfo,
                            QList<QRect>& gdRect, QList<QRect>& ngRect, cint minThre, cint maxThre);

private:
    CCheckTapeIncldHanPrivate* d;
    QString taskName = "CheckTapeIncldHan";
};
}

#endif // CCHECKTAPEINCLDHAN_H
