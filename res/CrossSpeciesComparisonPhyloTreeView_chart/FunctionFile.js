function assert(condition, message) {
  if (!condition) {
    throw new Error(message || "Assertion failed");
  }
}

function UpdateOverviewMeasures() {
  // calculate comparison measures for each pair of trees
  m_aComparisonOverviewMeasures = new Array(); //array of arrays to store the comparison measures of each tree
  m_aScoreDistribution = new Array(); //array of arrays to store the score distribution of each tree
  m_aAverageScore = new Array(); //array of arrays to store the average score of each tree
  allScoresStorage = new Array(); //array of arrays to store all scores of each tree

  var nReferenceTree = 0; //reference tree is always the first tree

  m_aComparisonOverviewMeasures[nReferenceTree] = new Array(); //array to store the comparison measures of the reference tree
  m_aScoreDistribution[nReferenceTree] = new Array(); //array to store the score distribution of the reference tree
  m_aAverageScore[nReferenceTree] = new Array(); //array to store the average score of the reference tree

  for (
    //calculate comparison measures for each pair of trees
    var nCompareTree = 0;
    nCompareTree < m_aLoadedTrees.length;
    nCompareTree++
  ) {
    m_aScoreDistribution[nReferenceTree][nCompareTree] = new Array();
    m_aAverageScore[nReferenceTree][nCompareTree] = 0;
    var tempScoreValues = [];
    for (var i = 0; i < 10; i++)
      m_aScoreDistribution[nReferenceTree][nCompareTree][i] = 0;

    //calculate score distributions
    //get all nodes of the comparing tree
    var aNodeList = m_aLoadedTrees[nCompareTree].GetNodeList();
    //compare all nodes of the comparing tree with the reference tree and store the distributions
    for (
      // iterate over all nodes of the comparing tree
      var nCompareNode = 0;
      nCompareNode < aNodeList.length;
      nCompareNode++
    ) {
      var resultTree = undefined;
      var fMeasure = undefined;

      assert(
        m_sGlobalMeasure == "leaf" ||
          m_sGlobalMeasure == "element" ||
          m_sGlobalMeasure == "edge",
        "Wrong measure set"
      ); // check for mesaure set

      if (m_sGlobalMeasure == "leaf") {
        resultTree = Vis2LeafMeasure(
          m_aLoadedTrees[nReferenceTree],
          aNodeList[nCompareNode]
        );
        fMeasure = resultTree.leafmeasure;
      } else if (m_sGlobalMeasure == "element") {
        // for element mesaure,
        resultTree = Vis2ElementMeasure(
          m_aLoadedTrees[nReferenceTree],
          aNodeList[nCompareNode]
        );
        fMeasure = resultTree.elementmeasure;
      } else if (m_sGlobalMeasure == "edge") {
        resultTree = Vis2EdgeMeasure(
          m_aLoadedTrees[nReferenceTree],
          aNodeList[nCompareNode]
        );
        fMeasure = resultTree.edgemeasure;
      } else assert(false, "no valid measure selected!");

      assert(resultTree != undefined, "resultTree not assigned");

      var value = Math.floor(fMeasure * 10);
      if (value == 10) value--;

      m_aScoreDistribution[nReferenceTree][nCompareTree][value] +=
        1 / aNodeList.length;

      m_aAverageScore[nReferenceTree][nCompareTree] += fMeasure;
      //log(aNodeList[nCompareNode]);
      //log(m_aLoadedTrees[nReferenceTree]);
      //log(fMeasure);
      tempScoreValues.push(fMeasure);
    }
    allScoresStorage.push(tempScoreValues);
    m_aAverageScore[nReferenceTree][nCompareTree] /= aNodeList.length;
  }
}

// function ItlParseString(sSubString) {
//   // TODO: handling wrong formated input

//   // debug how this method is called
//   // Debugger.log("ParseString('" + sSubString + "')");

//   // create new node
//   var rNewNode = new Vis2Node();
//   rNewNode.id = currId;
//   currId++;

//   // get positions of first bracket or '"'
//   var iPosOfFirstBracket = sSubString.indexOf("[");
//   var iPosOfFirstHochKomma = sSubString.indexOf('"');

//   // determinate if it is maybe a node, or a leaf
//   bMaybeNode = iPosOfFirstBracket != -1;
//   bMaybeLeaf = iPosOfFirstHochKomma != -1;

//   if (bMaybeNode == true) {
//     // Debugger.log("is node");
//     rNewNode.isleaf = false;

//     var iStartPos = iPosOfFirstBracket;
//     var iBrackets = 0;

//     // process string. for each [, increment iBrackets, for each ],
//     // decrement - if iBrackets==0,
//     // a substring containing a child node is found
//     for (var i = iStartPos; i < sSubString.length; i++) {
//       if (sSubString.charAt(i) == "[") iBrackets++;
//       if (sSubString.charAt(i) == "]") iBrackets--;

//       // must not happen
//       assert(
//         iBrackets >= 0,
//         "Too many closing brackets, wrong input string?"
//       );

//       if (iBrackets == 0) {
//         // get pos of semikolon
//         var iPosOfSemikolon = sSubString.indexOf(";", iStartPos);

//         assert(
//           iPosOfSemikolon != -1,
//           '";" expected but not found! Wrong input string?'
//         );

//         // extract the substring for the child node, without the edge
//         // length information
//         var sChildSubString = sSubString.substr(
//           iPosOfSemikolon + 1,
//           i - iPosOfSemikolon - 1
//         );

//         // extract the length information as substring
//         var sLengthOfEdgeString = sSubString.substr(
//           iStartPos + 1,
//           iPosOfSemikolon - iStartPos - 1
//         );

//         // convert substring to integer
//         var iLengthOfEdge = parseInt(sLengthOfEdgeString);

//         // Debugger.log("iLengthOfEdge = " + iLengthOfEdge);

//         // call ParseString recursively for extracted child substring
//         var rNewChildNode = ItlParseString(sChildSubString);

//         // add child node to current node
//         rNewNode.AddChild(rNewChildNode, iLengthOfEdge);

//         // set new starting position
//         iStartPos = i + 1;
//       }
//     }
//   } else if (bMaybeLeaf == true) {
//     // Debugger.log("is leaf");

//     rNewNode.isleaf = true;

//     rNewNode.name = sSubString.substr(1, sSubString.length - 2);
//   }

//   return rNewNode;
// }

// function CreateTreeFromString(sString) {
//   return ItlParseString(sString);
// }

function Vis2Node() {
  /*
   * Members
   */

  // ID
  this.id;

  // Name of the node (e.g. "A")
  this.name;

  // Parent of the node
  this.parent = undefined;

  // Weight of the edge between parent and this node
  this.edgeweight;

  // Accumulated weight of all edges between root node and this node
  this.accedgeweight = 0;

  // Array of the children
  this.children = new Array();

  // Variables needed for coordinates/drawing
  this.leftSpaceNeeded;
  this.rightSpaceNeeded;
  this.leftLineLength;
  this.rightLineLength;

  // variables for holding measures
  this.leafmeasure;
  this.elementmeasure;
  this.edgemeasure;
  this.averageleafmeasure;
  this.averageelementmeasure;
  this.averageedgemeasure;

  this.bIsSelected = false;

  this.bIsCollapsed = false;

  /*
   * Functions
   */

  // Adds a child to this node
  this.AddChild = function (child, weight) {
    // Debugger.log("Adding child (" + child.name + ", " + weight + ", "
    // + (child.isleaf ? "isleaf" : "isnode") + ")");
    this.children.push(child);
    child.parent = this;
    child.edgeweight = weight;
    child.accedgeweight = this.accedgeweight + weight;
  };

  // Returns the number of children of this node
  this.GetChildrenCount = function () {
    return this.children.length;
  };

  // returns an array list with all nodes under the current node
  this.GetNodeList = function () {
    var ReturnList = new Array();

    ReturnList.push(this);

    for (var i = 0; i < this.children.length; i++) {
      var child = this.children[i];

      if (child.GetChildrenCount() != 0) {
        ReturnList = ReturnList.concat(child.GetNodeList());
        // ReturnList.push(child);
      }
    }

    return ReturnList;
  };
}

function GetLeaves(rNode) {
  var vLeaves = new Array();

  // return name, if this is a leaf
  if (rNode.isleaf == true) vLeaves.push(rNode.name);
  else {
    // collect leaves under child nodes, if this is not a leaf
    for (var i = 0; i < rNode.GetChildrenCount(); i++) {
      var vLeavesOfChildNode = GetLeaves(rNode.children[i]);

      for (var j = 0; j < vLeavesOfChildNode.length; j++)
        vLeaves.push(vLeavesOfChildNode[j]);
    }
  }

  return vLeaves;
}

function GetElements_Worker(rNode, rElements, rLeaves) {
  // return name, if this is a leaf
  if (rNode.isleaf == true) {
    var bFound = false;

    for (var i = 0; i < rLeaves.length; i++) {
      if (rLeaves[i] == rNode.name) bFound = true;
    }

    if (bFound == false) rLeaves.push(rNode.name);
  } else {
    // collect leaves under child nodes, if this is not a leaf
    for (var i = 0; i < rNode.GetChildrenCount(); i++) {
      var vLeavesOfChildNode = GetLeaves(rNode.children[i]);

      GetElements_Worker(rNode.children[i], rElements, rLeaves);

      if (vLeavesOfChildNode.length > 1) rElements.push(vLeavesOfChildNode);
    }
  }
}

function GetElements(rNode) {
  var vElements = new Array();
  var vLeaves = new Array();

  // get elements and leaves by (recursive) worker method
  GetElements_Worker(rNode, vElements, vLeaves);

  // put them together in 1 array
  for (var j = 0; j < vElements.length; j++) vLeaves.push(vElements[j]);

  // return vLeaves, now contains leaves+elements
  return vLeaves;
}

function GetElementMeasure(rNode1, rNode2) {
  // for element mesaure,
  // get elements under node 1
  var vElementsOfNode1 = GetElements(rNode1);

  // get elements under node 2
  var vElementsOfNode2 = GetElements(rNode2);

  var iSameElements = 0; // number of same elements
  var iTotal = vElementsOfNode1.length + vElementsOfNode2.length; // total number of elements

  for (var i = 0; i < vElementsOfNode1.length; i++) {
    // iterate over all elements of node 1
    var bFound = false; // flag if element is found in node 2

    for (var j = 0; j < vElementsOfNode2.length; j++) {
      // iterate over all elements of node 2
      if (
        // check if element is found in node 2
        typeof vElementsOfNode2[j] == "object" &&
        typeof vElementsOfNode1[i] == "object"
      ) {
        // check if element is found in node 2
        var vSubArray1 = vElementsOfNode2[j]; // get sub array of node 2
        var vSubArray2 = vElementsOfNode1[i]; // get sub array of node 1

        if (vSubArray1.length == vSubArray2.length) {
          var bArraysIdent = true; // flag if arrays are identical

          for (
            var k = 0;
            k < vSubArray1.length;
            k++ // iterate over all elements of sub arrays
          )
            if (vSubArray1[k] != vSubArray2[k]) bArraysIdent = false; // check if arrays are identical

          if (bArraysIdent) {
            // check if arrays are identical
            bFound = true; // set flag if arrays are identical
            break; // break if arrays are identical
          }
        }
      } else if (vElementsOfNode2[j] == vElementsOfNode1[i]) {
        // check if element is found in node 2
        bFound = true; // set flag if element is found in node 2
        break; // break if element is found in node 2
      }
    }

    if (bFound == true) {
      // check if element is found in node 2
      iSameElements++; // increment number of same elements
      iTotal--; // decrement total number of elements
    }
  }

  return iSameElements / iTotal; // return element measure
}

function Vis2ElementMeasure(rReferenceTree, rTestTree) {
  // for element mesaure,
  var rNodesQueue = new Array(); // queue to store nodes of the comparing tree
  var rNodesTotal = new Array(); // array to store all nodes of the comparing tree
  var rNodesTotalReference = rReferenceTree.GetNodeList(); // get all nodes of the reference tree

  rNodesQueue.push(rTestTree); // add root node to queue

  while (rNodesQueue.length > 0) {
    // iterate over all nodes of the comparing tree
    var rCurrentNode = rNodesQueue.shift(); // get first node in queue

    rNodesTotal.push(rCurrentNode); // add node to total list

    for (var i = 0; i < rCurrentNode.GetChildrenCount(); i++) {
      // add children to queue
      /** @define {Vis2Node} */ // get child
      var child = rCurrentNode.children[i]; // get child

      if (child.isleaf == false) rNodesQueue.push(child); // add child to queue
    }
  }
  // calculate element measure for each node
  for (var iNode = 0; iNode < rNodesTotal.length; iNode++) {
    // iterate over all nodes of the comparing tree
    var fMaximum = 0; // maximum measure
    // calculate element measure for each node
    for (
      var iReferenceNode = 0;
      iReferenceNode < rNodesTotalReference.length;
      iReferenceNode++
    ) {
      // iterate over all nodes of the reference tree
      var fMeasure = GetElementMeasure(
        rNodesTotalReference[iReferenceNode],
        rNodesTotal[iNode]
      ); // get element measure
      if (fMeasure > fMaximum) fMaximum = fMeasure; // update maximum measure
    } // end for iReferenceNode

    rNodesTotal[iNode].elementmeasure = fMaximum; // set element measure
  } // end for iNode

  return rTestTree; // return tree with element measures
}

function CreateTreeFromData(data) {
  return ItlCreateNodeFromData(data);
}

function ItlCreateNodeFromData(data) {
  var rNewNode = new Vis2Node();
  rNewNode.id = data.id;

  if (data.children) {
    rNewNode.isleaf = false;

    for (var i = 0; i < data.children.length; i++) {
      var childData = data.children[i];
      var rNewChildNode = ItlCreateNodeFromData(childData);

      // Assuming there's no edge weight information in the data
      var weight = 1;

      rNewNode.AddChild(rNewChildNode, weight);
    }
  } else if (data.name) {
    rNewNode.isleaf = true;
    rNewNode.name = data.name;
  }

  return rNewNode;
}

function checkNumberNodes(node, count = 0) {
  if (!node.isleaf) {
    count = count + 1;
  }
  for (var i = 0; i < node.children.length; i++) {
    count = checkNumberNodes(node.children[i], count);
  }
  return count;
}

function createDataFromTree(node, scores) {
  var newData = {
    id: node.id,
  };

  // Add scores only for non-leaf nodes
  if (!node.isleaf) {
    newData.score = parseFloat(scores.shift().toFixed(1)); // Use 1 decimal place for scores
  }

  if (node.children.length > 0) {
    newData.children = node.children.map((child) =>
      createDataFromTree(child, scores)
    );
  }

  // Add names to leaf nodes
  if (node.isleaf && node.name) {
    return { id: node.id, name: node.name }; // Return an object with id and name for leaf nodes
  }

  return newData;
}

function rearrangeStructure(node, shuffleProbability = 1) {
  if (!node.children || node.children.length === 0) {
    return node;
  }

  // Separate leaf and non-leaf nodes
  const leafNodes = node.children.filter((child) => child.isleaf);
  const nonLeafNodes = node.children.filter((child) => !child.isleaf);

  // Shuffle leaf and non-leaf nodes independently
  const shuffledLeafNodes = randomRearrange(leafNodes);
  const shuffledNonLeafNodes = randomRearrange(nonLeafNodes);

  // Combine shuffled nodes
  node.children = shuffleSiblings(
    shuffledNonLeafNodes.concat(shuffledLeafNodes)
  );

  // Recursively rearrange structure for each child node
  node.children.forEach((child) =>
    rearrangeStructure(child, shuffleProbability)
  );

  return node;
}

// Helper function to randomly rearrange an array
function randomRearrange(array) {
  const rearrangedArray = [];

  while (array.length > 0) {
    const index = Math.floor(Math.random() * array.length);
    rearrangedArray.push(array.splice(index, 1)[0]);
  }

  return rearrangedArray;
}

// Helper function to shuffle siblings independently
function shuffleSiblings(children) {
  const shuffledChildren = [...children];

  for (let i = 0; i < shuffledChildren.length - 1; i += 2) {
    // Shuffle each pair of siblings independently
    const index = i + Math.floor(Math.random() * (shuffledChildren.length - i));
    const temp = shuffledChildren[i];
    shuffledChildren[i] = shuffledChildren[index];
    shuffledChildren[index] = temp;
  }

  return shuffledChildren;
}

//function compareTrees(tree1, tree2, differences) {
//   if (!tree1 && !tree2) {
//     return;
//   }

//   if (!tree1 || !tree2) {
//     differences.push("One tree is null, the other is not.");
//     return;
//   }

//   if (tree1.isleaf !== tree2.isleaf || tree1.name !== tree2.name) {
//     differences.push("Node: " + tree1.name + " is different.");
//   }

//   if (tree1.children.length !== tree2.children.length) {
//     differences.push(
//       "Number of children for node " + tree1.name + " is different."
//     );
//   }

//   for (
//     var i = 0;
//     i < tree1.children.length && i < tree2.children.length;
//     i++
//   ) {
//     compareTrees(tree1.children[i], tree2.children[i], differences);
//   }
// }

// function areTreesEqual(tree1, tree2) {
//   if (!tree1 && !tree2) {
//     return true;
//   }

//   if (!tree1 || !tree2) {
//     return false;
//   }

//   if (tree1.isleaf !== tree2.isleaf || tree1.name !== tree2.name) {
//     return false;
//   }

//   if (tree1.children.length !== tree2.children.length) {
//     return false;
//   }

//   for (var i = 0; i < tree1.children.length; i++) {
//     if (!areTreesEqual(tree1.children[i], tree2.children[i])) {
//       return false;
//     }
//   }

//   return true;
// }

// function printTree(node, indent) {
//   indent = indent || 0;
//   var indentation = "  ".repeat(indent);

//   log(indentation + (node.isleaf ? "Leaf: " : "Node: ") + node.name);

//   for (var i = 0; i < node.children.length; i++) {
//     printTree(node.children[i], indent + 1);
//   }
// }

function convertClusterToDesiredFormat(cluster) {
  // Base case: if the cluster is a leaf node (i.e., it has no left or right child)

  if (!cluster.left && !cluster.right) {
    return {
      id: cluster.id.toString(),

      name: cluster.speciesNames[0],

      iscollapsed: false,
    };
  }

  // Recursive case: if the cluster is not a leaf node, call the function recursively on the left and right children

  return {
    score: cluster.score,

    children: [
      convertClusterToDesiredFormat(cluster.left),

      convertClusterToDesiredFormat(cluster.right),
    ],

    iscollapsed: true,
  };
}

function replacer(key, value) {
  if (key === "parent") {
    return;
  }
  return value;
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
