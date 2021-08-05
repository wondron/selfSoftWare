﻿#pragma once
#include "algoframework.h"
#include "QObject"
#include "halconcpp/HalconCpp.h"
#include "../commonUsage/algorithm.h"

using namespace Detect;
using namespace HalconCpp;

#define CHECKERR(a) \
    if(a.isWrong()){ \
        return a; \
    }

#define CHECKEMPIMG(img, info) \
    if(Algorithm::objIsEmpty(img).isWrong()) \
        return CError(EMPTYOBJ, info);

#define CHECKEMPTUPLE(tup, info) \
if(Algorithm::tupleisEmpty(tup).isWrong())\
    return CError(EMPTYOBJ, info);

#define CHECKTHREVALUE(value, info) \
    if(value < 0 || value > 255) \
        return CError(PAMVALUE, info);

#define READPAM(target, name, res) \
{\
    auto iterMBA = res.find(name);\
    if (iterMBA != res.end()) {   \
        target = iterMBA->second.value;\
    }\
}

#define READPAMINT(target, name, res)\
    qDebug() << name << res.value(name).toInt();\
    target = res.value(name).toInt();

#define READPAMFLOAT(target, name, res)\
    target = res.value(name).toFloat();

enum errIndex {
    OK = 0,
    NG,
    LACKAREA,   //蓝胶覆盖过小
    CUPOS,      //铜片放置错误
    PAMVALUE,
    EMPTYOBJ,   //获取空区域
    REGIONNUM,  //蓝胶数量过少
    PAMREAD,
    ANGLE,      //蓝胶角度不良
    HANSIZE,    //焊印数量错误

    HanRegHig,  //焊印区域长度
    HanRegWidth,//焊印区域宽度
    HanRegNum,  //焊印区域数量
    UNEXCEPTION = 99
};

enum direction {
    UPDOWN = 0,
    LEFTRIGHT
};

enum Position {
    UP = 0,
    DOWN,
    LEFT,
    RIGHT
};

enum JiErPos {
    CUTOP = 0,
    ALTOP = 10,
    CUBTM = 20,
    ALBTM = 30
};

//坐标轴象限枚举
enum Quadrant {
    FIRST = 0,
    SECOND,
    THIRD,
    FOURTH
};

//MeaSurePos参数枚举
enum  PntSelect
{
    ALL = 0,
    FIRSTPNT,
    LASTPNT
};

//MeaSurePos参数枚举
enum  Transition
{
    ALLTRANS = 0,
    POSITIVE,
    NAGETIVE
};

//显示图像类型枚举
enum Imgtype {
    ORIGINAL = 0,
    RGBR,
    RGBG,
    RGBB,
    HSVH,
    HSVS,
    HSVV
};

//显示图像类型枚举
enum DetectType {
    BLACKBANG = 0,
    DARK,
    WHITE,
    HSVROI,
    DEEPRED,
    BLUE,
    WELD,
    TAPE
};
