#pragma once

#include <ViewPlugin.h>
#include <Dataset.h>
#include <CrossSpeciesComparisonTreeData.h>
#include <CrossSpeciesComparisonTreeMetaData.h>
#include "ChartWidget.h"
#include "ChartOptions.h"
#include <QWidget>
#include <widgets/DropWidget.h>
#include <actions/HorizontalToolbarAction.h>

/** All plugin related classes are in the HDPS plugin namespace */
using namespace mv::plugin;

/** Drop widget used in this plugin is located in the HDPS gui namespace */
using namespace mv::gui;

/** Dataset reference used in this plugin is located in the HDPS util namespace */
using namespace mv::util;



class CrossSpeciesComparisonPhyTVuPlugin : public ViewPlugin
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param factory Pointer to the plugin factory
     */
    CrossSpeciesComparisonPhyTVuPlugin(const PluginFactory* factory);

    /** Destructor */
    ~CrossSpeciesComparisonPhyTVuPlugin() override = default;

    /** This function is called by the core after the view plugin has been created */
    void init() override;
    void onDataEventTreeMeta(mv::DatasetEvent* dataEvent);
    void onDataEventTree(mv::DatasetEvent* dataEvent);
    /** Store a private reference to the data set that should be displayed */
    void loadData(const Dataset<CrossSpeciesComparisonTree>& dataset);

public: // Serialization

    /**
     * Load widget action from variant map
     * @param Variant map representation of the widget action
     */
    void fromVariantMap(const QVariantMap& variantMap) override;

    /**
     * Save widget action to variant map
     * @return Variant map representation of the widget action
     */
    QVariantMap toVariantMap() const override;

public slots:
    /** Converts ManiVault's point data to a json-like data structure that Qt can pass to the JS code */
    ChartWidget& getChartWidget() { return _chartWidget; }
    ChartOptions& getChartOptions() { return _chartOptions; }

    mv::Dataset<CrossSpeciesComparisonTree>& getReferenceTreeDataset() { return _referenceTree; }
    mv::Dataset<CrossSpeciesComparisonTreeMeta>& getMetaInfoDataset() { return _metaInfo; }

    bool getInitialLoadCompleteflag() { return _initialLoadComplete; }
    void setInitialLoadCompleteflag(int initialLoadComplete) { _initialLoadComplete = initialLoadComplete; }

    bool vectorsHaveSameOrder(const std::vector<QString>& vec1, const std::vector<QString>& vec2);
    void triggerChartJS();
    void triggerInitialChart();
    void ShowDataDropOptions();
private:
    /** Published selections received from the JS side to ManiVault's core */
    static void publishSelection(const std::vector<unsigned int>& selectedIDs);
    void removeSelectionFromScatterplot(std::string clusterName);
    void addSelectionToScatterplot(std::string clusterName);
    void addLeftRightSelectionToScatterplot(std::string clusterName);
    void passScatterplotLeafPointSelection(std::string selectedSpecies);
    void alterColorMirrorForRank(std::string selectedSpecies);
    void alterColorMap(std::string selectedSpecies);

private:
    ChartWidget            _chartWidget;       // WebWidget that sets up the HTML page
    //mv::Dataset<Points>   _currentDataSet;    // Reference to currently shown data set
    ChartOptions            _chartOptions;
    HorizontalToolbarAction     _toolbarAction;     // Toolbar action that is shown in the main window
    DropWidget* _dropWidget;                /** Widget for drag and drop behavior */
    QLabel* _comparisonDatasetNameLabel;   /** Label that show the current dataset name */
    QLabel* _referenceDatasetNameLabel;
    QLabel* _metaInfoDatasetNameLabel;
    mv::Dataset<CrossSpeciesComparisonTree>   _referenceTree;
    mv::Dataset<CrossSpeciesComparisonTreeMeta>   _metaInfo;
    bool _initialLoadComplete = false;
    QString _originalName;

};

/**
 * CrossSpeciesComparisonPhyTVu view plugin factory class
 *
 * Note: Factory does not need to be altered (merely responsible for generating new plugins when requested)
 */
class CrossSpeciesComparisonPhyTVuPluginFactory : public ViewPluginFactory
{
    Q_INTERFACES(mv::plugin::ViewPluginFactory mv::plugin::PluginFactory)
        Q_OBJECT
        Q_PLUGIN_METADATA(IID   "nl.BioVault.CrossSpeciesComparisonPhyTVuPlugin"
            FILE  "CrossSpeciesComparisonPhyTVuPlugin.json")

public:

    /** Default constructor */
    CrossSpeciesComparisonPhyTVuPluginFactory() {}

    /** Destructor */
    ~CrossSpeciesComparisonPhyTVuPluginFactory() override {}

    /** Get plugin icon */
    QIcon getIcon(const QColor& color = Qt::black) const override;

    /** Creates an instance of the CrossSpeciesComparisonPhyTVu view plugin */
    ViewPlugin* produce() override;

    /** Returns the data types that are supported by the CrossSpeciesComparisonPhyTVu view plugin */
    mv::DataTypes supportedDataTypes() const override;

    /**
     * Get plugin trigger actions given \p datasets
     * @param datasets Vector of input datasets
     * @return Vector of plugin trigger actions
     */
    PluginTriggerActions getPluginTriggerActions(const mv::Datasets& datasets) const override;
};
