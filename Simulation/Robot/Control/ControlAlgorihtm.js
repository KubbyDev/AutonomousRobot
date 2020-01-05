class ControlAlgorithm {

    //The length of a pixel of the map in the simulation world units
    static PIXEL_LENGTH = 9;
    static INTERNMAP_SIZE = 72; //Must be multiple of 8
    static LOWRESMAP_SIZERATIO = 3;
    static LOWRESMAP_SIZE = ControlAlgorithm.INTERNMAP_SIZE/ControlAlgorithm.LOWRESMAP_SIZERATIO;

    expectedPosition;
    expectedRotation;
    map;
    lowResMap;
    sonar;
    lastUpdateTime = timeSeconds();

    needsPathUpdate = true;
    target;

    constructor(sonar) {
        this.sonar = sonar;
        this.map = new InternMap(this);
        this.lowResMap = new Array(ControlAlgorithm.LOWRESMAP_SIZE * ControlAlgorithm.LOWRESMAP_SIZE);
    }

    resetPositionAndRotation(newPosition, newRotation) {
        this.expectedPosition = newPosition;
        this.expectedRotation = newRotation;
    }

    update() {

        let deltaTime = timeSeconds() - this.lastUpdateTime;
        this.lastUpdateTime = timeSeconds();

        let sonarData = this.sonar.getDistance();

        let forwardInput = 0;
        let turnInput = 0;
        let targetPosition = this.getNextPosition();
        if(targetPosition) {
            let inputs = this.getMovementInput(targetPosition);
            forwardInput = inputs[0];
            turnInput = inputs[1];
        }

/*
        //The last factors are the turn rate of the robot and its speed multiplied
        //by the size ratio between the intern map and the simulation world.
        //They will have to be determined experimentaly on the real robot
        this.expectedPosition.x += Math.cos(this.expectedRotation*Math.PI/180)
            * forwardInput * Robot.SPEED/ControlAlgorithm.PIXEL_LENGTH * deltaTime;
        this.expectedPosition.y += Math.sin(this.expectedRotation*Math.PI/180)
            * forwardInput * Robot.SPEED/ControlAlgorithm.PIXEL_LENGTH * deltaTime;
        this.expectedRotation += -turnInput * Robot.TURNRATE * deltaTime;
        this.expectedRotation = clampAngle(this.expectedRotation);
*/

        //Updates the map according to the data of the sonar
        //If the path needs to be recalculated, this function will change this.needsPathUpdate
        if(isFinite(sonarData)) //The sonar distance is infinite if the distance > Sonar.RANGE
            this.map.update();

        //Updates the path if the map update decided to
        if(this.needsPathUpdate)
            findPath(this);

        return [
            forwardInput,
            turnInput,
        ];
    }

    //Calculates the inputs in order to reach the target
    getMovementInput(target) {

        // Calculates the needed turn input to reach the target
        let targetAngle = 180/Math.PI * Math.atan2(target.y - this.expectedPosition.y, target.x - this.expectedPosition.x);
        targetAngle = clampAngle(targetAngle);
        if(targetAngle - this.expectedRotation > 180)
            targetAngle -= 360;
        if(targetAngle - this.expectedRotation < -180)
            targetAngle += 360;
        let angleDiff = Math.abs(targetAngle - this.expectedRotation);
        let turnInput = 0.5 * (targetAngle > this.expectedRotation ? -1 : 1);

        // If no turn is needed, goes forward
        let forwardInput = angleDiff > 8 ? 0 : 1;

        return [forwardInput, turnInput];
    }

    //Gets the position next "checkpoint" to get to the target
    getNextPosition() {

        let self = this;
        function get(x, y) {
            return self.lowResMap[y*ControlAlgorithm.LOWRESMAP_SIZE + x];
        }

        //List of all possible movements
        const offsets = [  [1,0], [0,1], [-1,0], [0,-1], [1,1], [-1,1], [-1,-1], [1,-1]  ];

        let pos = this.expectedPosition.divide(ControlAlgorithm.LOWRESMAP_SIZERATIO).round(); //TODO: comprendre pourquoi round
        let minIndex = 0;
        let min = Infinity;
        for(let i = 0; i < offsets.length; i++) {
            let newX = pos.x+offsets[i][0];
            let newY = pos.y+offsets[i][1];
            if(inBoundsLowRes(newX, newY) && get(newX, newY) < min) {
                min = get(newX, newY);
                minIndex = i;
            }
        }

        return pos
            .add(new Vector(offsets[minIndex][0], offsets[minIndex][1]))
            .multiply(ControlAlgorithm.LOWRESMAP_SIZERATIO);
    }
}