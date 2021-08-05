#include "cmylarmatch.h"
#include "QDebug"

namespace MylarDetect {

class CMylarMatchPrivate
{
public:
    CMylarMatchPrivate()
    {

    }

public:
    float positionRow1 = 380;
    float positionCol1 = 1295;
    float positionRow2 = 2208;
    float positionCol2 = 2126;

    float modelStartAngle = 0;
    float modelEndAngle = 0.1745;
    float modelScore = 0.8;
    float modelRow = 1285.93;
    float modelCol = 1709.35;
    float modelAngle = 0;
    float modelCenter = 0;

public:
    HTuple componentModelID;
    HObject measureObj;
    QList<HObject> glueList;
    QList<HObject> mylarList;
};

CMylarMatch::CMylarMatch():
    d(new CMylarMatchPrivate())
{

}

CMylarMatch::~CMylarMatch()
{
    delete  d;
}

CError CMylarMatch::pamRead(QMap<QString, QMap<QString, QVariant> >& pamMap)
{
    try {
        auto res = pamMap.value(m_taskName);

        READPAMFLOAT(d->positionRow1, "positionRow1", res);
        READPAMFLOAT(d->positionCol1, "positionCol1", res);
        READPAMFLOAT(d->positionRow2, "positionRow2", res);
        READPAMFLOAT(d->positionCol2, "positionCol2", res);
        READPAMFLOAT(d->modelStartAngle, "modelStartAngle", res);
        READPAMFLOAT(d->modelEndAngle, "modelEndAngle", res);
        READPAMFLOAT(d->modelScore, "modelScore", res);
        READPAMFLOAT(d->modelRow, "modelRow", res);
        READPAMFLOAT(d->modelCol, "modelCol", res);
        READPAMFLOAT(d->modelAngle, "modelAngle", res);
        READPAMFLOAT(d->modelCenter, "modelCenter", res);

        return 0;
    }  catch (...) {
        qDebug() << "CMylarMatch::pamRead crashed";
        return CError(UNEXCEPTION, "CMylarMatch::pamRead crashed");
    }
}

HTuple CMylarMatch::getModelHandle()
{
    return d->componentModelID;
}

CError CMylarMatch::getModelData()
{
    try {
        if (m_glueNum == 0)
            return CError(REGIONNUM, "glueNum is 0");

        CError err;
        HObject glueRegion;
        QString shmModelFile, measureFile, fileName;

        shmModelFile = m_modelFilePath + "/model.shm";
        measureFile = m_modelFilePath + "/measure.tif";

        err = Algorithm::isFileExist(shmModelFile);
        CHECKERR(err);
        ReadComponentModel(shmModelFile.toStdString().c_str(), &d->componentModelID);
        CHECKEMPTUPLE(d->componentModelID, "shmModelFile is empty!");

        err = Algorithm::isFileExist(measureFile);
        CHECKERR(err);
        ReadRegion(&d->measureObj, measureFile.toStdString().c_str());
        CHECKEMPIMG(d->measureObj, "measureObj is empty!");


        for (int i = 0; i < m_glueNum; i++) {
            fileName = m_modelFilePath + QString("/glue%1.tif").arg(i + 1);

            err = Algorithm::isFileExist(fileName);
            CHECKERR(err);
            ReadRegion(&glueRegion, fileName.toStdString().c_str());
            CHECKEMPIMG(glueRegion, QString("%1 region empty").arg(measureFile));
            d->glueList.push_back(glueRegion);
        }

        for (int i = 0; i < m_mylarNum; i++) {
            fileName = m_modelFilePath + QString("/mylar%1.tif").arg(i + 1);
            qDebug() << "filename:" << fileName;

            err = Algorithm::isFileExist(fileName);
            CHECKERR(err);

            ReadRegion(&glueRegion, fileName.toStdString().c_str());
            CHECKEMPIMG(glueRegion, QString("%1 region empty").arg(measureFile));
            d->mylarList.push_back(glueRegion);
        }

        return 0;
    }  catch (...) {
        qDebug() << "CMylarMatch::getModelData crashed";
        return CError(UNEXCEPTION, "CMylarMatch::getModelData crashed");
    }
}

CError CMylarMatch::detect(const HObject &dst, SMylarMatchData &matchData)
{
    try {
        HObject hShow;

        CError err = getModelData();
        CHECKERR(err);

        err = findComponentModel(dst, d->componentModelID, hShow, matchData);
        CHECKERR(err);

        qDebug() << "match data:" << matchData.row << matchData.col;

        HTuple tHomMat2D;
        HObject hAffine;
        VectorAngleToRigid(d->modelRow, d->modelCol, d->modelAngle,
                           matchData.row, matchData.col, matchData.angle, &tHomMat2D);

        AffineTransRegion(d->measureObj, &hAffine, tHomMat2D, "nearest_neighbor");

        matchData.lineAffineTrans = hAffine;
        matchData.glueNum = d->glueList.length();
        matchData.mylarNum = d->mylarList.length();
        matchData.glueRegionList.clear();
        matchData.mylarRegionList.clear();

        for(auto img : d->glueList){
            AffineTransRegion(img, &hAffine, tHomMat2D, "nearest_neighbor");
            matchData.glueRegionList.push_back(hAffine);
        }

        for(auto img : d->mylarList){
            AffineTransRegion(img, &hAffine, tHomMat2D, "nearest_neighbor");
            matchData.mylarRegionList.push_back(hAffine);
        }

        return 0;
    }  catch (...) {
        qDebug() << "CMylarMatch::detect crashed";
        return CError(UNEXCEPTION, "CMylarMatch::detect crashed");
    }
}

CError CMylarMatch::findComponentModel(const HObject& dst, HTuple componentModelID, RHOBJ hShow, SMylarMatchData& matchData)
{
    try {
        CHECKEMPIMG(dst, "CMylarMatch::findComponentModel dst is empty");
        CHECKEMPTUPLE(componentModelID, "CMylarMatch::findComponentModel componentModelID is empty");

        HObject hRect, hReduce;
        HTuple tModelStartAngle, tModelEndAngle;
        HTuple tModelStart, tModelEnd, tScore, tRowComp, tColComp, tAngleComp, tScoreComp, tModelComp;

        tModelStartAngle = d->modelStartAngle;
        tModelEndAngle = d->modelEndAngle;

        GenRectangle1(&hRect, d->positionRow1, d->positionCol1, d->positionRow2, d->positionCol2);
        ReduceDomain(dst, hRect, &hReduce);

        hShow = hRect;

        FindComponentModel(hReduce, componentModelID, 0, tModelStartAngle.TupleRad(), tModelEndAngle.TupleRad(),
                           d->modelScore, 1, 0.5, "stop_search", "search_from_best", "none", 0.3, "interpolation", 0, 0.8,
                           &tModelStart, &tModelEnd, &tScore,
                           &tRowComp, &tColComp, &tAngleComp, &tScoreComp, &tModelComp);

        if (tScore < 0.95) {
            FindComponentModel(hReduce, componentModelID, 0, tModelStartAngle.TupleRad(), tModelEndAngle.TupleRad(),
                               d->modelScore, 1, 0.5, "stop_search", "search_from_best", "none", 0.1, "interpolation", 0, 0.95,
                               &tModelStart, &tModelEnd, &tScore,
                               &tRowComp, &tColComp, &tAngleComp, &tScoreComp, &tModelComp);
        }

        if(tScore < d->modelScore)
            return CError(NG, QString("Model Match score is low %1").arg(tScore.D()));

        matchData.row = tRowComp.D();
        matchData.col = tColComp.D();
        matchData.angle = tAngleComp.D();
        matchData.score = tScoreComp.D();

        qDebug() << "row, col, angle" << matchData.row << matchData.col << matchData.angle;

        return 0;
    }  catch (...) {
        qDebug() << "CMylarMatch::findComponentModel crashed";
        return CError(UNEXCEPTION, "CMylarMatch::findComponentModel crashed");
    }
}


}
