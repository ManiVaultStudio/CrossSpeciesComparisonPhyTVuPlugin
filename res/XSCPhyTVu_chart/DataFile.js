var traitjsonsmall = {
    "✔Tail": {
        "Chimpanzee": { "isPresent": true },
        "Human": { "isPresent": false },
        "Gorilla": { "isPresent": false },
        "Rhesus": { "isPresent": true },
        "Marmoset": { "isPresent": true }
    },
    "✔Opposable thumbs": {
        "Chimpanzee": { "isPresent": true },
        "Human": { "isPresent": true },
        "Gorilla": { "isPresent": false },
        "Rhesus": { "isPresent": true },
        "Marmoset": { "isPresent": false }
    },
    "✔Large Canines": {
        "Chimpanzee": { "isPresent": true },
        "Human": { "isPresent": false },
        "Gorilla": { "isPresent": true },
        "Rhesus": { "isPresent": false },
        "Marmoset": { "isPresent": false }
    },
    "✔Bipedalism": {
        "Chimpanzee": { "isPresent": false },
        "Human": { "isPresent": true },
        "Gorilla": { "isPresent": false },
        "Rhesus": { "isPresent": false },
        "Marmoset": { "isPresent": false }
    },
    "✔Language Skills": {
        "Chimpanzee": { "isPresent": false },
        "Human": { "isPresent": true },
        "Gorilla": { "isPresent": false },
        "Rhesus": { "isPresent": false },
        "Marmoset": { "isPresent": false }
    }
};


var data1 = {
    id: 1,
    score: 1.0,
    width: 2,
    children: [
        {
            id: 2,
            score: 1.0,
            width: 5,
            children: [
                {
                    id: 3,
                    score: 1.0,
                    width: 9,
                    children: [
                        {
                            id: 4,
                            score: 1.0,
                            width: 8,
                            children: [{ name: "Human", color: "#000000", hastrait: true, iscollapsed: false }, { name: "Chimpanzee", color: "#000000", hastrait: true, iscollapsed: false }],
                        },
                        { name: "Gorilla", color: "#000000", hastrait: true, iscollapsed: false },
                    ],
                },
                { name: "Rhesus", color: "#000000", hastrait: true, iscollapsed: false },
            ],
        },
        { name: "Marmoset", color: "#000000", hastrait: true, iscollapsed: false },
    ],
};

var data2 = {
    id: 1,
    score: 1.0,
    children: [
        {
            id: 2,
            score: 1.0,
            children: [
                {
                    id: 4,
                    score: 1.0,
                    children: [{ name: "Human", color: "#000000", hastrait: true, iscollapsed: false }, { name: "Rhesus", color: "#000000", hastrait: true, iscollapsed: false }],
                },
                { name: "Chimpanzee", color: "#000000", hastrait: true, iscollapsed: false },
            ],
        },
        {
            id: 3,
            score: 1.0,
            children: [{ name: "Gorilla", color: "#000000", hastrait: true, iscollapsed: false }, { name: "Marmoset", color: "#000000", hastrait: true, iscollapsed: false }],
        },
    ],
};

var data3 = {
    id: 1,
    score: 1.0,
    children: [
        {
            id: 3,
            score: 1.0,
            children: [
                {
                    id: 4,
                    score: 1.0,
                    children: [{ name: "Human", color: "#000000", hastrait: true, iscollapsed: false }, { name: "Marmoset", color: "#000000", hastrait: true, iscollapsed: false }],
                },
                { name: "Rhesus", color: "#000000", hastrait: true, iscollapsed: false },
            ],
        },
        {
            id: 2,
            score: 1.0,
            children: [{ name: "Chimpanzee", color: "#000000", hastrait: true, iscollapsed: false }, { name: "Gorilla", color: "#000000", hastrait: true, iscollapsed: false }],
        },
    ],
};

var data4 = {
    id: 1,
    score: 1.0,
    children: [
        {
            id: 3,
            score: 1.0,
            children: [
                {
                    id: 4,
                    score: 1.0,
                    children: [{ name: "Human", color: "#000000", hastrait: true, iscollapsed: false }, { name: "Chimpanzee", color: "#000000", hastrait: true, iscollapsed: false }],
                },
                {
                    id: 5,
                    score: 1.0,
                    children: [{ name: "Gorilla", color: "#000000", hastrait: true, iscollapsed: false }, { name: "Marmoset", color: "#000000", hastrait: true, iscollapsed: false }],
                },
            ],
        },
        { name: "Rhesus" },
    ],
};

var dataLarge1 = {
    id: 1,
    score: 1,
    children: [
        {
            id: 2,
            score: Math.random().toFixed(2),
            children: [
                {
                    id: 3,
                    score: Math.random().toFixed(2),
                    children: [
                        {
                            id: 4,
                            score: Math.random().toFixed(2),
                            children: [{ name: "Human" }, { name: "Chimpanzee" }],
                        },
                        { name: "Gorilla" },
                    ],
                },
                { name: "Rhesus macaque" },
            ],
        },
        { name: "Marmoset" },
        {
            id: 5,
            score: Math.random().toFixed(2),
            children: [
                {
                    id: 6,
                    score: Math.random().toFixed(2),
                    children: [{ name: "Pig-tailed macaque" }, { name: "Olive baboon" }],
                },
                {
                    id: 7,
                    score: Math.random().toFixed(2),
                    children: [
                        { name: "African green monkey" },
                        {
                            id: 8,
                            score: Math.random().toFixed(2),
                            children: [
                                { name: "Squirrel monkey" },
                                { name: "Capuchin monkey" },
                            ],
                        },
                        {
                            id: 9,
                            score: Math.random().toFixed(2),
                            children: [{ name: "Owl monkey" }, { name: "Galago" }],
                        },
                    ],
                },
                {
                    id: 10,
                    score: Math.random().toFixed(2),
                    children: [
                        { name: "Mouse lemur" },
                        {
                            id: 11,
                            score: Math.random().toFixed(2),
                            children: [{ name: "Treeshrew" }, { name: "Mouse" }],
                        },
                        {
                            id: 12,
                            score: Math.random().toFixed(2),
                            children: [{ name: "Rat" }, { name: "Naked mole rat" }],
                        },
                    ],
                },
            ],
        },
        {
            id: 13,
            score: Math.random().toFixed(2),
            children: [
                { name: "Arctic ground squirrel" },
                {
                    id: 14,
                    score: Math.random().toFixed(2),
                    children: [
                        { name: "Rabbit" },
                        {
                            id: 15,
                            score: Math.random().toFixed(2),
                            children: [{ name: "Ferret" }, { name: "Coyote" }],
                        },
                    ],
                },
                {
                    id: 16,
                    score: Math.random().toFixed(2),
                    children: [{ name: "Cat" }, { name: "Egyptian fruit bat" }],
                },
                {
                    id: 17,
                    score: Math.random().toFixed(2),
                    children: [{ name: "Armadillo" }, { name: "Opossum" }],
                },
            ],
        },
    ],
};

var dataLarge = {
    id: 1,
    score: 0.7,
    children: [
        {
            id: 13,
            score: 0.8,
            children: [
                {
                    id: 16,
                    score: 1,
                    children: [
                        {
                            name: "Armadillo",
                        },
                        {
                            name: "Ferret",
                        },
                    ],
                },
                {
                    id: 14,
                    score: 1,
                    children: [
                        {
                            id: 15,
                            score: 1,
                            children: [
                                {
                                    name: "Coyote",
                                },
                                {
                                    name: "Gorilla",
                                },
                            ],
                        },
                        {
                            id: 17,
                            score: 1,
                            children: [
                                {
                                    name: "Rabbit",
                                },
                                {
                                    name: "Egyptian fruit bat",
                                },
                            ],
                        },
                    ],
                },
            ],
        },
        {
            id: 2,
            score: 1,
            children: [
                {
                    id: 4,
                    score: 1,
                    children: [
                        {
                            name: "Chimpanzee",
                        },
                        {
                            name: "Cat",
                        },
                    ],
                },
                {
                    id: 3,
                    score: 1,
                    children: [
                        {
                            name: "Squirrel monkey",
                        },
                        {
                            name: "Rhesus macaque",
                        },
                    ],
                },
            ],
        },
        {
            id: 5,
            score: 0.7,
            children: [
                {
                    id: 6,
                    score: 1,
                    children: [
                        {
                            name: "Pig-tailed macaque",
                        },
                        {
                            name: "Olive baboon",
                        },
                    ],
                },
                {
                    id: 7,
                    score: 1,
                    children: [
                        {
                            id: 8,
                            score: 1,
                            children: [
                                {
                                    name: "Capuchin monkey",
                                },
                                {
                                    name: "Human",
                                },
                            ],
                        },
                        {
                            id: 9,
                            score: 1,
                            children: [
                                {
                                    name: "Owl monkey",
                                },
                                {
                                    name: "Galago",
                                },
                            ],
                        },
                    ],
                },
                {
                    id: 10,
                    score: 1,
                    children: [
                        {
                            id: 11,
                            score: 1,
                            children: [
                                {
                                    name: "Mouse",
                                },
                                {
                                    name: "Treeshrew",
                                },
                            ],
                        },
                        {
                            id: 12,
                            score: 1,
                            children: [
                                {
                                    name: "Rat",
                                },
                                {
                                    name: "Naked mole rat",
                                },
                            ],
                        },
                    ],
                },
            ],
        },
        {
            id: 10,
            score: 1,
            children: [
                {
                    id: 11,
                    score: 1,
                    children: [
                        {
                            name: "Opossum",
                        },
                        {
                            name: "Marmoset",
                        },
                    ],
                },
                {
                    name: "African green monkey",
                },
            ],
        },
    ],
};
