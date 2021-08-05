#include "csingledtct.h"
#include "ccheckcuexit.h"
#include "cget68lregion.h"
#include "cchecktapeangle.h"
#include "cget68lregion.h"
#include "ccheckhanregion.h"
#include "qdebug.h"

namespace CSH68L {

#define CHECKAlGERR(err, di, pos) \
    if(err.isWrong()){\
        qDebug()<<"err info:"<< err.msg();\
        di.clsName = Algorithm::getErrDescri(err.code(), pos);\
        di.result = false; \
        goto thread_end;   \
}

class CSingleDtctPrivate
{
public:
    CSingleDtctPrivate()
    {
        m_cuExiteGrp = new CCheckCuExit();
        m_regionGrp  = new CGet68LRegion();
        m_tapeGrp = new CCheckTapeAngle();
        m_hanRegRrp = new CCheckHanRegion();
    }

    ~CSingleDtctPrivate()
    {
        delete m_cuExiteGrp;
        delete m_regionGrp;
        delete m_tapeGrp;
        delete m_hanRegRrp;
    }

public:
    CCheckCuExit* m_cuExiteGrp;
    CGet68LRegion* m_regionGrp;
    CCheckTapeAngle* m_tapeGrp;
    CCheckHanRegion* m_hanRegRrp;
    bool isCu = false;
};

CSingleDtct::CSingleDtct():
    d(new CSingleDtctPrivate())
{

}

CSingleDtct::~CSingleDtct()
{
    delete d;
}

void CSingleDtct::setCuPos(const bool isCu)
{
    d->isCu = isCu;
}

CError CSingleDtct::PamRead(const QMap<QString, QMap<QString, QMap<QString, QVariant> > >& pamVal)
{
    CError err;
    auto pam = pamVal.value(taskName);

    err = d->m_regionGrp->pamRead(pam);
    CHECKERR(err);
    err = d->m_cuExiteGrp->pamRead(pam);
    CHECKERR(err);
    err = d->m_tapeGrp->pamRead(pam);
    CHECKERR(err);
    err = d->m_hanRegRrp->pamRead(pam);
    CHECKERR(err);

    return 0;
}

DetectResult_t CSingleDtct::detect(CONSTIMG dst)
{
    //参数定义
    CError err;
    HObject hShowObj;
    int hanSizeNum;
    QString resString;
    QRect tapeRect(0, 0, 0, 0);
    L68Regions region;
    LineInfo dwLine;  //定位线
    DetectResult_t result;
    DetectDefectInfo_t di, mesInfo;
    DetectRectInfo_t rectInfo;
    DetectLineInfo_t lineRes;

    bool isCu, isAngleFinal, finalRes = true;
    QRect rect;
    QRect hanDtctRect, hanRect;
    LineInfo lineInfo;

    //输入图像序号
    QPair<QString, QString> mesIndexData("index", QString::number(m_index));
    mesInfo.detectAttribute.text = mesIndexData;
    result.detectDefect.push_back(mesInfo);

    //获取图像区域
    err = d->m_regionGrp->detect(dst, region, dwLine);
    CHECKAlGERR(err, di, m_index);
    lineRes.startPoint = QPoint(dwLine.startCol.D(), dwLine.startRow.D());
    lineRes.endPoint = QPoint(dwLine.endCol.D(), dwLine.endRow.D());
    lineRes.linewidth = 10;
    result.detectLines.push_back(lineRes);

    //获取铜片位置
    err = d->m_cuExiteGrp->detect(region, isCu, rect);
    CHECKAlGERR(err, di, m_index);
    rectInfo.rect = rect;
    if (isCu == d->isCu)
        result.detectRects.push_back(rectInfo);
    else
        di.Rects.push_back(rectInfo);

    //蓝胶角度判定
    //isAngleFinal判定最终是否获得直线参数
    err = d->m_tapeGrp->detect(region, tapeRect, lineInfo, isAngleFinal);

    //添加直线检测的显示
    if (isAngleFinal) {
        lineRes.startPoint = QPoint(lineInfo.startCol.D(), lineInfo.startRow.D());
        lineRes.endPoint = QPoint(lineInfo.endCol.D(), lineInfo.endRow.D());

        if (err.isWrong())
            di.Lines.push_back(lineRes);
        else
            result.detectLines.push_back(lineRes);
    }

    //添加蓝胶是否存在的检测判定
    if (tapeRect.width()) {
        rectInfo.rect = tapeRect;
        if (err.isWrong()) {
            di.clsName = Algorithm::getErrDescri(err.code());
            di.result = false;
            di.Rects.push_back(rectInfo);
            goto thread_end;
        }
    } else {
        CHECKAlGERR(err, di, m_index);
    }

    //焊印检测
    err = d->m_hanRegRrp->detect(region, hanDtctRect, hShowObj, hanSizeNum);
    CHECKAlGERR(err, di, m_index);

    mesIndexData = QPair<QString, QString>("height", QString::number(hanRect.height()));
    mesInfo.clsName = Algorithm::getErrDescri(HanRegHig, m_index);
    mesInfo.value = hanRect.height();
    mesInfo.detectAttribute.text = mesIndexData;
    result.detectDefect.push_back(mesInfo);

    mesIndexData = QPair<QString, QString>("width", QString::number(hanRect.width()));
    mesInfo.clsName = Algorithm::getErrDescri(HanRegWidth, m_index);
    mesInfo.value = hanRect.width();
    mesInfo.detectAttribute.text = mesIndexData;
    result.detectDefect.push_back(mesInfo);

    mesIndexData = QPair<QString, QString>("num", QString::number(20));
    mesInfo.clsName = Algorithm::getErrDescri(HanRegNum, m_index);
    mesInfo.value = 20;
    mesInfo.detectAttribute.text = mesIndexData;
    result.detectDefect.push_back(mesInfo);

    qDebug() << "result detectDefect size:" << result.detectDefect.size();
    rectInfo.rect = hanDtctRect;
    result.detectRects.push_back(rectInfo);

thread_end:
    finalRes = !err.isWrong();
    result.detectDefect.push_back(di);
    resString = finalRes ? "OK" : "NG";
    result.imageInfo.text = QPair<QString, QString>("result:  ", resString);
    result.bGood = finalRes;
    result.bTotolGood = finalRes;
    return result;
}


}
