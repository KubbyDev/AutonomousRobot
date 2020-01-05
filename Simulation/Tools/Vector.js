class Vector {

    //This class is a copy/paste from the LearningCars project

    x;
    y;

    constructor(x, y) {
        this.x = x;
        this.y = y;
    }

    static get zero() {
        return new Vector(0,0);
    }

    add(other) {
        return new Vector(this.x + other.x, this.y + other.y);
    }

    subtract(other) {
        return new Vector(this.x - other.x, this.y - other.y);
    }

    multiply(mult) {
        return new Vector(this.x * mult, this.y * mult);
    }

    divide(div) {
        return new Vector(this.x / div, this.y / div);
    }

    length() {
        return Math.sqrt(this.sqrLength());
    }

    sqrLength() {
        return this.x*this.x + this.y*this.y;
    }

    normalized() {
        return this.divide(this.length());
    }

    static sqrDistance(a, b) {
        return new Vector(b.x - a.x, b.y - a.y).sqrLength();
    }

    static distance(a, b) {
        return Math.sqrt(Vector.sqrDistance(a, b));
    }

    //Calculates a unit vector pointing in the angle direction (0 => (1,0), 1 => (0,1)). Angle in degrees
    static fromOrientation(angle) {
        angle = angle*Math.PI/180;
        return new Vector(Math.cos(angle), Math.sin(angle));
    }

    toString() {
        return "x: " + this.x + ", y: " + this.y;
    }

    disp() {
        console.log(this.toString());
    }

    //Constructs a line from startPoint to startPoint+thisVector
    toLine(startPoint) {
        return new Line().setStartEnd(startPoint, startPoint.add(this))
    }

    copy() {
        return new Vector(this.x, this.y);
    }

    equals(other, threshold) {
        if(threshold === undefined)
            threshold = 0.001;
        return Math.abs(this.x - other.x) < threshold && Math.abs(this.y - other.y) < threshold;
    }

    applyFunc(func) {
        return new Vector(func(this.x), func(this.y));
    }

    round() {
        return this.applyFunc(Math.round);
    }
}