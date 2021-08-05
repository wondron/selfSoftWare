#pragma once
#include "AlgoFramework_global.h"

#include "comStruct.h"
#include "cerror.h"
#include "qpoint.h"
#include "comStruct.h"
#include "halconcpp/HalconCpp.h"

namespace Detect {

#ifndef CONSTIMG
#define CONSTIMG const HalconCpp::HObject&
#endif

#ifndef RHOBJ
#define RHOBJ HalconCpp::HObject&
#endif

typedef const int cint;

class ALGOFRAMEWORK_EXPORT Algorithm
{
public:
    Algorithm();

    static CError objIsEmpty(CONSTIMG obj);
    static CError tupleisEmpty(const HalconCpp::HTuple& tuple);
    static CCircleInfo getAverCircle(QList<CCircleInfo> info);

    static CError isFileExist(const QString filename);

    static QPointF getCrossPoint(qreal pX, qreal pY, qreal lX1, qreal lY1, qreal lX2, qreal lY2);

    static LineInfo getCrossInfo(qreal pX, qreal pY, const LineInfo& info);

    static const QString getErrDescri(cint errIndex);
    static const QString getErrDescri(cint errIndex, cint Pos);

    /**
     * @brief saveHImage : 保存图像
     * @param img：  需要保存的
     * @param isRegion
     * @param filepath
     */
    static void saveHImage(CONSTIMG region, CONSTIMG dst, const char *filepath);

    /*!
     * \brief dynThre:  动态阈值算法
     * \param dst    :  输入图像
     * \param threVal:  阈值
     * \param meanSize: 核大小
     */
    static CError dynThre(CONSTIMG dst, RHOBJ res,cint threVal, cint meanSize);
    static CError dynThre(CONSTIMG dst, RHOBJ res,cint threVal, cint meanSize, cint ligORDark);

    /**
     * @brief edgeLineDtct: 检测边缘参数
     * @param dst：检测图像
     * @param showLine: 所生成的直线去区域
     * @param showPoints: 所选取的点集合
     * @param resLine: 获取的结果直线信息, angle:为角度制结果（-180 ~ 180）
     * @param direct：直线检测的方向， 0：上下，1：左右
     * @param directTime：检测的次数
     * @param dtctRegion：直线检测区域确定，起始点，终点
     * @param measurePam：measurepos使用的参数，
     *                      len1/len2:确定单点检测区域
     *                      transition：0：all, 1:positive, 2:nagetive
     *                      pntSelect : 0:all, 1: first, 2: second
     *                      sigma,
     *                      threshold
     * @param resLine：检测结果直线参数
     * @return
     */
    static CError edgeLineDtct(CONSTIMG dst, RHOBJ showLine, RHOBJ showPoints, LineInfo& resLine, cint direct, cint dtctTime,
                               const LineInfo dtctRegion, const MeasureposPam measurePam);

    /**
     * @brief useGridGetRegion: 快速分割区域
     * @param dst： 检测原图
     * @param gridWid：网格宽度
     * @param gridHigt：网格高度
     * @param minThre：目标区域最小的灰度值
     * @param eroValue：腐蚀量
     * @param slctNum：连通域个数（从最大的开始计算）
     * @param resRegion：结果obj
     */
    static CError useGridGetRegion(CONSTIMG dst, RHOBJ resRegion, cint gridWid, cint gridHigt, cint minThre, cint eroValue, cint slctNum);


    /**
     * @brief getRegionByQuadrant: 根据象限来选择对应的区域
     * @param dstObj：输入的区域
     * @param region：结果值
     * @param quadrant：需要获得的象限序号
     * @return
     */
    static CError getRegionByQuadrant(CONSTIMG dstObj, RHOBJ region, cint quadrant);

    /**
     * @brief getRectByQuadrant:: 根据象限来选择对应的矩形
     * @return
     */
    static CError getRectByQuadrant(const QList<QRect> rects, QRect& rect, cint quadrant);

    /**
     * @brief detectRegionExit: 检测某个区域的是否存在灰度范围内的蓝胶
     * @attention: 注意这里用的是len1， 和len2是长宽的一半，而不是全部。
     * @param dstObj：输入图像
     * @param inRect：根据基准点和自身的值来确定输入的检测区域，
     * @param gdRect：符合条件的区域
     * @param ngRect：不符合条件的区域
     * @param basePt: 基准点，用于定位矩形的位置
     * @param minthre: 阈值下限
     * @param maxThre: 阈值上限
     * @param percent: 所筛选region占矩形的最小比例
     * @param minSize: 获取区域的最小要求
     * @return
     */
    static CError detectRegionExit(CONSTIMG dstObj, RHOBJ showObj, const QList<RectInfo>& inRect, QList<RectInfo>& gdRect, QList<RectInfo>& ngRect,
                                   const QPoint basePt, cint minThre, cint maxThre, const qreal percent, const QSize minSize);

    /**
     * @brief getEdgePoint: 检测region内灰度突变的点，把他作为边界点
     * @param dst：检测原图
     * @param region：检测区域，一般选择细长的矩形区域作为检测区域
     * @param pts：检测到的突变点, x: 起始点开始第x点， y:一阶函数的函数值
     * @param maxVa：一阶函数的最大值
     * @param sigma：直线灰度曲线的平滑参数
     * @param maxThre：阈值，一阶函数值超过阈值时设为边界点
     */
    static CError getEdgePoint(CONSTIMG dst, RectInfo rectInfo, QList<QPointF> &pts, qreal& maxVa, const qreal sigma, const qreal maxThre);

    /**
     * @brief genSpotArrayRegion: 生成原点点阵
     * @param circle： 生成的region图
     * @param baseRow：基准点row值
     * @param baseCol：基准点col值
     * @param rowNum: 行数
     * @param colNum：列数
     * @param radius：原点的半径
     * @param startRowNum: 左上角的起始序号Row
     * @param startColNum: 左上角的起始序号Col
     * @return
     */
    static CError genSpotArrayRegion(SDotArray & array);

    static CError metrologyLineMeasure(CONSTIMG dst, const MetrologyPam dtctPam, LineInfo& line, RHOBJ hShow, HalconCpp::HTuple rows, HalconCpp::HTuple cols);
};

}
