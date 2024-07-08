#include "ChartOptions.h"

#include <iostream>
#include <set>
#include "CrossSpeciesComparisonPhyTVuPlugin.h"
#include<string>  
#include <QFileDialog>
#include <QPageLayout>
#include <QWebEngineView>
#include <string>
#include <typeinfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>
#include <QSet>
#include <QJsonValue>
using namespace mv;
using namespace mv::gui;


bool areStringListsEqual(const QStringList& list1, const QStringList& list2) {
    // Convert QStringList to QSet for efficient comparison using QSet constructor
    QSet<QString> set1(list1.begin(), list1.end());
    QSet<QString> set2(list2.begin(), list2.end());

    // Compare the sets
    return set1 == set2;
}

ChartOptions::ChartOptions(CrossSpeciesComparisonPhyTVuPlugin& CrossSpeciesComparisonPhyTVuPlugin, mv::CoreInterface* core) :
    WidgetAction(&CrossSpeciesComparisonPhyTVuPlugin, "CrossSpeciesComparisonPhyTVuPlugin Chart"),
    _viewerPlugin(CrossSpeciesComparisonPhyTVuPlugin),
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

    _metaDataSettingsHolder.getColorTraitAction().setSerializationName("CSCPTV:Color type trait selection");
    _metaDataSettingsHolder.getNumericTraitAction().setSerializationName("Numeric type trait selection");
    _metaDataSettingsHolder.getStringTraitAction().setSerializationName("CSCPTV:String type trait selection");
    _metaDataSettingsHolder.getDisableTraitOptions().setSerializationName("CSCPTV:Disable Trait Options");
    _metaDataSettingsHolder.getTraitDatasetSelectionAction().setSerializationName("CSCPTV:Trait Dataset selection");
    _extraSettingsHolder.getExpandAllAction().setSerializationName("CSCPTV:Expand all selection");
    _extraSettingsHolder.getDisableAcceptDatasetDrops().setSerializationName("CSCPTV:Disable Accept Dataset Drops");
    _updateSettingsHolder.getUpdateViewsButtonAction().setSerializationName("CSCPTV:Explore Species Button");
    _linkerSettingsHolder.getTreeLeafSelectionValueQT().setSerializationName("CSCPTV:Tree Leaf Selection Value");
    _linkerSettingsHolder.getSelectedLeafValues().setSerializationName("CSCPTV:Selected Leaf Values");
    _linkerSettingsHolder.getLeafDatasetPicker().setSerializationName("CSCPTV:Leaf Dataset Picker");

    _eventListener.addSupportedEventType(static_cast<std::uint32_t>(EventType::DatasetAdded));
    _eventListener.addSupportedEventType(static_cast<std::uint32_t>(EventType::DatasetRemoved));
    _eventListener.addSupportedEventType(static_cast<std::uint32_t>(EventType::DatasetChildAdded));
    _eventListener.addSupportedEventType(static_cast<std::uint32_t>(EventType::DatasetChildRemoved));
    _eventListener.addSupportedEventType(static_cast<std::uint32_t>(EventType::DatasetDataChanged));

    _extraSettingsHolder.setEnabled(false);
    _metaDataSettingsHolder.setEnabled(false);
    _linkerSettingsHolder.setEnabled(false);
    _mainSettingsHolder.getMainReferenceTreeSelectionAction().setFilterFunction([this](mv::Dataset<DatasetImpl> dataset) -> bool {
        return dataset->getDataType() == CrossSpeciesComparisonTreeType;
        });
    _metaDataSettingsHolder.getTraitDatasetSelectionAction().setFilterFunction([this](mv::Dataset<DatasetImpl> dataset) -> bool {
        return dataset->getDataType() == CrossSpeciesComparisonTreeMetaType;
        });


    _eventListener.registerDataEventByType(CrossSpeciesComparisonTreeType, std::bind(&ChartOptions::onDataEventTree, this, std::placeholders::_1));
    _eventListener.registerDataEventByType(CrossSpeciesComparisonTreeMetaType, std::bind(&ChartOptions::onDataEventTreeMeta, this, std::placeholders::_1));


    _mainSettingsHolder.getMainReferenceTreeSelectionAction().setDefaultWidgetFlags(OptionAction::ComboBox);


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

    _linkerSettingsHolder.getSelectedLeafValues().setDefaultWidgetFlags(OptionsAction::ComboBox);
    _linkerSettingsHolder.getSelectedLeafValues().initialize(QStringList({  }));
    _linkerSettingsHolder.getLeafDatasetPicker().setDefaultWidgetFlags(OptionAction::ComboBox);
    _linkerSettingsHolder.getLeafDatasetPicker().initialize(QStringList({  }));

    _metaDataSettingsHolder.getDisableTraitOptions().setDefaultWidgetFlags(ToggleAction::CheckBox);
    _metaDataSettingsHolder.getDisableTraitOptions().setChecked(false);
    _linkerSettingsHolder.getTreeLeafSelectionValueQT().setString("");
    _linkerSettingsHolder.getTreeLeafSelectionValueQT().setDefaultWidgetFlags(StringAction::LineEdit);

    _extraSettingsHolder.getExpandAllAction().setDefaultWidgetFlags(ToggleAction::CheckBox);
    _extraSettingsHolder.getExpandAllAction().setChecked(false);
    _extraSettingsHolder.getExpandAllAction().setVisible(true);
    _extraSettingsHolder.getDisableAcceptDatasetDrops().setDefaultWidgetFlags(ToggleAction::CheckBox);
    _extraSettingsHolder.getDisableAcceptDatasetDrops().setChecked(false);
    _extraSettingsHolder.getDisableAcceptDatasetDrops().setVisible(true);
    _updateSettingsHolder.getUpdateViewsButtonAction().setDefaultWidgetFlags(TriggerAction::IconText);
    _updateSettingsHolder.getRevertButtonAction().setDefaultWidgetFlags(TriggerAction::IconText);


    QIcon exploreIcon = Application::getIconFont("FontAwesome").getIcon("wpexplorer");
    _updateSettingsHolder.getUpdateViewsButtonAction().setIcon(exploreIcon);

    QIcon revertIcon = Application::getIconFont("FontAwesome").getIcon("history");
    _updateSettingsHolder.getRevertButtonAction().setIcon(revertIcon);


    const auto leafDatasetSelection = [this]() -> void
        {
            QStringList clusterValues = {};
            if (_linkerSettingsHolder.getLeafDatasetPicker().getCurrentDataset().isValid())
            {
                auto temp = mv::data().getDataset<Clusters>(_linkerSettingsHolder.getLeafDatasetPicker().getCurrentDataset()->getId());
                if (temp.isValid())
                {
                    auto clusters = temp->getClusters();
                    if (clusters.size() > 0)
                    {
                        for (auto cluster : clusters)
                        {
                            auto name = cluster.getName();
                            if (name != "")
                            {
                                clusterValues.append(name);
                            }

                        }
                    }

                }
            }
            _linkerSettingsHolder.getSelectedLeafValues().setOptions(clusterValues);
        };
    connect(&_linkerSettingsHolder.getLeafDatasetPicker(), &DatasetPickerAction::currentIndexChanged, this, leafDatasetSelection);


    const auto updatespeciesExplorerInMap = [this]() -> void
        {
            bool hasSelectedOptions = !_linkerSettingsHolder.getSelectedLeafValues().getSelectedOptions().isEmpty() && !_linkerSettingsHolder.getTreeLeafSelectionValueQT().getString().isEmpty();
            _updateSettingsHolder.getUpdateViewsButtonAction().setDisabled(!hasSelectedOptions);

            bool shouldDisableRevertButton = true;
            if (hasSelectedOptions) {
                QString currentSelection = _linkerSettingsHolder.getTreeLeafSelectionValueQT().getString();
                if (!currentSelection.isEmpty()) {
                    QStringList temp = currentSelection.split(" @%$,$%@ ");
                    QStringList species = _linkerSettingsHolder.getSelectedLeafValues().getSelectedOptions();

                    std::sort(temp.begin(), temp.end());
                    std::sort(species.begin(), species.end());

                    shouldDisableRevertButton = (temp == species);
                }
            }

            _updateSettingsHolder.getRevertButtonAction().setDisabled(shouldDisableRevertButton);


        };
    connect(&_linkerSettingsHolder.getSelectedLeafValues(), &OptionsAction::selectedOptionsChanged, this, updatespeciesExplorerInMap);
    const auto revertButtomUpdate = [this]() -> void
        {
            _updateSettingsHolder.getRevertButtonAction().setDisabled(true);
        };
    connect(&_updateSettingsHolder.getRevertButtonAction(), &TriggerAction::triggered, this, revertButtomUpdate);

    const auto leafStringUpdate = [this]() -> void
        {
            bool hasSelectedOptions = !_linkerSettingsHolder.getSelectedLeafValues().getSelectedOptions().isEmpty() && !_linkerSettingsHolder.getTreeLeafSelectionValueQT().getString().isEmpty();
            bool shouldDisableRevertButton = true;
            if (hasSelectedOptions) {
                QString currentSelection = _linkerSettingsHolder.getTreeLeafSelectionValueQT().getString();
                if (!currentSelection.isEmpty()) {
                    QStringList temp = currentSelection.split(" @%$,$%@ ");
                    QStringList species = _linkerSettingsHolder.getSelectedLeafValues().getSelectedOptions();

                    std::sort(temp.begin(), temp.end());
                    std::sort(species.begin(), species.end());

                    shouldDisableRevertButton = (temp == species);
                }
            }

            _updateSettingsHolder.getRevertButtonAction().setDisabled(shouldDisableRevertButton);


        };
    connect(&_linkerSettingsHolder.getTreeLeafSelectionValueQT(), &StringAction::changed, this, leafStringUpdate);


    const auto viewsButtomUpdate = [this]() -> void
        {
            bool hasSelectedOptions = !_linkerSettingsHolder.getSelectedLeafValues().getSelectedOptions().isEmpty() && !_linkerSettingsHolder.getTreeLeafSelectionValueQT().getString().isEmpty();
            bool shouldDisableRevertButton = true;
            if (hasSelectedOptions) {
                QString currentSelection = _linkerSettingsHolder.getTreeLeafSelectionValueQT().getString();
                if (!currentSelection.isEmpty()) {
                    QStringList temp = currentSelection.split(" @%$,$%@ ");
                    QStringList species = _linkerSettingsHolder.getSelectedLeafValues().getSelectedOptions();

                    std::sort(temp.begin(), temp.end());
                    std::sort(species.begin(), species.end());

                    shouldDisableRevertButton = (temp == species);
                }
            }

            _updateSettingsHolder.getRevertButtonAction().setDisabled(shouldDisableRevertButton);


        };
    connect(&_updateSettingsHolder.getUpdateViewsButtonAction(), &TriggerAction::triggered, this, viewsButtomUpdate);
    const auto referenceTreeSelection = [this]() -> void
        {
            if (_mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentDataset().isValid())
            {
                auto temp = mv::data().getDataset<CrossSpeciesComparisonTree>(_mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentDataset()->getId());
                if (temp.isValid())
                {
                    _viewerPlugin.getReferenceTreeDataset().setDataset(temp.getDataset());
                }


            }


        };
    connect(&_mainSettingsHolder.getMainReferenceTreeSelectionAction(), &DatasetPickerAction::currentIndexChanged, this, referenceTreeSelection);


    const auto traitSelectionColor = [this]() -> void
        {

            if (_metaDataSettingsHolder.getTraitDatasetSelectionAction().getCurrentText() != "" && _metaDataSettingsHolder.getTraitDatasetSelectionAction().getCurrentDataset().isValid() && _mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentText() != "" && _mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentDataset().isValid())
            {
                if (_metaDataSettingsHolder.getColorTraitAction().getCurrentText() != "")
                {
                    colorTraitCalculation();
                }
            }

        };
    
    const auto traitSelectionNumeric = [this]() -> void
        {

            if (_metaDataSettingsHolder.getTraitDatasetSelectionAction().getCurrentText() != "" && _metaDataSettingsHolder.getTraitDatasetSelectionAction().getCurrentDataset().isValid() && _mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentText() != "" && _mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentDataset().isValid())
            {

                if (_metaDataSettingsHolder.getNumericTraitAction().getCurrentText() != "")
                {
                    numericTraitCalculation();
                }

            }

        };
    const auto traitSelectionString = [this]() -> void
        {

            if (_metaDataSettingsHolder.getTraitDatasetSelectionAction().getCurrentText() != "" && _metaDataSettingsHolder.getTraitDatasetSelectionAction().getCurrentDataset().isValid() && _mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentText() != "" &&  _mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentDataset().isValid())
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

    const auto traitDatasetSelection = [this]() -> void
        {
            traitDatasetModify();

        };
    connect(&_metaDataSettingsHolder.getTraitDatasetSelectionAction(), &DatasetPickerAction::currentIndexChanged, this, traitDatasetSelection);

        const auto showTraitValues = [this]() -> void
        {
                disableTraitOptions();

        };
    connect(&_metaDataSettingsHolder.getDisableTraitOptions(), &ToggleAction::toggled, this, showTraitValues);




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


}



void ChartOptions::updateChartDataJS()
{

    std::string resultString = "";
    if (_mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentDataset().isValid()  && _mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentText() != "")
    {


        std::string valueStringReference = extractFormatData(_mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentDataset().getDatasetId());

        if (valueStringReference != "" )
        {
            std::string resultString = valueStringReference;

            _showDataDrop.setChecked(false);

            _viewerPlugin.getChartWidget().setData(QString::fromStdString(resultString));
        }

    }


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

std::string ChartOptions::extractFormatData(QString datasetValue)
{

    Dataset<CrossSpeciesComparisonTree> dataset = mv::data().getDataset<CrossSpeciesComparisonTree>(datasetValue);
    auto temp = dataset->getTreeData();
    std::string valueStringReference = QJsonDocument(temp).toJson().toStdString();

    return valueStringReference;

}

QStringList ChartOptions::extractTraitPropertyOptionValues(QString propertyType, QString propertyString) {
    QStringList lines = propertyString.split("\n");
    for (QString line : lines) {
        if (line.startsWith(propertyType)) {
            line = line.remove(0, propertyType.length() + 1); 
            return line.split(", ");
        }
    }
    return QStringList(); 
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

            _viewerPlugin.getChartWidget().setTraitNumeric(metaOptions);
        }

    }

}
void ChartOptions::disableTraitOptions()
{
    if (_metaDataSettingsHolder.getDisableTraitOptions().isChecked())
    {
        _viewerPlugin.getChartWidget().setDisableTraitOptions("True");
        
    }
    else
    {
       _viewerPlugin.getChartWidget().setDisableTraitOptions("False");
    }


}

void ChartOptions::traitDatasetModify()
{
    if (_metaDataSettingsHolder.getTraitDatasetSelectionAction().getCurrentDataset().isValid() && _metaDataSettingsHolder.getTraitDatasetSelectionAction().getCurrentText() != "")
    {
        Dataset<CrossSpeciesComparisonTreeMeta> metaDataDataset = mv::data().getDataset<CrossSpeciesComparisonTreeMeta>(_metaDataSettingsHolder.getTraitDatasetSelectionAction().getCurrentDataset()->getId());

        QStringList metaDataLeafNames = metaDataDataset->getTreeMetaLeafNames();
        if (metaDataLeafNames.size() > 0)
        {

    if ( _mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentText() != "" &&  _mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentDataset().isValid())
    {
        QStringList referenceTreeLeafNames = mv::data().getDataset<CrossSpeciesComparisonTree>(_mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentDataset()->getId())->getTreeLeafNames();
       
        if (areStringListsEqual(referenceTreeLeafNames, metaDataLeafNames))
        {
            _viewerPlugin.getMetaInfoDataset().setDataset(metaDataDataset.getDataset());
            std::vector<QString> compareDimensions;

            {


                if (metaDataDataset.isValid())
                {
                    auto propertyString = metaDataDataset->getTreeMetaPropertyNames();
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
        else
        {
            qDebug()<<"The leaf names of the reference tree and the meta data tree do not match";
        }
    }
    else
    {
        qDebug()<<"Tree Dataset not valid";
    }
    }
    }
    else
    {
        qDebug()<<"Meta Data Dataset not valid";
        }
}

void ChartOptions::onDataEventTree(mv::DatasetEvent* dataEvent)
{


    changeLoader();
}

void ChartOptions::onDataEventTreeMeta(mv::DatasetEvent* dataEvent)
{
    changeLoader();

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
        if (_metaDataSettingsHolder.getTraitDatasetSelectionAction().getCurrentText() != "" && _metaDataSettingsHolder.getTraitDatasetSelectionAction().getCurrentDataset().isValid() && _mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentText() != "" &&  _mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentDataset().isValid())
        {
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
        disableTraitOptions();
    }




}
void ChartOptions::triggerChart()
{
    if ( _mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentText() != "")
    {
        _mainSettingsHolder.getMainReferenceTreeSelectionAction().setEnabled(true);
        _extraSettingsHolder.setEnabled(true);
        _metaDataSettingsHolder.setEnabled(true);
        _linkerSettingsHolder.setEnabled(true);

        updateChartDataJS();
    }
    else
    {
        _mainSettingsHolder.getMainReferenceTreeSelectionAction().setEnabled(false);
        _extraSettingsHolder.setEnabled(false);
        _metaDataSettingsHolder.setEnabled(false);
        _linkerSettingsHolder.setEnabled(false);

    }
}
void ChartOptions::changeLoader()
{


    if (_mainSettingsHolder.getMainReferenceTreeSelectionAction().getCurrentText() != "")
    {
        _mainSettingsHolder.getMainReferenceTreeSelectionAction().setEnabled(true);
        _extraSettingsHolder.setEnabled(true);
        _metaDataSettingsHolder.setEnabled(true);
        _linkerSettingsHolder.setEnabled(true);

        updateChartDataJS();
    }





}


inline ChartOptions::ExtraSettingsHolder::ExtraSettingsHolder(ChartOptions& chartOptions) :
    VerticalGroupAction(&chartOptions, "Extra options"),
    _chartOptions(chartOptions),
    _treeColorMapAction(this, "Color map"),
    _expandAllAction(this, "Expand all"),
    _disableAcceptDatasetDrops(this, "Disable accept dataset drops")

{
    setText("Extra Options");
    setIcon(Application::getIconFont("FontAwesome").getIcon("toolbox"));
    setPopupSizeHint(QSize(350, 0));
    setConfigurationFlag(WidgetAction::ConfigurationFlag::ForceCollapsedInGroup);
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
    _stringTrait(this, "String type trait"),
    _disableTraitOptions(this, "Disable Trait Options")

{
    setText("Metadata Options");
    setIcon(Application::getIconFont("FontAwesome").getIcon("highlighter"));
    setPopupSizeHint(QSize(350, 0));
    setConfigurationFlag(WidgetAction::ConfigurationFlag::ForceCollapsedInGroup);
    addAction(&_traitDatasetSelectionAction);
    addAction(&_colorTrait);
    addAction(&_numericTrait);
    addAction(&_stringTrait);
    addAction(&_disableTraitOptions);
}


inline ChartOptions::LinkerSettingsHolder::LinkerSettingsHolder(ChartOptions& chartOptions) :
    VerticalGroupAction(&chartOptions, "Meta data options"),
    _chartOptions(chartOptions),
    _treeLeafSelectionValue(this, "Tree Leaf Selection Value"),
    _selectedLeafValues(this, "Selected Leaf Values"),
    _leafDatasetPicker(this, "Leaf Dataset Picker")

{
    setText("Linker Options");
    setIcon(Application::getIconFont("FontAwesome").getIcon("link"));
    setPopupSizeHint(QSize(350, 0));
    setConfigurationFlag(WidgetAction::ConfigurationFlag::ForceCollapsedInGroup);
    addAction(&_treeLeafSelectionValue);
    addAction(&_selectedLeafValues);
    addAction(&_leafDatasetPicker);
}

inline ChartOptions::MainSettingsHolder::MainSettingsHolder(ChartOptions& chartOptions) :
    VerticalGroupAction(&chartOptions, "Chart Options"),
    _chartOptions(chartOptions),
    _mainReferenceTreeSelectionAction(this, "Reference tree")





{
    setText("Chart Options");
    setIcon(Application::getIconFont("FontAwesome").getIcon("atom"));
    setPopupSizeHint(QSize(350, 0));
    setConfigurationFlag(WidgetAction::ConfigurationFlag::ForceCollapsedInGroup);
    setPopupSizeHint(QSize(350, 0));
    addAction(&_mainReferenceTreeSelectionAction);






}


inline ChartOptions::UpdateSettingsHolder::UpdateSettingsHolder(ChartOptions& chartOptions) :
    VerticalGroupAction(&chartOptions, "Update Options"),
    _chartOptions(chartOptions),

    _updateViewsButton(this, "Explore"),
    _revertButton(this, "Revert")


{
    setText("Update Options");
    setIcon(Application::getIconFont("FontAwesome").getIcon("sync"));
    setPopupSizeHint(QSize(350, 0));
    addAction(&_updateViewsButton);
    addAction(&_revertButton);

}

void ChartOptions::fromVariantMap(const QVariantMap& variantMap)
{
    WidgetAction::fromVariantMap(variantMap);

    _extraSettingsHolder.getColorMapAction().fromParentVariantMap(variantMap);
    _mainSettingsHolder.getMainReferenceTreeSelectionAction().fromParentVariantMap(variantMap);
    _linkerSettingsHolder.getLeafDatasetPicker().fromParentVariantMap(variantMap);
    _linkerSettingsHolder.getSelectedLeafValues().fromParentVariantMap(variantMap);
    _metaDataSettingsHolder.getTraitDatasetSelectionAction().fromParentVariantMap(variantMap);
    _metaDataSettingsHolder.getColorTraitAction().fromParentVariantMap(variantMap);
    _metaDataSettingsHolder.getNumericTraitAction().fromParentVariantMap(variantMap);
    _metaDataSettingsHolder.getStringTraitAction().fromParentVariantMap(variantMap);
    _metaDataSettingsHolder.getDisableTraitOptions().fromParentVariantMap(variantMap);
    _extraSettingsHolder.getExpandAllAction().fromParentVariantMap(variantMap);
    _extraSettingsHolder.getDisableAcceptDatasetDrops().fromParentVariantMap(variantMap);
    _updateSettingsHolder.getUpdateViewsButtonAction().fromParentVariantMap(variantMap);
    _linkerSettingsHolder.getTreeLeafSelectionValueQT().fromParentVariantMap(variantMap);

}

QVariantMap ChartOptions::toVariantMap() const
{
    QVariantMap variantMap = WidgetAction::toVariantMap();
    _extraSettingsHolder.getColorMapAction().insertIntoVariantMap(variantMap);
    _mainSettingsHolder.getMainReferenceTreeSelectionAction().insertIntoVariantMap(variantMap);
    _linkerSettingsHolder.getLeafDatasetPicker().insertIntoVariantMap(variantMap);
    _linkerSettingsHolder.getSelectedLeafValues().insertIntoVariantMap(variantMap);
    _metaDataSettingsHolder.getTraitDatasetSelectionAction().insertIntoVariantMap(variantMap);
    _metaDataSettingsHolder.getColorTraitAction().insertIntoVariantMap(variantMap);
    _metaDataSettingsHolder.getNumericTraitAction().insertIntoVariantMap(variantMap);
    _metaDataSettingsHolder.getStringTraitAction().insertIntoVariantMap(variantMap);
    _metaDataSettingsHolder.getDisableTraitOptions().insertIntoVariantMap(variantMap);
    _extraSettingsHolder.getExpandAllAction().insertIntoVariantMap(variantMap);
    _extraSettingsHolder.getDisableAcceptDatasetDrops().insertIntoVariantMap(variantMap);
    _updateSettingsHolder.getUpdateViewsButtonAction().insertIntoVariantMap(variantMap);
    _linkerSettingsHolder.getTreeLeafSelectionValueQT().insertIntoVariantMap(variantMap);
    return variantMap;
}