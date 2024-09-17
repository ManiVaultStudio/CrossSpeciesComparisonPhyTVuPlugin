#pragma once

//#include "SelectionListener.h"
#include "widgets/WebWidget.h"
#include <QMouseEvent>
#include <QComboBox>
#include <QList>
#include <QVariant>
class QWebEngineView;
class QWebEnginePage;
class QWebChannel;


class ChartWidget;

class ChartCommunicationObject : public mv::gui::WebCommunicationObject
{
    Q_OBJECT
public:
    ChartCommunicationObject(ChartWidget* parent);

signals:
    void qt_setData(QString data);
    void qt_setRemoveSelectionFromJavascript(QString data);
    void qt_setSpecies1(QString species1);
    void qt_setSpecies2(QString species2);
    void qt_setSelectedCrossspeciescluster(QString data);
    void qt_addAvailableData(QString name);
    void qt_updateSelectionColor(QString data);
    //void qt_setBarhighlight(QString clusterName);
    void qt_setTreeColorMap(QString data);
    //void qt_setShowReferenceTree(QString data);
    void qt_setLeafSelectionFromQT(QString data);
    void qt_setDisableTraitOptions(QString data);
    //void qt_setExpandAll(QString data);
    void qt_setTraitColor(QString data);
    void qt_setTraitString(QString data);
    void qt_setTraitNumeric(QString data);
    //void qt_setClusterMethod(QString data);

public slots:
    void js_passSelectionSpecies1ToQt(QString data);
    void js_passSelectionSpecies2ToQt(QString data);
    void js_crossspeciesclusterSelection(QString data);
    void js_addSelectionToScatterplot(QVariant data);
    void js_passScatterplotLeafPointSelectionToQt(QVariant data);
    void js_alterColorMirrorForRankToQt(QVariant data);
    void js_alterColorMapToQt(QVariant data);
    //void js_addLeftRightSelectionToScatterplot(QVariant data);
    void js_removeSelectionFromScatterplot(QVariant data);
private:
    ChartWidget* _parent;
};

class ChartWidget : public mv::gui::WebWidget
{
    Q_OBJECT
public:
    ChartWidget();
    ~ChartWidget() override;

    void addDataOption(const QString option);
    void setData(QString visDataContent);
    void setRemoveSelectionFromJavascript(QString visDataContent);
    void setSpecies1(QString species1);
    void setSpecies2(QString species2);
    void setSelectedCrossspeciescluster(QString visDataContent);
    void updateSelectionColor(QString visColorContent);
    //void setBarhighlight(QString clusterName);
    void setTreeColorMap(QString visColorContent);
    //void setShowReferenceTree(QString visColorContent);
    void setLeafSelectionFromQT(QString visColorContent);
    void setDisableTraitOptions(QString visColorContent);
    //void setExpandAll(QString visColorContent);
    void setTraitColor(QString visColorContent);
    void setTraitString(QString visColorContent);
    void setTraitNumeric(QString visColorContent);
    //void setClusterMethod(QString visColorContent);
protected:


signals:
    void widgetInitialized();
    void passSelectionSpecies1ToQt(std::string selectedIDs);
    void passSelectionSpecies2ToQt(std::string selectedIDs);
    void crossspeciesclusterSelection(std::string selectedIDs);
    void removeSelectionFromScatterplot(std::string clusterName);
    void addSelectionToScatterplot(std::string clusterName);
    void passScatterplotLeafPointSelectionToQt(std::string selectedSpecies);
    void alterColorMirrorForRankToQt(std::string selectedSpecies);
    void alterColorMapToQt(std::string selectedSpecies);
    void addLeftRightSelectionToScatterplot(std::string clusterName);

public:

    void js_passSelectionSpecies1ToQt(std::string selectedIDs);
    void js_passSelectionSpecies2ToQt(std::string selectedIDs);
    void js_crossspeciesclusterSelection(std::string selectedIDs);
    void js_addSelectionToScatterplot(std::string clusterName);
    void js_passScatterplotLeafPointSelectionToQt(std::string selectedSpecies);
    void js_alterColorMirrorForRankToQt(std::string selectedSpecies);
    void js_alterColorMapToQt(std::string selectedSpecies);
    //void js_addLeftRightSelectionToScatterplot(std::string clusterName);
    void js_removeSelectionFromScatterplot(std::string clusterName);

private slots:
    void initWebPage() override;

private:
    ChartCommunicationObject* _communicationObject;


    /** Whether the web view has loaded and web-functions are ready to be called. */
    bool loaded;
    /** Temporary storage for added data options until webview is loaded */
    QList<QString> dataOptionBuffer;

};