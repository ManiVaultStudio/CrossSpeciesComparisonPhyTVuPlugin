window.onresize = doALoadOfStuff;
var dataReference = "";
var treeData = "";
var qtColor = "Magma";
var m_aAverageScore = undefined;
var m_sGlobalMeasure = "element";
var clusteringMethod = "upgma";
var allScoresStorage = undefined;
var traitValueString = "";
var traitValueColor = "";
var traitValueNumeric = "";
var traitValueStringFlag = false;
var traitValueColorFlag = false;
var traitValueNumericFlag = false;
var traitValueStringContainer;
var traitValueColorContainer;
var traitValueNumericContainer;
var traitValueStringKey = "";
var traitValueColorKey = "";
var traitValueNumericKey = "";
var showTraitValueString = false;
var showTraitValueColor = false;
var showTraitValueNumeric = false;
let showReferenceTree = false;
let showExpandAll = false;
var colorMirror = false;
var isFirstRender = true;
var removeAnimation = true;
var leftSpeciesSelected = [];
var rightSpeciesSelected = [];
var speciesSelected = [];
var nodesWithChildren = [];
var showTestTree = 1;
var m_aLoadedTrees = [];
var mindistanceColor = 0;
var maxdistanceColor = 1;
var maxNameLength = 10;
var symbolsPerLine = 10;
var legendXValue = -45;
var legendYValue = 25;
var shiftPressed = false;
var removeLeafSelections = false;
var altPressed = false;
var removeSelection = false;
var splitGroupsAltKey = false;
var legendTextContainer = [];
var legendTextContainerAltFlag = false;
var legendTextContainerActivateFlag = false;
var expandedLeafNameID = "";
var svg;
//true for Debugging
var isDebug = false;
const colorScales = {
    qualitative: d3.schemeSet3,
    RdYlBu: d3.interpolateRdYlBu,
    YlGn: d3.interpolateYlGn,
    RdYlGn: d3.interpolateRdYlGn,
    GnBu: d3.interpolateGnBu,
    YlGnBu: d3.interpolateYlGnBu,
    Spectral: d3.interpolateSpectral,
    BrBG: d3.interpolateBrBG,
    YlOrBr: d3.interpolateYlOrBr,
    RdBu: d3.interpolateRdBu,
    RdPu: d3.interpolateRdPu,
    Plasma: d3.interpolatePlasma,
    PuOr: d3.interpolatePuOr,
    BuPu: d3.interpolateBuPu,
    Reds: d3.interpolateReds,
    Viridis: d3.interpolateViridis,
    Q_BlGrRd: d3.interpolateRdGy,
    Magma: d3.interpolateMagma,
    PiYG: d3.interpolatePiYG,
    default: d3.interpolateGreys,
};
//Resize on window dimension change
function doALoadOfStuff() {
    if (dataReference != "" && dataMainCompare != "") {
        generateVis();
    }
}

function convertToJSON(clusterValue) {
    if (clusterValue.left && clusterValue.right) {
        let leftJSON = convertToJSON(clusterValue.left);
        let rightJSON = convertToJSON(clusterValue.right);

        // Find all species in each branch
        let leftSpecies = findFirst(leftJSON);
        let rightSpecies = findFirst(rightJSON);

        // Find the species that appears first in the speciesArray among all species in each branch
        let leftFirst = speciesArray.find((species) =>
            leftSpecies.includes(species)
        );
        let rightFirst = speciesArray.find((species) =>
            rightSpecies.includes(species)
        );

        // Order the branches based on the first species
        let children;
        if (speciesArray.indexOf(rightFirst) < speciesArray.indexOf(leftFirst)) {
            children = [rightJSON, leftJSON];
        } else {
            children = [leftJSON, rightJSON];
        }

        const nodeId = currentId++;
        return {
            id: nodeId,
            score: 1,
            children: children,
        };
    } else {
        return {
            id: currentId++,
            name: speciesArray[clusterValue.key],
        };
    }
}

// Helper function to find all species in a branch
function findFirst(branch) {
    if (branch.children) {
        return branch.children.flatMap(findFirst);
    } else {
        return [branch.name];
    }
}
function findMaxNameLength(obj) {
    var maxLen = 0;
    for (var key in obj) {
        if (key === "name" && obj[key].length !== undefined) {
            maxLen = Math.max(maxLen, obj[key].length);
        } else if (typeof obj[key] === "object") {
            maxLen = Math.max(maxLen, findMaxNameLength(obj[key]));
        }
    }
    return maxLen;
}
function drawChart(jsonValue) {
    const delimiter = "@#&%$splithere%$&#@";

    const resultArray = jsonValue.split(delimiter);
    var jsonStringCompare = resultArray[1];
    var jsonStringReference = resultArray[0];
    leftSpeciesSelected = [];
    rightSpeciesSelected = [];
    speciesSelected = [];
    legendTextContainer = [];
    legendTextContainerAltFlag = false;
    legendTextContainerActivateFlag = false;
    speciesArray = [];
    //var jsonStringReference =
      //  '{ "children": [ { "children": [ { "children": [ { "children": [ { "children": [ { "children": [ { "children": [ { "children": [ { "children": [ { "children": [ { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "chimp" }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "human" } ], "id": 1, "score": 1, "width": 1 }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "gorilla" } ], "id": 2, "score": 1, "width": 1 }, { "children": [ { "children": [ { "children": [ { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "rhesus_macaque" }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "macaque_nemestrina" } ], "id": 3, "score": 1, "width": 1 }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "baboon" } ], "id": 4, "score": 1, "width": 1 }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "african_green_monkey" } ], "id": 5, "score": 1, "width": 1 } ], "id": 6, "score": 1, "width": 1 }, { "children": [ { "children": [ { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "south_american_squirrel_monkey" }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "capuchin" } ], "id": 7, "score": 1, "width": 1 }, { "children": [ { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "marmoset" }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "owl_monkey" } ], "id": 8, "score": 1, "width": 1 } ], "id": 9, "score": 1, "width": 1 } ], "id": 10, "score": 1, "width": 1 }, { "children": [ { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "galago" }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "gray_mouse_lemur" } ], "id": 11, "score": 1, "width": 1 } ], "id": 12, "score": 1, "width": 1 }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "treeshrew" } ], "id": 13, "score": 1, "width": 1 }, { "children": [ { "children": [ { "children": [ { "children": [ { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "mouse" }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "rat" } ], "id": 14, "score": 1, "width": 1 }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "naked_mole_rat" } ], "id": 15, "score": 1, "width": 1 }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "arctic_ground_squirrel" } ], "id": 16, "score": 1, "width": 1 }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "rabbit" } ], "id": 17, "score": 1, "width": 1 } ], "id": 18, "score": 1, "width": 1 }, { "children": [ { "children": [ { "children": [ { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "ferret" }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "coyote" } ], "id": 19, "score": 1, "width": 1 }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "cat" } ], "id": 20, "score": 1, "width": 1 }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "pig" } ], "id": 21, "score": 1, "width": 1 } ], "id": 22, "score": 1, "width": 1 }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "armadillo" } ], "id": 23, "score": 1, "width": 1 }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "opossum" } ], "id": 24, "score": 1, "width": 1 }';
    //var jsonStringCompare =
     //   '{ "children": [ { "children": [ { "children": [ { "children": [ { "children": [ { "children": [ { "children": [ { "children": [ { "children": [ { "children": [ { "children": [ { "children": [ { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "arctic_ground_squirrel" }, { "children": [ { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "south_american_squirrel_monkey" }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "treeshrew" } ], "id": 1, "score": 1, "width": 1 } ], "id": 1, "score": 1, "width": 1 }, { "children": [ { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "rat" }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "rhesus_macaque" } ], "id": 1, "score": 1, "width": 1 } ], "id": 1, "score": 1, "width": 1 }, { "children": [ { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "pig" }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "rabbit" } ], "id": 1, "score": 1, "width": 1 } ], "id": 1, "score": 1, "width": 1 }, { "children": [ { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "opossum" }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "owl_monkey" } ], "id": 1, "score": 1, "width": 1 } ], "id": 1, "score": 1, "width": 1 }, { "children": [ { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "mouse" }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "naked_mole_rat" } ], "id": 1, "score": 1, "width": 1 } ], "id": 1, "score": 1, "width": 1 }, { "children": [ { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "macaque_nemestrina" }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "marmoset" } ], "id": 1, "score": 1, "width": 1 } ], "id": 1, "score": 1, "width": 1 }, { "children": [ { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "gray_mouse_lemur" }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "human" } ], "id": 1, "score": 1, "width": 1 } ], "id": 1, "score": 1, "width": 1 }, { "children": [ { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "galago" }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "gorilla" } ], "id": 1, "score": 1, "width": 1 } ], "id": 1, "score": 1, "width": 1 }, { "children": [ { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "coyote" }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "ferret" } ], "id": 1, "score": 1, "width": 1 } ], "id": 1, "score": 1, "width": 1 }, { "children": [ { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "cat" }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "chimp" } ], "id": 1, "score": 1, "width": 1 } ], "id": 1, "score": 1, "width": 1 }, { "children": [ { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "baboon" }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "capuchin" } ], "id": 1, "score": 1, "width": 1 } ], "id": 1, "score": 1, "width": 1 }, { "children": [ { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "african_green_monkey" }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "name": "armadillo" } ], "id": 1, "score": 1, "width": 1 } ], "id": 1, "score": 1, "width": 1 }';
    //traitValueString =
      //  '{"data":{"african_green_monkey":"scansorial","arctic_ground_squirrel":"other","armadillo":"fossorial","baboon":"other","capuchin":"scansorial","cat":"scansorial","chimp":"scansorial","coyote":"other","ferret":"other","galago":"scansorial","gorilla":"other","gray_mouse_lemur":"scansorial","human":"other","macaque_nemestrina":"scansorial","marmoset":"scansorial","mouse":"other","naked_mole_rat":"fossorial","opossum":"other","owl_monkey":"scansorial","pig":"other","rabbit":"fossorial","rat":"other","rhesus_macaque":"scansorial","south_american_squirrel_monkey":"scansorial","treeshrew":"scansorial"},"type":"lifestyle"}';
    //traitValueStringFlag = true;
    if (jsonStringCompare !== "" && jsonStringReference !== "") {
        var dataVariableReference = JSON.parse(jsonStringReference);
        var dataVariableCompare = JSON.parse(jsonStringCompare);
        maxNameLength = findMaxNameLength(dataVariableCompare);
        /*
            
            speciesArray = Object.keys(dataVariableReference);
            
            var clustersReference = hierarchicalClustering(dataVariableReference, speciesArray, clusteringMethod);
    
            var hierarchicalJSONReference = convertClusterToDesiredFormat(clustersReference);
    
            var clustersCompare = hierarchicalClustering(dataVariableCompare, speciesArray, clusteringMethod);
    
            var hierarchicalJSONCompare = convertClusterToDesiredFormat(clustersCompare);
    
            */

        m_aLoadedTrees = [];
        m_aLoadedTrees.push(CreateTreeFromData(dataVariableReference)); //referenceTree
        m_aLoadedTrees.push(CreateTreeFromData(dataVariableCompare));

        UpdateOverviewMeasures();

        var jsonStringsArray = [];

        for (var i = 0; i < m_aLoadedTrees.length; i++) {
            var finalData = createDataFromTree(
                m_aLoadedTrees[i],
                allScoresStorage[i]
            );
            var jsonString = JSON.stringify(finalData, null, 2);
            jsonStringsArray.push(jsonString);
        }

        // Now you can access the JSON strings in the array
        for (var j = 0; j < jsonStringsArray.length; j++) {
            //log(jsonStringsArray[j]);
        }
        var parsedDataCompare;
        var parsedDataReference;

        parsedDataCompare = JSON.parse(jsonStringsArray[1]);
        parsedDataReference = JSON.parse(jsonStringsArray[0]);

        dataMainCompare = copyDataWithCollapsedFlag(parsedDataCompare);

        dataReference = copyDataWithCollapsedFlag(parsedDataReference);

        leftSpeciesSelected = [];
        rightSpeciesSelected = [];
        speciesSelected = [];
        if (dataReference != "" && dataMainCompare != "") {
            generateVis();
        }
    }
}

function removeSelectionFromJavascript(d) {
    if ((d = "T")) {
        removeSelection = true;
        leftSpeciesSelected = [];
        rightSpeciesSelected = [];
        speciesSelected = [];
        if (dataReference != "" && dataMainCompare != "") {
            generateVis();
        }
        removeSelection = false;
    }
}

//Color Options
function setTreeColorMap(d) {
    const valRa = d.split("*%*");
    qtColor = valRa[0];
    if (valRa[1] == "F") {
        colorMirror = false;
    } else {
        colorMirror = true;
    }
    if (dataReference != "" && dataMainCompare != "") {
        generateVis();
    }
}
function setShowReferenceTree(showReferenceTreeValue) {
    // log("showReferenceTreeValue: " + showReferenceTreeValue);

    if (showReferenceTreeValue !== "") {
        if (showReferenceTreeValue == "T") {
            showReferenceTree = true;
        } else {
            showReferenceTree = false;
        }
        if (dataReference != "" && dataMainCompare != "") {
            generateVis();
        }
    }
}
function setLeafSelectionFromQT(speciesStringValue) {
    // log("showReferenceTreeValue: " + showReferenceTreeValue);
    if (speciesStringValue !== "") {
        var speciesArray = speciesStringValue.split(" @%$,$%@ ");
        speciesSelected = speciesArray;
        if (dataReference != "" && dataMainCompare != "") {
           // generateVis();
        }
    }

}
function setExpandAll(setExpandAllValue) {
    // log("showReferenceTreeValue: " + showReferenceTreeValue);

    if (setExpandAllValue !== "") {
        if (setExpandAllValue == "T") {
            showExpandAll = true;
        } else {
            showExpandAll = false;
        }
        if (dataReference != "" && dataMainCompare != "") {
            generateVis();
        }
    }
}

function setTraitColor(setTraitValue) {
    // log("showReferenceTreeValue: " + showReferenceTreeValue);
    if (setTraitValue !== "" && dataReference != "" && dataMainCompare != "") {
        traitValueColor = setTraitValue;
        traitValueColorFlag = true;
        generateVis();
    } else {
        traitValueColorFlag = false;
    }
}

function setTraitString(setTraitValue) {
    // log("showReferenceTreeValue: " + showReferenceTreeValue);
    if (setTraitValue !== "" && dataReference != "" && dataMainCompare != "") {
        traitValueString = setTraitValue;
        traitValueStringFlag = true;

        generateVis();
    } else {
        traitValueStringFlag = false;
    }
}

function setTraitNumeric(setTraitValue) {
    // log("showReferenceTreeValue: " + showReferenceTreeValue);
    if (setTraitValue !== "" && dataReference != "" && dataMainCompare != "") {
        traitValueNumeric = setTraitValue;

        traitValueNumericFlag = true;
        generateVis();
    } else {
        traitValueNumericFlag = false;
    }
}
function setClusterMethod(setValue) {
    // log("showReferenceTreeValue: " + showReferenceTreeValue);
    /*
       leftSpeciesSelected = [];
   rightSpeciesSelected = [];
   speciesSelected = [];
    legendTextContainer = [];
    legendTextContainerAltFlag = false;
    legendTextContainerActivateFlag = false;
      */
    if (setValue !== "" && dataReference != "" && dataMainCompare != "") {
        clusteringMethod = setValue;
        generateVis();
    }
}
