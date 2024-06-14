#include "ReembedOptions.h"

#include <iostream>
#include <set>
#include "CrossSpeciesComparisonPhyloTreeViewPlugin.h"
#include<string>  
#include <QFileDialog>
#include <QPageLayout>
#include <QWebEngineView>
#include <string>
#include <typeinfo>
#include "lib/JSONnlohmann/json.hpp"
#include "lib/Clustering/fastcluster.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <vector>
#include <algorithm>
#include <iterator>
#include <QJsonValue>
#include <numeric> 
using namespace mv;
using namespace mv::gui;

std::vector<int> intersectVectors(std::vector<int>& vec1, std::vector<int>& vec2) {
    std::sort(vec1.begin(), vec1.end());
    std::sort(vec2.begin(), vec2.end());

    std::vector<int> result;
    std::set_intersection(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), std::back_inserter(result));

    return result;
}

std::vector<int> findCommonValues(std::vector<int> vec1, std::vector<int> vec2) {
    std::vector<int> commonValues;

    std::sort(vec1.begin(), vec1.end());
    std::sort(vec2.begin(), vec2.end());

    std::set_intersection(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), std::back_inserter(commonValues));

    return commonValues;
}

std::vector<int> convertToSigned(const std::vector<uint32_t>& vec) {
    std::vector<int> result;
    result.reserve(vec.size());

    for (uint32_t value : vec) {
        result.push_back(static_cast<int>(value));
    }

    return result;
}

ReembedOptions::ReembedOptions(CrossSpeciesComparisonPhyloTreeViewPlugin& CrossSpeciesComparisonPhyloTreeViewPlugin, mv::CoreInterface* core) :
    WidgetAction(&CrossSpeciesComparisonPhyloTreeViewPlugin, "CrossSpeciesComparisonPhyloTreeViewPlugin ReEmbed"),
    _viewerPlugin(CrossSpeciesComparisonPhyloTreeViewPlugin),
    _core(core),
    _reembeddingSettingsHolder(*this)

{
    setSerializationName("CSV:CrossSpecies Comparison Phylo Tree View Reembed Settings");


    _reembeddingSettingsHolder.getselectedLeftRightLeafNamesFlag().setSerializationName("CSV:getselectedLeftRightLeafNamesFlag");

    _reembeddingSettingsHolder.getselectedLeftRightLeafNames().setSerializationName("CSV:getselectedLeftRightLeafNames");
    _reembeddingSettingsHolder.gethsneDataset().setSerializationName("CSV:gethsneDataset");
    _reembeddingSettingsHolder.getPerformHSNETSNEFlag().setSerializationName("CSV:getPerformHSNETSNEFlag");
    _reembeddingSettingsHolder.getPerformHSNEFlag().setSerializationName("CSV:getPerformHSNEFlag");
    _reembeddingSettingsHolder.getPerformTSNEFlag().setSerializationName("CSV:getPerformTSNEFlag");
    _reembeddingSettingsHolder.getselectedLeafNames().setSerializationName("CSV:getselectedLeafNames");

    _reembeddingSettingsHolder.getembeddingDataset().setSerializationName("CSV:getembeddingDataset");
    _reembeddingSettingsHolder.getleafNamesDataset().setSerializationName("CSV:getleafNamesDataset");
    _reembeddingSettingsHolder.getselectedRowSelectionDataset().setSerializationName("CSV:getselectedRowSelectionDataset");

    _reembeddingSettingsHolder.getdeDatasetLeft().setSerializationName("CSV:getdeDatasetLeft");
    _reembeddingSettingsHolder.getdeDatasetRight().setSerializationName("CSV:getdeDatasetRight");

    _reembeddingSettingsHolder.getgeneNamesConnection().setSerializationName("CSV:getgeneNamesConnection");
    //_reembeddingSettingsHolder.getclusterSelectionFromPopulationPyramidDataset().setSerializationName("CSV:getclusterSelectionFromPopulationPyramidDataset");
    _reembeddingSettingsHolder.getdeUpdateSwitch().setSerializationName("CSV:getdeUpdateSwitch");
    _reembeddingSettingsHolder.getLeftGroupPointDatasetDE().setSerializationName("CSV:getLeftGroupPointDatasetDE");
    _reembeddingSettingsHolder.getRightGroupPointDatasetDE().setSerializationName("CSV:getRightGroupPointDatasetDE");
    _reembeddingSettingsHolder.getLeftGroupClusterDatasetDE().setSerializationName("CSV:getLeftGroupClusterDatasetDE");
    _reembeddingSettingsHolder.getRightGroupClusterDatasetDE().setSerializationName("CSV:getRightGroupClusterDatasetDE");
    _eventListener.addSupportedEventType(static_cast<std::uint32_t>(EventType::DatasetAdded));
    _eventListener.addSupportedEventType(static_cast<std::uint32_t>(EventType::DatasetRemoved));
    _eventListener.addSupportedEventType(static_cast<std::uint32_t>(EventType::DatasetChildAdded));
    _eventListener.addSupportedEventType(static_cast<std::uint32_t>(EventType::DatasetChildRemoved));
    _eventListener.addSupportedEventType(static_cast<std::uint32_t>(EventType::DatasetDataChanged));

    _reembeddingSettingsHolder.setEnabled(true);
    _reembeddingSettingsHolder.getselectedLeftRightLeafNamesFlag().setDefaultWidgetFlags(ToggleAction::CheckBox);
    
    _reembeddingSettingsHolder.getselectedLeftRightLeafNames().setDefaultWidgetFlags(StringAction::LineEdit);
    _reembeddingSettingsHolder.gethsneDataset().setDefaultWidgetFlags(OptionAction::ComboBox);
    _reembeddingSettingsHolder.getPerformHSNETSNEFlag().setDefaultWidgetFlags(ToggleAction::CheckBox);
    _reembeddingSettingsHolder.getPerformHSNEFlag().setDefaultWidgetFlags(ToggleAction::CheckBox);
    _reembeddingSettingsHolder.getPerformTSNEFlag().setDefaultWidgetFlags(ToggleAction::CheckBox);
    _reembeddingSettingsHolder.getselectedLeafNames().setDefaultWidgetFlags(StringAction::LineEdit);

    _reembeddingSettingsHolder.getembeddingDataset().setDefaultWidgetFlags(OptionAction::ComboBox);
    _reembeddingSettingsHolder.getleafNamesDataset().setDefaultWidgetFlags(OptionAction::ComboBox);
    _reembeddingSettingsHolder.getselectedRowSelectionDataset().setDefaultWidgetFlags(OptionAction::ComboBox);



    _reembeddingSettingsHolder.getdeDatasetLeft().setDefaultWidgetFlags(OptionAction::ComboBox);

    _reembeddingSettingsHolder.getdeDatasetRight().setDefaultWidgetFlags(OptionAction::ComboBox);

    
    _reembeddingSettingsHolder.getgeneNamesConnection().setDefaultWidgetFlags(StringAction::LineEdit);
    _reembeddingSettingsHolder.getLeftGroupPointDatasetDE().setDefaultWidgetFlags(OptionAction::ComboBox);
    _reembeddingSettingsHolder.getRightGroupPointDatasetDE().setDefaultWidgetFlags(OptionAction::ComboBox);
    _reembeddingSettingsHolder.getLeftGroupClusterDatasetDE().setDefaultWidgetFlags(OptionAction::ComboBox);
    _reembeddingSettingsHolder.getRightGroupClusterDatasetDE().setDefaultWidgetFlags(OptionAction::ComboBox);
    _reembeddingSettingsHolder.getselectedLeafNames().setString("");

    _reembeddingSettingsHolder.getselectedLeftRightLeafNames().setString("");

    _reembeddingSettingsHolder.getselectedLeftRightLeafNamesFlag().setChecked(false);

    _reembeddingSettingsHolder.getPerformHSNETSNEFlag().setChecked(true);
    _reembeddingSettingsHolder.getPerformHSNEFlag().setChecked(false);
    _reembeddingSettingsHolder.getPerformTSNEFlag().setChecked(true);
    const auto RadioButtonTSNEUpdate = [this]() -> void
        {
            if (_reembeddingSettingsHolder.getPerformTSNEFlag().isChecked())
            {
                _reembeddingSettingsHolder.getPerformHSNEFlag().setChecked(false);
            }
            else
            {
                _reembeddingSettingsHolder.getPerformHSNEFlag().setChecked(true);
            }


        };
    connect(&_reembeddingSettingsHolder.getPerformTSNEFlag(), &ToggleAction::toggled, this, RadioButtonTSNEUpdate);

    const auto RadioButtonHSNEUpdate = [this]() -> void
        {
            if (_reembeddingSettingsHolder.getPerformHSNEFlag().isChecked())
            {
                _reembeddingSettingsHolder.getPerformTSNEFlag().setChecked(false);
            }
            else
            {
                _reembeddingSettingsHolder.getPerformTSNEFlag().setChecked(true);
            }
        };
    connect(&_reembeddingSettingsHolder.getPerformHSNEFlag(), &ToggleAction::toggled, this, RadioButtonHSNEUpdate);

    const auto reembedUpdate = [this]() -> void
        {
            if (_reembeddingSettingsHolder.getPerformHSNETSNEFlag().isChecked())
            {
                _reembeddingSettingsHolder.getPerformTSNEFlag().setEnabled(true);
                _reembeddingSettingsHolder.getPerformHSNEFlag().setEnabled(true);
            }
            else
            {
                _reembeddingSettingsHolder.getPerformTSNEFlag().setEnabled(false);
                _reembeddingSettingsHolder.getPerformHSNEFlag().setEnabled(false);
            }
        };
    connect(&_reembeddingSettingsHolder.getPerformHSNETSNEFlag(), &ToggleAction::toggled, this, reembedUpdate);

}
void ReembedOptions::reembedSelection()
{
    if(_reembeddingSettingsHolder.getembeddingDataset().getCurrentDataset().isValid() && 
       _reembeddingSettingsHolder.getleafNamesDataset().getCurrentDataset().isValid() && 
       _reembeddingSettingsHolder.getselectedRowSelectionDataset().getCurrentDataset().isValid())
    { 
        auto embeddingParent = _reembeddingSettingsHolder.getembeddingDataset().getCurrentDataset()->getParent();
        auto embedding = _reembeddingSettingsHolder.getembeddingDataset().getCurrentDataset();
        auto leafNamesParent = _reembeddingSettingsHolder.getleafNamesDataset().getCurrentDataset()->getParent();
        auto rowSelectionParent = _reembeddingSettingsHolder.getselectedRowSelectionDataset().getCurrentDataset()->getParent();
        auto selectedIndices= _reembeddingSettingsHolder.getembeddingDataset().getCurrentDataset()->getSelectionIndices();
        
        if ((embedding == leafNamesParent || embeddingParent == leafNamesParent) && (embedding == rowSelectionParent || embeddingParent == rowSelectionParent))
        {

        
        if (leafNamesParent == rowSelectionParent && selectedIndices.size()>0)
        {
            _selectedSpeciesListLeft.clear();
            _selectedSpeciesListRight.clear();
            _allSelectedSpecies.clear();
            _leafMap.clear(); _clusterMap.clear(); _leftRightClusterMap.clear(); _indicesMap.clear();
            if (_reembeddingSettingsHolder.getselectedLeftRightLeafNamesFlag().isChecked())
            {
                
                QString tempValueLeftRightLeaves = _reembeddingSettingsHolder.getselectedLeftRightLeafNames().getString();
                if (tempValueLeftRightLeaves != "")
                {
                    auto  tempLeavesArray = tempValueLeftRightLeaves.split("@$LEFTSPLITRIGHT$@");
                    _selectedSpeciesListLeft = tempLeavesArray[0].split(" @%$,$%@ ");
                    _selectedSpeciesListRight = tempLeavesArray[1].split(" @%$,$%@ ");
                    _allSelectedSpecies = _selectedSpeciesListLeft + _selectedSpeciesListRight;
                }

            }
            else
            {
                QString tempValueLeaves = _reembeddingSettingsHolder.getselectedLeafNames().getString();
                if (tempValueLeaves != "")
                {
                    _selectedSpeciesListLeft = tempValueLeaves.split(" @%$,$%@ ");
                    _allSelectedSpecies = _selectedSpeciesListLeft;
                }
            }
            reembed();
            if (_reembeddingSettingsHolder.getselectedLeftRightLeafNamesFlag().isChecked() )
            {
                if (_tsneAvailable)
                {
                    differentialExpression();
                }
                
            }
        }
        else
        {
            qDebug() << "Embedding and/or Cluster and/or species dataset/s does not have the same parent";
        }
        }
        else
        {
            qDebug() << "datasets  not valid";
        }

    }
    else
    {
        qDebug() << "Embedding and/or Cluster and/or species dataset/s not valid";
    }


}
void ReembedOptions::populatePointData(QString& datasetId, std::vector<float>& pointVector, int& numPoints, int& numDimensions, std::vector<QString>& dimensionNames)
{
    auto pointDataset= mv::data().getDataset<Points>(datasetId);
    if (pointDataset.isValid())
    {
        if (pointVector.size()>0 && numPoints>0 && numDimensions>0) {
            pointDataset->setData(pointVector.data(), numPoints, numDimensions);
            pointDataset->setDimensionNames(dimensionNames);
            mv::events().notifyDatasetDataChanged(pointDataset);
        }

    }
}
void ReembedOptions::populateClusterData(QString& datasetId, std::map<QString, std::pair<QColor,std::vector<int>>>& clusterMap)
{
    
    auto colorDataset=mv::data().getDataset<Clusters>(datasetId);
    if (colorDataset.isValid())
    {
        for (const auto& pair : clusterMap)
        {
            QString clusterName = pair.first;
            std::pair<QColor, std::vector<int>> value = pair.second;
            QColor clusterColor = value.first;
            std::vector<std::uint32_t> clusterIndices(value.second.begin(), value.second.end());
            
            if (clusterIndices.size() > 0)
            {
                Cluster clusterValue;
                clusterValue.setName(clusterName);
                clusterValue.setColor(clusterColor);
                clusterValue.setIndices(clusterIndices);
                colorDataset->addCluster(clusterValue);
            }
        }

        mv::events().notifyDatasetDataChanged(colorDataset);
    }


}

void ReembedOptions::reembed()
{
    Dataset<Points> embeddingParentFull = mv::data().getDataset<Points>(_reembeddingSettingsHolder.getembeddingDataset().getCurrentDataset().getDatasetId());
    std::vector<QString> dimensionsNamesContainer;
    std::vector<int> dimensionsIndexContainer;
 
    
    QStringList selectedGenesStrList;

    if (_reembeddingSettingsHolder.getgeneNamesConnection().getString() != "")
    {
        if (_reembeddingSettingsHolder.getgeneNamesConnection().getString().contains("*%$@*@$%*"))
        {
            selectedGenesStrList = _reembeddingSettingsHolder.getgeneNamesConnection().getString().split("*%$@*@$%*");
        }
        else
        {
            selectedGenesStrList.append(_reembeddingSettingsHolder.getgeneNamesConnection().getString());
        }

    }

     
     
    //for (int i = 0; i < embeddingParentFull->getNumDimensions(); i++)
    if (selectedGenesStrList.size()>0)
    {
        for (int i = 0; i < embeddingParentFull->getDimensionNames().size(); i++)
        {
            if (selectedGenesStrList.contains(embeddingParentFull->getDimensionNames()[i]))
            {
                dimensionsNamesContainer.push_back(embeddingParentFull->getDimensionNames()[i]);
                dimensionsIndexContainer.push_back(i);
            }
        }
        
    }
    else
    {
        dimensionsNamesContainer= embeddingParentFull->getDimensionNames();
        for (int i = 0; i < embeddingParentFull->getNumDimensions(); i++)
        {
            dimensionsIndexContainer.push_back(i);
        }
            }
    int numOfDims = dimensionsNamesContainer.size();

        if (_lowDimTSNEDataset.isValid())
        {
            auto runningAction = _lowDimTSNEDataset->findChildByPath("TSNE/TsneComputationAction/Running");
            if (runningAction)
            {
                if (runningAction->isChecked())
                {
                    auto stopAction = _lowDimTSNEDataset->findChildByPath("TSNE/TsneComputationAction/Stop");
                    if (stopAction)
                    {
                        stopAction->trigger();
                        std::this_thread::sleep_for(std::chrono::seconds(5));
                    }
                }

            }
        }
        if (_lowDimTSNEDataset.isValid())
        {
            auto datasetIDLowRem = _lowDimTSNEDataset.getDatasetId();
            mv::events().notifyDatasetAboutToBeRemoved(_lowDimTSNEDataset);
            mv::data().removeDataset(_lowDimTSNEDataset);
            mv::events().notifyDatasetRemoved(datasetIDLowRem, PointType);
        }

        if(_tsneDataset.isValid())
        {
        auto datasetIDRem = _tsneDataset.getDatasetId();
        mv::events().notifyDatasetAboutToBeRemoved(_tsneDataset);
        mv::data().removeDataset(_tsneDataset);
        mv::events().notifyDatasetRemoved(datasetIDRem, PointType);
        //std::this_thread::sleep_for(std::chrono::seconds(2));
    }
        

    
    _tsneDataset = mv::data().createDataset("Points", "TSNE Dataset");
    mv::events().notifyDatasetAdded(_tsneDataset);
    QString datasetID = _tsneDataset->getId();
    
    _tsneDataset->setGroupIndex(4);

    auto leafNamesData = mv::data().getDataset<Clusters>(_reembeddingSettingsHolder.getleafNamesDataset().getCurrentDataset().getDatasetId());
    auto rowSelectionData= mv::data().getDataset<Clusters>(_reembeddingSettingsHolder.getselectedRowSelectionDataset().getCurrentDataset().getDatasetId());
    auto selectedIndices = _reembeddingSettingsHolder.getembeddingDataset().getCurrentDataset()->getSelectionIndices();
    //std::vector<int> filteredIndices;
    
    if (_allSelectedSpecies.empty())
        return;

    auto tempLeafClusters = leafNamesData->getClusters();
    std::vector<int> leftIndices, rightIndices, filteredIndices;

    for (const auto& cluster : tempLeafClusters)
    {
        if (!_allSelectedSpecies.contains(cluster.getName()))
            continue;

        auto temp = cluster.getIndices();
        if (_reembeddingSettingsHolder.getselectedLeftRightLeafNamesFlag().isChecked())
        {
            if (_selectedSpeciesListLeft.contains(cluster.getName()))
                leftIndices.insert(leftIndices.end(), temp.begin(), temp.end());
            else if (_selectedSpeciesListRight.contains(cluster.getName()))
                rightIndices.insert(rightIndices.end(), temp.begin(), temp.end());
        }

        std::copy_if(temp.begin(), temp.end(), std::back_inserter(filteredIndices), [&](int index) {
            return std::find(selectedIndices.begin(), selectedIndices.end(), index) != selectedIndices.end();
            });
    }

    //remove values from leftIndices and rightIndices that are not in filteredIndices
    leftIndices = intersectVectors(leftIndices, filteredIndices);
    rightIndices = intersectVectors(rightIndices, filteredIndices);

    _indicesMap.clear();
    for (int i = 0; i < filteredIndices.size(); i++)
        _indicesMap[filteredIndices[i]] = i;

    auto clusterVals = rowSelectionData->getClusters();
    for (auto& cluster : clusterVals)
    {
        std::vector<int> commonIndices = findCommonValues(filteredIndices, convertToSigned(cluster.getIndices()));
        std::transform(commonIndices.begin(), commonIndices.end(), commonIndices.begin(), [&](int index) { return _indicesMap[index]; });
        if (commonIndices.size() > 0)
        {
            _clusterMap[cluster.getName()] = { cluster.getColor(), commonIndices };
        }
       
    }

    for (auto& cluster : tempLeafClusters)
    {
        std::vector<int> commonIndices = findCommonValues(filteredIndices, convertToSigned(cluster.getIndices()));
        std::transform(commonIndices.begin(), commonIndices.end(), commonIndices.begin(), [&](int index) { return _indicesMap[index]; });
        if (commonIndices.size() > 0)
        {
            _leafMap[cluster.getName()] = { cluster.getColor(), commonIndices };
        }
       
    }

    if (_reembeddingSettingsHolder.getselectedLeftRightLeafNamesFlag().isChecked())
    {
        QColor colorLeft = QColor("#FF5733");
        QColor colorRight = QColor("#1D8ECE");
        //replace values of  leftIndices and rightIndices from _indicesMap
        std::transform(leftIndices.begin(), leftIndices.end(), leftIndices.begin(), [&](int index) { return _indicesMap[index]; });
        std::transform(rightIndices.begin(), rightIndices.end(), rightIndices.begin(), [&](int index) { return _indicesMap[index]; });

        _leftRightClusterMap["LeftGroup"] = { colorLeft, leftIndices };
        _leftRightClusterMap["RightGroup"] = { colorRight, rightIndices };
    }




    int numPoints = filteredIndices.size();
    _tsneAvailable = true;
    if (numPoints >30)
    {

        std::vector<float> pointVector(filteredIndices.size() * numOfDims);
        embeddingParentFull->populateDataForDimensions(pointVector, dimensionsIndexContainer, filteredIndices);

        populatePointData(datasetID, pointVector, numPoints, numOfDims, dimensionsNamesContainer);
        auto analysisPlugin = mv::plugins().requestPlugin<AnalysisPlugin>("tSNE Analysis", { _tsneDataset });
        if (!analysisPlugin) {
            qDebug() << "Could not find create TSNE Analysis";
            return;
        }

        auto scatterplotViewFactory = mv::plugins().getPluginFactory("Scatterplot View");
        mv::gui::DatasetPickerAction* colorDatasetPickerAction;
        mv::gui::DatasetPickerAction* pointDatasetPickerAction;
        _lowDimTSNEDataset = analysisPlugin->getOutputDataset();

        if (_lowDimTSNEDataset.isValid()) {
            for (auto plugin : mv::plugins().getPluginsByFactory(scatterplotViewFactory)) {
                if (plugin->getGuiName() == "Scatterplot ReEmbed View") {
                    pointDatasetPickerAction = dynamic_cast<DatasetPickerAction*>(plugin->findChildByPath("Settings/Datasets/Position"));
                    if (pointDatasetPickerAction) {
                        pointDatasetPickerAction->setCurrentText("");
                        pointDatasetPickerAction->setCurrentDataset(_lowDimTSNEDataset);
                        colorDatasetPickerAction = dynamic_cast<DatasetPickerAction*>(plugin->findChildByPath("Settings/Datasets/Color"));
                    }
                }
            }

            auto startAction = _lowDimTSNEDataset->findChildByPath("TSNE/TsneComputationAction/Start");
            if (startAction) {
               
                startAction->trigger();
                
                analysisPlugin->getOutputDataset()->setSelectionIndices({});
            }
        }

        auto leafDataset = mv::data().createDataset("Cluster", "Cluster Leaf Color Dataset", _tsneDataset);
        auto leafDataId= leafDataset->getId();
        leafDataset->setGroupIndex(4);
        _tsneColorDatasetIds["leafColorDataset"] = leafDataId;
        mv::events().notifyDatasetAdded(leafDataset);


        populateClusterData(leafDataId, _leafMap);

        auto clusterDataset = mv::data().createDataset("Cluster", "Cluster Row Color Dataset", _tsneDataset);
        auto clusterDataId = clusterDataset->getId();
        clusterDataset->setGroupIndex(4);
        _tsneColorDatasetIds["clusterColorDataset"] = clusterDataId;
        mv::events().notifyDatasetAdded(clusterDataset);

        populateClusterData(clusterDataId, _clusterMap);

        if (_reembeddingSettingsHolder.getselectedLeftRightLeafNamesFlag().isChecked())
        {
            auto leftRightClusterDataset = mv::data().createDataset("Cluster", "Left Right Cluster Dataset", _tsneDataset);
            leftRightClusterDataset->setGroupIndex(4);
            auto leftRightClusterDataId = leftRightClusterDataset->getId();
            _tsneColorDatasetIds["leftRightColorDataset"] = leftRightClusterDataId;

            mv::events().notifyDatasetAdded(leftRightClusterDataset);
  
            populateClusterData(leftRightClusterDataId, _leftRightClusterMap);
            if (colorDatasetPickerAction) {
                colorDatasetPickerAction->setCurrentText("");
                colorDatasetPickerAction->setCurrentDataset(leftRightClusterDataset->getId());
            }
        }
        else
        {
            if (colorDatasetPickerAction) {
                colorDatasetPickerAction->setCurrentText("");
                colorDatasetPickerAction->setCurrentDataset(leafDataset->getId());
            }
        }
    }
    else
    {
        _tsneAvailable = false;
        qDebug()<< "Number of points is less than 30";
    }

}


void ReembedOptions::differentialExpression()
{


    auto leftRightClusterDataId = _tsneColorDatasetIds["leftRightColorDataset"];
    auto clusterDataId = _tsneColorDatasetIds["clusterColorDataset"];
    
    if (leftRightClusterDataId != "" && clusterDataId != "" && _tsneDataset.isValid())
    {
        
        
        
        
        auto splitterClusterData = mv::data().getDataset<Clusters>(leftRightClusterDataId);
        auto clusterDataset = mv::data().getDataset<Clusters>(clusterDataId);

        if (splitterClusterData.isValid() && clusterDataset.isValid())
        {
            
            int numberofInputPoints = _tsneDataset->getNumPoints();
            auto inputPointDimensionNames = _tsneDataset->getDimensionNames();
            auto inputPointNumOfDimensions = _tsneDataset->getNumDimensions();


            std::vector<int>  inputPointDimensionIndex;
            for (int i = 0; i < inputPointNumOfDimensions; i++)
            {
                inputPointDimensionIndex.push_back(i);
            }

            auto splitterClusterDataClusters = splitterClusterData->getClusters();

            if (splitterClusterDataClusters.size() == 2)
            {
               
               

                if (!_leftDeDatasetPoints.isValid())
                {
                    _leftDeDatasetPoints = mv::data().createDataset("Points", "Left DE Dataset");
                    _leftDeDatasetPoints->setGroupIndex(6);
                    mv::events().notifyDatasetAdded(_leftDeDatasetPoints);
                }
                if (!_rightDeDatasetPoints.isValid())
                {
                    _rightDeDatasetPoints = mv::data().createDataset("Points", "Right DE Dataset");
                    _rightDeDatasetPoints->setGroupIndex(6);
                    mv::events().notifyDatasetAdded(_rightDeDatasetPoints);
                }
                if (_leftDeDatasetClusters.isValid())
                {
                    auto leftChildrendelete = _leftDeDatasetClusters->getChildren();
                    for (auto leftChild : leftChildrendelete)
                    {

                        mv::events().notifyDatasetAboutToBeRemoved(leftChild);
                        mv::data().removeDataset(leftChild);

                    }
                    _leftDeDatasetClusters->getClusters() = QVector<Cluster>();
                    mv::events().notifyDatasetDataChanged(_leftDeDatasetClusters);
                }
                else
                {
                    _leftDeDatasetClusters = mv::data().createDataset("Cluster", "Left DE Cluster Dataset", _leftDeDatasetPoints);
                    _leftDeDatasetClusters->setGroupIndex(6);
                    mv::events().notifyDatasetAdded(_leftDeDatasetClusters);
                }
                if (_rightDeDatasetClusters.isValid())
                {
                    auto rightChildrendelete = _rightDeDatasetClusters->getChildren();
                    for (auto rightChild : rightChildrendelete)
                    {

                        mv::events().notifyDatasetAboutToBeRemoved(rightChild);
                        mv::data().removeDataset(rightChild);

                    }
                    _rightDeDatasetClusters->getClusters() = QVector<Cluster>();
                    mv::events().notifyDatasetDataChanged(_rightDeDatasetPoints);
                }
                else
                {
                    _rightDeDatasetClusters = mv::data().createDataset("Cluster", "Right DE Cluster Dataset", _rightDeDatasetPoints);
                    _rightDeDatasetClusters->setGroupIndex(6);
                    mv::events().notifyDatasetAdded(_rightDeDatasetClusters);
                }

                Cluster* leftGroupReference;
                Cluster* rightGroupReference;
                if (splitterClusterDataClusters[0].getName()=="LeftGroup")
                {
                    leftGroupReference= &splitterClusterDataClusters[0];
                    rightGroupReference = &splitterClusterDataClusters[1];


                }
                else
                {
                    leftGroupReference = &splitterClusterDataClusters[1];
                    rightGroupReference = &splitterClusterDataClusters[0];
                }







                auto templeftIndices= leftGroupReference->getIndices();
                auto temprightIndices = rightGroupReference->getIndices();
                std::vector<int> leftIndices(templeftIndices.begin(), templeftIndices.end());
                std::vector<int> rightIndices(temprightIndices.begin(), temprightIndices.end());
                std::map<int, int> leftClusterIndexMap;
                std::map<int, int> rightClusterIndexMap;
                for (int i = 0; i < templeftIndices.size(); i++)
                {
                    leftClusterIndexMap[templeftIndices[i]] = i;
                }
                for (int i = 0; i < temprightIndices.size(); i++)
                {
                    rightClusterIndexMap[temprightIndices[i]] = i;
                }



                std::vector<float> pointsContainerLeft(leftIndices.size() * inputPointDimensionIndex.size());
                std::vector<float> pointsContainerRight(rightIndices.size() * inputPointDimensionIndex.size());
                _tsneDataset->populateDataForDimensions(pointsContainerLeft, inputPointDimensionIndex, leftIndices);
                _tsneDataset->populateDataForDimensions(pointsContainerRight, inputPointDimensionIndex, rightIndices);
                _leftDeDatasetPoints->setData(pointsContainerLeft.data(), leftIndices.size(), inputPointDimensionIndex.size());
                _leftDeDatasetPoints->setDimensionNames(inputPointDimensionNames);
                events().notifyDatasetDataChanged(_leftDeDatasetPoints);
                _rightDeDatasetPoints->setData(pointsContainerRight.data(), rightIndices.size(), inputPointDimensionIndex.size());
                _rightDeDatasetPoints->setDimensionNames(inputPointDimensionNames);
                events().notifyDatasetDataChanged(_rightDeDatasetPoints);

                    std::vector<QString> clusteringItem(numberofInputPoints, "");
                    std::map<QString, QColor> clusterColormap;
                    std::map<QString, std::pair<QColor, std::vector<int>>> clusterMapLeft;
                    std::map<QString, std::pair<QColor, std::vector<int>>> clusterMapRight;
                    auto clusters = clusterDataset->getClusters();
                    for (auto cluster : clusters)
                    {
                        auto clusterIndices = cluster.getIndices();
                        auto clusterName = cluster.getName();
                        auto clusterColor= cluster.getColor();
                        std::vector<int> clusterLeftIndices;
                        std::vector<int> clusterRightIndices;
                        //find intersection of clusterIndices with leftIndices and rightIndices and map with leftClusterIndexMap and rightClusterIndexMap
                        for (auto index : clusterIndices)
                        {
                            if (leftClusterIndexMap.find(index) != leftClusterIndexMap.end())
                            {
                                clusterLeftIndices.push_back(leftClusterIndexMap[index]);
                            }
                            if (rightClusterIndexMap.find(index) != rightClusterIndexMap.end())
                            {
                                clusterRightIndices.push_back(rightClusterIndexMap[index]);
                            }
                        }
                        if (clusterLeftIndices.size() > 0)
                        {
                             clusterMapLeft[clusterName] = { clusterColor, clusterLeftIndices };
                            }   
                        if (clusterRightIndices.size() > 0)
                        {
                            clusterMapRight[clusterName] = { clusterColor, clusterRightIndices };
                        }
                    }


 
                    auto tlID= _leftDeDatasetClusters->getId();
                    auto trID = _rightDeDatasetClusters->getId();
                    populateClusterData(tlID, clusterMapLeft);
                    populateClusterData(trID, clusterMapRight);


                    _reembeddingSettingsHolder.getLeftGroupPointDatasetDE().setCurrentDataset(_leftDeDatasetPoints);
                    _reembeddingSettingsHolder.getRightGroupPointDatasetDE().setCurrentDataset(_rightDeDatasetPoints);
                    _reembeddingSettingsHolder.getLeftGroupClusterDatasetDE().setCurrentDataset(_leftDeDatasetClusters);
                    _reembeddingSettingsHolder.getRightGroupClusterDatasetDE().setCurrentDataset(_rightDeDatasetClusters);
                    _reembeddingSettingsHolder.getdeUpdateSwitch().trigger();



                    auto leftChildren = _reembeddingSettingsHolder.getLeftGroupClusterDatasetDE().getCurrentDataset()->getChildren();
                    if (leftChildren.size() > 0)
                    {
                        for (auto leftChild : leftChildren)
                        {
                            if (leftChild->getDataType() == PointType)
                            {
                                if (leftChild->getGuiName().contains("DE_Statistics"))
                                {
                                    auto leftChildId = leftChild->getId();

                                    _reembeddingSettingsHolder.getdeDatasetLeft().setCurrentDataset(mv::data().getDataset<Points>(leftChildId));
                                }
                            }
                        }
                    }

                    auto rightChildren = _reembeddingSettingsHolder.getRightGroupClusterDatasetDE().getCurrentDataset()->getChildren();
                    if (rightChildren.size() > 0)
                    {
                        for (auto rightChild : rightChildren)
                        {
                            if (rightChild->getDataType() == PointType)
                            {
                                if (rightChild->getGuiName().contains("DE_Statistics"))
                                {
                                    auto rightChildId = rightChild->getId();

                                    _reembeddingSettingsHolder.getdeDatasetRight().setCurrentDataset(mv::data().getDataset<Points>(rightChildId));
                                }
                            }
                        }
                    }



            
            }

        }
    }
}



inline ReembedOptions::ReembeddingSettingsHolder::ReembeddingSettingsHolder(ReembedOptions& reembedOptions) :
    VerticalGroupAction(&reembedOptions, "Reembed Options"),
    _reembedOptions(reembedOptions),

    _selectedLeftRightLeafNamesFlagAction(this, "_selectedLeftRightLeafNamesFlagAction"),

    _selectedLeftRightLeafNamesAction(this, " _selectedLeftRightLeafNamesAction"),
    _hsneDatasetAction(this, " _hsneDatasetAction"),
    _performHSNETSNEFlagAction(this, "_performHSNETSNEFlagAction "),
    _performHSNEFlagAction(this, "_performHSNEFlagAction "),
    _performTSNEFlagAction(this, " _performTSNEFlagAction"),
    _selectedLeafNamesAction(this, " _selectedLeafNamesAction"),

    _embeddingDatasetAction(this, "_embeddingDatasetAction "),
    _leafNamesDatasetAction(this, " _leafNamesDatasetAction"),
    _selectedRowSelectionDatasetAction(this, "_selectedRowSelectionDatasetAction "),

    _deDatasetLeftAction(this, "_deDatasetLeftAction "),
    _deDatasetRightAction(this, "_deDatasetRightAction "),


    _deUpdateSwitchAction(this, "_deUpdateSwitchAction"),
    _geneNamesConnection(this, "Gene Names Connection"),
    _leftGroupPointDatasetDE(this, "Left Group Point Dataset DE"),
    _rightGroupPointDatasetDE(this, "Right Group Point Dataset DE"),
    _leftGroupClusterDatasetDE(this, "Left Group Cluster Dataset DE"),
    _rightGroupClusterDatasetDE(this, "Right Group Cluster Dataset DE")

{
    setText("Reembedding Options");
    setIcon(Application::getIconFont("FontAwesome").getIcon("exchange-alt"));
    setPopupSizeHint(QSize(350, 0));
    setConfigurationFlag(WidgetAction::ConfigurationFlag::ForceCollapsedInGroup);
    addAction(&_selectedLeftRightLeafNamesFlagAction);
    addAction(&_hsneDatasetAction);
    addAction(&_performHSNETSNEFlagAction);
    addAction(&_performHSNEFlagAction);
    addAction(&_performTSNEFlagAction);
    addAction(&_embeddingDatasetAction);
    addAction(&_leafNamesDatasetAction);
    addAction(&_selectedRowSelectionDatasetAction);

    addAction(&_deDatasetLeftAction);
    addAction(&_deDatasetRightAction);
    //addAction(&_clusterSelectionFromPopulationPyramidDatasetAction);
    addAction(&_deUpdateSwitchAction);


    addAction(&_selectedLeftRightLeafNamesAction);
    addAction(&_selectedLeafNamesAction);

    addAction(&_geneNamesConnection);
    addAction(&_leftGroupPointDatasetDE);
    addAction(&_rightGroupPointDatasetDE);
    addAction(&_leftGroupClusterDatasetDE);
    addAction(&_rightGroupClusterDatasetDE);



    _selectedLeftRightLeafNamesAction.setDisabled(true);
    _selectedLeafNamesAction.setDisabled(true);
 
    _geneNamesConnection.setDisabled(true);
}




void ReembedOptions::fromVariantMap(const QVariantMap& variantMap)
{
    WidgetAction::fromVariantMap(variantMap);

    _reembeddingSettingsHolder.getselectedLeftRightLeafNamesFlag().fromParentVariantMap(variantMap);
 
    _reembeddingSettingsHolder.getselectedLeftRightLeafNames().fromParentVariantMap(variantMap);
    _reembeddingSettingsHolder.gethsneDataset().fromParentVariantMap(variantMap);
    _reembeddingSettingsHolder.getPerformHSNETSNEFlag().fromParentVariantMap(variantMap);
    _reembeddingSettingsHolder.getPerformHSNEFlag().fromParentVariantMap(variantMap);
    _reembeddingSettingsHolder.getPerformTSNEFlag().fromParentVariantMap(variantMap);
    _reembeddingSettingsHolder.getselectedLeafNames().fromParentVariantMap(variantMap);

    _reembeddingSettingsHolder.getembeddingDataset().fromParentVariantMap(variantMap);
    _reembeddingSettingsHolder.getleafNamesDataset().fromParentVariantMap(variantMap);
    _reembeddingSettingsHolder.getselectedRowSelectionDataset().fromParentVariantMap(variantMap);



    _reembeddingSettingsHolder.getdeDatasetLeft().fromParentVariantMap(variantMap);
  
    _reembeddingSettingsHolder.getdeDatasetRight().fromParentVariantMap(variantMap);


    _reembeddingSettingsHolder.getgeneNamesConnection().fromParentVariantMap(variantMap);
    _reembeddingSettingsHolder.getdeUpdateSwitch().fromParentVariantMap(variantMap);
    _reembeddingSettingsHolder.getLeftGroupPointDatasetDE().fromParentVariantMap(variantMap);
    _reembeddingSettingsHolder.getRightGroupPointDatasetDE().fromParentVariantMap(variantMap);
    _reembeddingSettingsHolder.getLeftGroupClusterDatasetDE().fromParentVariantMap(variantMap);
    _reembeddingSettingsHolder.getRightGroupClusterDatasetDE().fromParentVariantMap(variantMap);
}

QVariantMap ReembedOptions::toVariantMap() const
{
    QVariantMap variantMap = WidgetAction::toVariantMap();
    _reembeddingSettingsHolder.getselectedLeftRightLeafNamesFlag().insertIntoVariantMap(variantMap);

    _reembeddingSettingsHolder.getselectedLeftRightLeafNames().insertIntoVariantMap(variantMap);
    _reembeddingSettingsHolder.gethsneDataset().insertIntoVariantMap(variantMap);
    _reembeddingSettingsHolder.getPerformHSNETSNEFlag().insertIntoVariantMap(variantMap);
    _reembeddingSettingsHolder.getPerformHSNEFlag().insertIntoVariantMap(variantMap);
    _reembeddingSettingsHolder.getPerformTSNEFlag().insertIntoVariantMap(variantMap);
    _reembeddingSettingsHolder.getselectedLeafNames().insertIntoVariantMap(variantMap);
   
    _reembeddingSettingsHolder.getembeddingDataset().insertIntoVariantMap(variantMap);
    _reembeddingSettingsHolder.getleafNamesDataset().insertIntoVariantMap(variantMap);
    _reembeddingSettingsHolder.getselectedRowSelectionDataset().insertIntoVariantMap(variantMap);

    _reembeddingSettingsHolder.getdeDatasetLeft().insertIntoVariantMap(variantMap);
    _reembeddingSettingsHolder.getdeDatasetRight().insertIntoVariantMap(variantMap);


    _reembeddingSettingsHolder.getgeneNamesConnection().insertIntoVariantMap(variantMap);
    _reembeddingSettingsHolder.getdeUpdateSwitch().insertIntoVariantMap(variantMap);
    _reembeddingSettingsHolder.getLeftGroupPointDatasetDE().insertIntoVariantMap(variantMap);
    _reembeddingSettingsHolder.getRightGroupPointDatasetDE().insertIntoVariantMap(variantMap);
    _reembeddingSettingsHolder.getLeftGroupClusterDatasetDE().insertIntoVariantMap(variantMap);
    _reembeddingSettingsHolder.getRightGroupClusterDatasetDE().insertIntoVariantMap(variantMap);
    return variantMap;
}
