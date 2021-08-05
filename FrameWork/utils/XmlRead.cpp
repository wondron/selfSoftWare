#include "XmlRead.h"
#include "QDebug"
//#include <QMessageBox>

XmlRead::XmlRead()
{
}

XmlRead::~XmlRead()
{
}

std::map<std::string,std::map<std::string, xmlInfo>> XmlRead::parseXML(const char* algorithmFilePath)
{
    std::map<std::string,std::map<std::string, xmlInfo>> resMap;
    std::map<std::string, xmlInfo> nodeMap; //存放结果，key对应参数名，value对应参数值
    resMap.clear();
    nodeMap.clear();

    //新建一个xmldocument对象
    tinyxml2::XMLDocument docXml;

    //调用对象的LoadFile函数加载文件，并将结果返回给loadRes
    tinyxml2::XMLError loadRes = docXml.LoadFile(algorithmFilePath);

    if (tinyxml2::XML_SUCCESS == loadRes)
    {
        //获取XML的root节点
        tinyxml2::XMLElement* root = docXml.RootElement();
        if (NULL == root)
        {
            std::cout << "Failed to load file: No root element." << std::endl;
            docXml.Clear();
            exit(1);
        }
        tinyxml2::XMLElement *p = root->FirstChildElement("Task");

        while (p)
        {
            const tinyxml2::XMLAttribute* parentNode = p->FirstAttribute();
            const char* xmlAlgorithmName = parentNode->Name();
            const char* xmlAlgorithmValue = parentNode->Value();
            std::string strAlgorithmName = xmlAlgorithmName;
            std::string strAlgorithmValue = xmlAlgorithmValue;
            if(strAlgorithmName == "name"){
                parentNode = parentNode->Next();
                const char* xmlTypeName = parentNode->Name();
                const char* xmlTypeValue = parentNode->Value();
                std::string strTypeName = xmlTypeName;
                std::string strTypeValue = xmlTypeValue;
                if(strTypeName == "type"){
                    if(strTypeValue == "float"){
                        tinyxml2::XMLElement* child = p->FirstChildElement();
                        nodeMap.clear();
                        while (child)
                        {
                            const tinyxml2::XMLAttribute* pNode = child->FirstAttribute();
                            std::string key = "";
                            std::string value = "";
                            xmlInfo m_xmlInfo;
                            while (pNode)
                            {
                                const char* attribName = pNode->Name();
                                const char* attribVal = pNode->Value();

                                std::string sName = attribName;
                                if ("name" == sName) //如果属性名字为“name”
                                {
                                    key = attribVal;
                                }
                                if ("value" == sName) //如果属性名字为“value”
                                {
                                    value = attribVal;
                                    if (key != "")
                                    {
                                        m_xmlInfo.value = std::stof(value);
                                    }
                                }
                                if ("min" == sName) //如果属性名字为“value”
                                {
                                    value = attribVal;
                                    if (key != "")
                                    {
                                        m_xmlInfo.min = std::stof(value);
                                    }
                                }
                                if ("max" == sName) //如果属性名字为“value”
                                {
                                    value = attribVal;
                                    if (key != "")
                                    {
                                        m_xmlInfo.max = std::stof(value);
                                    }
                                }
                                if ("desc" == sName) //如果属性名字为“value”
                                {
                                    m_xmlInfo.desc = attribVal;
                                    break;
                                }
                                pNode = pNode->Next();
                            }
                            nodeMap.insert(std::pair<std::string, xmlInfo>(key,m_xmlInfo));
                            child = child->NextSiblingElement();
                        }
                        if(!nodeMap.empty()){
                            resMap.insert(std::pair<std::string,std::map<std::string, xmlInfo>>(strAlgorithmValue,nodeMap));
                        }

                        p = p->NextSiblingElement();
                    }else {
                        p = p->NextSiblingElement();
                    }
                }else{
                   p = p->NextSiblingElement();
                }


            }else{
                p = p->NextSiblingElement();
            }
        }

    }
    else
    {
        qDebug() << "Error opening recipe file. Please check if XML exists";
    }

    return resMap;
}

std::map<std::string, std::map<std::string, xmlSwitchInfo> > XmlRead::parseSwitchXML(const char *algorithmFilePath)
{
    std::map<std::string,std::map<std::string, xmlSwitchInfo>> resMap;
    std::map<std::string, xmlSwitchInfo> nodeMap; //存放结果，key对应参数名，value对应参数值
    resMap.clear();
    nodeMap.clear();

    //新建一个xmldocument对象
    tinyxml2::XMLDocument docXml;

    //调用对象的LoadFile函数加载文件，并将结果返回给loadRes
    tinyxml2::XMLError loadRes = docXml.LoadFile(algorithmFilePath);

    if (tinyxml2::XML_SUCCESS == loadRes)
    {
        //获取XML的root节点
        tinyxml2::XMLElement* root = docXml.RootElement();
        if (NULL == root)
        {
            std::cout << "Failed to load file: No root element." << std::endl;
            docXml.Clear();
            exit(1);
        }
        tinyxml2::XMLElement *p = root->FirstChildElement("Task");

        while (p)
        {
            const tinyxml2::XMLAttribute* parentNode = p->FirstAttribute();
            const char* xmlAlgorithmName = parentNode->Name();
            const char* xmlAlgorithmValue = parentNode->Value();
            std::string strAlgorithmName = xmlAlgorithmName;
            std::string strAlgorithmValue = xmlAlgorithmValue;
            if(strAlgorithmName == "name"){
                parentNode = parentNode->Next();
                const char* xmlTypeName = parentNode->Name();
                const char* xmlTypeValue = parentNode->Value();
                std::string strTypeName = xmlTypeName;
                std::string strTypeValue = xmlTypeValue;
                if(strTypeName == "type"){
                    if(strTypeValue == "switch"){
                        tinyxml2::XMLElement* child = p->FirstChildElement();
                        nodeMap.clear();
                        while (child)
                        {
                            const tinyxml2::XMLAttribute* pNode = child->FirstAttribute();
                            std::string key = "";
                            std::string value = "";
                            xmlSwitchInfo m_xmlInfo;
                            while (pNode)
                            {
                                const char* attribName = pNode->Name();
                                const char* attribVal = pNode->Value();

                                std::string sName = attribName;
                                if ("name" == sName) //如果属性名字为“name”
                                {
                                    key = attribVal;
                                }
                                if ("value" == sName) //如果属性名字为“value”
                                {
                                    value = attribVal;
                                    if (key != "")
                                    {
                                        if(value.compare("true") == 0){
                                            m_xmlInfo.value = true;
                                        }else {
                                            m_xmlInfo.value = false;
                                        }
                                    }

                                }
                                if ("desc" == sName) //如果属性名字为“value”
                                {
                                    m_xmlInfo.desc = attribVal;
                                    break;
                                }
                                pNode = pNode->Next();
                            }
                            nodeMap.insert(std::pair<std::string, xmlSwitchInfo>(key,m_xmlInfo));
                            child = child->NextSiblingElement();
                        }
                        if(!nodeMap.empty()){
                            resMap.insert(std::pair<std::string,std::map<std::string, xmlSwitchInfo>>(strAlgorithmValue,nodeMap));
                        }

                        p = p->NextSiblingElement();
                    }else {
                        p = p->NextSiblingElement();
                    }
                }else{
                   p = p->NextSiblingElement();
                }


            }else{
                p = p->NextSiblingElement();
            }
        }

    }
    else
    {
        qDebug() << "Error opening recipe file. Please check if XML exists";
    }

    return resMap;
}

std::map<std::string, xmlInfo> XmlRead::parseXML(const char *algorithmFilePath, const char *algorithmName)
{
   std::map<std::string, xmlInfo> resMap;
    resMap.clear();

    //新建一个xmldocument对象
    tinyxml2::XMLDocument docXml;

    //调用对象的LoadFile函数加载文件，并将结果返回给loadRes
    tinyxml2::XMLError loadRes = docXml.LoadFile(algorithmFilePath);

    if (tinyxml2::XML_SUCCESS == loadRes)
    {
        //获取XML的root节点
        tinyxml2::XMLElement* root = docXml.RootElement();
        if (NULL == root)
        {
            std::cout << "Failed to load file: No root element." << std::endl;
            docXml.Clear();
            exit(1);
        }
        tinyxml2::XMLElement *p = root->FirstChildElement("Task");
        while (p)
        {
            const tinyxml2::XMLAttribute* parentNode = p->FirstAttribute();
            const char* xmlAlgorithmName = parentNode->Name();
            const char* xmlAlgorithmValue = parentNode->Value();
            std::string strAlgorithmName = xmlAlgorithmName;
            std::string strAlgorithmValue = xmlAlgorithmValue;
            if(strAlgorithmName == "name"){
                if(strAlgorithmValue == algorithmName){
                    parentNode = parentNode->Next();
                    const char* xmlTypeName = parentNode->Name();
                    const char* xmlTypeValue = parentNode->Value();
                    std::string strTypeName = xmlTypeName;
                    std::string strTypeValue = xmlTypeValue;
                    if(strTypeName == "type"){
                        if(strTypeValue == "float"){
                            tinyxml2::XMLElement* child = p->FirstChildElement();
                            //const tinyxml2::XMLAttribute* attrib = p->FirstAttribute();
                            //std::cout << attrib->Name() << ": " << attrib->Value() << std::endl; //输出Task的name和value
                            while (child)
                            {
                                //const char* name = child->Name(); //例如："regionW"
                                //std::string key = name;
                                //const char* val = child->GetText(); //例如："500"
                                //std::string value = val;
                                //resMap[key] = std::stof(value); //string转float
                                //child = child->NextSiblingElement();
                                const tinyxml2::XMLAttribute* pNode = child->FirstAttribute();
                                std::cout << pNode->Name() << ": " << pNode->Value() << std::endl; //输出属性的name和value
                                std::string key = "";
                                std::string value = "";
                                xmlInfo m_xmlInfo;
                                while (pNode)
                                {
                                    const char* attribName = pNode->Name();
                                    const char* attribVal = pNode->Value();

                                    std::string sName = attribName;
                                    if ("name" == sName) //如果属性名字为“name”
                                    {
                                        key = attribVal;
                                    }
                                    if ("value" == sName) //如果属性名字为“value”
                                    {
                                        value = attribVal;
                                        if (key != "")
                                        {
                                            m_xmlInfo.value = std::stof(value);
                                        }
                                    }
                                    if ("min" == sName) //如果属性名字为“value”
                                    {
                                        value = attribVal;
                                        if (key != "")
                                        {
                                            m_xmlInfo.min = std::stof(value);
                                        }
                                    }
                                    if ("max" == sName) //如果属性名字为“value”
                                    {
                                        value = attribVal;
                                        if (key != "")
                                        {
                                            m_xmlInfo.max = std::stof(value);
                                        }
                                    }
                                    if ("desc" == sName) //如果属性名字为“value”
                                    {
                                        m_xmlInfo.desc = attribVal;
                                        break;
                                    }
                                    pNode = pNode->Next();
                                }
                                resMap.insert(std::pair<std::string, xmlInfo>(key,m_xmlInfo));
                                child = child->NextSiblingElement();
                            }
                            break;

                        }else{
                            p = p->NextSiblingElement();
                        }
                    }else{
                       p = p->NextSiblingElement();
                    }
                }else{
                    p = p->NextSiblingElement();
                }


            }else{
                p = p->NextSiblingElement();
            }
        }
    }
    else
    {
        qDebug() << "Error opening recipe file. Please check if XML exists";
    }

    return resMap;

}

std::map<std::string, xmlSwitchInfo> XmlRead::parseSwitchXML(const char *algorithmFilePath, const char *algorithmName)
{
    std::map<std::string, xmlSwitchInfo> resMap;
     resMap.clear();

     //新建一个xmldocument对象
     tinyxml2::XMLDocument docXml;

     //调用对象的LoadFile函数加载文件，并将结果返回给loadRes
     tinyxml2::XMLError loadRes = docXml.LoadFile(algorithmFilePath);

     if (tinyxml2::XML_SUCCESS == loadRes)
     {
         //获取XML的root节点
         tinyxml2::XMLElement* root = docXml.RootElement();
         if (NULL == root)
         {
             std::cout << "Failed to load file: No root element." << std::endl;
             docXml.Clear();
             exit(1);
         }
         tinyxml2::XMLElement *p = root->FirstChildElement("Task");
         while (p)
         {
             const tinyxml2::XMLAttribute* parentNode = p->FirstAttribute();
             const char* xmlAlgorithmName = parentNode->Name();
             const char* xmlAlgorithmValue = parentNode->Value();
             std::string strAlgorithmName = xmlAlgorithmName;
             std::string strAlgorithmValue = xmlAlgorithmValue;
             if(strAlgorithmName == "name"){
                 if(strAlgorithmValue == algorithmName){
                     parentNode = parentNode->Next();
                     const char* xmlTypeName = parentNode->Name();
                     const char* xmlTypeValue = parentNode->Value();
                     std::string strTypeName = xmlTypeName;
                     std::string strTypeValue = xmlTypeValue;
                     if(strTypeName == "type"){
                         if(strTypeValue == "switch"){
                             tinyxml2::XMLElement* child = p->FirstChildElement();
                             //const tinyxml2::XMLAttribute* attrib = p->FirstAttribute();
                             //std::cout << attrib->Name() << ": " << attrib->Value() << std::endl; //输出Task的name和value
                             while (child)
                             {
                                 //const char* name = child->Name(); //例如："regionW"
                                 //std::string key = name;
                                 //const char* val = child->GetText(); //例如："500"
                                 //std::string value = val;
                                 //resMap[key] = std::stof(value); //string转float
                                 //child = child->NextSiblingElement();
                                 const tinyxml2::XMLAttribute* pNode = child->FirstAttribute();
                                 std::cout << pNode->Name() << ": " << pNode->Value() << std::endl; //输出属性的name和value
                                 std::string key = "";
                                 std::string value = "";
                                 xmlSwitchInfo m_xmlInfo;
                                 while (pNode)
                                 {
                                     const char* attribName = pNode->Name();
                                     const char* attribVal = pNode->Value();

                                     std::string sName = attribName;
                                     if ("name" == sName) //如果属性名字为“name”
                                     {
                                         key = attribVal;
                                     }
                                     if ("value" == sName) //如果属性名字为“value”
                                     {
                                         value = attribVal;
                                         if (key != "")
                                         {
                                             if(value.compare("true") == 0){
                                                 m_xmlInfo.value = true;
                                             }else {
                                                 m_xmlInfo.value = false;
                                             }
                                         }

                                     }

                                     if ("desc" == sName) //如果属性名字为“value”
                                     {
                                         m_xmlInfo.desc = attribVal;
                                         break;
                                     }
                                     pNode = pNode->Next();
                                 }
                                 resMap.insert(std::pair<std::string, xmlSwitchInfo>(key,m_xmlInfo));
                                 child = child->NextSiblingElement();
                             }
                             break;

                         }else{
                             p = p->NextSiblingElement();
                         }
                     }else{
                        p = p->NextSiblingElement();
                     }
                 }else{
                     p = p->NextSiblingElement();
                 }


             }else{
                 p = p->NextSiblingElement();
             }
         }
     }
     else
     {

         qDebug() << "Error opening recipe file. Please check if XML exists";
     }

     return resMap;
}

std::map<std::string, xmlInfo> XmlRead::getAlgoParameter(const char *algorithmFilePath, const char *taskName, const char *subtaskName)
{
    std::map<std::string, xmlInfo> resMap;
    return resMap;
}

std::map<std::string, std::map<std::string, bool>> XmlRead::getAlgoItem(const char *algorithmFilePath)
{
    std::map<std::string, std::map<std::string, bool>> resMap;


    return resMap;
}

QList<productParentStruct> XmlRead::getProductInfo(const char *productFilePath)
{
    QList<productParentStruct> configs;

//新建一个xmldocument对象
    tinyxml2::XMLDocument docXml;

    //调用对象的LoadFile函数加载文件，并将结果返回给loadRes
    tinyxml2::XMLError loadRes = docXml.LoadFile(productFilePath);

    if (tinyxml2::XML_SUCCESS == loadRes)
    {
        //获取XML的root节点
        tinyxml2::XMLElement* root = docXml.RootElement();
        if (NULL == root)
        {
            std::cout << "Failed to load file: No root element." << std::endl;
            docXml.Clear();
            return configs;
        }
        tinyxml2::XMLElement *p = root->FirstChildElement("Task");
        while (p)
        {
            productParentStruct ItemConfig;
            const tinyxml2::XMLAttribute* parentNode = p->FirstAttribute();
            const char* xmlAlgorithmName = parentNode->Name();
            const char* xmlAlgorithmValue = parentNode->Value();
            std::string strAlgorithmName = xmlAlgorithmName;
            std::string strAlgorithmValue = xmlAlgorithmValue;
            if(strAlgorithmName == "name"){
                ItemConfig.name = strAlgorithmValue;
                parentNode = p->FindAttribute("desc");
                if(parentNode != NULL){
                    ItemConfig.translate = parentNode->Value();
                }else {
                    ItemConfig.translate = "";
                }

                tinyxml2::XMLElement* element = p->FirstChildElement("Para");
                while (element) {
                    const tinyxml2::XMLAttribute* elementNode = element->FirstAttribute();
                    std::string key = "";
                    std::string value = "";
                    productConfInfo elementConfig;
                    while (elementNode) {

                        const char* attribName = elementNode->Name();
                        const char* attribVal = elementNode->Value();

                        std::string strName = attribName;
                        if("name" == strName){
                            elementConfig.name = attribVal;
                        }

                        if("value" == strName){
                            elementConfig.value = attribVal;
                        }


                        if("min" == strName){
                            elementConfig.min = attribVal;
                        }

                        if("max" == strName){
                            elementConfig.max = attribVal;
                        }

                        if("desc" == strName){
                            elementConfig.desc = attribVal;
                        }
                        elementNode = elementNode->Next();
                    }
                    ItemConfig.infos.append(elementConfig);
                    element = element ->NextSiblingElement();
                }

            }
            if(ItemConfig.infos.count()>0){
                configs.append(ItemConfig);
            }
            p = p->NextSiblingElement();
        }
    }else {

        qDebug() << "Error opening recipe file. Please check if XML exists";
    }
    return configs;
}

bool XmlRead::saveProductXmlConfig(const char *algoFilePath, const QList<productParentStruct> productItems)
{
    tinyxml2::XMLDocument docXml;
    bool result = false;
    std::string strAlgoFilePath = algoFilePath;
    if(strAlgoFilePath.length()<=0){
        return false;
    }

    tinyxml2::XMLError loadRes = docXml.LoadFile(algoFilePath);

    if (tinyxml2::XML_SUCCESS == loadRes)
    {
        //获取XML的root节点
        tinyxml2::XMLElement* root = docXml.RootElement();
        if (NULL == root)
        {
            std::cout << "Failed to load file: No root element." << std::endl;
            docXml.Clear();
            return false;
        }
        //tinyxml2::XMLElement *p = root->FirstChildElement("Task");
        for(int i=0; i<productItems.count(); i++){
            tinyxml2::XMLElement* userNode = queryUserNodeByName(root,productItems[i].name);
            if(userNode != NULL){
                const tinyxml2::XMLAttribute* userNodeEnable = userNode->FindAttribute("Enable");

                for(int j=0; j<productItems[i].infos.count(); j++){
                    tinyxml2::XMLElement* subNode = queryUserNodeByName(userNode,productItems[i].infos[j].name);
                    if(subNode != NULL){

                        std::string value = productItems[i].infos[j].value;
                        std::string xmlValue = subNode->Attribute("value");
                        if(value.compare(xmlValue)!=0){
                            subNode->SetAttribute("value",value.data());
                            result = true;
                        }
                    }

                }

            }
        }
        docXml.SaveFile(algoFilePath);
    }
    else
    {
        qDebug() << "Error opening recipe file. Please check if XML exists";
    }
    return result;
}

bool XmlRead::saveXML(const char *algorithmFilePath,const std::map<std::string, std::map<std::string, float> > &info)
{

    tinyxml2::XMLDocument docXml;
    bool result = false;

    //调用对象的LoadFile函数加载文件，并将结果返回给loadRes
    tinyxml2::XMLError loadRes = docXml.LoadFile(algorithmFilePath);

    if (tinyxml2::XML_SUCCESS == loadRes)
    {
        //获取XML的root节点
        tinyxml2::XMLElement* root = docXml.RootElement();
        if (NULL == root)
        {
            std::cout << "Failed to load file: No root element." << std::endl;
            docXml.Clear();
            exit(1);
        }
        //tinyxml2::XMLElement *p = root->FirstChildElement("Task");
        for(auto iter = info.cbegin();iter != info.cend();iter++){
            tinyxml2::XMLElement* userNode = queryUserNodeByName(root,iter->first);

            if(userNode != NULL){
                for(auto childIter = iter->second.cbegin();childIter != iter->second.cend();childIter++){
                    tinyxml2::XMLElement* childNode = queryUserNodeByName(userNode,childIter->first);
                    if(childNode != NULL){
                        float value = childIter->second;
                        float xmlValue = std::stof(childNode->Attribute("value"));
                        if(value != xmlValue){
                            childNode->SetAttribute("value",value);
                            result = true;
                        }
                    }
                }
            }
        }
        docXml.SaveFile(algorithmFilePath);
    }
    else
    {
        qDebug() << "Error opening recipe file. Please check if XML exists";
    }



    return result;
}

bool XmlRead::saveXML(const char *algorithmFilePath, const std::map<std::string, std::map<std::string, bool> > &info)
{
    tinyxml2::XMLDocument docXml;
    bool result = false;

    //调用对象的LoadFile函数加载文件，并将结果返回给loadRes
    tinyxml2::XMLError loadRes = docXml.LoadFile(algorithmFilePath);

    if (tinyxml2::XML_SUCCESS == loadRes)
    {
        //获取XML的root节点
        tinyxml2::XMLElement* root = docXml.RootElement();
        if (NULL == root)
        {
            std::cout << "Failed to load file: No root element." << std::endl;
            docXml.Clear();
            exit(1);
        }
        //tinyxml2::XMLElement *p = root->FirstChildElement("Task");
        for(auto iter = info.cbegin();iter != info.cend();iter++){
            tinyxml2::XMLElement* userNode = queryUserNodeByName(root,iter->first);

            if(userNode != NULL){
                for(auto childIter = iter->second.cbegin();childIter != iter->second.cend();childIter++){
                    tinyxml2::XMLElement* childNode = queryUserNodeByName(userNode,childIter->first);
                    if(childNode != NULL){
                        bool value = childIter->second;
                        std::string strXMLValue = childNode->Attribute("value");
                        bool xmlValue;
                        if(strXMLValue.compare("true") == 0){
                           xmlValue = true;
                           if(value != xmlValue){
                               childNode->SetAttribute("value",value);
                               result = true;
                           }
                        }else if(strXMLValue.compare("false") == 0){
                            xmlValue = false;
                            if(value != xmlValue){
                                childNode->SetAttribute("value",value);
                                result = true;
                            }
                        }else{
                            childNode->SetAttribute("value",value);
                            result = true;
                        }

                    }
                }
            }
        }
        docXml.SaveFile(algorithmFilePath);
    }
    else
    {
        qDebug() << "Error opening recipe file. Please check if XML exists";
    }
    return result;
}

QList<algoItemStruct> XmlRead::getAlgoConfig(const char *algoFilePath)
{
    QList<algoItemStruct> configs;

//新建一个xmldocument对象
    tinyxml2::XMLDocument docXml;

    //调用对象的LoadFile函数加载文件，并将结果返回给loadRes
    tinyxml2::XMLError loadRes = docXml.LoadFile(algoFilePath);

    if (tinyxml2::XML_SUCCESS == loadRes)
    {
        //获取XML的root节点
        tinyxml2::XMLElement* root = docXml.RootElement();
        if (NULL == root)
        {
            std::cout << "Failed to load file: No root element." << std::endl;
            docXml.Clear();
            return configs;
        }
        tinyxml2::XMLElement *p = root->FirstChildElement("Task");
        while (p)
        {
            algoItemStruct ItemConfig;
            const tinyxml2::XMLAttribute* parentNode = p->FirstAttribute();
            const char* xmlAlgorithmName = parentNode->Name();
            const char* xmlAlgorithmValue = parentNode->Value();
            std::string strAlgorithmName = xmlAlgorithmName;
            std::string strAlgorithmValue = xmlAlgorithmValue;
            if(strAlgorithmName == "name"){
                ItemConfig.item = strAlgorithmValue;
                parentNode = parentNode->Next();
                const char* xmlTypeName = parentNode->Name();
                const char* xmlTypeValue = parentNode->Value();
                std::string strTypeName = xmlTypeName;
                std::string strTypeValue = xmlTypeValue;
                if(strTypeName == "type"){
                    if(strTypeValue == "algo"){
                        parentNode = p->FindAttribute("Enable");
                        if(parentNode != NULL){
                            ItemConfig.enable = parentNode->BoolValue();
                        }else {
                            ItemConfig.enable = false;
                        }

                        parentNode = p->FindAttribute("Force");
                        if(parentNode != NULL){
                            ItemConfig.force = parentNode->BoolValue();
                        }else {
                            ItemConfig.force = false;
                        }

                        parentNode = p->FindAttribute("desc");
                        if(parentNode != NULL){
                            ItemConfig.translate = parentNode->Value();
                        }else {
                            ItemConfig.translate = "";
                        }
                        tinyxml2::XMLElement* child = p->FirstChildElement("Subtask");
                        while(child){
                            algoSubItemStruct subItemConfig;
                            const tinyxml2::XMLAttribute* childNode = child->FirstAttribute();
                            const char* childName = childNode->Name();
                            const char* childValue = childNode->Value();
                            std::string strChildName = childName;
                            std::string strChildValue = childValue;
                            if(strChildName == "name"){
                                subItemConfig.item =strChildValue;
                                childNode = child->FindAttribute("Enable");
                                if(childNode != NULL){
                                    subItemConfig.enable = childNode->BoolValue();
                                }else {
                                    subItemConfig.enable = false;
                                }

                                childNode = child->FindAttribute("Force");
                                if(childNode != NULL){
                                    subItemConfig.force = childNode->BoolValue();
                                }else {
                                    subItemConfig.force = false;
                                }

                                childNode = child->FindAttribute("desc");
                                if(childNode != NULL){
                                    subItemConfig.translate = childNode->Value();
                                }else {
                                    subItemConfig.translate = "";
                                }
                                tinyxml2::XMLElement* element = child->FirstChildElement("Para");
                                while (element) {
                                    const tinyxml2::XMLAttribute* elementNode = element->FirstAttribute();
                                    std::string key = "";
                                    std::string value = "";
                                    algoElementStruct elementConfig;
                                    while (elementNode) {

                                        const char* attribName = elementNode->Name();
                                        const char* attribVal = elementNode->Value();

                                        std::string strName = attribName;
                                        if("name" == strName){
                                            elementConfig.name = attribVal;
                                        }

                                        if("value" == strName){
                                            elementConfig.value = attribVal;
                                        }

                                        if("type" == strName){
                                            elementConfig.type = attribVal;
                                        }

                                        if("min" == strName){
                                            elementConfig.min = attribVal;
                                        }

                                        if("max" == strName){
                                            elementConfig.max = attribVal;
                                        }

                                        if("desc" == strName){
                                            elementConfig.translate = attribVal;
                                        }
                                        elementNode = elementNode->Next();
                                    }
                                    subItemConfig.paras.append(elementConfig);
                                    element = element ->NextSiblingElement();
                                }
                            }
                            ItemConfig.subItems.append(subItemConfig);
                            child = child -> NextSiblingElement();
                        }
                    }
                }
            }
            if(ItemConfig.subItems.count()>0){
                configs.append(ItemConfig);
            }
            p = p->NextSiblingElement();
        }
    }else {
        qDebug() << "Error opening recipe file. Please check if XML exists";
    }
    return configs;
}

bool XmlRead::saveAlgoXMLConfig(const char *algoFilePath, const QList<algoItemStruct> algoItems)
{

    tinyxml2::XMLDocument docXml;
    bool result = false;
    std::string strAlgoFilePath = algoFilePath;
    if(strAlgoFilePath.length()<=0){
        return false;
    }

    tinyxml2::XMLError loadRes = docXml.LoadFile(algoFilePath);

    if (tinyxml2::XML_SUCCESS == loadRes)
    {
        //获取XML的root节点
        tinyxml2::XMLElement* root = docXml.RootElement();
        if (NULL == root)
        {
            std::cout << "Failed to load file: No root element." << std::endl;
            docXml.Clear();
            return false;
        }
        //tinyxml2::XMLElement *p = root->FirstChildElement("Task");
        for(int i=0; i<algoItems.count(); i++){
            tinyxml2::XMLElement* userNode = queryUserNodeByName(root,algoItems[i].item);
            if(userNode != NULL){
                const tinyxml2::XMLAttribute* userNodeEnable = userNode->FindAttribute("Enable");
                bool userEnable = algoItems[i].enable;
                bool xmlUserEnable = userNodeEnable->BoolValue();
                if(userEnable != xmlUserEnable){
                    userNode->SetAttribute("Enable",userEnable);
                }

                for(int j=0; j<algoItems[i].subItems.count(); j++){
                    tinyxml2::XMLElement* subNode = queryUserNodeByName(userNode,algoItems[i].subItems[j].item);
                    if(subNode != NULL){
                        const tinyxml2::XMLAttribute* subNodeEnable = userNode->FindAttribute("Enable");
                        bool subEnable = algoItems[i].subItems[j].enable;
                        bool xmlsubEnable = subNodeEnable->BoolValue();
                        if(subEnable != xmlsubEnable){
                            subNode->SetAttribute("Enable",subEnable);
                        }

                        for(int k=0; k<algoItems[i].subItems[j].paras.count();k++){
                            tinyxml2::XMLElement* childNode = queryUserNodeByName(subNode,algoItems[i].subItems[j].paras[k].name);
                            if(childNode != NULL){
                                std::string value = algoItems[i].subItems[j].paras[k].value;
                                std::string xmlValue = childNode->Attribute("value");
                                if(value.compare(xmlValue)!=0){
                                    childNode->SetAttribute("value",value.data());
                                    result = true;
                                }
                            }
                        }
                    }

                }

            }
        }
        docXml.SaveFile(algoFilePath);
    }
    else
    {
        std::cout << "Error open file." << std::endl;
    }
    return result;
}

tinyxml2::XMLElement *XmlRead::queryUserNodeByName(tinyxml2::XMLElement *root, const std::string &userName)
{
    tinyxml2::XMLElement* userNode = root->FirstChildElement();
    while(userNode!= NULL){
        const tinyxml2::XMLAttribute* parentNode = userNode->FirstAttribute();
        const char* rootName = parentNode->Name();
        const char* rootValue = parentNode->Value();
        std::string strmName = rootName;
        std::string strValue = rootValue;
        if(strValue==userName)
            break;
        userNode = userNode->NextSiblingElement();
    }
    return userNode;
}
