#pragma once
#include <iostream>
#include <map>
#include <string>
#include "tinyxml2.h"
#include <QList>
#include "framework_global.h"

struct xmlInfo{
    float           value;
    float			min;
    float           max;
    std::string     desc;
};
struct xmlSwitchInfo{
    bool            value;
    std::string     desc;
};
struct productConfInfo{
    std::string name;
    std::string value;
    std::string min;
    std::string max;
    std::string desc;
};

struct productParentStruct{
    std::string name;
    std::string translate;
    QList<productConfInfo> infos;
};

struct algoElementStruct{
    std::string name;
    std::string type;
    std::string value;
    std::string min;
    std::string max;
    std::string translate;
};

struct algoSubItemStruct{
    std::string item;
    bool enable;
    bool force;
    std::string translate;
    QList<algoElementStruct> paras;

};

struct algoItemStruct{
    std::string item;
    bool enable;
    bool force;
    std::string translate;
    QList<algoSubItemStruct> subItems;
};

class FRAMEWORK_EXPORT XmlRead
{
    public:
        XmlRead();
        ~XmlRead();
      std::map<std::string,std::map<std::string, xmlInfo>> parseXML(const char* algorithmFilePath);
      std::map<std::string,std::map<std::string, xmlSwitchInfo>> parseSwitchXML(const char* algorithmFilePath);
      std::map<std::string, xmlInfo> parseXML(const char* algorithmFilePath , const char* algorithmName);
      std::map<std::string, xmlSwitchInfo> parseSwitchXML(const char* algorithmFilePath , const char* algorithmName);

      std::map<std::string, xmlInfo> getAlgoParameter(const char* algorithmFilePath , const char* taskName , const char* subtaskName);
      std::map<std::string,std::map<std::string,bool>> getAlgoItem(const  char* algorithmFilePath);


      bool saveXML(const char *algorithmFilePath,const std::map<std::string,std::map<std::string,float>> &info );
      bool saveXML(const char *algorithmFilePath,const std::map<std::string,std::map<std::string,bool>> &info );

      QList<algoItemStruct> getAlgoConfig(const char* algoFilePath);
      bool saveAlgoXMLConfig(const char *algoFilePath, const QList<algoItemStruct> algoItems);
      QList<productParentStruct> getProductInfo(const char *productFilePath);
      bool saveProductXmlConfig(const char *algoFilePath, const QList<productParentStruct> productItems);

private:
      tinyxml2::XMLElement* queryUserNodeByName(tinyxml2::XMLElement* root,const std::string& userName);

};
