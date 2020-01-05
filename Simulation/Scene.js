let sceneHitboxLines = [];
let sceneVisibleLines = [];
let objects = [];

//Updates all the objects of the scene
function updateScene() {
    for(let scobj of objects)
        scobj.update();
}

//Clears the canvas and draws all the objects of the scene
function drawScene() {
    for(let line of sceneVisibleLines)
       line.draw();
    for(let scobj of objects)
        scobj.draw();
}

//Adds an object to the scene.
function addObject(toAdd) {
    objects.push(toAdd);
    toAdd.calcCorners();
    for(let line of toAdd.hitboxLines) sceneHitboxLines.push(line);
    for(let line of toAdd.visibleLines) sceneVisibleLines.push(line);
}

//Adds a line to the scene (visible and with a hitbox)
function addWall(line) {
    sceneVisibleLines.push(line);
    sceneHitboxLines.push(line);
}