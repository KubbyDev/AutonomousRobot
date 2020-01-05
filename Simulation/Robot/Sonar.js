class Sonar  {

    parentRobot;
    line;

    hitPoint;
    distance;

    static RANGE = 400;

    constructor(parentRobot) {
        this.parentRobot = parentRobot;
        this.line = new Line(this.parentRobot.position, this.parentRobot.position).setColor("#00FF00");
    }

    update() {

        //Updates the line according to the new position/rotation of the robot
        this.line.setStartEnd(
            this.parentRobot.position,
            Vector.fromOrientation(this.parentRobot.rotation).multiply(Sonar.RANGE).add(this.parentRobot.position)
        );

        //Updates the hitPoint and the distance (closest hit)
        let closest = new Vector(Infinity, Infinity);
        let minDistance = Infinity;
        let lines = sceneHitboxLines.filter(line => this.parentRobot.hitboxLines.indexOf(line) === -1);
        for(let line of lines) {
            let collisionPoint = line.getCollisionPoint(this.line);
            if(collisionPoint != null && Vector.distance(collisionPoint, this.parentRobot.position) < minDistance) {
                minDistance = Vector.distance(collisionPoint, this.parentRobot.position);
                closest = collisionPoint;
            }
        }
        this.hitPoint = closest;
        this.distance = minDistance;
    }

    draw() {
        drawSquare(this.hitPoint, 2, "#FF0000");
    }

    getDistance() {
        if(this.distance > Sonar.RANGE)
            return Infinity;
        return this.distance + this.distance * noise(0.05);
    }
}