#pragma once

#include <actions/WidgetAction.h>
#include <actions/IntegralAction.h>
#include <actions/OptionAction.h>
#include <actions/OptionsAction.h>
#include <actions/VariantAction.h>
#include "PointData/PointData.h"
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


class ReembedOptions : public WidgetAction
{
public:

    class ReembeddingSettingsHolder : public VerticalGroupAction
    {
    public:
        ReembeddingSettingsHolder(ReembedOptions& reembedOptions);
        const ToggleAction& getselectedLeftRightLeafNamesFlag() const { return _selectedLeftRightLeafNamesFlagAction; }

        const StringAction& getselectedLeftRightLeafNames() const { return _selectedLeftRightLeafNamesAction; }
        const DatasetPickerAction& gethsneDataset() const { return _hsneDatasetAction; }
        const ToggleAction& getPerformHSNETSNEFlag() const { return _performHSNETSNEFlagAction; }
        const ToggleAction& getPerformHSNEFlag() const { return _performHSNEFlagAction; }
        const ToggleAction& getPerformTSNEFlag() const { return _performTSNEFlagAction; }
        const StringAction& getselectedLeafNames() const { return _selectedLeafNamesAction; }

        const DatasetPickerAction& getembeddingDataset() const { return _embeddingDatasetAction; }
        const DatasetPickerAction& getleafNamesDataset() const { return _leafNamesDatasetAction; }
        const DatasetPickerAction& getselectedRowSelectionDataset() const { return _selectedRowSelectionDatasetAction; }



        const DatasetPickerAction& getdeDatasetLeft() const { return _deDatasetLeftAction; }

        const DatasetPickerAction& getdeDatasetRight() const { return _deDatasetRightAction; }


       // const DatasetPickerAction& getclusterSelectionFromPopulationPyramidDataset() const { return _clusterSelectionFromPopulationPyramidDatasetAction; }
        const TriggerAction& getdeUpdateSwitch() const { return _deUpdateSwitchAction; }
        const StringAction& getgeneNamesConnection() const { return _geneNamesConnection; }

        const DatasetPickerAction& getLeftGroupPointDatasetDE() const { return _leftGroupPointDatasetDE; }
        const DatasetPickerAction& getRightGroupPointDatasetDE() const { return _rightGroupPointDatasetDE; }
        const DatasetPickerAction& getLeftGroupClusterDatasetDE() const { return _leftGroupClusterDatasetDE; }
        const DatasetPickerAction& getRightGroupClusterDatasetDE() const { return _rightGroupClusterDatasetDE; }
        
        ToggleAction& getselectedLeftRightLeafNamesFlag() { return _selectedLeftRightLeafNamesFlagAction; }
        StringAction& getselectedLeftRightLeafNames() { return _selectedLeftRightLeafNamesAction; }
        StringAction& getselectedLeafNames() { return _selectedLeafNamesAction; }

        ToggleAction& getPerformHSNETSNEFlag() { return _performHSNETSNEFlagAction; }
        ToggleAction& getPerformHSNEFlag() { return _performHSNEFlagAction; }
        ToggleAction& getPerformTSNEFlag() { return _performTSNEFlagAction; }



        DatasetPickerAction& gethsneDataset() { return _hsneDatasetAction; }

        DatasetPickerAction& getembeddingDataset() { return _embeddingDatasetAction; }
        DatasetPickerAction& getleafNamesDataset() { return _leafNamesDatasetAction; }
        DatasetPickerAction& getselectedRowSelectionDataset() { return _selectedRowSelectionDatasetAction; }
 
        DatasetPickerAction& getdeDatasetLeft() { return _deDatasetLeftAction; }
        DatasetPickerAction& getdeDatasetRight() { return _deDatasetRightAction; }


        TriggerAction& getdeUpdateSwitch() { return _deUpdateSwitchAction; }
        StringAction& getgeneNamesConnection() { return _geneNamesConnection; }

        DatasetPickerAction&    getLeftGroupPointDatasetDE() { return _leftGroupPointDatasetDE; }
        DatasetPickerAction&    getRightGroupPointDatasetDE() { return _rightGroupPointDatasetDE; }
        DatasetPickerAction& getLeftGroupClusterDatasetDE() { return _leftGroupClusterDatasetDE; }
        DatasetPickerAction& getRightGroupClusterDatasetDE() { return _rightGroupClusterDatasetDE; }


    protected:
        ReembedOptions& _reembedOptions;
        ToggleAction                _selectedLeftRightLeafNamesFlagAction;
        StringAction                _selectedLeftRightLeafNamesAction;
        StringAction                _selectedLeafNamesAction;

        DatasetPickerAction         _embeddingDatasetAction;
        DatasetPickerAction         _leafNamesDatasetAction;
        DatasetPickerAction         _selectedRowSelectionDatasetAction;

       
        DatasetPickerAction         _hsneDatasetAction;
        ToggleAction                _performHSNETSNEFlagAction;
        ToggleAction                _performHSNEFlagAction;
        ToggleAction                _performTSNEFlagAction;
        
        
        StringAction           _geneNamesConnection;
        

        DatasetPickerAction         _deDatasetLeftAction;
        DatasetPickerAction         _deDatasetRightAction;


        TriggerAction   _deUpdateSwitchAction;

        DatasetPickerAction     _leftGroupPointDatasetDE;
        DatasetPickerAction     _rightGroupPointDatasetDE;
        DatasetPickerAction     _leftGroupClusterDatasetDE;
        DatasetPickerAction     _rightGroupClusterDatasetDE;

    };




public:
    ReembedOptions(CrossSpeciesComparisonPhyloTreeViewPlugin& CrossSpeciesComparisonPhyloTreeViewPlugin, mv::CoreInterface* core);
    ReembeddingSettingsHolder& getReembeddingSettingsHolder() { return _reembeddingSettingsHolder; }

    
    void reembedSelection();
private:
    void reembed();
    void differentialExpression();
    void populatePointData(QString& datasetId, std::vector<float>& pointVector, int& numPoints, int& numDimensions, std::vector<QString>& dimensionNames);
    void populateClusterData(QString& datasetId, std::map<QString, std::pair<QColor, std::vector<int>>>& clusterMap);


public: 
    void fromVariantMap(const QVariantMap& variantMap) override;
    QVariantMap toVariantMap() const override;


protected:
    CrossSpeciesComparisonPhyloTreeViewPlugin& _viewerPlugin;
    mv::CoreInterface* _core;
    ReembeddingSettingsHolder       _reembeddingSettingsHolder;
    std::map<QString, QString>  _deDatasetColorMap;


    mv::EventListener     _eventListener;
    QStringList     _selectedSpeciesListLeft;
    QStringList     _selectedSpeciesListRight;
    QStringList     _allSelectedSpecies;
    Dataset<Points> _tsneDataset;
    Dataset<Points> _lowDimTSNEDataset;
    std::map<QString,QString>     _tsneColorDatasetIds;
    std::map<QString, std::pair<QColor, std::vector<int>>> _leafMap;
    std::map<QString, std::pair<QColor, std::vector<int>>> _clusterMap;
    std::map<QString, std::pair<QColor, std::vector<int>>> _leftRightClusterMap;
    std::map<int,int> _indicesMap;

    mv::Dataset<Points>    _leftDeDatasetPoints;
    mv::Dataset<Points>    _rightDeDatasetPoints;
    mv::Dataset<Clusters>   _leftDeDatasetClusters;
    mv::Dataset<Clusters>   _rightDeDatasetClusters;
    bool _tsneAvailable = false;
};
