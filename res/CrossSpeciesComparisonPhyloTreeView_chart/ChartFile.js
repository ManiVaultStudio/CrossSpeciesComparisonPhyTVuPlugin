function updateHastrait(node, traitValue, traitjson) {
  if (node.children) {
    node.children.forEach((child) => {
      updateHastrait(child, traitValue, traitjson);
    });
  }
  if (!node.hastrait) {
    node.hastrait = false; // If hastrait key is not present, add it with a default value
  }
  if (traitjson[traitValue] && traitjson[traitValue][node.name]) {
    node.hastrait = traitjson[traitValue][node.name].isPresent;
  }
}
function getNumberOfChildren(node) {
  let count = 0;
  let children = node.children || node._children; // consider both children and _children
  if (children) {
    children.forEach((child) => {
      count += getNumberOfChildren(child);
    });
  }
  return count + 1; // +1 to count the node itself
}
function getChildrenNames(node) {
  let names = [];
  let children = node.children || node._children; // consider both children and _children
  if (children) {
    children.forEach((child) => {
      if (child.data && child.data.name) {
        // check if 'name' property exists
        names.push(child.data.name);
      }
      // Get names of the child's children, regardless of whether the child has a name
      names = names.concat(getChildrenNames(child));
    });
  }
  return names;
}
function isRightChild(node) {
  // Check if the node has a parent and the parent has a children array
  if (!node.parent || !Array.isArray(node.parent.children)) return false;

  const index = node.parent.children.indexOf(node);

  return index === 1;
}
function generateVis() {
  if (expandedLeafNameID !== "") {
    d3.select(expandedLeafNameID).style("font-size", "12px");
    if (!isDebug) {
      passScatterplotLeafPointSelectionToQt("");
    }
  }
  d3.select("svg").remove();
  svg = d3.select("#my_dataviz");
  svg.selectAll("*").remove();
  treeData = dataReference;

  if (traitValueString !== "") {
    //updateTraitString(treeData, traitValueString);
    let parsedObject = JSON.parse(traitValueString);
    let data = parsedObject.data;
    let type = parsedObject.type;
    traitValueStringKey = type;
    traitValueStringContainer = data;
  }
  if (traitValueColor !== "") {
    //updateTraitColor(treeData, traitValueColor);
    let parsedObject = JSON.parse(traitValueColor);
    let data = parsedObject.data;
    let type = parsedObject.type;
    traitValueColorKey = type;
    traitValueColorContainer = data;
  }
  if (traitValueNumeric !== "") {
    //updateTraitNumeric(treeData, traitValueNumeric);
    let parsedObject = JSON.parse(traitValueNumeric);
    let data = parsedObject.data;
    let type = parsedObject.type;
    traitValueNumericKey = type;
    traitValueNumericContainer = data;
  }
  var valuesNumeric;
  var minValNumeric;
  var maxValNumeric;
  var sizeScale;

  if (traitValueNumericFlag) {
    valuesNumeric = Object.values(traitValueNumericContainer);
    minValNumeric = Math.min(...valuesNumeric);
    maxValNumeric = Math.max(...valuesNumeric);
    sizeScale = d3
      .scaleLinear()
      .domain([minValNumeric, maxValNumeric])
      .range([50, 200]);
  }

  var dataValuesString;
  var uniqueValuesString;
  var shapeScale;
  if (traitValueStringFlag) {
    dataValuesString = Object.values(traitValueStringContainer);
    uniqueValuesString = Array.from(new Set(dataValuesString));

    shapeScale = d3.scaleOrdinal().domain(uniqueValuesString).range(d3.symbols);
  }

  // Set the dimensions and margins of the diagram
  var margin = {
    top: 0,
    right: 0,
    bottom: 20,
    left: 29,
  };
  var width = window.innerWidth - margin.left - margin.right;
  var height = window.innerHeight - margin.top - margin.bottom;
  var removeAnimation = true;
  var sizeofShapes = height / 5;
  svg = d3
    .select("#my_dataviz")
    .append("svg")
    .attr("width", width)
    .attr("height", height)
    .append("g")
    .attr("transform", "translate(" + margin.left + "," + margin.top + ")");
  /*
  let colorScale;
  let scaleType = colorScales[qtColor]
    ? colorScales[qtColor]
    : colorScales["default"];
  let domain = colorMirror
    ? [mindistanceColor, maxdistanceColor]
    : [maxdistanceColor, mindistanceColor];

  if (qtColor === "qualitative") {
    colorScale = d3.scaleOrdinal(scaleType).domain(domain);
  } else {
    colorScale = d3.scaleSequential(scaleType).domain(domain);
  }*/
  var i = 0,
    duration = 750,
    root;

  var treemap = d3.cluster().size([height, width]);
  // Assigns parent, children, height, depth
  root = d3.hierarchy(treeData, function (d) {
    return d.children;
  });

  root.each(function (d) {
    if (d.depth > maxDepth) {
      maxDepth = d.depth;
    }
  });

  root.x0 = height / 2;
  root.y0 = 0;
  eachDepthWidth = (width - maxNameLength) / maxDepth;
  if (isFirstRender) {
    isFirstRender = false;
  }

  function toggleNode(node, action) {
    if (action === "collapse" && node.data.iscollapsed) {
      collapse(node);
    } else if (action === "expand" && node._children) {
      node.children = node._children;
      node._children = null;
    }

    if (node.children) {
      node.children.forEach((child) => toggleNode(child, action));
    }
  }

  // Toggle nodes based on the showExpandAll flag
  toggleNode(root, showExpandAll ? "expand" : "collapse");
  update(root);

  // Collapse the node and all it's children
  function collapse(d) {
    if (d.children) {
      d._children = d.children;
      d._children.forEach(collapse);
      d.children = null;
    }
  }

  function update(source) {
    // Assigns the x and y position for the nodes
    var treeData = treemap(root);

    // Compute the new tree layout.
    var nodes = treeData.descendants(),
      links = treeData.descendants().slice(1);

    /*     // Normalize for fixed-depth.
    nodes.forEach((d) => {
      const screenWidth = width - maxNameLength;
      let valTemp = screenWidth;
      //console.log("******");
      //console.log("d.y width: ", d.y);
      //console.log("width: ", width);
      //console.log("maxNameLength width: ", maxNameLength);
      //console.log("maxBranchLength width: ", maxBranchLength);
      //console.log("Current branch length width: ", d.data.branchLength);
      //console.log("desirable width: ", maxNameLength * 7);
      //console.log("branchLength: ", d.data.branchLength);
      //console.log("normalized: ",(d.data.branchLength / maxBranchLength) * eachDepthWidth);
      //console.log("depth: ", d.depth);
      //console.log("name: ", d.data.name);
      //console.log("maxDepth: ", maxDepth);
      //console.log("maxTotalDepthWidth: ", maxTotalDepthWidth);
      //console.log("eachDepthWidth: ", eachDepthWidth);
      //console.log("******");
      if (d.children || (d._children && !d.data.iscollapsed)) {
        valTemp = (d.depth * screenWidth) / 22;
      }
      //console.log("valTemp: ", valTemp);
      d.y = valTemp;
    }); */

    // ****************** Nodes section ***************************

    // Update the nodes...
    var node = svg.selectAll("g.node").data(nodes, function (d) {
      return d.id || (d.id = ++i);
    });

    // Enter any new modes at the parent's previous position.
    var nodeEnter = node
      .enter()
      .append("g")
      .attr("class", "node")
      .attr("transform", function (d) {
        return "translate(" + source.y0 + "," + source.x0 + ")";
      })
      .on("contextmenu", contextmenuNodes);

    // Add Square for the nodes
    nodeEnter
      .append("path") // Change "rect" to "path"
      .attr("class", "node")
      .attr("d", function (d) {
        if (
          d.data.name !== undefined &&
          d.data.name !== "" &&
          d.data.name !== "root"
        ) {
          let symbol = d3.symbol();

          if (traitValueStringFlag) {
            symbol = symbol.type(
              shapeScale(traitValueStringContainer[d.data.name])
            );
          } else {
            symbol = symbol.type(d3.symbolSquare);
          }

          if (traitValueNumericFlag) {
            symbol = symbol.size(
              sizeScale(traitValueNumericContainer[d.data.name])
            );
          } else {
            symbol = symbol.size(sizeofShapes);
          }

          return symbol();
        } else {
          return d3.symbol().type(d3.symbolSquare).size(sizeofShapes)();
        }
      })
      .attr("transform", "translate(2,0)") // Adjust position if necessary
      .style("cursor", "pointer")
      .style("stroke", "#000000")
      .style("stroke-width", "1px")
      .on("click", clickNodes)
      .style("fill", function (d) {
        if (
          d.data.name !== undefined &&
          d.data.name !== "" &&
          traitValueColorFlag
        ) {
          return traitValueColorContainer[d.data.name];
        } else {
          return "black"; //colorScale(d.data.score);
        }
      })
      .append("title")
      .text(function (d) {
        if (
          d.data.name !== undefined &&
          d.data.name !== "" &&
          d.data.name !== "root"
        ) {
          var returnStringADd = "";
          if (d.data.name !== undefined && d.data.name !== "") {
            if (traitValueStringFlag) {
              returnStringADd +=
                "\n" +
                traitValueStringKey +
                " : " +
                traitValueStringContainer[d.data.name];
            }
            if (traitValueNumericFlag) {
              returnStringADd +=
                "\n" +
                traitValueNumericKey +
                " : " +
                traitValueNumericContainer[d.data.name];
            }
          }

          if (traitValueStringFlag || traitValueNumericFlag) {
            return (
              "leaf : " +
              d.data.name +
              "\nMean: " +
              d.data.mean +
              returnStringADd
            );
          } else {
            return d.data.name;
          }
        } else {
          var childrennames = getChildrenNames(d);
          if (childrennames.length > 0) {
            var returnString = "";
            for (var i = 0; i < childrennames.length; i++) {
              let result = findNodeByName(childrennames[i], treeData);
              returnString +=
                (childrennames[i] ? childrennames[i].replace(/_/g, " ") : "") +
                "\n";
            }
            return returnString.trimEnd();
          }
        }
      });

    nodeEnter
      .append("text")
      .attr("id", "nodeScore")
      .attr("y", function (d) {
        return isRightChild(d) ? 9 : -4;
      })
      .attr("x", -20)
      .attr("dy", ".10em")
      .attr("dx", "-.55em")
      .style("font-size", "10px")

      .text(function (d) {
        if (showReferenceTree) {
        } else {
          return d.data.score; // Assuming the score is stored in the 'score' property
        }
      });
    // Add labels for the nodes
    nodeEnter
      .append("text")
      .attr("id", function (d) {
        return "leafName_" + d.data.name;
      })
      .attr("dy", ".35em")
      .style("cursor", "pointer")
      .style("font-size", "12px")
      .style("fill", function (d, i) {
        /*
                legendTextContainer = [];
      legendTextContainerAltFlag = false;
      legendTextContainerActivateFlag = false;
      changeLegendColor();
            */
        if (traitValueStringContainer) {
          let traitName = traitValueStringContainer[d.data.name];
          if (legendTextContainerActivateFlag && traitName !== undefined) {
            if (legendTextContainerAltFlag) {
              //if legendTextContainer includes d and index of d is 0

              if (
                legendTextContainer.includes(traitName) &&
                legendTextContainer.indexOf(traitName) === 0
              ) {
                rightSpeciesSelected.push(d.data.name);
                speciesSelected.push(d.data.name);
                return "#1D8ECE";
              }
              //else if legendTextContainer includes d and index of d is 1
              else if (
                legendTextContainer.includes(traitName) &&
                legendTextContainer.indexOf(traitName) === 1
              ) {
                leftSpeciesSelected.push(d.data.name);
                speciesSelected.push(d.data.name);
                return "#FF5733";
              } else {
                return "black";
              }
            } else {
              if (legendTextContainer.includes(traitName)) {
                speciesSelected.push(d.data.name);
                return "#1D8ECE";
              } else {
                return "black";
              }
            }
          } else {
            return "black";
          }
        } else {
          return "black";
        }
      })
      .on("click", clickNodes)
      .attr("x", function (d) {
        if (d.children || d._children) {
          return -15;
        } else {
          return 15;
        }
      })
      .attr("text-anchor", function (d) {
        return d.children || d._children ? "end" : "start";
      })
      .text(function (d) {
        // If false, return the name as is
        return d && d.data && d.data.name ? d.data.name.replace(/_/g, " ") : "";
      })
      .append("title") // Append title tag for hover text
      .text(function (d) {
        if (d.data.name !== undefined && d.data.name !== "") {
          var returnStringADd = "";
          if (traitValueStringFlag) {
            returnStringADd +=
              "\n" +
              traitValueStringKey +
              " : " +
              traitValueStringContainer[d.data.name];
          }
          if (traitValueNumericFlag) {
            returnStringADd +=
              "\n" +
              traitValueNumericKey +
              " : " +
              traitValueNumericContainer[d.data.name];
          }
        }

        if (traitValueStringFlag || traitValueNumericFlag) {
          return (
            "leaf : " + d.data.name + "\nMean: " + d.data.mean + returnStringADd
          );
        }
      });

    // UPDATE
    var nodeUpdate = nodeEnter.merge(node);

    // Transition to the proper position for the node
    nodeUpdate = removeAnimation
      ? nodeUpdate
      : nodeUpdate.transition().duration(duration);
    nodeUpdate.attr("transform", (d) => {
      // Optimized calculation by reducing repetitive operations and improving readability
      const scaledBranchLength =
        (d.data.branchLength / maxBranchLength) *
        (eachDepthWidth - maxNameLength);

      var scalefact = 1.28;
      if (width >= 2000) {
        scalefact = 1.064;
      } else if (width >= 1975) {
        scalefact = 1.07;
      } else if (width >= 1950) {
        scalefact = 1.074;
      } else if (width >= 1925) {
        scalefact = 1.08;
      } else if (width >= 1900) {
        scalefact = 1.084;
      } else if (width >= 1875) {
        scalefact = 1.09;
      } else if (width >= 1850) {
        scalefact = 1.094;
      } else if (width >= 1825) {
        scalefact = 1.1;
      } else if (width >= 1800) {
        scalefact = 1.104;
      } else if (width >= 1775) {
        scalefact = 1.11;
      } else if (width >= 1750) {
        scalefact = 1.114;
      } else if (width >= 1725) {
        scalefact = 1.12;
      } else if (width >= 1700) {
        scalefact = 1.124;
      } else if (width >= 1675) {
        scalefact = 1.13;
      } else if (width >= 1650) {
        scalefact = 1.134;
      } else if (width >= 1625) {
        scalefact = 1.14;
      } else if (width >= 1600) {
        scalefact = 1.144;
      } else if (width >= 1575) {
        scalefact = 1.15;
      } else if (width >= 1550) {
        scalefact = 1.154;
      } else if (width >= 1525) {
        scalefact = 1.16;
      } else if (width >= 1500) {
        scalefact = 1.164;
      } else if (width >= 1475) {
        scalefact = 1.17;
      } else if (width >= 1450) {
        scalefact = 1.174;
      } else if (width >= 1425) {
        scalefact = 1.18;
      } else if (width >= 1400) {
        scalefact = 1.184;
      } else if (width >= 1375) {
        scalefact = 1.19;
      } else if (width >= 1350) {
        scalefact = 1.194;
      } else if (width >= 1325) {
        scalefact = 1.2;
      } else if (width >= 1300) {
        scalefact = 1.204;
      } else if (width >= 1275) {
        scalefact = 1.21;
      } else if (width >= 1250) {
        scalefact = 1.214;
      } else if (width >= 1225) {
        scalefact = 1.22;
      } else if (width >= 1200) {
        scalefact = 1.224;
      } else if (width >= 1175) {
        scalefact = 1.23;
      } else if (width >= 1150) {
        scalefact = 1.234;
      } else if (width >= 1125) {
        scalefact = 1.27;
      } else if (width >= 1100) {
        scalefact = 1.27;
      } else if (width >= 1075) {
        scalefact = 1.27;
      } else if (width >= 1050) {
        scalefact = 1.27;
      } else if (width >= 1025) {
        scalefact = 1.28;
      } else if (width >= 1000) {
        scalefact = 1.29;
      } else if (width >= 975) {
        scalefact = 1.3;
      } else if (width >= 950) {
        scalefact = 1.31;
      } else if (width >= 925) {
        scalefact = 1.32;
      } else if (width >= 900) {
        scalefact = 1.33;
      } else if (width >= 875) {
        scalefact = 1.34;
      } else if (width >= 850) {
        scalefact = 1.35;
      } else if (width >= 825) {
        scalefact = 1.37;
      } else if (width >= 800) {
        scalefact = 1.38;
      } else if (width >= 775) {
        scalefact = 1.39;
      } else if (width >= 750) {
        scalefact = 1.42;
      } else if (width >= 725) {
        scalefact = 1.44;
      } else if (width >= 700) {
        scalefact = 1.46;
      } else if (width >= 675) {
        scalefact = 1.48;
      } else if (width >= 650) {
        scalefact = 1.5;
      } else if (width >= 625) {
        scalefact = 1.54;
      } else if (width >= 600) {
        scalefact = 1.58;
      } else if (width >= 575) {
        scalefact = 1.62;
      } else if (width >= 550) {
        scalefact = 1.66;
      } else if (width >= 525) {
        scalefact = 1.7;
      } else if (width >= 500) {
        scalefact = 1.76;
      } else if (width >= 475) {
        scalefact = 1.84;
      } else if (width >= 450) {
        scalefact = 1.9;
      } else if (width >= 425) {
        scalefact = 1.99;
      } else if (width >= 400) {
        scalefact = 2.15;
      } else if (width >= 375) {
        scalefact = 2.3;
      } else if (width >= 350) {
        scalefact = 2.5;
      } else if (width >= 325) {
        scalefact = 3;
      } else if (width >= 300) {
        scalefact = 3.5;
      } else if (width >= 275) {
        scalefact = 4;
      } else if (width >= 250) {
        scalefact = 4.5;
      } else if (width >= 225) {
        scalefact = 5;
      } else if (width >= 200) {
        scalefact = 5.5;
      } else if (width >= 175) {
        scalefact = 6;
      } else if (width >= 150) {
        scalefact = 6.5;
      } else if (width >= 125) {
        scalefact = 7;
      } else if (width >= 100) {
        scalefact = 7.5;
      } else if (width >= 75) {
        scalefact = 8;
      } else if (width >= 50) {
        scalefact = 8.5;
      } else if (width >= 25) {
        scalefact = 9;
      } else if (width >= 0) {
        scalefact = 9.5;
      } else {
        scalefact = 10;
      }

      d.y = d.y / scalefact + scaledBranchLength;
      return `translate(${d.y},${d.x})`;
    });

    // Update the node attributes and style
    nodeUpdate
      .select("circle.node")
      .attr("r", 5)
      .style("fill", function (d) {
        // If the node is a parent, set the color to something other than pink
        if (d.parent) {
          return colorScale(d.data.score); // replace with the color you want
        }
        // Otherwise, return the original color
        return d.data.color;
      })
      .attr("cursor", "pointer");

    // Remove any exiting nodes
    var nodeExit = node
      .exit()
      .transition()
      .duration(duration)
      .attr("transform", function (d) {
        return "translate(" + source.y + "," + source.x + ")";
      })
      .remove();

    // On exit reduce the node circles size to 0
    nodeExit.select("circle").attr("r", 1e-6);

    // On exit reduce the opacity of text labels
    nodeExit.select("text").style("fill-opacity", 1e-6);

    // ****************** links section ***************************

    // Update the links...
    var link = svg.selectAll("path.link").data(links, function (d) {
      return d.id;
    });

    // Enter any new links at the parent's previous position.
    var linkEnter = link
      .enter()
      .insert("path", "g")
      .attr("class", "link")
      .attr("d", function (d) {
        var o = { x: source.x0, y: source.y0 };
        return diagonal(o, o);
      })
      .attr("stroke", function (d) {
        // Color based on the score of the parent node
        var parentScore = d.parent ? d.parent.data.score : 0;
        //var color = colorScale(parentScore);
        var color = "black"; //colorScale(d.data.score);
        return color;
      });

    // UPDATE
    var linkUpdate = linkEnter.merge(link);

    // Transition back to the parent element position
    linkUpdate = removeAnimation
      ? linkUpdate
      : linkUpdate.transition().duration(duration);
    linkUpdate
      .attr("d", function (d) {
        return diagonal(d, d.parent);
      })
      .attr("stroke", function (d) {
        // Color based on the score of the parent node
        var parentScore = d.parent ? d.parent.data.score : 0;
        //var color = colorScale(parentScore);
        var color = "black"; //colorScale(d.data.score);
        return color;
      });

    // Remove any exiting links
    var linkExit = link
      .exit()
      .transition()
      .duration(duration)
      .attr("d", function (d) {
        var o = { x: source.x, y: source.y };
        return diagonal(o, o);
      })
      .remove();

    // Store the old positions for transition.
    nodes.forEach(function (d) {
      d.x0 = d.x;
      d.y0 = d.y;
    });

    // Creates a curved (diagonal) path from parent to the child nodes
    function diagonal(s, d) {
      return `M ${s.y} ${s.x}
L ${d.y} ${s.x}
L ${d.y} ${d.x}`;
    }
  }
  function extractSpeciesNamesCollapsed(node) {
    var speciesNames = [];

    function extract(node) {
      if (node.data.name) {
        speciesNames.push(node.data.name);
      }
      if (node._children) {
        node._children.forEach(extract);
      } else if (node.children) {
        node.children.forEach(extract);
      }
    }

    extract(node);
    return speciesNames;
  }
  // Toggle children on click.
  function contextmenuNodes(d) {
    //check if leaf node clicked else do something
    if (!d.children && !d._children) {
      //(d.data.name); //TODO: change here
      if (expandedLeafNameID !== "") {
        d3.select(expandedLeafNameID).style("font-size", "12px");
        if (!isDebug) {
          passScatterplotLeafPointSelectionToQt("");
        }
      }
      if (expandedLeafNameID !== "#leafName_" + d.data.name) {
        expandedLeafNameID = "#leafName_" + d.data.name;
        d3.select(expandedLeafNameID).style("font-size", "14px");
        if (!isDebug) {
          passScatterplotLeafPointSelectionToQt(d.data.name);
        }
      }
    } else {
      if (d.children) {
        d._children = d.children;
        d.children = null;
        d.data.iscollapsed = true;
      } else {
        d.children = d._children;
        d._children = null;
        d.data.iscollapsed = false;
      }
      update(d);
      updateNamesBelowNodes();
      updateNodeStyles();
      legendUpdate();
    }
  }
  function findNodeByName(name, data, parent) {
    if (data.name === name) {
      return {
        node: data,
        parentScore: parent ? parent.score : null,
        parentWidth: parent ? parent.width : null,
      };
    }
    let result;
    if (data.children) {
      for (let i = 0; i < data.children.length && !result; i++) {
        result = findNodeByName(name, data.children[i], data);
      }
    }
    return result;
  }
  function updateNamesBelowNodes() {
    // Remove existing names below nodes
    svg.selectAll(".species-names").remove();

    // Select all nodes with children
    var nodesWithChildren = svg.selectAll(".node").filter(function (d) {
      return d._children && d.data.iscollapsed;
    });

    nodesWithChildren
      .append("g") // Create a group element for each node
      .attr("class", "species-names")
      .selectAll("path") // Append a path element for each name
      .data(function (d) {
        // Assuming extractSpeciesNamesCollapsed(d) returns an array of strings
        var names = extractSpeciesNamesCollapsed(d);
        // Map each name to an object that includes the original data
        return names.map(function (name) {
          return {
            name: name,
            color: d.data.color,
            numberofChildrenRecursively: getNumberOfChildren(d),
            isCollapsed: d.iscollapsed,
            score: d.data.score,
          };
        });
      })
      .enter()
      .append("path")
      .attr(
        "d",
        d3
          .symbol()
          .type(function (d, i) {
            if (traitValueStringFlag) {
              return shapeScale(traitValueStringContainer[d.name]); //d3.symbolCircle;
            } else {
              return d3.symbolCircle;
            }
          })
          .size(function (d) {
            if (traitValueNumericFlag) {
              return sizeScale(traitValueNumericContainer[d.name]);
            } else {
              return 50;
            }
          })
      )
      .attr("transform", function (d, i) {
        // Calculate x and y positions
        var x = (i % symbolsPerLine) * symbolsPerLine * 2 + 40; // Every 10th symbol, start a new line
        var y = Math.floor(i / symbolsPerLine) * symbolsPerLine * 2 + 2 - 7; // Increase y position for every new line
        return "translate(" + x + "," + y + ")";
      })
      .style("stroke", function (d, i) {
        if (traitValueStringContainer) {
          let traitName = traitValueStringContainer[d.name];
          if (legendTextContainerActivateFlag && traitName !== undefined) {
            if (legendTextContainerAltFlag) {
              //if legendTextContainer includes d and index of d is 0

              if (
                legendTextContainer.includes(traitName) &&
                legendTextContainer.indexOf(traitName) === 0
              ) {
                rightSpeciesSelected.push(d.name);
                speciesSelected.push(d.name);
                return "#1D8ECE";
              }
              //else if legendTextContainer includes d and index of d is 1
              else if (
                legendTextContainer.includes(traitName) &&
                legendTextContainer.indexOf(traitName) === 1
              ) {
                leftSpeciesSelected.push(d.name);
                speciesSelected.push(d.name);
                return "#FF5733";
              } else {
                return "black";
              }
            } else {
              if (legendTextContainer.includes(traitName)) {
                speciesSelected.push(d.name);
                return "#1D8ECE";
              } else {
                return "black";
              }
            }
          } else {
            return "black";
          }
        } else {
          return "black";
        }
      })
      .style("stroke-width", "1px")
      .style("fill", function (d) {
        // If the node is a parent, set the color to something other than pink
        if (d.name !== undefined && d.name !== "" && traitValueColorFlag) {
          return traitValueColorContainer[d.name];
        } else if (d && colorScale(d.score)) {
          return colorScale(d.score);
        } else {
          return "black";
        }
      })
      .style("cursor", "pointer")
      .on("click", clickNames)
      .append("title") // Add a title element for hover text
      .style("font-size", "12px")
      .text(function (d) {
        if (d.name !== undefined && d.name !== "") {
          var returnStringADd = "";
          if (traitValueStringFlag) {
            returnStringADd +=
              "\n" +
              traitValueStringKey +
              " : " +
              traitValueStringContainer[d.name];
          }
          if (traitValueNumericFlag) {
            returnStringADd +=
              "\n" +
              traitValueNumericKey +
              " : " +
              traitValueNumericContainer[d.name];
          }
        }

        if (traitValueStringFlag || traitValueNumericFlag) {
          return "leaf : " + d.name + "\nMean: " + d.mean + returnStringADd;
        } else {
          return d.name;
        }
      });
  }

  function extractSpeciesNames(node) {
    var speciesNames = [];

    function extractSpecies(valueData) {
      if (valueData.data.name) {
        speciesNames.push(valueData.data.name);
      }
      if (valueData.children) {
        valueData.children.forEach(extractSpecies);
      } else if (valueData._children) {
        valueData._children.forEach(extractSpecies);
      }
    }

    extractSpecies(node); // Ensure the function is called with the node data

    return speciesNames;
  }

  function extractLeftRightSpeciesNames(node, side) {
    var speciesNames = [];

    function extractSpecies(valueData) {
      if (valueData.data.name) {
        speciesNames.push(valueData.data.name);
      }
      if (valueData.children) {
        valueData.children.forEach(extractSpecies);
      } else if (valueData._children) {
        valueData._children.forEach(extractSpecies);
      }
    }

    // Determine which side of the tree to traverse based on the input
    var targetChildren = side === "left" ? node.children[1] : node.children[0];

    if (targetChildren) {
      extractSpecies(targetChildren);
    }

    return speciesNames;
  }

  function clickNodes(d) {
    legendTextContainer = [];
    legendTextContainerAltFlag = false;
    legendTextContainerActivateFlag = false;
    if (expandedLeafNameID !== "") {
      d3.select(expandedLeafNameID).style("font-size", "12px");
      if (!isDebug) {
        passScatterplotLeafPointSelectionToQt("");
      }
    }
    changeLegendColor();
    if (d3.event.shiftKey) {
      shiftPressed = true;
    } else {
      shiftPressed = false;
    }
    if (d3.event.altKey) {
      altPressed = true;
    } else {
      altPressed = false;
    }

    if (!altPressed && !shiftPressed) {
      splitGroupsAltKey = false;
      speciesSelected = [];
      leftSpeciesSelected = [];
      rightSpeciesSelected = [];
    } else if (shiftPressed && !altPressed) {
      splitGroupsAltKey = false;
      leftSpeciesSelected = [];
      rightSpeciesSelected = [];
    } else if (altPressed && !shiftPressed) {
      splitGroupsAltKey = false;
      speciesSelected = [];
      leftSpeciesSelected = [];
      rightSpeciesSelected = [];
      if (d.children && altPressed) {
        leftSpeciesSelected = extractLeftRightSpeciesNames(d, "left");
        rightSpeciesSelected = extractLeftRightSpeciesNames(d, "right");
        splitGroupsAltKey = true;
      }
    } else if (shiftPressed && altPressed) {
      //splitGroupsAltKey = false;
    }

    var clickedSpecies = extractSpeciesNames(d);

    // if all elements of clickedSpecies are in speciesSelected, then remove them
    // else add the ones that are not in speciesSelected
    var allIn = true;
    for (var i = 0; i < clickedSpecies.length; i++) {
      if (!speciesSelected.includes(clickedSpecies[i])) {
        allIn = false;
        break;
      }
    }
    if (allIn) {
      for (var i = 0; i < clickedSpecies.length; i++) {
        speciesSelected = speciesSelected.filter(
          (item) => item !== clickedSpecies[i]
        );
      }
    } else {
      for (var i = 0; i < clickedSpecies.length; i++) {
        if (!speciesSelected.includes(clickedSpecies[i])) {
          speciesSelected.push(clickedSpecies[i]);
        }
      }
    }
    // Update styles based on selection
    updateNodeStyles();

    // Select the corresponding icons
    svg.selectAll(".species-names path").each(function (d) {
      if (clickedSpecies.includes(d.name)) {
        d3.select(this).classed("selected", speciesSelected.includes(d.name));
      }
    });

    shiftPressed = false;
    altPressed = false;
  }
  function changeLegendColor() {
    // change the text color of .selectAll(".shape-legend-label") text to red
    svg.selectAll(".shape-legend-label").style("fill", "black");
  }
  function clickLegendText(d) {
    if (expandedLeafNameID !== "") {
      d3.select(expandedLeafNameID).style("font-size", "12px");
      if (!isDebug) {
        passScatterplotLeafPointSelectionToQt("");
      }
    }
    if (legendTextContainer.includes(d)) {
      //remove d from legendTextContainer
      legendTextContainer = legendTextContainer.filter((item) => item !== d);
    }

    if (d3.event.altKey) {
      if (legendTextContainer.length >= 2) {
        legendTextContainer.shift();
        legendTextContainer.push(d);
      } else {
        legendTextContainer.push(d);
      }
      legendTextContainerAltFlag = true;
    } else {
      legendTextContainer = [];
      legendTextContainer.push(d);
      legendTextContainerAltFlag = false;
    }

    if (legendTextContainer.length > 0) {
      legendTextContainerActivateFlag = true;
      speciesSelected = [];
      rightSpeciesSelected = [];
      leftSpeciesSelected = [];
      generateVis();

      if (speciesSelected.length > 0) {
        if (legendTextContainerAltFlag) {
          var totalString = "";
          var leftString = "";
          var rightString = "";
          if (leftSpeciesSelected.length > 0) {
            leftString = leftSpeciesSelected.join(" @%$,$%@ ");
          }
          if (rightSpeciesSelected.length > 0) {
            rightString = rightSpeciesSelected.join(" @%$,$%@ ");
          }
          totalString = leftString + "@$LEFTSPLITRIGHT$@" + rightString;
          if (!isDebug) {
            passLeftRightSelectionToQT(totalString);
          }
        } else {
          speciesString = speciesSelected.join(" @%$,$%@ ");

          if (!isDebug) {
            passAddSelectionToQt(speciesString);
          }
        }
      } else {
        if (!isDebug) {
          passRemoveSelectionToQt("");
        }
      }
    } else {
      speciesSelected = [];
      rightSpeciesSelected = [];
      leftSpeciesSelected = [];
      legendTextContainer = [];
      legendTextContainerAltFlag = false;
      legendTextContainerActivateFlag = false;
      changeLegendColor();
    }
  }
  function clickNames(d) {
    if (expandedLeafNameID !== "") {
      d3.select(expandedLeafNameID).style("font-size", "12px");
      if (!isDebug) {
        passScatterplotLeafPointSelectionToQt("");
      }
    }
    legendTextContainer = [];
    legendTextContainerAltFlag = false;
    legendTextContainerActivateFlag = false;
    changeLegendColor();

    var clickedSpecies = d.name; // Extract the name from the data
    if (speciesSelected.includes(clickedSpecies)) {
      speciesSelected = speciesSelected.filter(
        (item) => item !== clickedSpecies
      );
    } else {
      speciesSelected.push(clickedSpecies);
    }

    // Update styles based on selection
    updateNodeStyles();
  }

  function updateNodeStyles() {
    // Update styles for .node text

    svg.selectAll(".node text").style("fill", function (d) {
      if (d && d.data) {
        if (speciesSelected.includes(d.data.name)) {
          if (altPressed || splitGroupsAltKey) {
            if (leftSpeciesSelected.includes(d.data.name)) {
              return "#FF5733"; // Bright Orange
            } else if (rightSpeciesSelected.includes(d.data.name)) {
              return "#1D8ECE"; // Deep Sky Blue
            }
          } else {
            return "#1D8ECE";
          }
        } else {
          if (d.name !== undefined && d.name !== "" && traitValueColorFlag) {
            return traitValueColorContainer[d.name];
          } else {
            return "black"; //colorScale(d.score);
          }
        }
      } else {
        if (d.name !== undefined && d.name !== "" && traitValueColorFlag) {
          return traitValueColorContainer[d.name];
        } else {
          return "black"; //colorScale(d.score);
        }
      }
    });

    // Update styles for .species-names text
    svg.selectAll(".species-names text").style("fill", function (d) {
      if (speciesSelected.includes(d)) {
        return "#1D8ECE";
      } else {
        if (d.name !== undefined && d.name !== "") {
          return traitValueColorContainer[d.name];
        } else {
          return "black"; //colorScale(d.score);
        }
      }
    });

    // Update styles for .species-names path (the symbols)
    svg.selectAll(".species-names path").style("fill", function (d) {
      if (d.name !== undefined && d.name !== "" && traitValueColorFlag) {
        return traitValueColorContainer[d.name];
      } else {
        return "black"; //colorScale(d.score);
      }
    });

    // Update styles for .species-names path (the symbols)
    svg.selectAll(".species-names path").style("stroke", function (d) {
      if (speciesSelected.includes(d.name)) {
        return "#1D8ECE";
      } else {
        return "black";
      }
    });
    // Update styles for .species-names path (the symbols)
    svg.selectAll(".species-names path").style("stroke-width", function (d) {
      if (speciesSelected.includes(d.name)) {
        return "2px";
      } else {
        return "1px";
      }
    });

    // Update styles for .node path (the symbols)
    svg.selectAll(".node path").style("stroke", function (d) {
      if (splitGroupsAltKey) {
        if (leftSpeciesSelected.includes(d.name)) {
          return "#FF5733"; // Bright Orange
        } else if (rightSpeciesSelected.includes(d.name)) {
          return "#1D8ECE"; // Deep Sky Blue
        } else {
          return "black";
        }
      } else {
        if (speciesSelected.includes(d.name)) {
          return "#1D8ECE";
        } else {
          return "black";
        }
      }
    });

    svg.selectAll(".node path").style("stroke-width", function (d) {
      if (speciesSelected.includes(d.name)) {
        return "2px";
      } else {
        return "1px";
      }
    });

    var speciesString = "";

    if (speciesSelected.length > 0) {
      if (altPressed) {
        var totalString = "";
        var leftString = "";
        var rightString = "";
        if (leftSpeciesSelected.length > 0) {
          leftString = leftSpeciesSelected.join(" @%$,$%@ ");
        }
        if (rightSpeciesSelected.length > 0) {
          rightString = rightSpeciesSelected.join(" @%$,$%@ ");
        }
        totalString = leftString + "@$LEFTSPLITRIGHT$@" + rightString;

        if (!isDebug) {
          passLeftRightSelectionToQT(totalString);
        }
      } else {
        //Comment for Debugging
        speciesString = speciesSelected.join(" @%$,$%@ ");

        if (!isDebug) {
          passAddSelectionToQt(speciesString);
        }
      }
    } else {
      if (!isDebug) {
        passRemoveSelectionToQt("");
      }
    }
  }
  updateNamesBelowNodes();
  var removeAnimation = false;

  if (traitValueStringFlag || traitValueNumericFlag) {
    legendUpdate();
  }

  function legendUpdate() {
    svg.selectAll(".size-legend").remove();
    svg.selectAll(".shape-legend").remove();
    if (root.x > window.innerHeight / 2) {
      legendYValue = 25;
    } else {
      legendYValue =
        window.innerHeight -
        window.innerHeight / (uniqueValuesString.length + 2);
    }
    var legendX = legendXValue;
    var legendY = 0 + legendYValue;
    var sizeLegend;
    if (traitValueNumericFlag) {
      // Create a group for the size legend
      sizeLegend = svg
        .append("g")
        .attr("class", "size-legend")
        .attr("transform", "translate(" + legendX + "," + legendY + ")"); // Adjust these values to move the legend

      // Add title for size legend
      sizeLegend
        .append("text")
        .style("font-size", "12px")
        .attr("class", "legend-title")
        .attr("y", -10)
        .attr("x", -15)
        .style("font-weight", "bold")
        .text(traitValueNumericKey);

      // Add circles for size legend
      sizeLegend
        .selectAll(".size-legend-circle")
        .data([minValNumeric, maxValNumeric])
        .enter()
        .append("circle")
        .attr("class", "size-legend-circle")
        .attr("cy", function (d, i) {
          return i * 20;
        }) // Adjust this value to change the vertical spacing
        .attr("r", function (d) {
          return sizeScale(d) / 20;
        });

      // Add labels for size legend
      sizeLegend
        .selectAll(".size-legend-label")
        .data([minValNumeric, maxValNumeric])
        .enter()
        .append("text")
        .style("font-size", "12px")
        .attr("class", "size-legend-label")
        .attr("y", function (d, i) {
          return i * 20;
        }) // Adjust this value to change the vertical spacing
        .attr("dx", 10) // Adjust this value to move the label horizontally
        .attr("dy", 6)
        .text(function (d) {
          return d;
        });
    }
    var shapeLegend;
    if (traitValueStringFlag) {
      var shapeLegendY;
      if (traitValueNumericFlag) {
        shapeLegendY = legendY + 70;
      } else {
        shapeLegendY = legendY;
      }

      // Create a group for the shape legend
      shapeLegend = svg
        .append("g")
        .attr("class", "shape-legend")
        .attr("transform", "translate(" + legendX + "," + shapeLegendY + ")"); // Adjust these values to move the legend

      // Add title for shape legend
      shapeLegend
        .append("text")
        .style("font-size", "12px")
        .attr("class", "legend-title")
        .attr("y", -10)
        .attr("x", -15)
        .style("font-weight", "bold")
        .text(traitValueStringKey);

      // Add symbols for shape legend
      shapeLegend
        .selectAll(".shape-legend-symbol")
        .data(uniqueValuesString)
        .enter()
        .append("path")
        .attr("class", "shape-legend-symbol")
        .attr("transform", function (d, i) {
          return "translate(0," + i * 20 + ")";
        }) // Adjust this value to change the vertical spacing
        .attr("d", function (d) {
          return d3.symbol().type(shapeScale(d))();
        });

      // Add labels for shape legend
      shapeLegend
        .selectAll(".shape-legend-label")
        .data(uniqueValuesString)
        .enter()
        .append("text")
        .style("font-size", "12px")
        .attr("class", "shape-legend-label")
        .attr("y", function (d, i) {
          return i * 20;
        }) // Adjust this value to change the vertical spacing
        .attr("dx", 10) // Adjust this value to move the label horizontally
        .attr("dy", 6)
        .attr("cursor", "pointer")
        .attr("fill", function (d, i) {
          if (legendTextContainerActivateFlag) {
            if (legendTextContainerAltFlag) {
              //if legendTextContainer includes d and index of d is 0
              if (
                legendTextContainer.includes(d) &&
                legendTextContainer.indexOf(d) === 0
              ) {
                return "#1D8ECE";
              }
              //else if legendTextContainer includes d and index of d is 1
              else if (
                legendTextContainer.includes(d) &&
                legendTextContainer.indexOf(d) === 1
              ) {
                return "#FF5733";
              } else {
                return "black";
              }
            } else {
              if (legendTextContainer.includes(d)) {
                return "#1D8ECE";
              } else {
                return "black";
              }
            }
          } else {
            return "black";
          }
        })
        .on("click", clickLegendText)
        .text(function (d) {
          return d;
        });
    }
  }
  updateNodeStyles();
}

//add a function to trigger on esc click
document.addEventListener("keydown", function (event) {
  if (event.key === "Escape") {
    speciesSelected = [];
    leftSpeciesSelected = [];
    rightSpeciesSelected = [];
    legendTextContainer = [];
    legendTextContainerActivateFlag = false;
    legendTextContainerAltFlag = false;
    removeLeafSelections = true;
    if (expandedLeafNameID !== "") {
      d3.select(expandedLeafNameID).style("font-size", "12px");
      if (!isDebug) {
        passScatterplotLeafPointSelectionToQt("");
      }
    }
    if (!isDebug) {
      passRemoveSelectionToQt("");
    }

    generateVis();
  }
});
