class Line {

    //This class is a copy/paste from the LearningCars project

    start = new Vector(0,0);
    end = new Vector(0,0);
    color = "#000000";
    width = 2; //This parameter only influences the display

    constructor(start, end) {
        this.start = start;
        this.end = end;
    }

    setStartEnd(start, end) {
        this.start = start;
        this.end = end;
        return this;
    }

    setWidth(width) {
        this.width = width;
        return this;
    }

    setColor(color) {
        this.color = color;
        return this;
    }

    //Checks if one of the lines is colliding with one of the otherLines
    static isColliding(lines, otherLines) {
        for(let line of lines)
            if(line.isColliding(otherLines))
                return true;
        return false;
    }

    //Checks if this line collides with one of the lines in parameter
    isColliding(lines) {
        for(let line of lines)
            if(this.collidesWith(line))
                return true;
        return false;
    }

    //Checks if this line collides with the line in parameter
    collidesWith(line) {
        return this.getCollisionPoint(line) !== null;
    }

    //Returns the collision point between this line and the one in parameter
    getCollisionPoint(other) {

        let point; //Le point d'intersection

        let slopeThis = (this.end.y - this.start.y) / (this.end.x - this.start.x);
        let slopeOther = (other.end.y - other.start.y) / (other.end.x - other.start.x);

        //Si les lignes sont paralleles
        if (slopeThis === slopeOther)
            return null;

        let interceptThis = this.start.y - slopeThis * this.start.x;
        let interceptOther = other.start.y - slopeOther * other.start.x;

        //Si une des lignes est verticale
        if (Math.abs(slopeThis) === Infinity)
            point = new Vector(this.start.x, slopeOther * this.start.x + interceptOther);
        else if (Math.abs(slopeOther) === Infinity)
            point = new Vector(other.start.x, slopeThis * other.start.x + interceptThis);
        else {

            //Cas normal (lignes non verticales et non paralleles)
            let x = (interceptOther - interceptThis) / (slopeThis - slopeOther);
            point = new Vector(x, slopeThis * x + interceptThis);
        }

        //Si le point est bien sur les deux segments
        if (point.x >= Math.min(this.start.x, this.end.x) && point.x <= Math.max(this.start.x, this.end.x) &&
            point.y >= Math.min(this.start.y, this.end.y) && point.y <= Math.max(this.start.y, this.end.y) &&
            point.x >= Math.min(other.start.x, other.end.x) && point.x <= Math.max(other.start.x, other.end.x) &&
            point.y >= Math.min(other.start.y, other.end.y) && point.y <= Math.max(other.start.y, other.end.y))
            return point;

        return null;
    }

    draw() {
        ctx.beginPath();
        ctx.strokeWidth = this.width;
        ctx.strokeStyle = this.color;
        ctx.moveTo(this.start.x, this.start.y);
        ctx.lineTo(this.end.x, this.end.y);
        ctx.stroke();
        ctx.closePath();
    }
}