#include "cmylarmeasure.h"
#include "QDebug"

namespace MylarDetect {

class CMylarMeasurePrivate
{
public:
    CMylarMeasurePrivate(){

    }

public:
    float topCoverLine = 25;
    float firstMeasurHigh = 20;
    float firstMeasurTh ;
    float firstLineScore;
    float secondMeasurHigh;
    float secondMeasurTh;
    float secondLineScore;
    float distance;
    float samplingNum;
    float factor;
    float reviewTh;
    float reviewHeight;
    float mult;
    float mAdd;
};

CMylarMeasure::CMylarMeasure() :
    d(new CMylarMeasurePrivate())
{

}

CError CMylarMeasure::pamRead(QMap<QString, QMap<QString, QVariant> > &pamMap)
{
    try {
        auto res = pamMap.value(m_taskName);

        READPAMFLOAT(d->topCoverLine,     "positionRow1", res);
        READPAMFLOAT(d->firstMeasurHigh,  "positionCol1", res);
        READPAMFLOAT(d->firstMeasurTh,    "positionRow2", res);
        READPAMFLOAT(d->firstLineScore,   "positionCol2", res);

        READPAMFLOAT(d->secondMeasurHigh, "secondMeasurHigh", res);
        READPAMFLOAT(d->secondMeasurTh,   "secondMeasurTh", res);
        READPAMFLOAT(d->secondLineScore,  "secondLineScore", res);
        READPAMFLOAT(d->distance,         "distance", res);
        READPAMFLOAT(d->samplingNum,      "samplingNum", res);
        READPAMFLOAT(d->factor,           "factor", res);
        READPAMFLOAT(d->reviewTh,         "reviewTh", res);
        READPAMFLOAT(d->reviewHeight,     "reviewHeight", res);
        READPAMFLOAT(d->mult,             "mult", res);
        READPAMFLOAT(d->mAdd,             "mAdd", res);

        return 0;
    }  catch (...) {
        qDebug() << "CMylarMatch::pamRead crashed";
        return CError(UNEXCEPTION, "CMylarMatch::pamRead crashed");
    }
}


}
