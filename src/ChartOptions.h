#pragma once

#include <actions/WidgetAction.h>
#include <actions/IntegralAction.h>
#include <actions/OptionAction.h>
#include <actions/OptionsAction.h>
#include <actions/VariantAction.h>
#include "ClusterData/ClusterData.h"
#include <actions/ToggleAction.h>
#include "actions/DatasetPickerAction.h"
#include <CrossSpeciesComparisonTreeData.h>
#include <CrossSpeciesComparisonTreeMetaData.h>
#include "event/EventListener.h"
#include "actions/Actions.h"
#include "Plugin.h"
#include "DataHierarchyItem.h"
#include "Set.h"
#include <AnalysisPlugin.h>
#include <memory>
#include <algorithm>    
#include <QDebug>
#include <QLabel>
#include <QComboBox>
#include <QGroupBox>
#include <QPushButton>
#include <QGridLayout>
#include <QFormLayout>
#include <QString>
#include <string>
#include<QRadioButton>
#include <event/Event.h>
#include <QDebug>
#include <QLabel>
#include <string>
#include<actions/ColorMap1DAction.h>
#include <set>
#include <actions/HorizontalToolbarAction.h>
#include <actions/VerticalToolbarAction.h>
using namespace mv::gui;
class QMenu;
class CrossSpeciesComparisonPhyloTreeViewPlugin;
namespace mv
{
    class CoreInterface;
}


class ChartOptions : public WidgetAction
{
public:

    class ExtraSettingsHolder : public VerticalGroupAction
    {
    public:
        ExtraSettingsHolder(ChartOptions& chartOptions);



        const ToggleAction& getShowReferenceTreeAction() const { return _showReferenceTree; }
        const ColorMap1DAction& getColorMapAction() const { return _treeColorMapAction; }
        const ToggleAction& getExpandAllAction() const { return _expandAllAction; }
        ToggleAction& getExpandAllAction() { return _expandAllAction; }
        ColorMap1DAction& getColorMapAction() { return _treeColorMapAction; }
        ToggleAction& getShowReferenceTreeAction() { return _showReferenceTree; }
        const ToggleAction& getDisableAcceptDatasetDrops() const { return _disableAcceptDatasetDrops; }
        ToggleAction& getDisableAcceptDatasetDrops() { return _disableAcceptDatasetDrops; }

    protected:
        ChartOptions& _chartOptions;

        ColorMap1DAction              _treeColorMapAction;
        ToggleAction                _expandAllAction;
        ToggleAction                _showReferenceTree;
        ToggleAction                _disableAcceptDatasetDrops;

    };

    class MetaDataSettingsHolder : public VerticalGroupAction
    {
    public:
        MetaDataSettingsHolder(ChartOptions& chartOptions);

        const DatasetPickerAction& getTraitDatasetSelectionAction() const { return _traitDatasetSelectionAction; }
        const OptionAction& getColorTraitAction() const { return _colorTrait; }
        const OptionAction& getNumericTraitAction() const { return _numericTrait; }
        const OptionAction& getStringTraitAction() const { return _stringTrait; }


        DatasetPickerAction& getTraitDatasetSelectionAction() { return _traitDatasetSelectionAction; }
        OptionAction& getColorTraitAction() { return _colorTrait; }
        OptionAction& getNumericTraitAction() { return _numericTrait; }
        OptionAction& getStringTraitAction() { return _stringTrait; }

    protected:
        ChartOptions& _chartOptions;

        DatasetPickerAction        _traitDatasetSelectionAction;
        OptionAction               _colorTrait;
        OptionAction                _numericTrait;
        OptionAction                _stringTrait;

    };

    class LinkerSettingsHolder : public VerticalGroupAction
    {
    public:
        LinkerSettingsHolder(ChartOptions& chartOptions);

        const StringAction& getScatterplotLeafSelectionValue() const { return _scatterplotLeafSelectionValue; }
        StringAction& getScatterplotLeafSelectionValue() { return _scatterplotLeafSelectionValue; }
        const ToggleAction& getReembeddingOptions() const { return _reembeddingOptions; }
        ToggleAction& getReembeddingOptions() { return _reembeddingOptions; }
        const StringAction& getTreeLeafSelectionValueQT() const { return _treeLeafSelectionValue; }
        StringAction& getTreeLeafSelectionValueQT() { return _treeLeafSelectionValue; }

    protected:
        ChartOptions& _chartOptions;

        StringAction                _scatterplotLeafSelectionValue;
        StringAction                _treeLeafSelectionValue;
        ToggleAction                _reembeddingOptions;

    };

    class MainSettingsHolder : public VerticalGroupAction
    {
    public:
        MainSettingsHolder(ChartOptions& chartOptions);
        const DatasetPickerAction& getComparisonTreeSelectionAction() const { return _comparisonTreeSelectionAction; }
        const DatasetPickerAction& getMainReferenceTreeSelectionAction() const { return _mainReferenceTreeSelectionAction; }


        DatasetPickerAction& getComparisonTreeSelectionAction() { return _comparisonTreeSelectionAction; }
        DatasetPickerAction& getMainReferenceTreeSelectionAction() { return _mainReferenceTreeSelectionAction; }




    protected:
        ChartOptions& _chartOptions;
        DatasetPickerAction            _comparisonTreeSelectionAction;
        DatasetPickerAction            _mainReferenceTreeSelectionAction;


    };

    class UpdateSettingsHolder : public VerticalGroupAction
    {
    public:
        UpdateSettingsHolder(ChartOptions& chartOptions);

        const TriggerAction& getUpdateViewsButtonAction() const { return _updateViewsButton; }


        TriggerAction& getUpdateViewsButtonAction() { return _updateViewsButton; }




    protected:
        ChartOptions& _chartOptions;
        TriggerAction                   _updateViewsButton;


    };



public:
    void onDataEventTree(mv::DatasetEvent* dataEvent);
    void onDataEventTreeMeta(mv::DatasetEvent* dataEvent);

protected:

public:
    ChartOptions(CrossSpeciesComparisonPhyloTreeViewPlugin& CrossSpeciesComparisonPhyloTreeViewPlugin, mv::CoreInterface* core);

private:
    //void createJsonStringCluster();
    std::string extractFormatData(QString datasetValue);
    void referenceDatasetPickerActionModify();
    void traitDatasetPickerActionModify();
    void treeDatasetPickerActionModify();


public: // Action getters
    ExtraSettingsHolder& getExtraSettingsHolder() { return _extraSettingsHolder; }
    MetaDataSettingsHolder& getMetaDataSettingsHolder() { return _metaDataSettingsHolder; }
    LinkerSettingsHolder& getLinkerSettingsHolder() { return _linkerSettingsHolder; }
    MainSettingsHolder& getMainSettingsHolder() { return _mainSettingsHolder; }
    UpdateSettingsHolder& getUpdateSettingsHolder() { return _updateSettingsHolder; }
    QCheckBox& getShowDataDrop() { return _showDataDrop; }
public:
    void initLoader();
    void changeLoader();
    void updateChartDataJS();
    void colorTraitCalculation();
    void stringTraitCalculation();
    void numericTraitCalculation();
    QStringList extractTraitPropertyOptionValues(QString propertyType, QString propertyString);
    QString filterData(const QString& key, const QString& category, const QJsonObject& data);
    void triggerChart();
public: // Serialization
    void fromVariantMap(const QVariantMap& variantMap) override;
    QVariantMap toVariantMap() const override;


protected:
    CrossSpeciesComparisonPhyloTreeViewPlugin& _viewerPlugin;
    mv::CoreInterface* _core;
    ExtraSettingsHolder              _extraSettingsHolder;
    MetaDataSettingsHolder              _metaDataSettingsHolder;
    LinkerSettingsHolder              _linkerSettingsHolder;
    MainSettingsHolder              _mainSettingsHolder;
    UpdateSettingsHolder            _updateSettingsHolder;

    QCheckBox _showDataDrop;
    mv::EventListener     _eventListener;
    friend class ChannelAction;
};