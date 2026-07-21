#include "XSCPhyTVuPlugin.h"
#include "ChartWidget.h"
#include <vector>
#include <random>
#include <QString>
#include <QStringList>
#include <QMimeData>
#include <DatasetsMimeData.h>

#include <util/Serialization.h>

Q_PLUGIN_METADATA(IID "nl.BioVault.XSCPhyTVuPlugin")

using namespace mv;

XSCPhyTVuPlugin::XSCPhyTVuPlugin(const PluginFactory* factory) :
    ViewPlugin(factory),
    _chartWidget(),
    _chartOptions(*this, _core),
    _toolbarAction(this, "Toolbar")
    , _originalName(getGuiName())
{
    setSerializationName("CSCPTV:XSCPhyTVu");
    _toolbarAction.addAction(&_chartOptions.getUpdateSettingsHolder(), 5, GroupAction::Horizontal);
    _toolbarAction.addAction(&_chartOptions.getMetaDataSettingsHolder(), 2, GroupAction::Horizontal);
    _toolbarAction.addAction(&_chartOptions.getExtraSettingsHolder(), 3, GroupAction::Horizontal);
    //_toolbarAction.addAction(&_chartOptions.getMainSettingsHolder(), 1, GroupAction::Horizontal);
    //_toolbarAction.addAction(&_chartOptions.getLinkerSettingsHolder(), 4, GroupAction::Horizontal);


    
}

void XSCPhyTVuPlugin::init()
{
    auto& shortcuts = getShortcuts();

    shortcuts.add({ QKeySequence(Qt::SHIFT), "Leaf Multi-Selection", "Mouse left click on tree nodes" });
    shortcuts.add({ QKeySequence(Qt::Key_Escape), "Deselect", "Remove all leaf selection" });
    shortcuts.add({ QKeySequence(Qt::MouseButton::LeftButton), "Leaf Single-Selection", "Mouse left click on tree nodes"});
    shortcuts.add({ QKeySequence(Qt::MouseButton::LeftButton), "Toggle Attribute", "Mouse left click on colormap to toggle attributes" });
    shortcuts.add({ QKeySequence(Qt::MouseButton::LeftButton), "Node Info", "Hover node to see children names" });
    shortcuts.add({ QKeySequence(Qt::MouseButton::LeftButton), "Leaf Info", "Hover leaf to see leaf details" });
    shortcuts.add({ QKeySequence(Qt::MouseButton::LeftButton), "Explore", "Explore multiple selected species" });
    shortcuts.add({ QKeySequence(Qt::MouseButton::LeftButton), "Revert", "Revert to initial selected species" });
    //getLearningCenterAction().setPluginTitle("Phylogenetic Tree View");

    connect(&_chartWidget, &ChartWidget::widgetInitialized, this, &XSCPhyTVuPlugin::triggerInitialChart);

    getWidget().setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    _chartWidget.setPage(":XSCPhyTVu/XSCPhyTVu_chart/XSCPhyTVu_chart.html", "qrc:/XSCPhyTVu/XSCPhyTVu_chart/");
    _chartWidget.setContentsMargins(0, 0, 0, 0);
    _chartWidget.layout()->setContentsMargins(0, 0, 0, 0);

    const auto triggerUpdateButtonForChartSelection = [this]() -> void
        {


        };
    connect(&_chartOptions.getUpdateSettingsHolder().getUpdateViewsButtonAction(), &TriggerAction::triggered, this, triggerUpdateButtonForChartSelection);


    const auto disableAcceptDrops = [this]() -> void
        {
            if (_chartOptions.getExtraSettingsHolder().getDisableAcceptDatasetDrops().isChecked())
            {
                _chartWidget.setAcceptDrops(false);
                _dropWidget->setShowDropIndicator(false);
            }
            else
            {
                _chartWidget.setAcceptDrops(true);
               if(!_referenceTree.isValid())
                _dropWidget->setShowDropIndicator(true);
            }
        };
    connect(&_chartOptions.getExtraSettingsHolder().getDisableAcceptDatasetDrops(), &ToggleAction::toggled, this, disableAcceptDrops);

    _chartOptions.getShowDataDrop().setChecked(true);
    connect(&_chartOptions.getShowDataDrop(), &QCheckBox::stateChanged, this, &XSCPhyTVuPlugin::ShowDataDropOptions);

    connect(&_chartWidget, &ChartWidget::removeSelectionFromScatterplot, this, &XSCPhyTVuPlugin::removeSelectionFromScatterplot);

    connect(&_chartWidget, &ChartWidget::addSelectionToScatterplot, this, &XSCPhyTVuPlugin::addSelectionToScatterplot);
    connect(&_chartWidget, &ChartWidget::addLeftRightSelectionToScatterplot, this, &XSCPhyTVuPlugin::addLeftRightSelectionToScatterplot);

    connect(&_chartWidget, &ChartWidget::passScatterplotLeafPointSelectionToQt, this, &XSCPhyTVuPlugin::passScatterplotLeafPointSelection);

    connect(&_chartWidget, &ChartWidget::alterColorMirrorForRankToQt, this, &XSCPhyTVuPlugin::alterColorMirrorForRank);

    connect(&_chartWidget, &ChartWidget::alterColorMapToQt, this, &XSCPhyTVuPlugin::alterColorMap);
    
    const auto updateTreeLeafSelectionFromQT = [this]() -> void
        {

            QString selectionString = _chartOptions.getLinkerSettingsHolder().getTreeLeafSelectionValueQT().getString();
            _chartWidget.setLeafSelectionFromQT(selectionString);
        };

    connect(&_chartOptions.getLinkerSettingsHolder().getTreeLeafSelectionValueQT(), &StringAction::stringChanged, this, updateTreeLeafSelectionFromQT);


    _chartWidget.setAcceptDrops(true);
    const auto layout = new QVBoxLayout();

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(_toolbarAction.createWidget(&getWidget()));
    layout->addWidget(&_chartWidget, 1);

    getWidget().setLayout(layout);

    // Instantiate new drop widget
    _dropWidget = new DropWidget(&_chartWidget);
    // Set the drop indicator widget (the widget that indicates that the view is eligible for data dropping)
    _dropWidget->setDropIndicatorWidget(new DropWidget::DropIndicatorWidget(&getWidget(), "No data loaded", "Drag a tree dataset from the data hierarchy and drop it here to compare and visualize the data..."));

    _dropWidget->initialize([this](const QMimeData* mimeData) -> DropWidget::DropRegions {
        // A drop widget can contain zero or more drop regions
        DropWidget::DropRegions dropRegions;

        const auto datasetsMimeData = dynamic_cast<const DatasetsMimeData*>(mimeData);

        if (datasetsMimeData == nullptr || datasetsMimeData->getDatasets().count() < 1)
        {
            dropRegions << new DropWidget::DropRegion(this, "Null data error", "This type of data is not supported", "exclamation-circle", false);
            return dropRegions;
        }

        if (datasetsMimeData->getDatasets().count() > 1)
        {
            dropRegions << new DropWidget::DropRegion(this, "Too many datasets error", "Select one tree dataset", "exclamation-circle", false);
            return dropRegions;

        }

        // Gather information to generate appropriate drop regions
        const auto dataset = datasetsMimeData->getDatasets().first();
        const auto datasetGuiName = dataset->getGuiName();
        const auto datasetId = dataset->getId();
        const auto dataType = dataset->getDataType();
        const auto dataTypes = DataTypes({ XSCTreeType ,XSCTreeMetaType });

        if (!dataTypes.contains(dataType))
        {
            dropRegions << new DropWidget::DropRegion(this, "Incompatible data error", "This type of data is not supported", "exclamation-circle", false);
            return dropRegions;
        }
        Dataset<XSCTreeMeta> candidateMetadataDataset;
        Dataset<XSCTree> candidateTreeDataset;
        QStringList leaves;
        if (dataType== XSCTreeType)
        {
            candidateTreeDataset = mv::data().getDataset<XSCTree>(datasetId);
            leaves = candidateTreeDataset->getTreeLeafNames();

            {
                if (!_referenceTree.isValid())
                {
                    if (candidateTreeDataset.isValid())
                    {

                        dropRegions << new DropWidget::DropRegion(this, "Reference XSCTree", QString("Load %1 into cross-species comparison view").arg(datasetGuiName), "project-diagram", true, [this, candidateTreeDataset]() {
                            _referenceTree = candidateTreeDataset;

                            if ( _referenceTree.isValid())
                            {
                                triggerChartJS();
                            }

                            });


                    }

                }


                else
                {
                    if (_referenceTree == candidateTreeDataset)
                    {
                        // Dataset cannot be dropped because it is already loaded
                        dropRegions << new DropWidget::DropRegion(this, "Reference XSCTree Warning", "Data already loaded", "exclamation-circle", false);
                    }
                    else
                    {
                        dropRegions << new DropWidget::DropRegion(this, "Reference XSCTree", QString("Compare %1 tree ").arg(datasetGuiName), "project-diagram", true, [this, candidateTreeDataset]() {
                            _referenceTree = candidateTreeDataset;

                            if ( _referenceTree.isValid())
                            {
                                triggerChartJS();
                            }
                            });
                    }


                }

            }
        }
        else if (dataType== XSCTreeMetaType)
        {
            candidateMetadataDataset = mv::data().getDataset<XSCTreeMeta>(datasetId);

            leaves = candidateMetadataDataset->getTreeMetaLeafNames();
            auto comparisonDatasetDimensions = _referenceTree->getTreeLeafNames();
            if (_referenceTree.isValid())
            {
                /* TODO: MEtadata */
                if (leaves.size() > 0 && comparisonDatasetDimensions.size() > 0)
                {
                    leaves.sort();
                    comparisonDatasetDimensions.sort();
                    // Check if the lists are equal
                    if (leaves == comparisonDatasetDimensions)
                    {
              


                            if (_metaInfo == candidateMetadataDataset)
                            {
                                // Dataset cannot be dropped because it is already loaded
                                dropRegions << new DropWidget::DropRegion(this, "Meta data Warning", "Data already loaded", "exclamation-circle", false);
                            }
                            else
                            {
                                // The number of points is equal, so offer the option to use the points dataset as source for points opacity
                                dropRegions << new DropWidget::DropRegion(this, "Meta data", QString("Set %1 metadata ").arg(datasetGuiName), "table", true, [this, candidateMetadataDataset]() {
                                    _metaInfo = candidateMetadataDataset;
                                    });
                            }

                    }
                    else
                    {
                        dropRegions << new DropWidget::DropRegion(this, "Meta data error", "Data does not have the same leaves as the comparison tree. Please try with a different dataset or change the comparison tree dataset", "exclamation-circle", false);
                    }

                }
                else
                {
                    dropRegions << new DropWidget::DropRegion(this, "Meta data error", "Please try with a different dataset or change the comparison tree dataset", "exclamation-circle", false);
                }



            }

        }
        else {
            dropRegions << new DropWidget::DropRegion(this, "Incompatible data error", "This type of data is not supported", "exclamation-circle", false);
            return dropRegions;
        }





        return dropRegions;
        });



    connect(&_metaInfo, &Dataset<XSCTreeMeta>::guiNameChanged, this, [this]() {

        auto newDatasetName = _metaInfo->getGuiName();

        // Update the current dataset name label
        _metaInfoDatasetNameLabel->setText(QString("Current points dataset: %1").arg(newDatasetName));

        });

    connect(&_referenceTree, &Dataset<XSCTreeMeta>::guiNameChanged, this, [this]() {

        auto newDatasetName = _referenceTree->getGuiName();

        _referenceDatasetNameLabel->setText(QString("Current points dataset: %1").arg(newDatasetName));

        });


    _eventListener.registerDataEventByType(XSCTreeType, std::bind(&XSCPhyTVuPlugin::onDataEventTree, this, std::placeholders::_1));
    _eventListener.registerDataEventByType(XSCTreeMetaType, std::bind(&XSCPhyTVuPlugin::onDataEventTreeMeta, this, std::placeholders::_1));
}
void XSCPhyTVuPlugin::triggerInitialChart()
{
    if(_chartOptions.getMainSettingsHolder().getMainReferenceTreeSelectionAction().getCurrentText() != "")
    {

        Dataset<XSCTree> tempRefTree;

        if (_chartOptions.getMainSettingsHolder().getMainReferenceTreeSelectionAction().getCurrentText() != "" && _chartOptions.getMainSettingsHolder().getMainReferenceTreeSelectionAction().getCurrentDataset().isValid())
        {
            _chartOptions.getMainSettingsHolder().getMainReferenceTreeSelectionAction().setCurrentText("");
            tempRefTree = _chartOptions.getMainSettingsHolder().getMainReferenceTreeSelectionAction().getCurrentDataset();
            _chartOptions.getMainSettingsHolder().getMainReferenceTreeSelectionAction().setCurrentDataset(_referenceTree);
        }

        if (!_initialLoadComplete)
        {
            _chartOptions.initLoader();
        }

    }

}
void XSCPhyTVuPlugin::triggerChartJS()
{
    _dropWidget->setShowDropIndicator(false);

    if (_chartOptions.getMainSettingsHolder().getMainReferenceTreeSelectionAction().getCurrentDataset()!= _referenceTree)
    {
        _chartOptions.getMainSettingsHolder().getMainReferenceTreeSelectionAction().setCurrentDataset(_referenceTree);
    }

   

    if (_chartOptions.getMainSettingsHolder().getMainReferenceTreeSelectionAction().getCurrentText() != "")
    {
        if (!_initialLoadComplete)
        {
            _chartOptions.initLoader();
        }
        else
        {
            _chartOptions.triggerChart();
        }
    }

    //_chartOptions.updateChartDataJS();
}

bool XSCPhyTVuPlugin::vectorsHaveSameOrder(const std::vector<QString>& vec1, const std::vector<QString>& vec2) {
    // Check if the vectors have the same size
    if (vec1.size() != vec2.size()) {
        return false;
    }

    // Iterate through each element and compare
    for (size_t i = 0; i < vec1.size(); ++i) {
        if (vec1[i] != vec2[i]) {
            return false; // Mismatch found
        }
    }

    // If no mismatch is found, vectors have same order
    return true;
}

void XSCPhyTVuPlugin::ShowDataDropOptions()
{
    if (_chartOptions.getShowDataDrop().isChecked())
    {
        _dropWidget->setShowDropIndicator(true);
    }
    else
    {
        _dropWidget->setShowDropIndicator(false);
    }
}

void XSCPhyTVuPlugin::loadData(const Dataset<XSCTree>& dataset)
{
    // Exit if there is nothing to load
    if (!dataset.isValid())
        return;




    if(_referenceTree != dataset)
    {
        _referenceTree = dataset;
    }

    if( _referenceTree.isValid() )
    {
        triggerChartJS();
    }
}


void XSCPhyTVuPlugin::onDataEventTreeMeta(mv::DatasetEvent* dataEvent)
{
    // Get smart pointer to dataset that changed
    const auto changedDataSet = dataEvent->getDataset();

    // Get GUI name of the dataset that changed
    const auto datasetGuiName = changedDataSet->getGuiName();

    // The data event has a type so that we know what type of data event occurred (e.g. data added, changed, removed, renamed, selection changes)
    switch (dataEvent->getType()) {

        // A points dataset was added
    case EventType::DatasetAdded:
    {
        // Cast the data event to a data added event
        const auto dataAddedEvent = static_cast<DatasetAddedEvent*>(dataEvent);

        // Get the GUI name of the added points dataset and print to the console
        //qDebug() << datasetGuiName << "was added";

        break;
    }

    // Points dataset data has changed
    case EventType::DatasetDataChanged:
    {
        // Cast the data event to a data changed event
        const auto dataChangedEvent = static_cast<DatasetDataChangedEvent*>(dataEvent);

        // Get the name of the points dataset of which the data changed and print to the console
        //qDebug() << datasetGuiName << "data changed";

        break;
    }

    // Points dataset data was removed
    case EventType::DatasetRemoved:
    {
        // Cast the data event to a data removed event
        const auto dataRemovedEvent = static_cast<DatasetRemovedEvent*>(dataEvent);

        // Get the name of the removed points dataset and print to the console
        //qDebug() << datasetGuiName << "was removed";

        break;
    }

    // Points dataset selection has changed
    case EventType::DatasetDataSelectionChanged:
    {
        // Cast the data event to a data selection changed event
        const auto dataSelectionChangedEvent = static_cast<DatasetDataSelectionChangedEvent*>(dataEvent);

        // Print to the console
        //qDebug() << datasetGuiName << "selection has changed";

        break;
    }

    default:
        break;
    }
}
void XSCPhyTVuPlugin::onDataEventTree(mv::DatasetEvent* dataEvent)
{
    // Get smart pointer to dataset that changed
    const auto changedDataSet = dataEvent->getDataset();

    // Get GUI name of the dataset that changed
    const auto datasetGuiName = changedDataSet->getGuiName();

    // The data event has a type so that we know what type of data event occurred (e.g. data added, changed, removed, renamed, selection changes)
    switch (dataEvent->getType()) {

        // A points dataset was added
    case EventType::DatasetAdded:
    {
        // Cast the data event to a data added event
        const auto dataAddedEvent = static_cast<DatasetAddedEvent*>(dataEvent);

        // Get the GUI name of the added points dataset and print to the console
        //qDebug() << datasetGuiName << "was added";

        break;
    }

    // Points dataset data has changed
    case EventType::DatasetDataChanged:
    {
        // Cast the data event to a data changed event
        const auto dataChangedEvent = static_cast<DatasetDataChangedEvent*>(dataEvent);

        // Get the name of the points dataset of which the data changed and print to the console
        //qDebug() << datasetGuiName << "data changed";

        break;
    }

    // Points dataset data was removed
    case EventType::DatasetRemoved:
    {
        // Cast the data event to a data removed event
        const auto dataRemovedEvent = static_cast<DatasetRemovedEvent*>(dataEvent);

        // Get the name of the removed points dataset and print to the console
        //qDebug() << datasetGuiName << "was removed";

        break;
    }


    default:
        break;
    }
}




void XSCPhyTVuPlugin::publishSelection(const std::vector<unsigned int>& selectedIDs)
{

}


// =============================================================================
// Plugin Factory 
// =============================================================================

XSCPhyTVuPluginFactory::XSCPhyTVuPluginFactory()
{
    setIconByName("project-diagram");
}

ViewPlugin* XSCPhyTVuPluginFactory::produce()
{
    return new XSCPhyTVuPlugin(this);
}

mv::DataTypes XSCPhyTVuPluginFactory::supportedDataTypes() const
{
    // This example analysis plugin is compatible with points datasets
    DataTypes supportedTypes;
    supportedTypes.append(XSCTreeType);
    return supportedTypes;
}

mv::gui::PluginTriggerActions XSCPhyTVuPluginFactory::getPluginTriggerActions(const mv::Datasets& datasets) const
{
    PluginTriggerActions pluginTriggerActions;

    const auto getPluginInstance = [this]() -> XSCPhyTVuPlugin* {
        return dynamic_cast<XSCPhyTVuPlugin*>(plugins().requestViewPlugin(getKind()));
        };

    const auto number_of_datasets = datasets.count();

    if (number_of_datasets == 1 && PluginFactory::areAllDatasetsOfTheSameType(datasets, XSCTreeType)) {
        Dataset<XSCTree> checkDataset = datasets[0]->getFullDataset<XSCTree>();

        {
            const auto pluginTriggerAction = new PluginTriggerAction(
                const_cast<XSCPhyTVuPluginFactory*>(this),
                this,
                "XSCPhyTVu",
                "View JavaScript visualization",
                StyledIcon("project-diagram"),
                [this, getPluginInstance, checkDataset](PluginTriggerAction& pluginTriggerAction) -> void {
                    getPluginInstance()->loadData(checkDataset);
                }
            );

            pluginTriggerActions << pluginTriggerAction;
        }
    }

    return pluginTriggerActions;
}

void XSCPhyTVuPlugin::removeSelectionFromScatterplot(std::string clusterName)
{
    QStringList listofLeaves = {};

    if (_chartOptions.getLinkerSettingsHolder().getSelectedLeafValues().getNumberOfOptions() > 0)
    {
        _chartOptions.getLinkerSettingsHolder().getSelectedLeafValues().setSelectedOptions(listofLeaves);
    }

}

void XSCPhyTVuPlugin::addSelectionToScatterplot(std::string clusterName)
{
    QStringList listofLeaves = {};
    if (!clusterName.empty())
    {
        QString qClusterName = QString::fromStdString(clusterName);
        listofLeaves = qClusterName.split(" @%$,$%@ ");
    }
    if (_chartOptions.getLinkerSettingsHolder().getSelectedLeafValues().getNumberOfOptions() > 0)
    {
        _chartOptions.getLinkerSettingsHolder().getSelectedLeafValues().setSelectedOptions(listofLeaves);
    }
    
}
void XSCPhyTVuPlugin::addLeftRightSelectionToScatterplot(std::string clusterName)
{

}

void XSCPhyTVuPlugin::passScatterplotLeafPointSelection(std::string selectedSpecies)
{
}

void XSCPhyTVuPlugin::alterColorMirrorForRank(std::string selectedSpecies)
{
    auto& mirrorAction = _chartOptions.getExtraSettingsHolder().getColorMapAction().getMirrorAction(ColorMapAction::Axis::X);
    if (selectedSpecies=="T")
    {
        mirrorAction.setChecked(true);
    }
    else
    {
        mirrorAction.setChecked(false);
    }
    
}

void XSCPhyTVuPlugin::alterColorMap(std::string selectedSpecies)
{
    if (selectedSpecies!="")
    {
        _chartOptions.getExtraSettingsHolder().getColorMapAction().setColorMap(QString::fromStdString(selectedSpecies));
    }



}


void XSCPhyTVuPlugin::fromVariantMap(const QVariantMap& variantMap)
{
    ViewPlugin::fromVariantMap(variantMap);
    mv::util::variantMapMustContain(variantMap, "CSCPTV:CrossSpecies Comparison Phylo Tree View Settings");
    _chartOptions.fromVariantMap(variantMap["CSCPTV:CrossSpecies Comparison Phylo Tree View Settings"].toMap());
}

QVariantMap XSCPhyTVuPlugin::toVariantMap() const
{
    QVariantMap variantMap = ViewPlugin::toVariantMap();

    _chartOptions.insertIntoVariantMap(variantMap);
    return variantMap;
}
