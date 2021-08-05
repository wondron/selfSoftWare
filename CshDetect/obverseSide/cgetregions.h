#pragma once
#include "../commonUsage/cerror.h"
#include "../commonUsage/comStruct.h"

namespace CSHDetect {

#ifndef CONSTIMG
    #define CONSTIMG const HalconCpp::HObject&
#endif

typedef const int cint ;

class CGetRegionsPrivate;
class CGetRegions
{
public:
    CGetRegions();
    ~CGetRegions();
    /**
     * @brief detect: 程序外部使用接口，获取各部分的参数。
     */
    CError detect(CONSTIMG obj, RegionS& res);

    /**
     * @brief transform: 转换图像格式
     */
    CError transformImg(CONSTIMG obj, RegionS& res);

    /**
     * @brief pamRead: 读取检测参数
     * @return
     */
    CError pamRead(const char *xmlfilePath);
    CError pamRead(QMap<QString, QMap<QString, QVariant>>& pamMap);

public:
    //以下四个是基本区域的求解
    /**
     * @brief getBlackBangRoi：中间固定棒
     */
    CError getBlackBangRoi(RegionS& res, cint maxThre = 20);

    /**
     * @brief getDarkRegion: 获取图像的黑色区域
     * @return
     */
    CError getDarkRegion(RegionS& res, cint maxR = 30, cint maxG = 30, cint maxB = 30);
    /**
     * @brief getWhiteRegion：获取图像的白色区域
     */
    CError getWhiteRegion(RegionS& res, cint minR = 200, cint minG = 200, cint minB = 200, cint maxArea = 1500);

    /**
     * @brief getTapeRegion: 底部的特氟龙胶带区域
     */
    CError getTapeRegion(RegionS& res, double guassSigma = 2.3, double guassFactor = 1.6, cint thresMax = -3, cint tapeHole = 10);

public:

    /**
     * @brief getBlueRegion : 纯蓝色区域获取
     */
    CError getBlueRegion(RegionS& res, cint blueSubVal = 51);


    /**
     * @brief getMidRect: 获取中间部分的矩形区域
     * @param res： Hobject 保存的对象
     * @param direct： 铜铝两极的方向， 0：上下，1：左右
     * @param rectWid： 矩形区域的宽度
     * @param eroWidth：对整张图像区域缩减他的涨肚或者宽度，使得电池能够贯穿整张图片
     * @param slceNum：确定中间矩形的区域个数。
     * @return
     */
    CError getMidRect(RegionS& res, cint direct = 1, cint rectWid = 400, cint eroWidth = 0, cint slctNum = 3);

    /**
     * @brief getBlueTape: 获取中间区域中的蓝色胶带区域（包括了大区域的白色）
     * @param res
     * @param tapeNum ：输出胶带数量
     * @param blueTapeWid：胶带宽度设置
     * @param ringWid: 反光的铝极圆环宽度
     * @return
     */
    CError getBlueTape(RegionS& res, int& tapeNum, cint blueTapeWid = 1500, cint ringWid = 40);

private:
    CGetRegionsPrivate* d;
    QString taskName = "GetRegions";
};

}
