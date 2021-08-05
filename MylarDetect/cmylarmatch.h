#ifndef CMYLARMATCH_H
#define CMYLARMATCH_H

#include "halconcpp/HalconCpp.h"
#include "commonUsage/commonDef.h"
#include "cerror.h"

namespace MylarDetect {

struct SMylarMatchData {
    HObject lineAffineTrans;       //线区域
    QList<HObject> glueRegionList; //焊印检测区域
    QList<HObject> mylarRegionList;    //mylar膜检测
    int glueNum = 0;
    int mylarNum =0;

    float row = 0;                //模板匹配之后的Row
    float col = 0;                //模板匹配之后的Col
    float angle = 0;              //模板匹配之后的Angle
    float score = 0;             //模板匹配之后的得分
};

class CMylarMatchPrivate;
class CMylarMatch
{
public:
    CMylarMatch();
    ~CMylarMatch();

    inline void setModelFilePath(const QString& filepath) { m_modelFilePath = filepath; }

    inline void setTaskName(const QString& task) { m_taskName = task;}

    /**
     * @brief pamRead: 参数读取
     * @param pamMap：传入的检测参数
     */
    CError pamRead(QMap<QString, QMap<QString, QVariant>>& pamMap);

    /**
     * @brief setGlueNum:设置焊印的检测数量
     */
    void setGlueNum(cint glueNum) { m_glueNum = glueNum; }

    /**
     * @brief setMylarNum:设置mylar膜检测数量
     */
    void setMylarNum(cint mylarNum) { m_mylarNum = mylarNum; }

    HTuple getModelHandle();

    /**
     * @brief getModelData
     * @return
     */
    CError getModelData();

public:
    CError detect(CONSTIMG dst, SMylarMatchData& matchData);

    CError findComponentModel(CONSTIMG dst, HTuple componentModelID, RHOBJ hShow, SMylarMatchData& matchData);

private:
    QString m_taskName = "MylarMatch";      //参数任务名
    QString m_modelFilePath = "";
    int m_glueNum = 0;
    int m_mylarNum = 0;
    CMylarMatchPrivate* d;
};

}


#endif // CMYLARMATCH_H
