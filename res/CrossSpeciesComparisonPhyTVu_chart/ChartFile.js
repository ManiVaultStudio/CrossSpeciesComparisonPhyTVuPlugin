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
function getColorScalePermanent(
    colorname,
    minScore,
    maxScore,
    colorMirror = false
) {
    const colorScales = {
        Blues: d3.interpolateBlues,
        Greens: d3.interpolateGreens,
        Greys: d3.interpolateGreys,
        Oranges: d3.interpolateOranges,
        Purples: d3.interpolatePurples,
        Reds: d3.interpolateReds,
        BuGn: d3.interpolateBuGn,
        BuPu: d3.interpolateBuPu,
        GnBu: d3.interpolateGnBu,
        OrRd: d3.interpolateOrRd,
        PuBuGn: d3.interpolatePuBuGn,
        PuBu: d3.interpolatePuBu,
        PuRd: d3.interpolatePuRd,
        RdPu: d3.interpolateRdPu,
        YlGnBu: d3.interpolateYlGnBu,
        YlGn: d3.interpolateYlGn,
        YlOrBr: d3.interpolateYlOrBr,
        YlOrRd: d3.interpolateYlOrRd,
        Cividis: d3.interpolateCividis,
        Viridis: d3.interpolateViridis,
        Inferno: d3.interpolateInferno,
        Magma: d3.interpolateMagma,
        Plasma: d3.interpolatePlasma,
        Warm: d3.interpolateWarm,
        Cool: d3.interpolateCool,
        CubehelixDefault: d3.interpolateCubehelixDefault,
        Turbo: d3.interpolateTurbo,
        BrBG: d3.interpolateBrBG,
        PRGn: d3.interpolatePRGn,
        PiYG: d3.interpolatePiYG,
        PuOr: d3.interpolatePuOr,
        RdBu: d3.interpolateRdBu,
        RdGy: d3.interpolateRdGy,
        RdYlBu: d3.interpolateRdYlBu,
        RdYlGn: d3.interpolateRdYlGn,
        Spectral: d3.interpolateSpectral,
        Rainbow: d3.interpolateRainbow,
        Sinebow: d3.interpolateSinebow,
        Observable10: d3.schemeObservable10,
        Category10: d3.schemeCategory10,
        Accent: d3.schemeAccent,
        Dark2: d3.schemeDark2,
        Paired: d3.schemePaired,
        Pastel1: d3.schemePastel1,
        Pastel2: d3.schemePastel2,
        Set1: d3.schemeSet1,
        Set2: d3.schemeSet2,
        Set3: d3.schemeSet3,
        Tableau10: d3.schemeTableau10,
    };

    let scaleType = colorScales[colorname]
        ? colorScales[colorname]
        : colorScales["Category10"];
    let domain = colorMirror ? [minScore, maxScore] : [maxScore, minScore];

    if (
        [
            "Observable10",
            "Category10",
            "Accent",
            "Dark2",
            "Paired",
            "Pastel1",
            "Pastel2",
            "Set1",
            "Set2",
            "Set3",
            "Tableau10",
        ].includes(colorname)
    ) {
        return d3.scaleOrdinal(scaleType).domain(domain);
    } else if (
        [
            "BrBG",
            "PRGn",
            "PiYG",
            "PuOr",
            "RdBu",
            "RdGy",
            "RdYlBu",
            "RdYlGn",
            "Spectral",
        ].includes(colorname)
    ) {
        return d3
            .scaleDiverging(scaleType)
            .domain([minScore, (minScore + maxScore) / 2, maxScore]);
    } else {
        return d3.scaleSequential(scaleType).domain(domain);
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
    //if (expandedLeafNameID !== "")
    //{
    //d3.select(expandedLeafNameID).style("font-size", "12px");
    //if (!isDebug) {
    //passScatterplotLeafPointSelectionToQt("");
    //}
    //}
    legendXValue = 15;
    legendYValue = 20;
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

    if (traitValueNumericFlag && showTraitValues) {
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
    if (traitValueStringFlag && showTraitValues) {
        dataValuesString = Object.values(traitValueStringContainer);
        uniqueValuesString = Array.from(new Set(dataValuesString));

        shapeScale = d3.scaleOrdinal().domain(uniqueValuesString).range(d3.symbols);
    }
    // Define the color scale using d3.scaleDiverging and d3.interpolateRdBu
    var colorScoresPermanent = getColorScalePermanent(
        qtColor,
        mindistanceColor,
        maxdistanceColor
    );
    // Set the dimensions and margins of the diagram
    var margin = {
        top: 0,
        right: 1,
        bottom: 20,
        left: 32,
    };
    var width = window.innerWidth - margin.left - margin.right;
    var height = window.innerHeight - margin.top - margin.bottom;
    var removeAnimation = true;
    var sizeofShapes = height / 4.5;
    svg = d3
        .select("#my_dataviz")
        .append("svg")
        .attr("width", width)
        .attr("height", height)
        .append("g")
        .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

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
    }
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
    /*
      toggleNode(root, showExpandAll ? "expand" : "collapse");
      
      */
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
            });
        //.on("contextmenu", contextmenuNodes)
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

                    if (traitValueStringFlag && showTraitValues) {
                        symbol = symbol.type(
                            shapeScale(traitValueStringContainer[d.data.name])
                        );
                    } else {
                        symbol = symbol.type(d3.symbolSquare);
                    }

                    if (traitValueNumericFlag && showTraitValues) {
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
                {
                    return colorScoresPermanent(d.data.score);
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
                        if (traitValueStringFlag && showTraitValues) {
                            returnStringADd +=
                                "\n" +
                                traitValueStringKey +
                                " : " +
                                traitValueStringContainer[d.data.name];
                        }
                        if (traitValueNumericFlag && showTraitValues) {
                            returnStringADd +=
                                "\n" +
                                traitValueNumericKey +
                                " : " +
                                traitValueNumericContainer[d.data.name];
                        }
                    }

                    //if (
                    //(traitValueStringFlag && showTraitValues) ||
                    //(traitValueNumericFlag && showTraitValues)
                    // )
                    {
                        return (
                            "leaf : " +
                            d.data.name +
                            "\nCell counts : " +
                            d.data.cellCounts +
                            "\nMean: " +
                            d.data.mean +
                            returnStringADd
                        );
                    }
                    //else
                    {
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
                return isRightChild(d) ? 10 : -5;
            })
            .attr("x", function (d) {
                if (d.data.score == 0) {
                    return -10;
                } else if (d.data.score > 100) {
                    // check if decimal number
                    if (d.data.score % 1 !== 0) {
                        return -40;
                    } else {
                        return -30;
                    }
                } else if (d.data.score > 10) {
                    // check if decimal number
                    if (d.data.score % 1 !== 0) {
                        return -30;
                    } else {
                        return -20;
                    }
                } else if (d.data.score > 0) {
                    // check if decimal number
                    if (d.data.score % 1 !== 0) {
                        return -25;
                    } else {
                        return -10;
                    }
                } else if (d.data.score < -100) {
                    // check if decimal number
                    if (d.data.score % 1 !== 0) {
                        return -50;
                    } else {
                        return -40;
                    }
                } else if (d.data.score < -10) {
                    // check if decimal number
                    if (d.data.score % 1 !== 0) {
                        return -40;
                    } else {
                        return -30;
                    }
                } else if (d.data.score < 0) {
                    // check if decimal number
                    if (d.data.score % 1 !== 0) {
                        return -30;
                    } else {
                        return -20;
                    }
                }
            })
            //.attr("x", -24)
            .attr("dy", ".10em")
            .attr("dx", "-.55em")
            .style("font-size", "12px")

            .text(function (d) {
                {
                    return d.data.score;
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
                if (traitValueStringContainer) {
                    let traitName = traitValueStringContainer[d.data.name];
                    if (legendTextContainerActivateFlag && traitName !== undefined) {
                        if (legendTextContainer.includes(traitName)) {
                            speciesSelected.push(d.data.name);
                            return "#1D8ECE";
                        } else {
                            return "black";
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
                    if (traitValueStringFlag && showTraitValues) {
                        returnStringADd +=
                            "\n" +
                            traitValueStringKey +
                            " : " +
                            traitValueStringContainer[d.data.name];
                    }
                    if (traitValueNumericFlag && showTraitValues) {
                        returnStringADd +=
                            "\n" +
                            traitValueNumericKey +
                            " : " +
                            traitValueNumericContainer[d.data.name];
                    }
                }

                //if (
                //(traitValueStringFlag && showTraitValues) ||
                //(traitValueNumericFlag && showTraitValues)
                //)
                {
                    return (
                        "Leaf : " +
                        d.data.name +
                        "\nCell counts : " +
                        d.data.cellCounts +
                        "\nMean: " +
                        d.data.mean +
                        returnStringADd
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
                    return colorScoresPermanent(d.data.score); // replace with the color you want
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
                //var color = colorScoresPermanent(parentScore);
                var color = "black"; //colorScoresPermanent(d.data.score);
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
                //var color = colorScoresPermanent(parentScore);
                var color = "black"; //colorScoresPermanent(d.data.score);
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
    /*
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
      */
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
        svg.selectAll(".species-names").remove(); // Remove existing names

        svg
            .selectAll(".node")
            .filter((d) => d._children && d.data.iscollapsed) // Filter nodes with children and collapsed
            .append("g")
            .attr("class", "species-names")
            .selectAll("path")
            .data((d) =>
                extractSpeciesNamesCollapsed(d).map((name) => ({
                    ...d.data, // Spread operator to include all data properties
                    name,
                    numberofChildrenRecursively: getNumberOfChildren(d),
                    isCollapsed: d.iscollapsed,
                }))
            )
            .enter()
            .append("path")
            .attr(
                "d",
                d3
                    .symbol()
                    .type((d) =>
                        traitValueStringFlag && showTraitValues
                            ? shapeScale(traitValueStringContainer[d.name])
                            : d3.symbolCircle
                    )
                    .size((d) =>
                        traitValueNumericFlag && showTraitValues
                            ? sizeScale(traitValueNumericContainer[d.name])
                            : 50
                    )
            )
            .attr("transform", (d, i) => {
                let x = (i % symbolsPerLine) * symbolsPerLine * 2 + 40;
                let y = Math.floor(i / symbolsPerLine) * symbolsPerLine * 2 + 2 - 7;
                return `translate(${x},${y})`;
            })
            .style("stroke", (d) => {
                if (traitValueStringContainer) {
                    let traitName = traitValueStringContainer[d.name];
                    if (legendTextContainerActivateFlag && traitName !== undefined) {
                        {
                            return legendTextContainer.includes(traitName)
                                ? "#1D8ECE"
                                : "black";
                        }
                    }
                }
                return "black";
            })
            .style("stroke-width", "1px")
            .style("fill", (d) => {
                {
                    return colorScoresPermanent(d.score) || "black";
                }
            })
            .style("cursor", "pointer")
            .on("click", clickNames)
            .append("title")
            .style("font-size", "12px")
            .text((d) => {
                let info = d.name;
                if (traitValueStringFlag && showTraitValues)
                    info += `\n${traitValueStringKey} : ${traitValueStringContainer[d.name]
                        }`;
                if (traitValueNumericFlag && showTraitValues)
                    info += `\n${traitValueNumericKey} : ${traitValueNumericContainer[d.name]
                        }`;
                return info;
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
        legendTextContainerActivateFlag = false;
        //if (expandedLeafNameID !== "")
        //{
        //d3.select(expandedLeafNameID).style("font-size", "12px");
        //if (!isDebug) {
        //passScatterplotLeafPointSelectionToQt("");
        //}
        //}
        changeLegendColor();
        if (d3.event.shiftKey) {
            shiftPressed = true;
        } else {
            shiftPressed = false;
        }

        if (!shiftPressed) {
            splitGroupsAltKey = false;
            speciesSelected = [];
        } else if (shiftPressed) {
            splitGroupsAltKey = false;
        } else if (shiftPressed) {
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
            if (d.data && clickedSpecies.includes(d.data.name)) {
                d3.select(this).classed(
                    "selected",
                    speciesSelected.includes(d.data.name)
                );
            } else {
                if (d && clickedSpecies.includes(d.name)) {
                    d3.select(this).classed("selected", speciesSelected.includes(d.name));
                }
            }
        });

        shiftPressed = false;
    }
    function changeLegendColor() {
        // change the text color of .selectAll(".shape-legend-label") text to red
        svg.selectAll(".shape-legend-label").style("fill", "black");
    }
    function clickLegendText(d) {
        //if shift pressed
        if (d3.event.shiftKey) {
            shiftPressed = true;
        } else {
            shiftPressed = false;
        }
        //if (expandedLeafNameID !== "")
        //{
        //d3.select(expandedLeafNameID).style("font-size", "12px");
        //if (!isDebug) {
        //passScatterplotLeafPointSelectionToQt("");
        //}
        //}

        if (shiftPressed) {
            if (legendTextContainer.includes(d)) {
                legendTextContainer = legendTextContainer.filter((item) => item !== d);
            } else {
                legendTextContainer.push(d);
            }
        } else {
            legendTextContainer = [];
            legendTextContainer.push(d);
        }

        if (legendTextContainer.length > 0) {
            legendTextContainerActivateFlag = true;
            speciesSelected = [];

            generateVis();

            if (speciesSelected.length > 0) {
                {
                    speciesString = speciesSelected.join(" @%$,$%@ ");

                    if (!isDebug) {
                        passAddSelectionToQt(speciesString);
                    } else {
                        //console.log("passAddSelectionToQt(speciesString): ", speciesString);
                    }
                }
            } else {
                if (!isDebug) {
                    passRemoveSelectionToQt("");
                } else {
                    //console.log("passRemoveSelectionToQt('')");
                }
            }
        } else {
            speciesSelected = [];
            legendTextContainer = [];
            legendTextContainerActivateFlag = false;
            changeLegendColor();
        }
    }
    function clickNames(d) {
        //if (expandedLeafNameID !== "")
        //{
        //d3.select(expandedLeafNameID).style("font-size", "12px");
        //if (!isDebug) {
        //passScatterplotLeafPointSelectionToQt("");
        //}
        //}
        legendTextContainer = [];
        legendTextContainerActivateFlag = false;
        changeLegendColor();

        var clickedSpecies = d.data.name; // Extract the name from the data
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
        // Helper function to determine fill color for nodes
        function getNodeFillColor(d) {
            // Check if node is selected
            if (d?.data && speciesSelected.includes(d.data.name)) {
                // Handle key press combinations for selected nodes
                if (altShiftPressed) {
                    // Alt+Shift+mouse click
                    return "#A52A2A"; // Unique color for Alt+Shift+mouse click
                } else {
                    // Default color for selected species
                    return "#1D8ECE";
                }
            }
            // Default color if none of the above conditions are met
            return "black";
        }

        // Helper function to determine stroke color for paths
        function getPathStrokeColor(d) {
            // Check if d or d.data is undefined
            if (!d || !d.data) return "black"; // Return default color if d or d.data is undefined

            if (splitGroupsAltKey) {
            } else if (speciesSelected.includes(d.data.name)) {
                return "#1D8ECE";
            }
            return "black";
        }

        // Helper function to determine stroke width
        function getStrokeWidth(d) {
            if (
                d !== undefined &&
                d.data !== undefined &&
                d.data.name !== undefined &&
                d.data.name !== ""
            ) {
                return speciesSelected.includes(d.data.name) ? "2px" : "1px";
            }
        }
        function getFontWeight(d) {
            if (
                d !== undefined &&
                d.data !== undefined &&
                d.data.name !== undefined &&
                d.data.name !== ""
            ) {
                return speciesSelected.includes(d.data.name) ? "bold" : "normal";
            }
        }
        function getFontSize(d) {
            if (
                d !== undefined &&
                d.data !== undefined &&
                d.data.name !== undefined &&
                d.data.name !== ""
            ) {
                return speciesSelected.includes(d.data.name) ? 11 : 12;
            }
        }
        // Update styles for .node text and .species-names text/path
        svg
            .selectAll(".node text, .species-names text, .species-names path")
            .style("fill", getNodeFillColor)
            .style("font-weight", getFontWeight)
            .style("font-size", getFontSize)
            .filter(".species-names path")
            .style("stroke", getPathStrokeColor)
            .style("stroke-width", getStrokeWidth);

        // Simplified update styles for .node path
        svg
            .selectAll(".node path")
            .style("stroke", getPathStrokeColor)
            .style("stroke-width", getStrokeWidth);

        if (speciesSelected.length > 0) {
            {
                speciesString = speciesSelected.join(" @%$,$%@ ");

                if (!isDebug) {
                    passAddSelectionToQt(speciesString);
                } else {
                    //console.log("passAddSelectionToQt(speciesString): ", speciesString);
                }
            }
        } else {
            if (!isDebug) {
                passRemoveSelectionToQt("");
            } else {
                //console.log("passRemoveSelectionToQt('')");
            }
        }
    }
    updateNamesBelowNodes();
    var removeAnimation = false;
    permanentLegendUpdate();
    if (
        (traitValueStringFlag && showTraitValues) ||
        (traitValueNumericFlag && showTraitValues)
    ) {
        legendUpdate();
    }

    function permanentLegendUpdate() {
        // Ensure the SVG element is selected correctly
        var svg = d3.select("svg");
        if (svg.empty()) {
            return;
        }

        // Define minScore and maxScore
        var minScore = mindistanceColor; // Replace with actual logic to determine minScore
        var maxScore = maxdistanceColor; // Replace with actual logic to determine maxScore

        // Remove any existing color legend
        svg.selectAll(".permanentcolorscale-legend").remove();

        var colorLegend = svg
            .append("g")
            .attr("class", "permanentcolorscale-legend")
            .attr(
                "transform",
                "translate(" + legendXValue + "," + legendYValue + ")"
            );

        // Add title for color legend
        colorLegend
            .append("text")
            .style("font-size", "12px")
            .attr("class", "legend-title")
            .attr("y", -10)
            .attr("x", -15)
            .style("font-weight", "bold")
            .text(typeOfColoringScore + " scores for " + geneName + " gene");

        // Add rectangles for color legend
        var gradient = colorLegend
            .append("defs")
            .append("linearGradient")
            .attr("id", "gradient")
            .attr("x1", "0%")
            .attr("y1", "0%")
            .attr("x2", "100%")
            .attr("y2", "0%");

        gradient
            .append("stop")
            .attr("offset", "0%")
            .attr("stop-color", colorScoresPermanent(minScore))
            .attr("stop-opacity", 1);

        gradient
            .append("stop")
            .attr("offset", "50%")
            .attr("stop-color", colorScoresPermanent((minScore + maxScore) / 2))
            .attr("stop-opacity", 1);

        gradient
            .append("stop")
            .attr("offset", "100%")
            .attr("stop-color", colorScoresPermanent(maxScore))
            .attr("stop-opacity", 1);

        colorLegend
            .append("rect")
            .attr("width", 100)
            .attr("height", 10)
            .attr("x", -12)
            .attr("y", 0)
            .style("fill", "url(#gradient)");

        // Add labels for color legend
        colorLegend
            .append("text")
            .attr("x", -12)
            .attr("y", 25)
            .style("font-size", "12px")
            .text(minScore);

        colorLegend
            .append("text")
            .attr("x", 82)
            .attr("y", 25)
            .style("font-size", "12px")
            .text(maxScore);

        // Bring the permanent legend to the front
        colorLegend.raise();
    }

    function legendUpdate() {
        svg.selectAll(".size-legend").remove();
        svg.selectAll(".shape-legend").remove();

        var legendX = legendXValue - 35; // Move the legend more to the left
        var legendY = legendYValue + 55;
        var sizeLegend;
        if (traitValueNumericFlag && showTraitValues) {
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
        if (traitValueStringFlag && showTraitValues) {
            var shapeLegendY;
            if (traitValueNumericFlag && showTraitValues) {
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
                        if (legendTextContainer.includes(d)) {
                            return "#1D8ECE";
                        } else {
                            return "black";
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
        legendTextContainer = [];
        legendTextContainerActivateFlag = false;
        removeLeafSelections = true;
        //if (expandedLeafNameID !== "")
        //{
        //d3.select(expandedLeafNameID).style("font-size", "12px");
        //if (!isDebug) {
        //passScatterplotLeafPointSelectionToQt("");
        //}
        //}

        if (!isDebug) {
            passRemoveSelectionToQt("");
        } else {
            //console.log("passRemoveSelectionToQt('')");
        }

        generateVis();
    }
});
