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
class CrossSpeciesComparisonPhyTVuPlugin;
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

        const ColorMap1DAction& getColorMapAction() const { return _treeColorMapAction; }
        const ToggleAction& getExpandAllAction() const { return _expandAllAction; }
        ToggleAction& getExpandAllAction() { return _expandAllAction; }
        ColorMap1DAction& getColorMapAction() { return _treeColorMapAction; }
        const ToggleAction& getDisableAcceptDatasetDrops() const { return _disableAcceptDatasetDrops; }
        ToggleAction& getDisableAcceptDatasetDrops() { return _disableAcceptDatasetDrops; }

    protected:
        ChartOptions& _chartOptions;

        ColorMap1DAction              _treeColorMapAction;
        ToggleAction                _expandAllAction;
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
        const ToggleAction& getDisableTraitOptions() const { return _disableTraitOptions; }


        DatasetPickerAction& getTraitDatasetSelectionAction() { return _traitDatasetSelectionAction; }
        OptionAction& getColorTraitAction() { return _colorTrait; }
        OptionAction& getNumericTraitAction() { return _numericTrait; }
        OptionAction& getStringTraitAction() { return _stringTrait; }
        ToggleAction& getDisableTraitOptions() { return _disableTraitOptions; }

    protected:
        ChartOptions& _chartOptions;

        DatasetPickerAction        _traitDatasetSelectionAction;
        OptionAction               _colorTrait;
        OptionAction                _numericTrait;
        OptionAction                _stringTrait;
        ToggleAction                _disableTraitOptions;

    };

    class LinkerSettingsHolder : public VerticalGroupAction
    {
    public:
        LinkerSettingsHolder(ChartOptions& chartOptions);
        const StringAction& getTreeLeafSelectionValueQT() const { return _treeLeafSelectionValue; }
        StringAction& getTreeLeafSelectionValueQT() { return _treeLeafSelectionValue; }
        const DatasetPickerAction& getLeafDatasetPicker() const { return _leafDatasetPicker; }
        DatasetPickerAction& getLeafDatasetPicker() { return _leafDatasetPicker; }
        const OptionsAction& getSelectedLeafValues() const { return _selectedLeafValues; }
        OptionsAction& getSelectedLeafValues() { return _selectedLeafValues; }

    protected:
        ChartOptions& _chartOptions;
        StringAction                _treeLeafSelectionValue;
        DatasetPickerAction                _leafDatasetPicker;
        OptionsAction                        _selectedLeafValues;

    };

    class MainSettingsHolder : public VerticalGroupAction
    {
    public:
        MainSettingsHolder(ChartOptions& chartOptions);
        const DatasetPickerAction& getMainReferenceTreeSelectionAction() const { return _mainReferenceTreeSelectionAction; }



        DatasetPickerAction& getMainReferenceTreeSelectionAction() { return _mainReferenceTreeSelectionAction; }




    protected:
        ChartOptions& _chartOptions;
        DatasetPickerAction            _mainReferenceTreeSelectionAction;


    };

    class UpdateSettingsHolder : public VerticalGroupAction
    {
    public:
        UpdateSettingsHolder(ChartOptions& chartOptions);

        const TriggerAction& getUpdateViewsButtonAction() const { return _updateViewsButton; }
        TriggerAction& getUpdateViewsButtonAction() { return _updateViewsButton; }

        const TriggerAction& getRevertButtonAction() const { return _revertButton; }
        TriggerAction& getRevertButtonAction() { return _revertButton; }





    protected:
        ChartOptions& _chartOptions;
        TriggerAction                   _updateViewsButton;
        TriggerAction                   _revertButton;

    };



public:
    void onDataEventTree(mv::DatasetEvent* dataEvent);
    void onDataEventTreeMeta(mv::DatasetEvent* dataEvent);

protected:

public:
    ChartOptions(CrossSpeciesComparisonPhyTVuPlugin& CrossSpeciesComparisonPhyTVuPlugin, mv::CoreInterface* core);

private:
    //void createJsonStringCluster();
    std::string extractFormatData(QString datasetValue);
    void traitDatasetModify();

public: // Action getters
    ExtraSettingsHolder& getExtraSettingsHolder() { return _extraSettingsHolder; }
    MetaDataSettingsHolder& getMetaDataSettingsHolder() { return _metaDataSettingsHolder; }
    LinkerSettingsHolder& getLinkerSettingsHolder() { return _linkerSettingsHolder; }
    MainSettingsHolder& getMainSettingsHolder() { return _mainSettingsHolder; }
    UpdateSettingsHolder& getUpdateSettingsHolder() { return _updateSettingsHolder; }
    QCheckBox& getShowDataDrop() { return _showDataDrop; }
public:
    void disableTraitOptions();
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
    CrossSpeciesComparisonPhyTVuPlugin& _viewerPlugin;
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