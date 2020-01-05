//Draws a 4 points polygon defined by the points in the points array
function drawRect(points, color) {
    ctx.fillStyle = color;
    ctx.beginPath();
    ctx.moveTo(points[0].x, points[0].y);
    ctx.lineTo(points[1].x, points[1].y);
    ctx.lineTo(points[2].x, points[2].y);
    ctx.lineTo(points[3].x, points[3].y);
    ctx.closePath();
    ctx.fill();
}

//Draws a square with given position (center), halfside (size length / 2), and color
function drawSquare(position, halfside, color) {
    ctx.fillStyle = color;
    ctx.rect(position.x - halfside, position.y - halfside, halfside*2, halfside*2);
    ctx.fill();
}

//Generates a random noise between -amount and +amount
function noise(amount) {
    return (Math.random()*2 -1)*amount;
}

//Returns the current time in seconds
function timeSeconds() {
    return new Date().getTime()/1000;
}

let mouseX = 0;
let mouseY = 0;
document.addEventListener('mousemove', function(event) {
    mouseX = event.clientX - canvas.offsetLeft;
    mouseY = event.clientY - canvas.offsetTop;
});

function getMousePosition() {
    return new Vector(mouseX, mouseY);
}

//Wraps the angle to this interval [0,360] (degrees)
function clampAngle(angle) {
    angle = angle % 360;
    if(angle < 0)
        angle += 360;
    return angle;
}

function inBoundsLowRes(x, y) {
    return x >= 0 && y >= 0 && x < ControlAlgorithm.LOWRESMAP_SIZE && y < ControlAlgorithm.LOWRESMAP_SIZE;
}

function inBoundsReal(x, y) {
    return x >= 0 && y >= 0 && x < ControlAlgorithm.INTERNMAP_SIZE && y < ControlAlgorithm.INTERNMAP_SIZE;
}