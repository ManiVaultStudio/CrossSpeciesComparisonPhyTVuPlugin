#include "ChartOptions.h"

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

#include <QJsonValue>
using namespace mv;
using namespace mv::gui;

ChartOptions::ChartOptions(CrossSpeciesComparisonPhyloTreeViewPlugin& CrossSpeciesComparisonPhyloTreeViewPlugin, mv::CoreInterface* core) :
    WidgetAction(&CrossSpeciesComparisonPhyloTreeViewPlugin, "CrossSpeciesComparisonPhyloTreeViewPlugin Chart"),
    _viewerPlugin(CrossSpeciesComparisonPhyloTreeViewPlugin),
    _core(core),
    _extraSettingsHolder(*this),
    _mainSettingsHolder(*this),
    _metaDataSettingsHolder(*this),
    _linkerSettingsHolder(*this),
    _updateSettingsHolder(*this)

{
    setSerializationName("CSCPTV:CrossSpecies Comparison Phylo Tree View Settings");
    _mainSettingsHolder.getMainReferenceTreeSelectionAction().setSerializationName("CSCPTV:Reference tree selection");
    _extraSettingsHolder.getColorMapAction().setSerializationName("CSCPTV:Color map selection");
    _mainSettingsHolder.getComparisonTreeSelectionAction().setSerializationName("CSCPTV:Comparison tree selection");
    _metaDataSettingsHolder.getColorTraitAction().setSerializationName("CSCPTV:Color type trait selection");
    _metaDataSettingsHolder.getNumericTraitAction().setSerializationName("Numeric type trait selection");
    _metaDataSettingsHolder.getStringTraitAction().setSerializationName("CSCPTV:String type trait selection");
    //_extraSettingsHolder.getClusteringMethodAction().setSerializationName("ClusterMethod");
    _metaDataSettingsHolder.getTraitDatasetSelectionAction().setSerializationName("CSCPTV:Trait Dataset selection");
    _extraSettingsHolder.getShowReferenceTreeAction().setSerializationName("CSCPTV:Show reference tree selection");
    _extraSettingsHolder.getExpandAllAction().setSerializationName("CSCPTV:Expand all selection");
    _extraSettingsHolder.getDisableAcceptDatasetDrops().setSerializationName("CSCPTV:Disable Accept Dataset Drops");

    _updateSettingsHolder.getUpdateViewsButtonAction().setSerializationName("CSCPTV:Update Views Button");

    _linkerSettingsHolder.getScatterplotLeafSelectionValue().setSerializationName("CSCPTV:Scatterplot Leaf Selection Value");
    _linkerSettingsHolder.getTreeLeafSelectionValueQT().setSerializationName("CSCPTV:Tree Leaf Selection Value");
    _linkerSettingsHolder.getReembeddingOptions().setSerializationName("CSCPTV:Reembedding Options");

    //_extraSettingsHolder.getNumOfClustersAction().setSerializationName("Number of clusters");

    _eventListener.addSupportedEventType(static_cast<std::uint32_t>(EventType::DatasetAdded));
    _eventListener.addSupportedEventType(static_cast<std::uint32_t>(EventType::DatasetRemoved));
    _eventListener.addSupportedEventType(static_cast<std::uint32_t>(EventType::DatasetChildAdded));
    _eventListener.addSupportedEventType(static_cast<std::uint32_t>(EventType::DatasetChildRemoved));
    _eventListener.addSupportedEventType(static_cast<std::uint32_t>(EventType::DatasetDataChanged));

    _extraSettingsHolder.setEnabled(false);
    _metaDataSettingsHolder.setEnabled(false);
    _linkerSettingsHolder.setEnabled(false);
    //_updateSettingsHolder.setEnabled(false);
    //if (_updateSettingsHolder.getUpdateViewsButtonAction().isConnected())
    //{
        //_updateSettingsHolder.setVisible(true);
    //}
    //else
    //{
        //_updateSettingsHolder.setVisible(false);
    //}
    _mainSettingsHolder.getComparisonTreeSelectionAction().setEnabled(false);
    _eventListener.registerDataEventByType(CrossSpeciesComparisonTreeType, std::bind(&ChartOptions::onDataEventTree, this, std::placeholders::_1));
    _eventListener.registerDataEventByType(CrossSpeciesComparisonTreeMetaType, std::bind(&ChartOptions::onDataEventTreeMeta, this, std::placeholders::_1));


    //_mainSettingsHolder.getMainReferenceTreeSelectionAction().setShowFullPathName(false);
    //_mainSettingsHolder.getComparisonTreeSelectionAction().setShowFullPathName(false);
    _mainSettingsHolder.getComparisonTreeSelectionAction().setDefaultWidgetFlags(OptionAction::ComboBox);
    _mainSettingsHolder.getMainReferenceTreeSelectionAction().setDefaultWidgetFlags(OptionAction::ComboBox);
    /*_extraSettingsHolder.getNumOfClustersAction().setDefaultWidgetFlags(DecimalAction::SpinBox| DecimalAction::Slider);
    _extraSettingsHolder.getNumOfClustersAction().setMaximum(0);
    _extraSettingsHolder.getNumOfClustersAction().setMinimum(0);
    _extraSettingsHolder.getNumOfClustersAction().setValue(0);*/
    /*
    _mainSettingsHolder.getMainReferenceTreeSelectionAction().setDatasetsFilterFunction([this](const mv::Datasets& datasets) ->mv::Datasets {
        Datasets treeDatasets;
        if (!datasets.isEmpty())
        {

            for (auto dataset : datasets)
            {
                if (dataset->getDataType() == PointType)
                {
                    Dataset<Points> checkDataset = mv::data().getDataset(dataset.getDatasetId());// dataset->getFullDataset<Points>();

                    auto tempDimensionNames = checkDataset->getDimensionNames();
                    if (tempDimensionNames.size() == 1)
                    {
                        if (tempDimensionNames[0] == "CrossspeciesComparisonTreeDataset")
                        {
                            treeDatasets << dataset;

                        }

                    }
                }

            }

        }


        return treeDatasets;
        });
    */
    //_metaDataSettingsHolder.getTraitDatasetSelectionAction().setShowFullPathName(false);






    Datasets treeDatasets;
    _mainSettingsHolder.getComparisonTreeSelectionAction().setDatasets(treeDatasets);

    _extraSettingsHolder.getColorMapAction().getSettingsAction().getColorMapAction().getDiscretizeAction().setDisabled(true);
    _extraSettingsHolder.getColorMapAction().getSettingsAction().getColorMapAction().getDiscretizeAction().setVisible(false);
    _extraSettingsHolder.getColorMapAction().getSettingsAction().getColorMapAction().getNumberOfDiscreteStepsAction().setDisabled(true);
    _extraSettingsHolder.getColorMapAction().getSettingsAction().getColorMapAction().getNumberOfDiscreteStepsAction().setVisible(false);
    _extraSettingsHolder.getColorMapAction().getSettingsAction().getColorMapAction().getDiscretizeAlphaAction().setDisabled(true);
    _extraSettingsHolder.getColorMapAction().getSettingsAction().getColorMapAction().getDiscretizeAlphaAction().setVisible(false);
    _extraSettingsHolder.getColorMapAction().getSettingsAction().getColorMapAction().getCustomColorMapGroupAction().setDisabled(true);
    _extraSettingsHolder.getColorMapAction().getSettingsAction().getColorMapAction().getCustomColorMapGroupAction().setVisible(false);


    _metaDataSettingsHolder.getColorTraitAction().setDefaultWidgetFlags(OptionAction::ComboBox);
    _metaDataSettingsHolder.getColorTraitAction().initialize(QStringList({  }));
    _metaDataSettingsHolder.getNumericTraitAction().setDefaultWidgetFlags(OptionAction::ComboBox);
    _metaDataSettingsHolder.getNumericTraitAction().initialize(QStringList({  }));
    _metaDataSettingsHolder.getStringTraitAction().setDefaultWidgetFlags(OptionAction::ComboBox);
    _metaDataSettingsHolder.getStringTraitAction().initialize(QStringList({  }));



    _linkerSettingsHolder.getScatterplotLeafSelectionValue().setString("");
    _linkerSettingsHolder.getScatterplotLeafSelectionValue().setDefaultWidgetFlags(StringAction::LineEdit);
    _linkerSettingsHolder.getReembeddingOptions().setDefaultWidgetFlags(TriggerAction::IconText);
    _linkerSettingsHolder.getReembeddingOptions().setChecked(false);
    _linkerSettingsHolder.getTreeLeafSelectionValueQT().setString("");
    _linkerSettingsHolder.getTreeLeafSelectionValueQT().setDefaultWidgetFlags(StringAction::LineEdit);

    _extraSettingsHolder.getExpandAllAction().setDefaultWidgetFlags(ToggleAction::CheckBox);
    _extraSettingsHolder.getExpandAllAction().setChecked(false);
    _extraSettingsHolder.getExpandAllAction().setVisible(true);
    _extraSettingsHolder.getDisableAcceptDatasetDrops().setDefaultWidgetFlags(ToggleAction::CheckBox);
    _extraSettingsHolder.getDisableAcceptDatasetDrops().setChecked(false);
    _extraSettingsHolder.getDisableAcceptDatasetDrops().setVisible(true);
    _updateSettingsHolder.getUpdateViewsButtonAction().setDefaultWidgetFlags(TriggerAction::IconText);


    const auto treeSelection = [this]() -> void
        {

            if (_mainSettingsHolder.getComparisonTreeSelectionAction().getCurrentText() != "" && _mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentText() != "")
            {
                _mainSettingsHolder.getMainReferenceTreeSelectionAction().setEnabled(true);
                _extraSettingsHolder.setEnabled(true);
                updateChartDataJS();
            }
            if (_mainSettingsHolder.getComparisonTreeSelectionAction().getCurrentDataset().isValid())
            {
                auto temp = mv::data().getDataset<CrossSpeciesComparisonTree>(_mainSettingsHolder.getComparisonTreeSelectionAction().getCurrentDataset()->getId());
                if (temp.isValid())
                {


                    _viewerPlugin.getComparisonTreeDataset().setDataset(temp.getDataset());
                    //_extraSettingsHolder.getNumOfClustersAction().setMaximum(temp->getTreeSpeciesNames().size());
                    //_extraSettingsHolder.getNumOfClustersAction().setMinimum(1);
                    //_extraSettingsHolder.getNumOfClustersAction().setValue(temp->getTreeSpeciesNames().size() - 1);
                }

            }



        };
    connect(&_mainSettingsHolder.getComparisonTreeSelectionAction(), &DatasetPickerAction::currentIndexChanged, this, treeSelection);

    const auto referenceTreeSelection = [this]() -> void
        {

            treeDatasetPickerActionModify();
            traitDatasetPickerActionModify();
            if (_mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentDataset().isValid())
            {
                auto temp = mv::data().getDataset<CrossSpeciesComparisonTree>(_mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentDataset()->getId());
                if (temp.isValid())
                {
                    _viewerPlugin.getReferenceTreeDataset().setDataset(temp.getDataset());

                    //_extraSettingsHolder.getNumOfClustersAction().setMaximum(temp->getTreeSpeciesNames().size());
                    //_extraSettingsHolder.getNumOfClustersAction().setMinimum(1);
                    //_extraSettingsHolder.getNumOfClustersAction().setValue(temp->getTreeSpeciesNames().size() - 1);
                }


            }


        };
    connect(&_mainSettingsHolder.getMainReferenceTreeSelectionAction(), &DatasetPickerAction::currentIndexChanged, this, referenceTreeSelection);


    const auto traitSelectionColor = [this]() -> void
        {

            if (_metaDataSettingsHolder.getTraitDatasetSelectionAction().getCurrentText() != "" && _metaDataSettingsHolder.getTraitDatasetSelectionAction().getCurrentDataset().isValid() && _mainSettingsHolder.getComparisonTreeSelectionAction().getCurrentText() != "" && _mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentText() != "" && _mainSettingsHolder.getComparisonTreeSelectionAction().getCurrentDataset().isValid() && _mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentDataset().isValid())
            {
                if (_metaDataSettingsHolder.getColorTraitAction().getCurrentText() != "")
                {
                    colorTraitCalculation();
                }
            }

        };
    
    const auto updateTreeLeafSelectionFromQT = [this]() -> void
        {



        };

    connect(&_linkerSettingsHolder.getTreeLeafSelectionValueQT(), &StringAction::stringChanged, this, updateTreeLeafSelectionFromQT);

    const auto traitSelectionNumeric = [this]() -> void
        {

            if (_metaDataSettingsHolder.getTraitDatasetSelectionAction().getCurrentText() != "" && _metaDataSettingsHolder.getTraitDatasetSelectionAction().getCurrentDataset().isValid() && _mainSettingsHolder.getComparisonTreeSelectionAction().getCurrentText() != "" && _mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentText() != "" && _mainSettingsHolder.getComparisonTreeSelectionAction().getCurrentDataset().isValid() && _mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentDataset().isValid())
            {

                if (_metaDataSettingsHolder.getNumericTraitAction().getCurrentText() != "")
                {
                    numericTraitCalculation();
                }

            }

        };
    const auto traitSelectionString = [this]() -> void
        {

            if (_metaDataSettingsHolder.getTraitDatasetSelectionAction().getCurrentText() != "" && _metaDataSettingsHolder.getTraitDatasetSelectionAction().getCurrentDataset().isValid() && _mainSettingsHolder.getComparisonTreeSelectionAction().getCurrentText() != "" && _mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentText() != "" && _mainSettingsHolder.getComparisonTreeSelectionAction().getCurrentDataset().isValid() && _mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentDataset().isValid())
            {
                if (_metaDataSettingsHolder.getStringTraitAction().getCurrentText() != "")
                {
                    stringTraitCalculation();
                }
            }

        };

    connect(&_metaDataSettingsHolder.getColorTraitAction(), &OptionAction::currentIndexChanged, this, traitSelectionColor);
    connect(&_metaDataSettingsHolder.getStringTraitAction(), &OptionAction::currentIndexChanged, this, traitSelectionString);
    connect(&_metaDataSettingsHolder.getNumericTraitAction(), &OptionAction::currentIndexChanged, this, traitSelectionNumeric);
    /*const auto clusteringMethod = [this]() -> void
        {
            if (_mainSettingsHolder.getComparisonTreeSelectionAction().getCurrentText() != "" && _mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentText() != "")
            {
                updateChartDataJS();
            }
        };
    connect(&_extraSettingsHolder.getClusteringMethodAction(), &OptionAction::currentIndexChanged, this, clusteringMethod);
    const auto numOfClustersAction = [this]() -> void
        {
            if (_mainSettingsHolder.getComparisonTreeSelectionAction().getCurrentText() != "" && _mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentText() != "")
            {
                updateChartDataJS();
            }
        };
    connect(&_extraSettingsHolder.getNumOfClustersAction(), &IntegralAction::valueChanged, this, numOfClustersAction); */
    const auto traitDatasetSelection = [this]() -> void
        {
            //TODO :

/*
            if (_extraSettingsHolder.getTraitDatasetSelectionAction().getCurrentText() != "" && _extraSettingsHolder.getTraitDatasetSelectionAction().getCurrentDataset().isValid())
            {
                QStringList optionvalues = {};
                _extraSettingsHolder.getTraitSelectionAction().initialize(optionvalues);

                auto childrendatasetsPoint1 = _extraSettingsHolder.getTraitDatasetSelectionAction().getCurrentDataset()->getChildren();
                for (auto childPoint1 : childrendatasetsPoint1)
                {
                    if (childPoint1->getDataType() == PointType)
                    {
                        auto checkPointDataset1 = mv::data().getDataset<Points>(childPoint1.getDatasetId());
                        auto checkPointDimensions1 = checkPointDataset1->getDimensionNames();

                        auto childrendatasetsPoint2 = checkPointDataset1->getChildren();
                        for (auto childPoint2 : childrendatasetsPoint1)
                        {
                            if (childPoint2->getDataType() == ClusterType)
                            {
                                auto checkPClusterDataset2 = mv::data().getDataset<Clusters>(childPoint2.getDatasetId());
                                auto checkClusternames = checkPClusterDataset2->getClusters();
                                for(auto cluster: checkClusternames)
                                {
                                    //if(cluster.getName()!="")
                                    {
                                        optionvalues.append(cluster.getName());
                                    }
                                }

                            }
                        }

                    }
                }
                _extraSettingsHolder.getTraitSelectionAction().initialize(optionvalues);
                if (_extraSettingsHolder.getTraitSelectionAction().getNumberOfOptions() > 0)
                {
                    _extraSettingsHolder.getTraitSelectionAction().setCurrentIndex(0);
                }

                if (_extraSettingsHolder.getTraitDatasetSelectionAction().getCurrentDataset().isValid())
                {
                    auto temp = mv::data().getDataset<Points>(_extraSettingsHolder.getTraitDatasetSelectionAction().getCurrentDataset()->getId());
                    if (temp.isValid())
                    {
                        _viewerPlugin.getMetaInfoDataset().setDataset(temp.getDataset());
                        _extraSettingsHolder.getNumOfClustersAction().setMaximum(temp->getNumDimensions());
                        _extraSettingsHolder.getNumOfClustersAction().setMinimum(1);
                        _extraSettingsHolder.getNumOfClustersAction().setValue(temp->getNumDimensions() - 1);
                }
                }

            }

            */
            auto dataset = _metaDataSettingsHolder.getTraitDatasetSelectionAction().getCurrentDataset();
            auto datasetText = _metaDataSettingsHolder.getTraitDatasetSelectionAction().getCurrentText();
            if (dataset.isValid() && datasetText != "")
            {

                auto data = mv::data().getDataset<CrossSpeciesComparisonTreeMeta>(dataset->getId());
                if (data.isValid())
                {

                    _viewerPlugin.getMetaInfoDataset().setDataset(data.getDataset());
                    auto propertyString = data->getTreeMetaPropertyNames();
                    QStringList propertyOptionsString = extractTraitPropertyOptionValues("String", propertyString);
                    QStringList propertyOptionsNumeric = extractTraitPropertyOptionValues("Numeric", propertyString);
                    QStringList propertyOptionsColor = extractTraitPropertyOptionValues("Color", propertyString);

                    auto traitSelectedColor = _metaDataSettingsHolder.getColorTraitAction().getCurrentText();
                    auto traitSelectedNumeric = _metaDataSettingsHolder.getNumericTraitAction().getCurrentText();
                    auto traitSelectedString = _metaDataSettingsHolder.getStringTraitAction().getCurrentText();

                    _metaDataSettingsHolder.getStringTraitAction().setOptions(propertyOptionsString);
                    _metaDataSettingsHolder.getColorTraitAction().setOptions(propertyOptionsColor);
                    _metaDataSettingsHolder.getNumericTraitAction().setOptions(propertyOptionsNumeric);
                    if (traitSelectedColor != "")
                    {
                        if (propertyOptionsColor.contains(traitSelectedColor))
                        {
                            _metaDataSettingsHolder.getColorTraitAction().setCurrentText(traitSelectedColor);
                        }
                    }
                    if (traitSelectedString != "")
                    {
                        if (propertyOptionsString.contains(traitSelectedString))
                        {
                            _metaDataSettingsHolder.getStringTraitAction().setCurrentText(traitSelectedString);
                        }
                    }
                    if (traitSelectedNumeric != "")
                    {
                        if (propertyOptionsNumeric.contains(traitSelectedNumeric))
                        {
                            _metaDataSettingsHolder.getNumericTraitAction().setCurrentText(traitSelectedNumeric);
                        }
                    }

                }

            }



        };
    connect(&_metaDataSettingsHolder.getTraitDatasetSelectionAction(), &DatasetPickerAction::currentIndexChanged, this, traitDatasetSelection);




    const auto colormapFilter = [this]() -> void
        {
            const auto& mirrorAction = _extraSettingsHolder.getColorMapAction().getMirrorAction(ColorMapAction::Axis::X);
            std::string s1 = _extraSettingsHolder.getColorMapAction().getColorMap().toStdString();
            std::string s2 = "*%*";
            std::string s3;
            if (mirrorAction.isChecked())
            {
                s3 = "T";
            }
            else
            {
                s3 = "F";
            }
            std::string full = s1 + s2 + s3;
            _viewerPlugin.getChartWidget().setTreeColorMap(QString::fromStdString(full));
        };

    connect(&_extraSettingsHolder.getColorMapAction(), &ColorMap1DAction::imageChanged, this, colormapFilter);


    const auto showReferenceTreeSelection = [this]() -> void
        {
            if (_extraSettingsHolder.getShowReferenceTreeAction().isChecked())
            {
                _viewerPlugin.getChartWidget().setShowReferenceTree("T");
            }
            else
            {
                _viewerPlugin.getChartWidget().setShowReferenceTree("F");
            }
        };
    connect(&_extraSettingsHolder.getShowReferenceTreeAction(), &ToggleAction::toggled, this, showReferenceTreeSelection);


    const auto showExpandAllSelection = [this]() -> void
        {
            if (_extraSettingsHolder.getExpandAllAction().isChecked())
            {
                _viewerPlugin.getChartWidget().setExpandAll("T");
            }
            else
            {
                _viewerPlugin.getChartWidget().setExpandAll("F");
            }
        };
    connect(&_extraSettingsHolder.getExpandAllAction(), &ToggleAction::toggled, this, showExpandAllSelection);


    const auto updateViewsButtonUpdate = [this]() -> void
        {

        };
    connect(&_updateSettingsHolder.getUpdateViewsButtonAction(), &TriggerAction::triggered, this, updateViewsButtonUpdate);

    referenceDatasetPickerActionModify();
}



void ChartOptions::updateChartDataJS()
{
    /*

        Datasets datasets = mv::data().getAllDatasets(std::vector<DataType>({ PointType }));


        if (!datasets.isEmpty())
        {
            QString selectedSubCluster = _extraSettingsHolder.getSubClusterSelectionAction().getCurrentText();
            //QStringList clusterSet = _extraSettingsHolder.getSubClusterSelectionAction().getOptions();

            std::map<std::string, double> elements;
            double totalCount = 0.0;
            for (auto dataset : datasets)
            {
                std::string str1 = dataset->getGuiName().toStdString();
                //qDebug() << "Size: " << _totalSpeciesForTreeConstruction.size();
                std::string str2 = std::to_string(_totalSpeciesForTreeConstruction.size()) + "Species-Hierarchy";
                //qDebug()<<"***str1***" <<QString::fromStdString(str1);
                //qDebug()<< "***str2***" << QString::fromStdString(str2);
                //Use std::string::find instead of strstr for string searching
                if (str1 == str2)
                {
                    std::map<std::string, std::string> referenceValues;

                    auto candidateDataset = mv::data().getDataset<Points>(dataset->getId());

                    auto dimensionNames = candidateDataset->getDimensionNames();
                    int numTemp = 0;
                    for (const QString& dimensionNameValue : dimensionNames)
                    {
                        std::string tempDimensionNameValue = dimensionNameValue.toStdString();
                        std::string tempValue = ":[";
                        std::vector<float> dimensionValuesTemp;
                        candidateDataset->extractDataForDimension(dimensionValuesTemp, numTemp);
                        numTemp = numTemp + 1;
                        for (int i = 0; i < dimensionValuesTemp.size(); i++)
                        {
                            tempValue = tempValue + std::to_string(int(dimensionValuesTemp[i]));
                            tempValue = tempValue + ",";
                        }
                        if (!tempValue.empty()) {
                            //qDebug() << "********************************Four*************************";
                            // Remove the last character using erase
                            tempValue.erase(tempValue.end() - 1);
                        }
                        tempValue = tempValue + "]";
                        referenceValues[tempDimensionNameValue] = tempValue;

                    }



                    std::string valueStringReference = "{";
                    for (const auto& pair : referenceValues) {
                        valueStringReference += "\"" + pair.first + "\"" + pair.second + ",";
                    }

                    // Remove the trailing comma and close the JSON string
                    valueStringReference.pop_back();
                    valueStringReference += "}";

                    //std::cout << valueStringReference << std::endl;

                    //std::string valueStringReference = "{\"Human\":[1,0,0,0,0],\"Chimpanzee\":[0,1,0,0,0],\"Gorilla\":[0,0,1,0,0],\"Rhesus\":[0,0,0,1,0],\"Marmoset\":[0,0,0,0,1]}";
                    std::string resultString = valueStringReference + "%$%$" + jsonString;

                    //std::cout << "\nMatrix as a string QT:\n" << resultString << "\n";

                    _viewerPlugin.getChartWidget().setData(QString::fromStdString(resultString));




                }
            }





        }
        */
    std::string resultString = "";
    if (_mainSettingsHolder.getComparisonTreeSelectionAction().getCurrentDataset().isValid() && _mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentDataset().isValid() && _mainSettingsHolder.getComparisonTreeSelectionAction().getCurrentText() != "" && _mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentText() != "")
    {
        std::string valueString = extractFormatData(_mainSettingsHolder.getComparisonTreeSelectionAction().getCurrentDataset().getDatasetId());

        std::string valueStringReference = extractFormatData(_mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentDataset().getDatasetId());

        if (valueStringReference != "" && valueString != "")
        {
            std::string resultString = valueStringReference + "@#&%$splithere%$&#@" + valueString;

            _showDataDrop.setChecked(false);

            _viewerPlugin.getChartWidget().setData(QString::fromStdString(resultString));
        }

    }

    /*
        std::string valueStringReference = "{\"Human\":[1,0,0,0,0],\"Chimpanzee\":[0,1,0,0,0],\"Gorilla\":[0,0,1,0,0],\"Rhesus\":[0,0,0,1,0],\"Marmoset\":[0,0,0,0,1]}";
        std::string resultString = valueStringReference + "%$%$" + valueStringReference;//jsonString;
        _showDataDrop.setChecked(false);

        _viewerPlugin.getChartWidget().setData(QString::fromStdString(resultString));

        */
}
std::vector<double> getCondensedDistanceMatrix(const std::vector<double>& distanceMatrix, int n) {
    std::vector<double> condensedMatrix;
    condensedMatrix.reserve(n * (n - 1) / 2); // Reserve space for upper triangle excluding diagonal

    int index = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            condensedMatrix.push_back(distanceMatrix[i * n + j]); // Accessing upper triangle elements
            index++;
        }
    }

    return condensedMatrix;
}

nlohmann::json createLeafNode(QString name, int cluster) {
    nlohmann::json node;
    node["cluster"] = cluster;
    node["name"] = name.toStdString();
    node["color"] = "#000000";
    node["hastrait"] = false;
    node["iscollapsed"] = false;
    return node;
}

nlohmann::json createInternalNode(int id, double score, int width, QString description) {
    nlohmann::json node;
    node["id"] = id;
    node["score"] = score;
    node["width"] = width;
    node["description"] = description.toStdString();
    node["children"] = nlohmann::json::array();
    return node;
}

std::string createDendrogramJson(int* merge, std::vector<QString> dimensionNames, int n) {
    std::vector<nlohmann::json> nodes(2 * n - 1);

    // Initialize leaf nodes
    for (int i = 0; i < n; i++) {
        nodes[i] = createLeafNode(dimensionNames[i], i);
    }
    for (int i = 0; i < n - 1; i++) {
        QString description = "Merge Step " + QString::number(i + 1) + ": ";
        int cluster1 = merge[2 * i];
        int cluster2 = merge[2 * i + 1];

        // Decode cluster1
        if (cluster1 < 0) {
            description += dimensionNames[-cluster1 - 1];
            nodes[n + i] = createInternalNode(i + 1, 1.0, 2, description);  // Adjust score and width as needed
        }
        else {
            description += "Cluster formed at step " + QString::number(cluster1);
            nodes[n + i] = createInternalNode(i + 1, 1.0, 2, description);  // Adjust score and width as needed
        }
        description += " merged with ";

        // Decode cluster2
        if (cluster2 < 0) {
            description += dimensionNames[-cluster2 - 1];
            nodes[n + i] = createInternalNode(i + 1, 1.0, 2, description);  // Adjust score and width as needed
        }
        else {
            description += "Cluster formed at step " + QString::number(cluster2);
            nodes[n + i] = createInternalNode(i + 1, 1.0, 2, description);  // Adjust score and width as needed
        }

        int left = cluster1 < 0 ? -cluster1 - 1 : cluster1 + n - 1;
        int right = cluster2 < 0 ? -cluster2 - 1 : cluster2 + n - 1;
        nodes[n + i]["children"].push_back(nodes[left]);
        nodes[left] = {};  // Clear the node after using it
        nodes[n + i]["children"].push_back(nodes[right]);
        nodes[right] = {};  // Clear the node after using it
    }
    // Build internal nodes
    /*for (int i = 0; i < n - 1; i++) {
        QString description = "Merge Step " + QString::number(i + 1) + ": ";
        int cluster1 = merge[2 * i];
        int cluster2 = merge[2 * i + 1];

        // Decode cluster1
        if (cluster1 < 0) {
            description += dimensionNames[-cluster1 - 1];
        }
        else {
            description += "Cluster formed at step " + QString::number(cluster1);
        }
        description += " merged with ";

        // Decode cluster2
        if (cluster2 < 0) {
            description += dimensionNames[-cluster2 - 1];
        }
        else {
            description += "Cluster formed at step " + QString::number(cluster2);
        }

        nodes[n + i] = createInternalNode(i + 1, 1.0, 2, description);  // Adjust score and width as needed
        int left = cluster1 < 0 ? -cluster1 - 1 : cluster1 + n - 1;
        int right = cluster2 < 0 ? -cluster2 - 1 : cluster2 + n - 1;
        nodes[n + i]["children"].push_back(nodes[left]);
        nodes[left] = {};  // Clear the node after using it
        nodes[n + i]["children"].push_back(nodes[right]);
        nodes[right] = {};  // Clear the node after using it
    }*/

    nlohmann::json root = nodes.back();
    std::string jsonString = root.dump(4);

    //std::cout << jsonString << std::endl;
    return jsonString;
}

std::string ChartOptions::extractFormatData(QString datasetValue)
{
    //std::string valueStringReference = "";
    Dataset<CrossSpeciesComparisonTree> dataset = mv::data().getDataset<CrossSpeciesComparisonTree>(datasetValue);
    auto temp = dataset->getTreeData();
    std::string valueStringReference = QJsonDocument(temp).toJson().toStdString();


    /*
    if(dataset->getNumDimensions()== dataset->getNumPoints())
  {

    auto dimensionNames = dataset->getDimensionNames();
    int n = dataset->getNumDimensions();

    std::vector<std::vector<float>> distanceMatrix(n, std::vector<float>(n));
    int ittr = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            distanceMatrix[i][j]=dataset->getValueAt(ittr) ;
            ittr++;
        }
    }
    // Print the distanceMatrix in a n*n format
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cout << distanceMatrix[i][j] << " ";
        }
        std::cout << "\n";
    }

    double* distmat = new double[(n * (n - 1)) / 2];
    int k = 0;

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            distmat[k++] = distanceMatrix[i][j];
        }
    }

    // Print the condensed distance matrix in a triangular format
    k = 0;
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < i + 1; ++j) {
            std::cout << "   ";
        }
        for (int j = i + 1; j < n; ++j) {
            std::cout << distmat[k++] << " ";
        }
        std::cout << "\n";
    }



    // Clustering (assuming hclust_fast, cutree_k, and cutree_cdist are defined)


        int method = 0;
        if(_extraSettingsHolder.getClusteringMethodAction().getCurrentText()=="single")
        {
            method = 0;

        }
        else if(_extraSettingsHolder.getClusteringMethodAction().getCurrentText() == "complete")
        {
            method = 1;
        }
        else if (_extraSettingsHolder.getClusteringMethodAction().getCurrentText() == "average")
        {
            method = 2;
        }
        else if (_extraSettingsHolder.getClusteringMethodAction().getCurrentText() == "median")
        {
            method = 3;
        }
    int* merge = new int[2 * (n - 1)];
    double* height = new double[n - 1];
    hclust_fast(n, distmat, method, merge, height);
    */

    /* https://www.rdocumentation.org/packages/stats/versions/3.6.2/topics/hclust, https://github.com/cdalitz/hclust-cpp/tree/master* /
        /*
         *
         *
merge: an n-1 by 2 matrix. Row i of merge describes the merging of clusters at step i of the clustering. If an element j in the row is negative, then observation -j was merged at this stage. If j is positive then the merge was with the cluster formed at the (earlier) stage j of the algorithm. Thus negative entries in merge indicate agglomerations of singletons, and positive entries indicate agglomerations of non-singletons.
height: a set of n-1 real values (non-decreasing for ultrametric trees). The clustering height: that is, the value of the criterion associated with the clustering method for the particular agglomeration.
order: a vector giving the permutation of the original observations suitable for plotting, in the sense that a cluster plot using this ordering and matrix merge will not have crossings of the branches.
labels: labels for each of the objects being clustered.
method: the cluster method that has been used.
     */
     /* int* labels = new int[n];
      //cutree_k(n, merge, _extraSettingsHolder.getNumOfClustersAction().getValue(), labels);
      cutree_cdist(n, merge, height, 0, labels);*/
      /*
      // Print labels
      for (int i = 0; i < n; i++) {
          std::cout << dimensionNames.at(i).toStdString()<< " belongs to cluster " << labels[i] << std::endl;
      }
      // stop clustering at step with custer distance >= cdist
      //cutree_cdist(n, merge, height, 0, labels);
      // Print merge matrix
      std::cout << "Merge matrix:" << std::endl;
      for (int i = 0; i < n - 1; i++) {
          std::cout << "Merge " << i << ": " << merge[2 * i] << " and " << merge[2 * i + 1]
              << " at height " << height[i] << std::endl;
      } */
      // inspect dendrogram ----------------------------------------begin
      /*
      std::cout << "Clustering method: " << _extraSettingsHolder.getClusteringMethodAction().getCurrentText() .toStdString()<< std::endl;
      std::cout << "Number of clusters: " << std::to_string(_extraSettingsHolder.getNumOfClustersAction().getValue()) << std::endl;
      for (int i = 0; i < n - 1; ++i) { // For each merge step
          int cluster1 = merge[2 * i];
          int cluster2 = merge[2 * i + 1];

          std::cout << "Merge Step " << (i + 1) << ": ";

          // Decode cluster1
          if (cluster1 < 0) {
              std::cout << dimensionNames[-cluster1 - 1].toStdString();
          }
          else {
              std::cout << "Cluster formed at step " << cluster1;
          }
          std::cout << " merged with ";

          // Decode cluster2
          if (cluster2 < 0) {
              std::cout << dimensionNames[-cluster2 - 1].toStdString();
          }
          else {
              std::cout << "Cluster formed at step " << cluster2;
          }

          std::cout << std::endl;
      }
      // inspect dendrogram ----------------------------------------end
      std::cout << "\n**************************************\n";
      std::string valueStringReference =createDendrogramJson( merge,dimensionNames,  n);

      // Clean up memory
      delete[] distmat;
      delete[] merge;
      delete[] height;
      delete[] labels;

      */
      /*
          std::map<std::string, std::string> referenceValues;
          int numTemp = 0;
          for (const QString& dimensionNameValue : dimensionNames)
          {
              std::string tempDimensionNameValue = dimensionNameValue.toStdString();
              std::string tempValue = ":[";
              std::vector<float> dimensionValuesTemp;
              dataset->extractDataForDimension(dimensionValuesTemp, numTemp);
              numTemp = numTemp + 1;
              for (int i = 0; i < dimensionValuesTemp.size(); i++)
              {
                  tempValue = tempValue + std::to_string(int(dimensionValuesTemp[i]));
                  tempValue = tempValue + ",";
              }
              if (!tempValue.empty()) {
                  //qDebug() << "********************************Four*************************";
                  // Remove the last character using erase
                  tempValue.erase(tempValue.end() - 1);
              }
              tempValue = tempValue + "]";
              referenceValues[tempDimensionNameValue] = tempValue;

          }
          std::string valueStringReference = "{";
          for (const auto& pair : referenceValues) {
              valueStringReference += "\"" + pair.first + "\"" + pair.second + ",";
          }

          // Remove the trailing comma and close the JSON string
          valueStringReference.pop_back();
          valueStringReference += "}";
              */
    return valueStringReference;

    //}

}

QStringList ChartOptions::extractTraitPropertyOptionValues(QString propertyType, QString propertyString) {
    QStringList lines = propertyString.split("\n");
    for (QString line : lines) {
        if (line.startsWith(propertyType)) {
            line = line.remove(0, propertyType.length() + 1); // remove the property type and the colon
            return line.split(", ");
        }
    }
    return QStringList(); // return an empty list if the property type is not found
}



QString extractMetaOptionValues(QString key, QJsonObject data) {
    QJsonObject result;

    // Iterate over all keys in the input data
    for (auto it = data.begin(); it != data.end(); ++it) {
        // If the value associated with this key is an object and contains the desired key
        if (it.value().isObject() && it.value().toObject().contains(key)) {
            // Add this key-value pair to the result
            result.insert(it.key(), QJsonObject({ {key, it.value().toObject().value(key)} }));
        }
    }

    // Convert the result to a JSON string and return it
    return QString(QJsonDocument(result).toJson(QJsonDocument::Compact));
}

QString ChartOptions::filterData(const QString& key, const QString& category, const QJsonObject& data) {
    QJsonObject result;
    QJsonObject keyObject;
    for (auto it = data.begin(); it != data.end(); ++it) {
        QJsonObject subObject = it.value().toObject();
        if (subObject.contains(category)) {
            QJsonObject categoryObject = subObject.value(category).toObject();
            if (categoryObject.contains(key)) {
                keyObject.insert(it.key(), categoryObject.value(key)); // Directly insert the value
            }
        }
    }
    result.insert("type", key);
    result.insert("data", keyObject);
    QJsonDocument doc(result);
    return QString::fromUtf8(doc.toJson(QJsonDocument::Compact)); // Use Compact to remove newlines
}





void ChartOptions::colorTraitCalculation()
{
    auto value = _metaDataSettingsHolder.getColorTraitAction().getCurrentText();
    auto dataset = _metaDataSettingsHolder.getTraitDatasetSelectionAction().getCurrentDataset();
    auto datasetText = _metaDataSettingsHolder.getTraitDatasetSelectionAction().getCurrentText();
    if (value != "" && dataset.isValid() && datasetText != "")
    {

        auto data = mv::data().getDataset<CrossSpeciesComparisonTreeMeta>(dataset->getId());
        if (data.isValid())
        {
            auto dataString = data->getTreeMetaData();
            QString metaOptions = filterData(value, "Color", dataString);
            //qDebug() << "\n\npropertyOptionsColor =" << metaOptions;
            _viewerPlugin.getChartWidget().setTraitColor(metaOptions);
        }

    }


}



void ChartOptions::stringTraitCalculation()
{
    auto value = _metaDataSettingsHolder.getStringTraitAction().getCurrentText();
    auto dataset = _metaDataSettingsHolder.getTraitDatasetSelectionAction().getCurrentDataset();
    auto datasetText = _metaDataSettingsHolder.getTraitDatasetSelectionAction().getCurrentText();
    if (value != "" && dataset.isValid() && datasetText != "")
    {
        auto data = mv::data().getDataset<CrossSpeciesComparisonTreeMeta>(dataset->getId());
        if (data.isValid())
        {
            auto dataString = data->getTreeMetaData();

            QString metaOptions = filterData(value, "String", dataString);

            //qDebug() << "\n\npropertyOptionsString =" << metaOptions;
            _viewerPlugin.getChartWidget().setTraitString(metaOptions);
        }

    }


}

void ChartOptions::numericTraitCalculation()
{
    auto value = _metaDataSettingsHolder.getNumericTraitAction().getCurrentText();
    auto dataset = _metaDataSettingsHolder.getTraitDatasetSelectionAction().getCurrentDataset();
    auto datasetText = _metaDataSettingsHolder.getTraitDatasetSelectionAction().getCurrentText();
    if (value != "" && dataset.isValid() && datasetText != "")
    {
        auto data = mv::data().getDataset<CrossSpeciesComparisonTreeMeta>(dataset->getId());
        if (data.isValid())
        {
            auto dataString = data->getTreeMetaData();
            QString metaOptions = filterData(value, "Numeric", dataString);

            //qDebug() << "\n\npropertyOptionsNumeric =" << metaOptions;

            _viewerPlugin.getChartWidget().setTraitNumeric(metaOptions);
        }

    }

}

void ChartOptions::referenceDatasetPickerActionModify()
{
    Datasets treeDatasets;
    Dataset<CrossSpeciesComparisonTree> currectDataset;
    auto datasets = mv::data().getAllDatasets(std::vector<DataType>({ CrossSpeciesComparisonTreeType }));

    if (_mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentText() != "")
    {
        currectDataset = _mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentDataset();

    }

    bool available = false;

    if (!datasets.isEmpty())
    {

        for (auto dataset : datasets)
        {
            if (dataset->getDataType() == CrossSpeciesComparisonTreeType)
            {

                treeDatasets << dataset;
                if (dataset == currectDataset)
                {
                    available = true;
                }

            }
        }
        if (available)
        {
            _mainSettingsHolder.getMainReferenceTreeSelectionAction().setDatasets(treeDatasets);
            _mainSettingsHolder.getMainReferenceTreeSelectionAction().setCurrentDataset(currectDataset);
        }
        else
        {
            _mainSettingsHolder.getMainReferenceTreeSelectionAction().setDatasets(treeDatasets);
            if (_mainSettingsHolder.getMainReferenceTreeSelectionAction().getNumberOfOptions() > 0)
            {
                //_mainSettingsHolder.getMainReferenceTreeSelectionAction().setCurrentIndex(0);
            }



        }

        /*

        */


    }

}

void ChartOptions::traitDatasetPickerActionModify()
{
    if (_mainSettingsHolder.getComparisonTreeSelectionAction().getCurrentText() != "" && _mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentText() != "" && _mainSettingsHolder.getComparisonTreeSelectionAction().getCurrentDataset().isValid() && _mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentDataset().isValid())
    {
        QStringList referenceTreeLeafNames = mv::data().getDataset<CrossSpeciesComparisonTree>(_mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentDataset()->getId())->getTreeLeafNames();
        QStringList comparisonTreeLeafNames = mv::data().getDataset<CrossSpeciesComparisonTree>(_mainSettingsHolder.getComparisonTreeSelectionAction().getCurrentDataset()->getId())->getTreeLeafNames();
        referenceTreeLeafNames.sort();
        comparisonTreeLeafNames.sort();


        if (referenceTreeLeafNames == comparisonTreeLeafNames)
        {

            bool compareFlag = false;
            Dataset<CrossSpeciesComparisonTreeMeta> tempDatasetholder;
            if (_metaDataSettingsHolder.getTraitDatasetSelectionAction().getCurrentDataset().isValid() && _metaDataSettingsHolder.getTraitDatasetSelectionAction().getCurrentText() != "")
            {
                tempDatasetholder = _metaDataSettingsHolder.getTraitDatasetSelectionAction().getCurrentDataset();
                compareFlag = true;
            }

            Datasets treeMetaDatasets;
            bool tempDatasetholderflag = false;
            std::vector<QString> compareDimensions;



            auto datasets = mv::data().getAllDatasets(std::vector<DataType>({ CrossSpeciesComparisonTreeMetaType }));


            for (auto dataset : datasets)
            {
                if (dataset->getDataType() == CrossSpeciesComparisonTreeMetaType)
                {
                    Dataset<CrossSpeciesComparisonTreeMeta> checkDataset = mv::data().getDataset<CrossSpeciesComparisonTreeMeta>(dataset.getDatasetId());

                    QStringList tempLeafNames = checkDataset->getTreeMetaLeafNames();
                    tempLeafNames.sort();
                    if (referenceTreeLeafNames == tempLeafNames)
                    {
                        treeMetaDatasets.append(dataset);
                        if (compareFlag)
                        {
                            if (dataset.getDatasetId() == tempDatasetholder.getDatasetId())
                            {
                                tempDatasetholderflag = true;
                            }
                        }


                    }

                }

            }








            if (_mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentText() != "" && _mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentDataset().isValid() && tempDatasetholderflag && compareFlag && _mainSettingsHolder.getComparisonTreeSelectionAction().getCurrentText() != "" && _mainSettingsHolder.getComparisonTreeSelectionAction().getCurrentDataset().isValid())
            {
                _metaDataSettingsHolder.getTraitDatasetSelectionAction().setDatasets(treeMetaDatasets);
                _metaDataSettingsHolder.getTraitDatasetSelectionAction().setCurrentText("");
                if (_metaDataSettingsHolder.getTraitDatasetSelectionAction().getNumberOfOptions() > 0)
                {
                    _metaDataSettingsHolder.getTraitDatasetSelectionAction().setCurrentDataset(tempDatasetholder);

                    if (tempDatasetholder.isValid())
                    {
                        _viewerPlugin.getMetaInfoDataset().setDataset(tempDatasetholder.getDataset());

                    }

                }



            }
            else
            {

                _metaDataSettingsHolder.getTraitDatasetSelectionAction().setDatasets(treeMetaDatasets);
                _metaDataSettingsHolder.getTraitDatasetSelectionAction().setCurrentText("");
                if (_metaDataSettingsHolder.getTraitDatasetSelectionAction().getNumberOfOptions() > 0)
                {
                    _metaDataSettingsHolder.getTraitDatasetSelectionAction().setCurrentIndex(0);

                }


            }

            //TODO : Need to populate the color, string and the numeric options
            auto dataset = _metaDataSettingsHolder.getTraitDatasetSelectionAction().getCurrentDataset();
            auto datasetText = _metaDataSettingsHolder.getTraitDatasetSelectionAction().getCurrentText();
            if (dataset.isValid() && datasetText != "")
            {

                auto data = mv::data().getDataset<CrossSpeciesComparisonTreeMeta>(dataset->getId());
                if (data.isValid())
                {
                    auto propertyString = data->getTreeMetaPropertyNames();
                    QStringList propertyOptionsString = extractTraitPropertyOptionValues("String", propertyString);
                    QStringList propertyOptionsNumeric = extractTraitPropertyOptionValues("Numeric", propertyString);
                    QStringList propertyOptionsColor = extractTraitPropertyOptionValues("Color", propertyString);

                    auto traitSelectedColor = _metaDataSettingsHolder.getColorTraitAction().getCurrentText();
                    auto traitSelectedNumeric = _metaDataSettingsHolder.getNumericTraitAction().getCurrentText();
                    auto traitSelectedString = _metaDataSettingsHolder.getStringTraitAction().getCurrentText();

                    _metaDataSettingsHolder.getStringTraitAction().setOptions(propertyOptionsString);
                    _metaDataSettingsHolder.getColorTraitAction().setOptions(propertyOptionsColor);
                    _metaDataSettingsHolder.getNumericTraitAction().setOptions(propertyOptionsNumeric);
                    if (traitSelectedColor != "")
                    {
                        if (propertyOptionsColor.contains(traitSelectedColor))
                        {
                            _metaDataSettingsHolder.getColorTraitAction().setCurrentText(traitSelectedColor);
                        }
                    }
                    if (traitSelectedString != "")
                    {
                        if (propertyOptionsString.contains(traitSelectedString))
                        {
                            _metaDataSettingsHolder.getStringTraitAction().setCurrentText(traitSelectedString);
                        }
                    }
                    if (traitSelectedNumeric != "")
                    {
                        if (propertyOptionsNumeric.contains(traitSelectedNumeric))
                        {
                            _metaDataSettingsHolder.getNumericTraitAction().setCurrentText(traitSelectedNumeric);
                        }
                    }

                }

            }



        }

    }

}





void ChartOptions::treeDatasetPickerActionModify()
{
    _mainSettingsHolder.getComparisonTreeSelectionAction().setEnabled(false);
    _extraSettingsHolder.setEnabled(false);
    _metaDataSettingsHolder.setEnabled(false);
    _linkerSettingsHolder.setEnabled(false);
    //_updateSettingsHolder.setEnabled(false);
    //if (_updateSettingsHolder.getUpdateViewsButtonAction().isConnected())
    //{
    //    _updateSettingsHolder.setVisible(true);
    //}
    //else
    //{
    //    _updateSettingsHolder.setVisible(false);
    //}

    if (_mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentText() != "")
    {
        Dataset<CrossSpeciesComparisonTree> tempDatasetholder;
        bool tempDatasetholderflag = false;
        Datasets treeDatasets;
        QStringList compareDimensions;
        if (_mainSettingsHolder.getComparisonTreeSelectionAction().getCurrentText() != "")
        {
            tempDatasetholder = _mainSettingsHolder.getComparisonTreeSelectionAction().getCurrentDataset();
        }



        auto currentReference = mv::data().getDataset<CrossSpeciesComparisonTree>(_mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentDataset()->getId());

        auto currentReferencedimensionNames = currentReference->getTreeLeafNames();


        auto datasets = mv::data().getAllDatasets(std::vector<DataType>({ CrossSpeciesComparisonTreeType }));


        for (auto dataset : datasets)
        {
            if (dataset->getDataType() == CrossSpeciesComparisonTreeType)
            {
                Dataset<CrossSpeciesComparisonTree> checkDataset = mv::data().getDataset<CrossSpeciesComparisonTree>(dataset.getDatasetId());

                QStringList tempDimensionNames = checkDataset->getTreeLeafNames();
                if (tempDimensionNames.size() > 0 && currentReferencedimensionNames.size() > 0)
                {
                    currentReferencedimensionNames.sort();
                    tempDimensionNames.sort();
                    if (tempDimensionNames == currentReferencedimensionNames) {
                        treeDatasets << dataset;
                        if (dataset.getDatasetId() == tempDatasetholder.getDatasetId())
                        {
                            tempDatasetholderflag = true;
                        }
                    }

                }







            }

        }

        if (_mainSettingsHolder.getComparisonTreeSelectionAction().getCurrentText() != "" && _mainSettingsHolder.getComparisonTreeSelectionAction().getCurrentDataset().isValid() && tempDatasetholderflag)
        {
            //tempDatasetholder = _mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentDataset();
            _mainSettingsHolder.getComparisonTreeSelectionAction().setDatasets(treeDatasets);
            _mainSettingsHolder.getComparisonTreeSelectionAction().setCurrentText("");
            _mainSettingsHolder.getComparisonTreeSelectionAction().setCurrentDataset(tempDatasetholder);

        }
        else
        {

            _mainSettingsHolder.getComparisonTreeSelectionAction().setDatasets(treeDatasets);
            _mainSettingsHolder.getComparisonTreeSelectionAction().setCurrentText("");
            if (_mainSettingsHolder.getComparisonTreeSelectionAction().getNumberOfOptions() > 0)
            {
                _mainSettingsHolder.getComparisonTreeSelectionAction().setCurrentIndex(0);
            }

        }

        //_viewerPlugin.getComparisonTreeDataset() = _mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentDataset();

    }
    if (_mainSettingsHolder.getComparisonTreeSelectionAction().getCurrentText() != "" && _mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentText() != "")
    {
        _mainSettingsHolder.getComparisonTreeSelectionAction().setEnabled(true);
        _extraSettingsHolder.setEnabled(true);
        _metaDataSettingsHolder.setEnabled(true);
        _linkerSettingsHolder.setEnabled(true);
        //_updateSettingsHolder.setEnabled(true);
        //if (_updateSettingsHolder.getUpdateViewsButtonAction().isConnected())
        //{
        //    _updateSettingsHolder.setVisible(true);
        //}
        //else
        //{
        ////    _updateSettingsHolder.setVisible(false);
       // }
    }
}
void ChartOptions::onDataEventTree(mv::DatasetEvent* dataEvent)
{
    if (dataEvent->getType() == mv::EventType::DatasetAdded)
    {
        referenceDatasetPickerActionModify();
        traitDatasetPickerActionModify();
        treeDatasetPickerActionModify();

    }
    if (dataEvent->getType() == mv::EventType::DatasetRemoved)
    {
        referenceDatasetPickerActionModify();
        traitDatasetPickerActionModify();
        treeDatasetPickerActionModify();

    }
    if (dataEvent->getType() == mv::EventType::DatasetChildAdded)
    {
        referenceDatasetPickerActionModify();
        traitDatasetPickerActionModify();
        treeDatasetPickerActionModify();

    }
    if (dataEvent->getType() == mv::EventType::DatasetChildRemoved)
    {
        referenceDatasetPickerActionModify();
        traitDatasetPickerActionModify();
        treeDatasetPickerActionModify();


    }
    if (dataEvent->getType() == mv::EventType::DatasetDataChanged)
    {
        referenceDatasetPickerActionModify();
        treeDatasetPickerActionModify();
        traitDatasetPickerActionModify();
        changeLoader();
    }


}

void ChartOptions::onDataEventTreeMeta(mv::DatasetEvent* dataEvent)
{
    if (dataEvent->getType() == mv::EventType::DatasetAdded)
    {
        traitDatasetPickerActionModify();

    }
    if (dataEvent->getType() == mv::EventType::DatasetRemoved)
    {
        traitDatasetPickerActionModify();

    }
    if (dataEvent->getType() == mv::EventType::DatasetChildAdded)
    {
        traitDatasetPickerActionModify();

    }
    if (dataEvent->getType() == mv::EventType::DatasetChildRemoved)
    {
        traitDatasetPickerActionModify();


    }
    if (dataEvent->getType() == mv::EventType::DatasetDataChanged)
    {
        traitDatasetPickerActionModify();
        changeLoader();
    }


}
void ChartOptions::initLoader()
{

    if (!_viewerPlugin.getInitialLoadCompleteflag())
    {
        _viewerPlugin.getChartWidget().setTreeColorMap(QString::fromStdString("Magma*%*F"));

        if (_extraSettingsHolder.getExpandAllAction().isChecked())
        {
            _viewerPlugin.getChartWidget().setExpandAll("T");
        }
        else
        {
            _viewerPlugin.getChartWidget().setExpandAll("F");
        }

        if (_extraSettingsHolder.getShowReferenceTreeAction().isChecked())
        {
            _viewerPlugin.getChartWidget().setShowReferenceTree("T");
        }
        else
        {
            _viewerPlugin.getChartWidget().setShowReferenceTree("F");
        }
        if (_metaDataSettingsHolder.getTraitDatasetSelectionAction().getCurrentText() != "" && _metaDataSettingsHolder.getTraitDatasetSelectionAction().getCurrentDataset().isValid() && _mainSettingsHolder.getComparisonTreeSelectionAction().getCurrentText() != "" && _mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentText() != "" && _mainSettingsHolder.getComparisonTreeSelectionAction().getCurrentDataset().isValid() && _mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentDataset().isValid())
        {
            //TODO: Add trait calculation
            if (_metaDataSettingsHolder.getColorTraitAction().getCurrentText() != "")
            {
                colorTraitCalculation();
            }
            if (_metaDataSettingsHolder.getNumericTraitAction().getCurrentText() != "")
            {
                numericTraitCalculation();
            }
            if (_metaDataSettingsHolder.getStringTraitAction().getCurrentText() != "")
            {
                stringTraitCalculation();
            }

        }

        _viewerPlugin.setInitialLoadCompleteflag(true);
        triggerChart();
    }




}
void ChartOptions::triggerChart()
{
    if (_mainSettingsHolder.getComparisonTreeSelectionAction().getCurrentText() != "" && _mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentText() != "")
    {
        _mainSettingsHolder.getMainReferenceTreeSelectionAction().setEnabled(true);
        _extraSettingsHolder.setEnabled(true);
        _metaDataSettingsHolder.setEnabled(true);
        _linkerSettingsHolder.setEnabled(true);
        //_updateSettingsHolder.setEnabled(true);
        //if (_updateSettingsHolder.getUpdateViewsButtonAction().isConnected())
        //{
        //    _updateSettingsHolder.setVisible(true);
       // }
       // else
       // {
       //     _updateSettingsHolder.setVisible(false);
        //}
        updateChartDataJS();
    }
    else
    {
        _mainSettingsHolder.getMainReferenceTreeSelectionAction().setEnabled(false);
        _extraSettingsHolder.setEnabled(false);
        _metaDataSettingsHolder.setEnabled(false);
        _linkerSettingsHolder.setEnabled(false);
        //_updateSettingsHolder.setEnabled(false);
        //if (_updateSettingsHolder.getUpdateViewsButtonAction().isConnected())
        //{
        //    _updateSettingsHolder.setVisible(true);
        //}
       // else
        //{
       //     _updateSettingsHolder.setVisible(false);
       // }
    }
}
void ChartOptions::changeLoader()
{


    if (_mainSettingsHolder.getComparisonTreeSelectionAction().getCurrentText() != "" && _mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentText() != "")
    {
        _mainSettingsHolder.getMainReferenceTreeSelectionAction().setEnabled(true);
        _extraSettingsHolder.setEnabled(true);
        _metaDataSettingsHolder.setEnabled(true);
        _linkerSettingsHolder.setEnabled(true);
        //_updateSettingsHolder.setEnabled(true);
        //if (_updateSettingsHolder.getUpdateViewsButtonAction().isConnected())
        //{
        //    _updateSettingsHolder.setVisible(true);
       // }
       // else
       // {
        //    _updateSettingsHolder.setVisible(false);
       // }
        updateChartDataJS();
    }





}


inline ChartOptions::ExtraSettingsHolder::ExtraSettingsHolder(ChartOptions& chartOptions) :
    VerticalGroupAction(&chartOptions, "Extra options"),
    _chartOptions(chartOptions),
    _showReferenceTree(this, "Show reference tree"),
    _treeColorMapAction(this, "Color map"),
    _expandAllAction(this, "Expand all"),
    _disableAcceptDatasetDrops(this, "Disable accept dataset drops")

{
    setText("Extra Options");
    setIcon(Application::getIconFont("FontAwesome").getIcon("toolbox"));
    setPopupSizeHint(QSize(350, 0));
    setConfigurationFlag(WidgetAction::ConfigurationFlag::ForceCollapsedInGroup);
    addAction(&_showReferenceTree);
    addAction(&_expandAllAction);
    addAction(&_treeColorMapAction);
    addAction(&_disableAcceptDatasetDrops);
}

inline ChartOptions::MetaDataSettingsHolder::MetaDataSettingsHolder(ChartOptions& chartOptions) :
    VerticalGroupAction(&chartOptions, "Meta data options"),
    _chartOptions(chartOptions),
    _traitDatasetSelectionAction(this, "Trait dataset"),
    _colorTrait(this, "Color type trait"),
    _numericTrait(this, "Numeric type trait"),
    _stringTrait(this, "String type trait")

{
    setText("Metadata Options");
    setIcon(Application::getIconFont("FontAwesome").getIcon("highlighter"));
    setPopupSizeHint(QSize(350, 0));
    setConfigurationFlag(WidgetAction::ConfigurationFlag::ForceCollapsedInGroup);
    addAction(&_traitDatasetSelectionAction);
    addAction(&_colorTrait);
    addAction(&_numericTrait);
    addAction(&_stringTrait);
}


inline ChartOptions::LinkerSettingsHolder::LinkerSettingsHolder(ChartOptions& chartOptions) :
    VerticalGroupAction(&chartOptions, "Meta data options"),
    _chartOptions(chartOptions),
    _scatterplotLeafSelectionValue(this, "Scatterplot Leaf Selection Value"),
    _reembeddingOptions(this, "Reembedding Options"),
    _treeLeafSelectionValue(this, "Tree Leaf Selection Value")

{
    setText("Linker Options");
    setIcon(Application::getIconFont("FontAwesome").getIcon("link"));
    setPopupSizeHint(QSize(350, 0));
    setConfigurationFlag(WidgetAction::ConfigurationFlag::ForceCollapsedInGroup);
    addAction(&_scatterplotLeafSelectionValue);
    addAction(&_treeLeafSelectionValue);
    addAction(&_reembeddingOptions);
}

inline ChartOptions::MainSettingsHolder::MainSettingsHolder(ChartOptions& chartOptions) :
    VerticalGroupAction(&chartOptions, "Chart Options"),
    _chartOptions(chartOptions),
    _mainReferenceTreeSelectionAction(this, "Reference tree"),
    _comparisonTreeSelectionAction(this, "Comparison tree")





{
    setText("Chart Options");
    setIcon(Application::getIconFont("FontAwesome").getIcon("atom"));
    setPopupSizeHint(QSize(350, 0));
    addAction(&_mainReferenceTreeSelectionAction);
    addAction(&_comparisonTreeSelectionAction);





}


inline ChartOptions::UpdateSettingsHolder::UpdateSettingsHolder(ChartOptions& chartOptions) :
    VerticalGroupAction(&chartOptions, "Update Options"),
    _chartOptions(chartOptions),

    _updateViewsButton(this, "Update Views")


{
    setText("Update Options");
    setIcon(Application::getIconFont("FontAwesome").getIcon("sync"));
    setPopupSizeHint(QSize(350, 0));
    _updateViewsButton.setIcon(Application::getIconFont("FontAwesome").getIcon("sync"));
    addAction(&_updateViewsButton);

}

void ChartOptions::fromVariantMap(const QVariantMap& variantMap)
{
    WidgetAction::fromVariantMap(variantMap);

    _mainSettingsHolder.getComparisonTreeSelectionAction().fromParentVariantMap(variantMap);
    _extraSettingsHolder.getColorMapAction().fromParentVariantMap(variantMap);
    _mainSettingsHolder.getMainReferenceTreeSelectionAction().fromParentVariantMap(variantMap);
    _metaDataSettingsHolder.getTraitDatasetSelectionAction().fromParentVariantMap(variantMap);
    _metaDataSettingsHolder.getColorTraitAction().fromParentVariantMap(variantMap);
    _metaDataSettingsHolder.getNumericTraitAction().fromParentVariantMap(variantMap);
    _metaDataSettingsHolder.getStringTraitAction().fromParentVariantMap(variantMap);
    _linkerSettingsHolder.getScatterplotLeafSelectionValue().fromParentVariantMap(variantMap);
    _linkerSettingsHolder.getReembeddingOptions().fromParentVariantMap(variantMap);
    _extraSettingsHolder.getShowReferenceTreeAction().fromParentVariantMap(variantMap);
    _extraSettingsHolder.getExpandAllAction().fromParentVariantMap(variantMap);
    _extraSettingsHolder.getDisableAcceptDatasetDrops().fromParentVariantMap(variantMap);
    _updateSettingsHolder.getUpdateViewsButtonAction().fromParentVariantMap(variantMap);
    _linkerSettingsHolder.getTreeLeafSelectionValueQT().fromParentVariantMap(variantMap);

}

QVariantMap ChartOptions::toVariantMap() const
{
    QVariantMap variantMap = WidgetAction::toVariantMap();
    _mainSettingsHolder.getComparisonTreeSelectionAction().insertIntoVariantMap(variantMap);
    _extraSettingsHolder.getColorMapAction().insertIntoVariantMap(variantMap);
    _mainSettingsHolder.getMainReferenceTreeSelectionAction().insertIntoVariantMap(variantMap);
    _metaDataSettingsHolder.getTraitDatasetSelectionAction().insertIntoVariantMap(variantMap);
    _metaDataSettingsHolder.getColorTraitAction().insertIntoVariantMap(variantMap);
    _metaDataSettingsHolder.getNumericTraitAction().insertIntoVariantMap(variantMap);
    _metaDataSettingsHolder.getStringTraitAction().insertIntoVariantMap(variantMap);
    _linkerSettingsHolder.getScatterplotLeafSelectionValue().insertIntoVariantMap(variantMap);
    _linkerSettingsHolder.getReembeddingOptions().insertIntoVariantMap(variantMap);
    _extraSettingsHolder.getShowReferenceTreeAction().insertIntoVariantMap(variantMap);
    _extraSettingsHolder.getExpandAllAction().insertIntoVariantMap(variantMap);
    _extraSettingsHolder.getDisableAcceptDatasetDrops().insertIntoVariantMap(variantMap);
    _updateSettingsHolder.getUpdateViewsButtonAction().insertIntoVariantMap(variantMap);
    _linkerSettingsHolder.getTreeLeafSelectionValueQT().insertIntoVariantMap(variantMap);
    return variantMap;
}