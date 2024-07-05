window.onresize = doALoadOfStuff;
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
var showTraitValues = false;
let showReferenceTree = false;
let showExpandAll = true;
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
var legendXValue = 0;
var legendYValue = 0;
var shiftPressed = false;
var removeLeafSelections = false;
var altPressed = false;
var altShiftPressed = false;
var removeSelection = false;
var splitGroupsAltKey = false;
var legendTextContainer = [];
var legendTextContainerAltFlag = false;
var legendTextContainerActivateFlag = false;
var expandedLeafNameID = "";
var svg;
var maxBranchLength = 0;
var minBranchLength = 0;
var maxDepth = 0;
var eachDepthWidth = 0;
var maxTotalDepthWidth = 0;
var dataReference = "";
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
  if (dataReference != "") {
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
function sumMeanValues(node) {
  // Base case: If the node has no children, return its mean
  if (!node.children || node.children.length === 0) {
    node.score = parseFloat((node.mean || 0).toFixed(2)); // Ensure leaf nodes without mean are handled and rounded to 2 decimal places
    return node.score;
  }

  // Recursive case: Sum mean values of all children
  let sum = 0;
  node.children.forEach((child) => {
    sum += sumMeanValues(child); // Recursively sum up the mean values
  });

  // Update the current node's score with the sum of mean values from its children, rounded to 2 decimal places
  node.score = parseFloat(sum.toFixed(2));

  // If the current node has its own mean, add it to the score and ensure the result is rounded to 2 decimal places
  if (node.mean) {
    node.score += parseFloat(node.mean.toFixed(2));
    node.score = parseFloat(node.score.toFixed(2)); // Round again after addition to ensure 2 decimal places
  }

  return node.score;
}
function averageMeanValues(node) {
  // Base case: If the node has no children, return its mean
  if (!node.children || node.children.length === 0) {
    node.score = parseFloat((node.mean || 0).toFixed(2)); // Ensure leaf nodes without mean are handled and rounded to 2 decimal places
    return node.score;
  }

  // Recursive case: Calculate average mean values of all children
  let sum = 0;
  let count = 0;
  node.children.forEach((child) => {
    sum += averageMeanValues(child); // Recursively sum up the mean values
    count++; // Increment count for each child
  });

  // If the current node has its own mean, include it in the average calculation
  if (node.mean) {
    sum += parseFloat(node.mean.toFixed(2));
    count++; // Include the current node in the count if it has a mean
  }

  // Calculate the average, update the current node's score, and round to 2 decimal places
  node.score = count > 0 ? parseFloat((sum / count).toFixed(2)) : 0;

  return node.score;
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

function findMaxBranchLength(obj) {
  var maxLen = 0;
  for (var key in obj) {
    if (key === "branchLength" && obj[key] !== undefined) {
      maxLen = Math.max(maxLen, obj[key]);
    } else if (typeof obj[key] === "object") {
      maxLen = Math.max(maxLen, findMaxBranchLength(obj[key]));
    }
  }
  return maxLen;
}

function findMinBranchLength(obj) {
  var minLen = Infinity; // Initialize minLen to Infinity
  for (var key in obj) {
    if (key === "branchLength" && obj[key] !== undefined) {
      minLen = Math.min(minLen, obj[key]);
    } else if (typeof obj[key] === "object") {
      minLen = Math.min(minLen, findMinBranchLength(obj[key]));
    }
  }
  return minLen === Infinity ? 0 : minLen; // Return 0 if minLen is Infinity, indicating no branchLength was found
}
// This function calculates the maximum depth of a phylogenetic tree.
// The tree is assumed to be represented as a nested object structure where each node has a 'children' property,
// which is an array of its child nodes. Leaf nodes do not have a 'children' property.
// The depth of the root node is considered 0.
function findMaxPathTotalDepthLength(node) {
  let maxDepthLength = 0;

  function traverse(currentNode, currentDepthLength) {
    if (currentNode.children && currentNode.children.length > 0) {
      currentNode.children.forEach((child) => {
        const newDepthLength =
          currentDepthLength + (currentNode.branchLength || 0);
        traverse(child, newDepthLength);
      });
    } else {
      // Leaf node, update maxDepthLength if currentDepthLength is greater
      maxDepthLength = Math.max(maxDepthLength, currentDepthLength);
    }
  }

  traverse(node, 0);
  return maxDepthLength;
}
function collectScores(node) {
  let scores = [];
  if (node.hasOwnProperty("score") && typeof node.score === "number") {
    scores.push(node.score);
  }
  if (node.children) {
    node.children.forEach((child) => {
      const childScores = collectScores(child);
      if (childScores.length > 0) {
        scores = scores.concat(childScores);
      }
    });
  }
  return scores;
}
function drawChart(jsonValue) {
  leftSpeciesSelected = [];
  rightSpeciesSelected = [];
  speciesSelected = [];
  legendTextContainer = [];
  legendTextContainerAltFlag = false;
  legendTextContainerActivateFlag = false;
  speciesArray = [];
  /*
  traitValueString =
    '{"data":{"african_green_monkey":"scansorial","arctic_ground_squirrel":"other","armadillo":"fossorial","baboon":"other","capuchin":"scansorial","cat":"scansorial","chimp":"scansorial","coyote":"other","ferret":"other","galago":"scansorial","gorilla":"other","gray_mouse_lemur":"scansorial","human":"other","macaque_nemestrina":"scansorial","marmoset":"scansorial","mouse":"other","naked_mole_rat":"fossorial","opossum":"other","owl_monkey":"scansorial","pig":"other","rabbit":"fossorial","rat":"other","rhesus_macaque":"scansorial","south_american_squirrel_monkey":"scansorial","treeshrew":"scansorial"},"type":"lifestyle"}';
  traitValueStringFlag = true;
  */
  /*
  jsonStringReference =
    '{ "children": [ { "children": [ { "children": [ { "children": [ { "children": [ { "children": [ { "children": [ { "children": [ { "children": [ { "children": [ { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 5.7363, "mean":  0.24, "name": "chimp" }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 5.7363, "mean":  0.564, "name": "human" } ], "id": 1, "score": 1, "branchLength": 1.3997, "width": 1 }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 7.1361, "mean":  1.80, "name": "gorilla" } ], "id": 2, "score": 1, "branchLength": 20.7383, "width": 1 }, { "children": [ { "children": [ { "children": [ { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 1.8457, "mean":  0.006, "name": "rhesus_macaque" }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 1.8457, "mean":  0.55, "name": "macaque_nemestrina" } ], "id": 3, "score": 1, "branchLength": 4.1818, "width": 1 }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 6.0275, "mean":  0.234, "name": "baboon" } ], "id": 4, "score": 1, "branchLength": 2.5083, "width": 1 }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 8.5359, "mean":  1.870, "name": "african_green_monkey" } ], "id": 5, "score": 1, "branchLength": 19.3385, "width": 1 } ], "id": 6, "score": 1, "branchLength": 10.8321, "width": 1 }, { "children": [ { "children": [ { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 14.915, "mean":  0.0876, "name": "south_american_squirrel_monkey" }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 14.9151, "mean":  0.0076, "name": "capuchin" } ], "id": 7, "score": 1, "branchLength": 2.6597, "width": 1 }, { "children": [ { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 16.9859, "mean":  1.440, "name": "marmoset" }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 16.9859, "mean":  0.543, "name": "owl_monkey" } ], "id": 8, "score": 1, "branchLength": 0.5888, "width": 1 } ], "id": 9, "score": 1, "branchLength": 21.1318, "width": 1 } ], "id": 10, "score": 1, "branchLength": 24.7761, "width": 1 }, { "children": [ { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 50.7455, "mean":  0.5555, "name": "galago" }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 50.7453, "mean":  0.3456, "name": "gray_mouse_lemur" } ], "id": 11, "score": 1, "branchLength": 12.7371, "width": 1 } ], "id": 12, "score": 1, "branchLength": 15.9431, "width": 1 }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 79.4258, "mean":  1.80, "name": "treeshrew" } ], "id": 13, "score": 1, "branchLength": 2.6377, "width": 1 }, { "children": [ { "children": [ { "children": [ { "children": [ { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 15.2015, "mean":  0.00001, "name": "mouse" }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 15.2015, "mean":  0.110, "name": "rat" } ], "id": 14, "score": 1, "branchLength": 50.5322, "width": 1 }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 64.0935, "mean":  0.543, "name": "naked_mole_rat" } ], "id": 15, "score": 1, "branchLength": 64.0935, "width": 1 }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 1.64, "mean":  1.980, "name": "arctic_ground_squirrel" } ], "id": 16, "score": 1, "branchLength": 12.0554, "width": 1 }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 77.7891, "mean":  1.900, "name": "rabbit" } ], "id": 17, "score": 1, "branchLength": 4.2744, "width": 1 } ], "id": 18, "score": 1, "branchLength": 17.1999, "width": 1 }, { "children": [ { "children": [ { "children": [ { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 44.9114, "mean":  0.5432, "name": "ferret" }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 44.9114, "mean":  6.7650, "name": "coyote" } ], "id": 19, "score": 1, "branchLength": 10.0799, "width": 1 }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 54.9913, "mean":  0.86543, "name": "cat" } ], "id": 20, "score": 1, "branchLength": 29.862, "width": 1 }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 84.8534, "mean":  0.4567, "name": "pig" } ], "id": 21, "score": 1, "branchLength": 14.4099, "width": 1 } ], "id": 22, "score": 1, "branchLength": 4.5413, "width": 1 }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 103.8046, "mean":  7.0000, "name": "armadillo" } ], "id": 23, "score": 1, "branchLength": 68.9951, "width": 1 }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 172.8, "mean":  0.6598, "name": "opossum" } ], "id": 24, "score": 1, "branchLength": 1, "width": 1 }';
    log("jsonValue: " + jsonValue);
    log("jsonStringReference: " + jsonStringReference);
    */
    var jsonStringReference = jsonValue;
  if (jsonStringReference !== "") {
    var dataVariableReference = JSON.parse(jsonStringReference);

    maxNameLength = findMaxNameLength(dataVariableReference);
    maxBranchLength = findMaxBranchLength(dataVariableReference);
    minBranchLength = findMinBranchLength(dataVariableReference);
    maxTotalDepthWidth = findMaxPathTotalDepthLength(dataVariableReference);
    dataReference = copyDataWithCollapsedFlag(dataVariableReference);
    averageMeanValues(dataReference);

    const allScores = collectScores(dataReference);
    // Step 3: Calculate min and max scores
    mindistanceColor = d3.min(allScores);
    maxdistanceColor = d3.max(allScores);
    leftSpeciesSelected = [];
    rightSpeciesSelected = [];
    speciesSelected = [];
    if (dataReference != "") {
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
    if (dataReference != "") {
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
  if (dataReference != "") {
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
    if (dataReference != "") {
      generateVis();
    }
  }
}
function setLeafSelectionFromQT(speciesStringValue) {
  // log("showReferenceTreeValue: " + showReferenceTreeValue);
  //if (speciesStringValue !== "")
  {
    var speciesArray = speciesStringValue.split(" @%$,$%@ ");
    speciesSelected = speciesArray;
    if (dataReference != "") {
      generateVis();
    }
  }
}
function setDisableTraitOptions(traitHideFlag) {
    if (traitHideFlag == "True") {
        showTraitValues = false;
    }
    else {
        showTraitValues = true;
    }

    if (dataReference != "") {
        generateVis();
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
    if (dataReference != "") {
      generateVis();
    }
  }
}

function setTraitColor(setTraitValue) {
  // log("showReferenceTreeValue: " + showReferenceTreeValue);
  if (setTraitValue !== "" && dataReference != "") {
    traitValueColor = setTraitValue;
    traitValueColorFlag = true;
    generateVis();
  } else {
    traitValueColorFlag = false;
  }
}

function setTraitString(setTraitValue) {
  // log("showReferenceTreeValue: " + showReferenceTreeValue);
  if (setTraitValue !== "" && dataReference != "") {
    traitValueString = setTraitValue;
    traitValueStringFlag = true;

    generateVis();
  } else {
    traitValueStringFlag = false;
  }
}

function setTraitNumeric(setTraitValue) {
  // log("showReferenceTreeValue: " + showReferenceTreeValue);
  if (setTraitValue !== "" && dataReference != "") {
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
  if (setValue !== "" && dataReference != "") {
    clusteringMethod = setValue;
    generateVis();
  }
}
