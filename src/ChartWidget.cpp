#include "ChartWidget.h"
#include "util/FileUtil.h"
#include <fstream>
#include <string>
#include <iostream>
#include <QVBoxLayout>
#include <chrono>
#include <cassert>

ChartCommunicationObject::ChartCommunicationObject(ChartWidget* parent)
    :
    _parent(parent)
{

}



void ChartCommunicationObject::js_passSelectionSpecies1ToQt(QString data)
{
    //qDebug() << "\nReceived from Javascript to QT\n";
    _parent->js_passSelectionSpecies1ToQt(data.toStdString());
}


void ChartCommunicationObject::js_passSelectionSpecies2ToQt(QString data)
{
    //qDebug() << "\nReceived from Javascript to QT\n";
    _parent->js_passSelectionSpecies2ToQt(data.toStdString());
}

void ChartCommunicationObject::js_crossspeciesclusterSelection(QString data)
{
    //qDebug() << "\nReceived from Javascript to QT\n";
    _parent->js_crossspeciesclusterSelection(data.toStdString());
}


void ChartCommunicationObject::js_removeSelectionFromScatterplot(QVariant data)
{

    _parent->js_removeSelectionFromScatterplot(data.toString().toStdString());
}


void ChartCommunicationObject::js_addSelectionToScatterplot(QVariant data)
{

    _parent->js_addSelectionToScatterplot(data.toString().toStdString());
}
void ChartCommunicationObject::js_passScatterplotLeafPointSelectionToQt(QVariant data)
{

    _parent->js_passScatterplotLeafPointSelectionToQt(data.toString().toStdString());
}

void ChartCommunicationObject::js_addLeftRightSelectionToScatterplot(QVariant data)
{

    _parent->js_addLeftRightSelectionToScatterplot(data.toString().toStdString());
}

ChartWidget::ChartWidget() :
    loaded(false)
{
    Q_INIT_RESOURCE(CrossSpeciesComparisonPhyloTreeView_chart);
    _communicationObject = new ChartCommunicationObject(this);
    init(_communicationObject);
    setMinimumHeight(140);
}

ChartWidget::~ChartWidget()
{

}

void ChartWidget::setData(QString visDataContent)
{
    emit _communicationObject->qt_setData(visDataContent);
}

void ChartWidget::setRemoveSelectionFromJavascript(QString visDataContent)
{
    emit _communicationObject->qt_setRemoveSelectionFromJavascript(visDataContent);
}

void ChartWidget::updateSelectionColor(QString visColorContent)
{
    emit _communicationObject->qt_updateSelectionColor(visColorContent);
}
void ChartWidget::setTreeColorMap(QString visColorContent)
{
    emit _communicationObject->qt_setTreeColorMap(visColorContent);
}

void ChartWidget::setShowReferenceTree(QString visColorContent)
{
    emit _communicationObject->qt_setShowReferenceTree(visColorContent);
}

void ChartWidget::setLeafSelectionFromQT(QString visColorContent)
{
    emit _communicationObject->qt_setLeafSelectionFromQT(visColorContent);
}

void ChartWidget::setExpandAll(QString visColorContent)
{
    emit _communicationObject->qt_setExpandAll(visColorContent);
}
void ChartWidget::setTraitColor(QString visColorContent)
{
    emit _communicationObject->qt_setTraitColor(visColorContent);
}
void ChartWidget::setTraitString(QString visColorContent)
{
    emit _communicationObject->qt_setTraitString(visColorContent);
}
void ChartWidget::setTraitNumeric(QString visColorContent)
{
    emit _communicationObject->qt_setTraitNumeric(visColorContent);
}
void ChartWidget::setClusterMethod(QString visColorContent)
{
       emit _communicationObject->qt_setClusterMethod(visColorContent);
}

void ChartWidget::setSpecies1(QString species1)
{

    emit _communicationObject->qt_setSpecies1(species1);
}

void ChartWidget::setSpecies2(QString species2)
{

    emit _communicationObject->qt_setSpecies2(species2);
}

void ChartWidget::setSelectedCrossspeciescluster(QString visDataContent)
{

    emit _communicationObject->qt_setSelectedCrossspeciescluster(visDataContent);
}


void ChartWidget::js_passSelectionSpecies1ToQt(std::string selectedIDs)
{
    emit passSelectionSpecies1ToQt(selectedIDs);
}


void ChartWidget::js_removeSelectionFromScatterplot(std::string selectedIDs)
{
    emit removeSelectionFromScatterplot(selectedIDs);
}

void ChartWidget::js_addSelectionToScatterplot(std::string selectedIDs)
{
    emit addSelectionToScatterplot(selectedIDs);
}
void ChartWidget::js_passScatterplotLeafPointSelectionToQt(std::string selectedIDs)
{
    emit passScatterplotLeafPointSelectionToQt(selectedIDs);
}
void ChartWidget::js_addLeftRightSelectionToScatterplot(std::string selectedIDs)
{
    emit addLeftRightSelectionToScatterplot(selectedIDs);
}
void ChartWidget::js_passSelectionSpecies2ToQt(std::string selectedIDs)
{
    emit passSelectionSpecies2ToQt(selectedIDs);
}

void ChartWidget::js_crossspeciesclusterSelection(std::string selectedIDs)
{
    emit crossspeciesclusterSelection(selectedIDs);
}

void ChartWidget::initWebPage()
{
    loaded = true;

    for (QString option : dataOptionBuffer) {
        emit _communicationObject->qt_addAvailableData(option);
    }
    dataOptionBuffer.clear();

    emit widgetInitialized();
}

void ChartWidget::addDataOption(const QString option)
{
    if (loaded)
        emit _communicationObject->qt_addAvailableData(option);
    else
        dataOptionBuffer.append(option);
}

