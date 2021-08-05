#ifndef CCHECKHANREGION_H
#define CCHECKHANREGION_H

#include "commonUsage/commonDef.h"
#include "commonUsage/comStruct.h"

namespace CSH68L {

class CCheckHanRegionPrivate;

class CCheckHanRegion
{
public:
    CCheckHanRegion();
    ~CCheckHanRegion();

    /**
     * @brief pamRead: 参数读取
     * @param pamMap：传入的检测参数
     */
    CError pamRead(QMap<QString, QMap<QString, QVariant>>& pamMap);

    /**
     * @brief region: 获取的区域
     * @param hanDtctRect，：焊印检测区域
     * @param hanRect：焊点区域
     */
    CError detect(const L68Regions& region, QRect& hanDtctRect, HObject &hShowObj, int& hanNum);

    void setBiaValue(cint colBia, cint rowBia, cint len1, cint len2);

    void setdynPam(int dynVal, int hanSize);

    /**
     * @brief setRowSlectPam: 设置行选取参数
     * @param rowNumMin：一行最少个数
     * @param rowRange: 一行的上下浮动范围
     */
    void setRowSlectPam(int rowNumMin, int rowRange);

    /**
     * @brief setGenDotArrayPam: 生成点阵的参数配置
     * @param arrayData
     */
    void setGenDotArrayPam(const SDotArray arrayData);

    /**
     * @brief setSlctPam：设置pam参数
     * @param pamList：pam参数
     */
    void setSlctPam(QList<float> pamList);
public:
    /**
     * @brief getHanCheckReg: 根据基准点确认检测区域
     * @param region：检测的输入图像
     * @param basePt：确认检测矩形的基准点
     * @return
     */
    CError getHanCheckReg(const L68Regions& region, QRect& rect, cint baseRow, cint baseCol);

    /**
     * @brief getHanSpotNum: 获取焊印区域的个数
     * @param region: 检测图像
     * @param rect: 检测
     * @param num
     * @return
     */
    CError getHanSpotNum(const L68Regions& region, const QRect rect, RHOBJ showObj, int &num);

    /**
     * @brief getRegionWithMask: 使用点阵区域进一步筛选区域
     * @param slctObj: 需要选择的obj
     * @param allRegion：和mask对比重合的图片
     * @param mask： mask图像
     * @param resObj：结果region
     * @param rowRange: 视为同一行的变化范围
     * @param rowNum：最少数量，视为一行的最少数量
     * @return
     */
    CError getRegionWithMask(CONSTIMG slctObj, CONSTIMG allRegion, const SDotArray mask, RHOBJ resObj, int& num, cint rowRange, cint rowNum, cint closeCircle);
private:
    QString taskName = "L68CheckHanRegion";
    CCheckHanRegionPrivate* d;
};

}

#endif // CCHECKHANREGION_H
