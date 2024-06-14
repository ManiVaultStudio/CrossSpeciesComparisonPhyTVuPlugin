var isQtAvailable = false;

// Here, we establish the connection to Qt
// Any signals that we want to send from ManiVault to
// the JS side have to be connected here
try {
    new QWebChannel(qt.webChannelTransport, function (channel) {
        // Establish connection
        QtBridge = channel.objects.QtBridge;

        // register signals   // drawChart is defined in radar_chart.tools.js
        QtBridge.qt_setData.connect(function () {
            drawChart(arguments[0]);
        });
        QtBridge.qt_setTreeColorMap.connect(function () {
            setTreeColorMap(arguments[0]);
        });
        QtBridge.qt_setShowReferenceTree.connect(function () {
            setShowReferenceTree(arguments[0]);
        });
        QtBridge.qt_setExpandAll.connect(function () {
            setExpandAll(arguments[0]);
        });
        QtBridge.qt_setTraitColor.connect(function () {
            setTraitColor(arguments[0]);
        });
        QtBridge.qt_setTraitString.connect(function () {
            setTraitString(arguments[0]);
        });
        QtBridge.qt_setTraitNumeric.connect(function () {
            setTraitNumeric(arguments[0]);
        });

        QtBridge.qt_setClusterMethod.connect(function () {
            setClusterMethod(arguments[0]);
        });

        // confirm successful connection
        isQtAvailable = true;
        notifyBridgeAvailable();
    });
} catch (error) {
    log("SimianViewerSpeciesSelectorPlugin: qwebchannel: could not connect qt");
}

// The slot js_available is defined by ManiVault's WebWidget and will
// invoke the initWebPage function of our web widget (here, ChartWidget)
function notifyBridgeAvailable() {
    if (isQtAvailable) {
        QtBridge.js_available();
    } else {
        log(
            "SimianViewerSpeciesSelectorPlugin: qwebchannel: QtBridge is not available - something went wrong"
        );
    }
}

// The QtBridge is connected to our WebCommunicationObject (ChartCommObject)
// and we can call all slots defined therein
function passSelectionToQt(dat) {
    if (isQtAvailable) {
        QtBridge.js_qt_passSelectionToQt(dat);
    }
}
function passScatterplotLeafPointSelectionToQt(dat) {
    if (isQtAvailable) {
        QtBridge.js_passScatterplotLeafPointSelectionToQt(dat);
    }
}
function passAddSelectionToQt(speciesString) {
    if (isQtAvailable) {
        QtBridge.js_addSelectionToScatterplot(speciesString);
    }
}

function passLeftRightSelectionToQT(speciesString) {
    if (isQtAvailable) {
        QtBridge.js_addLeftRightSelectionToScatterplot(speciesString);
    }
}

function passRemoveSelectionToQt(speciesString) {
    if (isQtAvailable) {
        QtBridge.js_removeSelectionFromScatterplot("");
    }
}

// utility function: pipe errors to log
window.onerror = function (msg, url, num) {
    log(
        "SimianViewerSpeciesSelectorPlugin: qwebchannel: Error: " +
        msg +
        "\nURL: " +
        url +
        "\nLine: " +
        num
    );
};

// utility function: auto log for Qt and console
function log(logtext) {
    if (isQtAvailable) {
        QtBridge.js_debug(logtext.toString());
    } else {
        log(logtext);
    }
}
