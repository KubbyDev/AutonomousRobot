class SceneObject {

    hitboxLines = [new Line(), new Line(), new Line(), new Line()];     //Contains the lines forming the hitbox
    visibleLines = [];     //Contains the visible lines of this object

    corners = []; //Positions of the corners of the object
    areCornersCorrect = false; //True just after a call of calcCorners, False just after a movement of the object.

    color = '#' + (Math.random().toString(16) + "000000").substring(2,8);

    position = new Vector(0,0);
    rotation = 0;
    width = 5;
    length = 12;

    update() {
        //areCornersCorrect don't go false because the object doesn't move here
    }

    //Returns the position of the corners of the object (calculates them if necessary)
    getCorners() {
        if(!this.areCornersCorrect)
            this.calcCorners();
        return this.corners;
    }

    //Calculates the position of the corners of the object
    calcCorners() {

        let degR = this.rotation*Math.PI/180;

        this.corners[0] = new Vector(
            this.position.x + this.length*Math.cos(degR) - this.width*Math.sin(degR),
            this.position.y + this.length*Math.sin(degR) + this.width*Math.cos(degR)
        );

        this.corners[1] = new Vector(
            this.position.x + this.length*Math.cos(degR) + this.width*Math.sin(degR),
            this.position.y + this.length*Math.sin(degR) - this.width*Math.cos(degR)
        );

        this.corners[2] = new Vector(
            this.position.x - this.length*Math.cos(degR) + this.width*Math.sin(degR),
            this.position.y - this.length*Math.sin(degR) - this.width*Math.cos(degR)
        );

        this.corners[3] = new Vector(
            this.position.x - this.length*Math.cos(degR) - this.width*Math.sin(degR),
            this.position.y - this.length*Math.sin(degR) + this.width*Math.cos(degR)
        );

        this.hitboxLines[0].setStartEnd(this.corners[0], this.corners[1]);
        this.hitboxLines[1].setStartEnd(this.corners[1], this.corners[2]);
        this.hitboxLines[2].setStartEnd(this.corners[2], this.corners[3]);
        this.hitboxLines[3].setStartEnd(this.corners[3], this.corners[0]);

        this.areCornersCorrect = true;
    }

    draw() {
        drawRect(this.getCorners(), this.color);
    }

    //Checks if one of the lines of the object is colliding with one of the other lines of the scene
    isColliding() {
        this.getCorners();
        return Line.isColliding(this.hitboxLines, sceneHitboxLines.filter(line => this.hitboxLines.indexOf(line) === -1));
    }
}