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
let showExpandAll = true;
var colorMirror = false;
var isFirstRender = true;
var removeAnimation = true;
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
var altShiftPressed = false;
var removeSelection = false;
var splitGroupsAltKey = false;
var legendTextContainer = [];
var legendTextContainerActivateFlag = false;
//var expandedLeafNameID = "";
var svg;
var tooltipTextVal = "";
var maxBranchLength = 0;
var minBranchLength = 0;
var maxDepth = 0;
var eachDepthWidth = 0;
var maxTotalDepthWidth = 0;
var dataReference = "";
var typeOfColoringScore = "mean expression"; //mean or differential expression or rank or abundanceTop or abundanceMiddle
var geneName = "";
var middleAbundanceClusterName = "";
var clusterName = "";
var jsonValueStore;
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
function averageMeanValuesAllLeafChildren(node) {
    if (!node.children || node.children.length === 0) {
        // Leaf node
        node.score = parseFloat(node.mean.toFixed(2));
        return { totalMean: node.mean, count: 1 };
    } else {
        // Non-leaf node
        let totalMean = 0;
        let count = 0;
        for (let child of node.children) {
            let result = averageMeanValuesAllLeafChildren(child);
            totalMean += result.totalMean;
            count += result.count;
        }
        node.score = parseFloat((totalMean / count).toFixed(2));
        return { totalMean: totalMean, count: count };
    }
}
function averageDifferentialValuesAllLeafChildren(node) {
    if (!node.children || node.children.length === 0) {
        // Leaf node
        node.score = parseFloat(node.differential.toFixed(2));
        return { totalMean: node.differential, count: 1 };
    } else {
        // Non-leaf node
        let totalMean = 0;
        let count = 0;
        for (let child of node.children) {
            let result = averageDifferentialValuesAllLeafChildren(child);
            totalMean += result.totalMean;
            count += result.count;
        }
        node.score = parseFloat((totalMean / count).toFixed(2));
        return { totalMean: totalMean, count: count };
    }
}

function averageAbundanceTopValuesAllLeafChildren(node) {
    if (!node.children || node.children.length === 0) {
        // Leaf node
        node.score = parseFloat(node.abundanceTop.toFixed(2));
        return { totalMean: node.abundanceTop, count: 1 };
    } else {
        // Non-leaf node
        let totalMean = 0;
        let count = 0;
        for (let child of node.children) {
            let result = averageAbundanceTopValuesAllLeafChildren(child);
            totalMean += result.totalMean;
            count += result.count;
        }
        node.score = parseFloat((totalMean / count).toFixed(2));
        return { totalMean: totalMean, count: count };
    }
}

function averageAbundanceMiddleValuesAllLeafChildren(node) {
    if (!node.children || node.children.length === 0) {
        // Leaf node
        node.score = parseFloat(node.abundanceMiddle.toFixed(2));
        return { totalMean: node.abundanceMiddle, count: 1 };
    } else {
        // Non-leaf node
        let totalMean = 0;
        let count = 0;
        for (let child of node.children) {
            let result = averageAbundanceMiddleValuesAllLeafChildren(child);
            totalMean += result.totalMean;
            count += result.count;
        }
        node.score = parseFloat((totalMean / count).toFixed(2));
        return { totalMean: totalMean, count: count };
    }
}



function averageRankValuesAllLeafChildren(node) {
    if (!node.children || node.children.length === 0) {
        // Leaf node
        node.score = parseFloat(node.rank.toFixed(2));
        return { totalMean: node.rank, count: 1 };
    } else {
        // Non-leaf node
        let totalMean = 0;
        let count = 0;
        for (let child of node.children) {
            let result = averageRankValuesAllLeafChildren(child);
            totalMean += result.totalMean;
            count += result.count;
        }
        node.score = parseInt((totalMean / count));
        return { totalMean: totalMean, count: count };
    }
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

function copyDataWithCollapsedFlag(node) {
    let copy = { ...node };

    if ("score" in node) {
        copy.iscollapsed = node.score < -10; //=== 1;
    }

    if (node.children) {
        copy.children = node.children.map(copyDataWithCollapsedFlag);
    }

    return copy;
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
    speciesSelected = [];
    legendTextContainer = [];
    legendTextContainerActivateFlag = false;
    speciesArray = [];
    jsonValueStore = jsonValue;
    var jsonStringReference;
    if (isDebug) {
        //jsonStringReference ='{ "children": [ { "children": [ { "children": [ { "children": [ { "children": [ { "children": [ { "children": [ { "children": [ { "children": [ { "children": [ { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 5.7363, "mean":  0.24, "cellCounts": 10, "name": "chimp" }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 5.7363, "mean":  0.564, "cellCounts": 10, "name": "human" } ], "id": 1, "score": 1, "branchLength": 1.3997, "width": 1 }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 7.1361, "mean":  1.80, "cellCounts": 10, "name": "gorilla" } ], "id": 2, "score": 1, "branchLength": 20.7383, "width": 1 }, { "children": [ { "children": [ { "children": [ { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 1.8457, "mean":  0.006, "cellCounts": 10, "name": "rhesus_macaque" }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 1.8457, "mean":  0.55, "cellCounts": 10, "name": "macaque_nemestrina" } ], "id": 3, "score": 1, "branchLength": 4.1818, "width": 1 }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 6.0275, "mean":  0.234, "cellCounts": 10, "name": "baboon" } ], "id": 4, "score": 1, "branchLength": 2.5083, "width": 1 }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 8.5359, "mean":  1.870, "cellCounts": 10, "name": "african_green_monkey" } ], "id": 5, "score": 1, "branchLength": 19.3385, "width": 1 } ], "id": 6, "score": 1, "branchLength": 10.8321, "width": 1 }, { "children": [ { "children": [ { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 14.915, "mean":  0.0876, "cellCounts": 10, "name": "south_american_squirrel_monkey" }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 14.9151, "mean":  0.0076, "cellCounts": 10, "name": "capuchin" } ], "id": 7, "score": 1, "branchLength": 2.6597, "width": 1 }, { "children": [ { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 16.9859, "mean":  1.440, "cellCounts": 10, "name": "marmoset" }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 16.9859, "mean":  0.543, "cellCounts": 10, "name": "owl_monkey" } ], "id": 8, "score": 1, "branchLength": 0.5888, "width": 1 } ], "id": 9, "score": 1, "branchLength": 21.1318, "width": 1 } ], "id": 10, "score": 1, "branchLength": 24.7761, "width": 1 }, { "children": [ { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 50.7455, "mean":  0.5555, "cellCounts": 10, "name": "galago" }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 50.7453, "mean":  0.3456, "cellCounts": 10, "name": "gray_mouse_lemur" } ], "id": 11, "score": 1, "branchLength": 12.7371, "width": 1 } ], "id": 12, "score": 1, "branchLength": 15.9431, "width": 1 }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 79.4258, "mean":  1.80, "cellCounts": 10, "name": "treeshrew" } ], "id": 13, "score": 1, "branchLength": 2.6377, "width": 1 }, { "children": [ { "children": [ { "children": [ { "children": [ { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 15.2015, "mean":  0.00001, "cellCounts": 10, "name": "mouse" }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 15.2015, "mean":  0.110, "cellCounts": 10, "name": "rat" } ], "id": 14, "score": 1, "branchLength": 50.5322, "width": 1 }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 64.0935, "mean":  0.543, "cellCounts": 10, "name": "naked_mole_rat" } ], "id": 15, "score": 1, "branchLength": 64.0935, "width": 1 }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 1.64, "mean":  1.980, "cellCounts": 10, "name": "arctic_ground_squirrel" } ], "id": 16, "score": 1, "branchLength": 12.0554, "width": 1 }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 77.7891, "mean":  1.900, "cellCounts": 10, "name": "rabbit" } ], "id": 17, "score": 1, "branchLength": 4.2744, "width": 1 } ], "id": 18, "score": 1, "branchLength": 17.1999, "width": 1 }, { "children": [ { "children": [ { "children": [ { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 44.9114, "mean":  0.5432, "cellCounts": 10, "name": "ferret" }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 44.9114, "mean":  6.7650, "cellCounts": 10, "name": "coyote" } ], "id": 19, "score": 1, "branchLength": 10.0799, "width": 1 }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 54.9913, "mean":  0.86543, "cellCounts": 10, "name": "cat" } ], "id": 20, "score": 1, "branchLength": 29.862, "width": 1 }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 84.8534, "mean":  0.4567, "cellCounts": 10, "name": "pig" } ], "id": 21, "score": 1, "branchLength": 14.4099, "width": 1 } ], "id": 22, "score": 1, "branchLength": 4.5413, "width": 1 }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 103.8046, "mean":  7.0000, "cellCounts": 10, "name": "armadillo" } ], "id": 23, "score": 1, "branchLength": 68.9951, "width": 1 }, { "color": "#000000", "hastrait": true, "iscollapsed": false, "branchLength": 172.8, "mean":  0.6598, "cellCounts": 10, "name": "opossum" } ], "id": 24, "score": 1, "branchLength": 1, "width": 1 }';

        jsonStringReference = '{ "branchLength": 1, "children": [ { "branchLength": 68.9951, "children": [ { "branchLength": 4.5413, "children": [ { "branchLength": 17.1999, "children": [ { "branchLength": 2.6377, "children": [ { "branchLength": 15.9431, "children": [ { "branchLength": 24.7761, "children": [ { "branchLength": 10.8321, "children": [ { "branchLength": 20.7383, "children": [ { "branchLength": 1.3997, "children": [ { "abundanceMiddle": 1.82, "abundanceTop": 1.82, "branchLength": 5.7363, "cellCounts": 261, "color": "#000000", "differential": 0.01, "gene": "CADPS2", "hastrait": true, "iscollapsed": false, "mean": 0.15, "name": "chimp", "rank": 2588 }, { "abundanceMiddle": 0.32, "abundanceTop": 0.32, "branchLength": 5.7363, "cellCounts": 37, "color": "#000000", "differential": 0, "gene": "CADPS2", "hastrait": true, "iscollapsed": false, "mean": 0.06, "name": "human", "rank": 3611 } ], "id": 1, "score": 1, "width": 1 }, { "abundanceMiddle": 1.37, "abundanceTop": 1.37, "branchLength": 7.1361, "cellCounts": 202, "color": "#000000", "differential": 1.36, "gene": "CADPS2", "hastrait": true, "iscollapsed": false, "mean": 2.5, "name": "gorilla", "rank": 8 } ], "id": 2, "score": 1, "width": 1 }, { "branchLength": 19.3385, "children": [ { "branchLength": 2.5083, "children": [ { "branchLength": 4.1818, "children": [ { "abundanceMiddle": 1.37, "abundanceTop": 1.37, "branchLength": 1.8457, "cellCounts": 223, "color": "#000000", "differential": 0.01, "gene": "CADPS2", "hastrait": true, "iscollapsed": false, "mean": 0.36, "name": "rhesus_macaque", "rank": 2425 }, { "abundanceMiddle": 1.33, "abundanceTop": 1.33, "branchLength": 1.8457, "cellCounts": 217, "color": "#000000", "differential": 0.65, "gene": "CADPS2", "hastrait": true, "iscollapsed": false, "mean": 1.23, "name": "macaque_nemestrina", "rank": 5 } ], "id": 3, "score": 1, "width": 1 }, { "abundanceMiddle": 1.37, "abundanceTop": 1.37, "branchLength": 6.0275, "cellCounts": 213, "color": "#000000", "differential": -0.01, "gene": "CADPS2", "hastrait": true, "iscollapsed": false, "mean": 0.11, "name": "baboon", "rank": 5417 } ], "id": 4, "score": 1, "width": 1 }, { "abundanceMiddle": 1.37, "abundanceTop": 1.37, "branchLength": 8.5359, "cellCounts": 221, "color": "#000000", "differential": 0.05, "gene": "CADPS2", "hastrait": true, "iscollapsed": false, "mean": 0.35, "name": "african_green_monkey", "rank": 331 } ], "id": 5, "score": 1, "width": 1 } ], "id": 6, "score": 1, "width": 1 }, { "branchLength": 21.1318, "children": [ { "branchLength": 2.6597, "children": [ { "abundanceMiddle": 1.77, "abundanceTop": 1.77, "branchLength": 14.915, "cellCounts": 282, "color": "#000000", "differential": 0, "gene": "CADPS2", "hastrait": true, "iscollapsed": false, "mean": 0.08, "name": "south_american_squirrel_monkey", "rank": 3675 }, { "abundanceMiddle": 0.47, "abundanceTop": 0.47, "branchLength": 14.9151, "cellCounts": 53, "color": "#000000", "differential": -0.02, "gene": "CADPS2", "hastrait": true, "iscollapsed": false, "mean": 0.15, "name": "capuchin", "rank": 5797 } ], "id": 7, "score": 1, "width": 1 }, { "branchLength": 0.5888, "children": [ { "abundanceMiddle": 1.12, "abundanceTop": 1.12, "branchLength": 16.9859, "cellCounts": 98, "color": "#000000", "differential": 0.02, "gene": "CADPS2", "hastrait": true, "iscollapsed": false, "mean": 0.12, "name": "marmoset", "rank": 1534 }, { "abundanceMiddle": 0.82, "abundanceTop": 0.82, "branchLength": 16.9859, "cellCounts": 122, "color": "#000000", "differential": 0.02, "gene": "CADPS2", "hastrait": true, "iscollapsed": false, "mean": 0.17, "name": "owl_monkey", "rank": 2790 } ], "id": 8, "score": 1, "width": 1 } ], "id": 9, "score": 1, "width": 1 } ], "id": 10, "score": 1, "width": 1 }, { "branchLength": 12.7371, "children": [ { "abundanceMiddle": 0, "abundanceTop": 0, "branchLength": 50.7455, "cellCounts": 0, "color": "#000000", "differential": -0.16, "gene": "CADPS2", "hastrait": true, "iscollapsed": false, "mean": 0, "name": "galago", "rank": 3688 }, { "abundanceMiddle": 0.07, "abundanceTop": 0.07, "branchLength": 50.7453, "cellCounts": 6, "color": "#000000", "differential": -0.03, "gene": "CADPS2", "hastrait": true, "iscollapsed": false, "mean": 0.12, "name": "gray_mouse_lemur", "rank": 4804 } ], "id": 11, "score": 1, "width": 1 } ], "id": 12, "score": 1, "width": 1 }, { "abundanceMiddle": 0, "abundanceTop": 0, "branchLength": 79.4258, "cellCounts": 0, "color": "#000000", "differential": -0.06, "gene": "CADPS2", "hastrait": true, "iscollapsed": false, "mean": 0, "name": "treeshrew", "rank": 2026 } ], "id": 13, "score": 1, "width": 1 }, { "branchLength": 4.2744, "children": [ { "branchLength": 12.0554, "children": [ { "branchLength": 64.0935, "children": [ { "branchLength": 50.5322, "children": [ { "abundanceMiddle": 0.04, "abundanceTop": 0.04, "branchLength": 15.2015, "cellCounts": 8, "color": "#000000", "differential": 0, "gene": "CADPS2", "hastrait": true, "iscollapsed": false, "mean": 0.19, "name": "mouse", "rank": 3377 }, { "abundanceMiddle": 0, "abundanceTop": 0, "branchLength": 15.2015, "cellCounts": 0, "color": "#000000", "differential": -0.28, "gene": "CADPS2", "hastrait": true, "iscollapsed": false, "mean": 0, "name": "rat", "rank": 4550 } ], "id": 14, "score": 1, "width": 1 }, { "abundanceMiddle": 2.87, "abundanceTop": 2.87, "branchLength": 64.0935, "cellCounts": 75, "color": "#000000", "differential": 0.01, "gene": "CADPS2", "hastrait": true, "iscollapsed": false, "mean": 1.66, "name": "naked_mole_rat", "rank": 2697 } ], "id": 15, "score": 1, "width": 1 }, { "abundanceMiddle": 0, "abundanceTop": 0, "branchLength": 1.64, "cellCounts": 0, "color": "#000000", "differential": -0.14, "gene": "CADPS2", "hastrait": true, "iscollapsed": false, "mean": 0, "name": "arctic_ground_squirrel", "rank": 3581 } ], "id": 16, "score": 1, "width": 1 }, { "abundanceMiddle": 0, "abundanceTop": 0, "branchLength": 77.7891, "cellCounts": 0, "color": "#000000", "differential": -0.06, "gene": "CADPS2", "hastrait": true, "iscollapsed": false, "mean": 0, "name": "rabbit", "rank": 2320 } ], "id": 17, "score": 1, "width": 1 } ], "id": 18, "score": 1, "width": 1 }, { "branchLength": 14.4099, "children": [ { "branchLength": 29.862, "children": [ { "branchLength": 10.0799, "children": [ { "abundanceMiddle": 0.17, "abundanceTop": 0.17, "branchLength": 44.9114, "cellCounts": 30, "color": "#000000", "differential": 0.93, "gene": "CADPS2", "hastrait": true, "iscollapsed": false, "mean": 2, "name": "ferret", "rank": 50 }, { "abundanceMiddle": 0.97, "abundanceTop": 0.97, "branchLength": 44.9114, "cellCounts": 135, "color": "#000000", "differential": 2.07, "gene": "CADPS2", "hastrait": true, "iscollapsed": false, "mean": 2.72, "name": "coyote", "rank": 2 } ], "id": 19, "score": 1, "width": 1 }, { "abundanceMiddle": 0.21, "abundanceTop": 0.21, "branchLength": 54.9913, "cellCounts": 35, "color": "#000000", "differential": 1.55, "gene": "CADPS2", "hastrait": true, "iscollapsed": false, "mean": 2.33, "name": "cat", "rank": 5 } ], "id": 20, "score": 1, "width": 1 }, { "abundanceMiddle": 1.1, "abundanceTop": 1.1, "branchLength": 84.8534, "cellCounts": 130, "color": "#000000", "differential": 1.88, "gene": "CADPS2", "hastrait": true, "iscollapsed": false, "mean": 2.79, "name": "pig", "rank": 2 } ], "id": 21, "score": 1, "width": 1 } ], "id": 22, "score": 1, "width": 1 }, { "abundanceMiddle": 0.48, "abundanceTop": 0.48, "branchLength": 103.8046, "cellCounts": 78, "color": "#000000", "differential": -0.03, "gene": "CADPS2", "hastrait": true, "iscollapsed": false, "mean": 0.06, "name": "armadillo", "rank": 6698 } ], "id": 23, "score": 1, "width": 1 }, { "abundanceMiddle": 0.03, "abundanceTop": 0.02, "branchLength": 172.8, "cellCounts": 2, "color": "#000000", "differential": -0.12, "gene": "CADPS2", "hastrait": true, "iscollapsed": false, "mean": 0, "name": "opossum", "rank": 6046 } ], "id": 24, "score": 1, "width": 1 }';
        traitValueString =
            '{"data":{"african_green_monkey":"scansorial","arctic_ground_squirrel":"other","armadillo":"fossorial","baboon":"other","capuchin":"scansorial","cat":"scansorial","chimp":"scansorial","coyote":"other","ferret":"other","galago":"scansorial","gorilla":"other","gray_mouse_lemur":"scansorial","human":"other","macaque_nemestrina":"scansorial","marmoset":"scansorial","mouse":"other","naked_mole_rat":"fossorial","opossum":"other","owl_monkey":"scansorial","pig":"other","rabbit":"fossorial","rat":"other","rhesus_macaque":"scansorial","south_american_squirrel_monkey":"scansorial","treeshrew":"scansorial"},"type":"lifestyle"}';
        traitValueStringFlag = true;
        showTraitValues = true;
    } else {
        jsonStringReference = jsonValueStore;
    }

    if (jsonStringReference !== "") {
        var dataVariableReference = JSON.parse(jsonStringReference);

        maxNameLength = findMaxNameLength(dataVariableReference);
        maxBranchLength = findMaxBranchLength(dataVariableReference);
        minBranchLength = findMinBranchLength(dataVariableReference);
        maxTotalDepthWidth = findMaxPathTotalDepthLength(dataVariableReference);
        dataReference = copyDataWithCollapsedFlag(dataVariableReference);

        if (typeOfColoringScore == "rank") {
            averageRankValuesAllLeafChildren(dataReference);
            tooltipTextVal = "Appearance rank for " + geneName + " in " + clusterName;
        }
        else if (typeOfColoringScore == "differential expression") {
            averageDifferentialValuesAllLeafChildren(dataReference);
            tooltipTextVal = "Mean differential expression for " + geneName + " in " + clusterName;
        }
        else if (typeOfColoringScore == "abundanceTop") {
            averageAbundanceTopValuesAllLeafChildren(dataReference);
            tooltipTextVal = "Fraction of neuronal cells in " + clusterName;
        }
        else if (typeOfColoringScore == "abundanceMiddle") {
            averageAbundanceMiddleValuesAllLeafChildren(dataReference);
            tooltipTextVal = "Fraction of " + middleAbundanceClusterName + " cells in " + clusterName;
        }
        else {
            averageMeanValuesAllLeafChildren(dataReference);
            tooltipTextVal = "Mean selected cells for " + geneName+" in " + clusterName;
        }

        geneName = dataVariableReference.children[0].children[0].children[0].children[0].children[0].children[0].children[0].children[0].children[0].children[0].gene;
        const allScores = collectScores(dataReference);
        // Step 3: Calculate min and max scores
        mindistanceColor = d3.min(allScores);
        maxdistanceColor = d3.max(allScores);
        speciesSelected = [];
        if (dataReference != "") {
            generateVis();
        }
    }
}

function removeSelectionFromJavascript(d) {
    if ((d = "T")) {
        removeSelection = true;
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

function setLeafSelectionFromQT(speciesStringValue) {
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
    } else {
        showTraitValues = true;
    }

    if (dataReference != "") {
        generateVis();
    }
}
/*
function setExpandAll(setExpandAllValue) {
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
*/
function setTraitColor(setTraitValue) {
    if (setTraitValue !== "" && dataReference != "") {
        traitValueColor = setTraitValue;
        traitValueColorFlag = true;
        generateVis();
    } else {
        traitValueColorFlag = false;
    }
}

function setTraitString(setTraitValue) {
    if (setTraitValue !== "" && dataReference != "") {
        traitValueString = setTraitValue;
        traitValueStringFlag = true;

        generateVis();
    } else {
        traitValueStringFlag = false;
    }
}

function setTraitNumeric(setTraitValue) {
    if (setTraitValue !== "" && dataReference != "") {
        traitValueNumeric = setTraitValue;

        traitValueNumericFlag = true;
        generateVis();
    } else {
        traitValueNumericFlag = false;
    }
}
function setClusterMethod(setValue) {
    if (setValue !== "" && dataReference != "") {
        clusteringMethod = setValue;
        generateVis();
    }
}

if (isDebug) {
    drawChart();
}