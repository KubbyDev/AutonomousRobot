class Robot extends SceneObject {

    static SPEED = 80;
    static TURNRATE = 80;
    sonar;

    ctrlAlgoTime = 0; //The time left to update the control algorithm
    lastInput = [0,0];

    lastUpdateTime = timeSeconds(); //The moment the update function was last called

    constructor() {
        super();
        this.sonar = new Sonar(this);
        this.controlAlgorithm = new ControlAlgorithm(this.sonar);
        this.visibleLines.push(this.sonar.line);
    }

    update() {

        //Sets the target position in the intern map coordinates
        this.controlAlgorithm.target = getMousePosition().subtract(new Vector(550, 10)).divide(6).round();

        let deltaTime = timeSeconds() - this.lastUpdateTime;
        this.lastUpdateTime = timeSeconds();

        super.update();

        //Updates the sonar
        this.sonar.update();

        //Asks the control algorithm what to do
        //Only 20 times per second
        if(this.ctrlAlgoTime <= 0) {
            this.lastInput = this.controlAlgorithm.update();
            this.ctrlAlgoTime = 0.05;
        }
        else
            this.ctrlAlgoTime -= deltaTime;
        //this.moveForward(this.lastInput[0], deltaTime);
        //this.turn(this.lastInput[1], deltaTime);

        //Updates the position of the corners of the robot (and the hitbox at the same time) if necessary
        this.getCorners();

        //Collisions with walls
        if(this.isColliding()) //This function is in the super class (SceneObject)
            console.log("Collision !");
    }

    draw() {
        super.draw();

        this.sonar.draw();

        //Draws the intern map of the control algorithm

        let robotPosition = this.controlAlgorithm.expectedPosition;
        let robotRotation = this.controlAlgorithm.expectedRotation; //TOOD Show the orientation
        let matrix = this.controlAlgorithm.map.matrix;
        let offsetX = 550;
        let offsetY = 10;

        //Clears the map with grey so we can see the bounds
        ctx.fillStyle = "#CCCCCC";
        ctx.fillRect(offsetX, offsetY, ControlAlgorithm.INTERNMAP_SIZE*6, ControlAlgorithm.INTERNMAP_SIZE*6);

        //Sets every pixel where there is something to black
        ctx.fillStyle = "#000000";
        for (let y = 0; y < matrix.sizeY; y++)
            for (let x = 0; x < matrix.sizeX; x++)
                if (matrix.getValue(x, y))
                    ctx.fillRect(offsetX + (x*6), offsetY + (y*6), 6, 6);

        //Draws the robot
        drawSquare(robotPosition.add(new Vector(0.5,0.5)).multiply(6).add(new Vector(offsetX, offsetY)), 5, "#ff0938");

        //Draws the lowResMap of the control algorithm

        matrix = this.controlAlgorithm.lowResMap;
        offsetX = 1100;
        offsetY = 10;

        //Clears the map with grey so we can see the bounds
        ctx.fillStyle = "#CCCCCC";
        ctx.fillRect(offsetX, offsetY, ControlAlgorithm.LOWRESMAP_SIZE*16, ControlAlgorithm.LOWRESMAP_SIZE*16);

        //Draws each pixel
        for (let y = 0; y < ControlAlgorithm.LOWRESMAP_SIZE; y++) {
            for (let x = 0; x < ControlAlgorithm.LOWRESMAP_SIZE; x++) {
                let value = matrix[y*ControlAlgorithm.LOWRESMAP_SIZE + x];
                //Displays the value
                ctx.fillStyle = "#888888";
                ctx.fillText(value, offsetX + (x * 16), offsetY + (y * 16)+12);
                //Background
                if (value === 255) {
                    ctx.fillStyle = "#000000";
                    ctx.fillRect(offsetX + (x * 16), offsetY + (y * 16), 16, 16);
                }
            }
        }
    }

    moveForward(enginePower, deltaTime) {
        this.position.x += Math.cos(this.rotation*Math.PI/180) * (enginePower + noise(0.05)) * Robot.SPEED * deltaTime;
        this.position.y += Math.sin(this.rotation*Math.PI/180) * (enginePower + noise(0.05)) * Robot.SPEED * deltaTime;
        this.areCornersCorrect = false;
    }

    turn(input, deltaTime) {
        this.rotation += Robot.TURNRATE * -(input + noise(0.05)) * deltaTime;
        this.rotation = clampAngle(this.rotation);
        this.areCornersCorrect = false;
    }
}