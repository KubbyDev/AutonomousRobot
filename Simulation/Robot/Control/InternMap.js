class InternMap {

    matrix;
    controlAlgorithm;

    constructor(controlAlgorithm) {
        this.controlAlgorithm = controlAlgorithm;
        this.matrix = new BooleanMatrix(ControlAlgorithm.INTERNMAP_SIZE, ControlAlgorithm.INTERNMAP_SIZE);
    }

    update() {

        let wallDirection = Vector.fromOrientation(this.controlAlgorithm.expectedRotation);

        let hitDistance = this.controlAlgorithm.sonar.getDistance() / ControlAlgorithm.PIXEL_LENGTH;

        //Empties all the pixels between the robot and the hit point
        for(let i = 0; i < hitDistance; i++) {
            let position = wallDirection.multiply(i).add(this.controlAlgorithm.expectedPosition);
            this.turnPixelOff(Math.round(position.x), Math.round(position.y));
        }

        //Fills the pixel at the hit point
        let wallPosition = wallDirection.multiply(hitDistance).add(this.controlAlgorithm.expectedPosition);
        this.turnPixelOn(Math.round(wallPosition.x), Math.round(wallPosition.y));
    }

    // The lowResMap is the intern map but with a smaller resolution
    // A pixel in the lowResMap is on if at least 1 pixel of the intern map that touches it is on

    // Turns a pixel ON on the intern map
    // Also updates the lowResMap and controlAlgorithm.needsPathUpdate
    turnPixelOn(x, y) {

        if(!inBoundsReal(x,y))
            return;

        //If it is already on, does nothing
        if(this.matrix.getValue(x, y) === true)
            return;

        this.matrix.setValue(x, y, true);

        // The new pixel will set a wall on every pixel on the lowResMap it touches
        for(let j = -1; j <= 1; j++) {
            for(let i = -1; i <= 1; i++) {
                let lrX = Math.floor((x+i)/ControlAlgorithm.LOWRESMAP_SIZERATIO);
                let lrY = Math.floor((y+j)/ControlAlgorithm.LOWRESMAP_SIZERATIO);
                if(inBoundsLowRes(lrX, lrY) && (this.controlAlgorithm.lowResMap[lrY*ControlAlgorithm.LOWRESMAP_SIZE + lrX] !== 255)) {
                    this.controlAlgorithm.lowResMap[lrY*ControlAlgorithm.LOWRESMAP_SIZE + lrX] = 255;
                    this.controlAlgorithm.needsPathUpdate = true;
                }
            }
        }
    }

    // Turns a pixel OFF on the intern map
    // Also updates the lowResMap and controlAlgorithm.needsPathUpdate
    turnPixelOff(x, y) {

        if(!inBoundsReal(x,y))
            return;

        //If it is already off, does nothing
        if(this.matrix.getValue(x, y) !== true)
            return;

        this.matrix.setValue(x, y, false);

        // ONLY WORKS FOR LOWRESMAP_SIZERATION = 3
        let toCheck = [[0,0]];
        if(x % 3 === 0) toCheck.push([-1, 0]);
        if(x % 3 === 2) toCheck.push([1, 0]);
        if(y % 3 === 0) toCheck.push([0, -1]);
        if(y % 3 === 2) toCheck.push([0, 1]);
        if(x % 3 === 0 && y % 3 === 0) toCheck.push([-1, -1]);
        if(x % 3 === 2 && y % 3 === 2) toCheck.push([1, 1]);
        if(x % 3 === 0 && y % 3 === 2) toCheck.push([-1, 1]);
        if(x % 3 === 2 && y % 3 === 0) toCheck.push([1, -1]);

        for(let offset of toCheck) {

            let lrX = Math.floor(x/ControlAlgorithm.LOWRESMAP_SIZERATIO)+offset[0];
            let lrY = Math.floor(y/ControlAlgorithm.LOWRESMAP_SIZERATIO)+offset[1];

            if(!inBoundsLowRes(lrX,lrY) || this.controlAlgorithm.lowResMap[lrY*ControlAlgorithm.LOWRESMAP_SIZE + lrX] !== 255)
                continue;

            if(this.isPixelOff(lrX, lrY)) {
                this.controlAlgorithm.lowResMap[lrY*ControlAlgorithm.LOWRESMAP_SIZE + lrX] = 254;
                this.controlAlgorithm.needsPathUpdate = true;
            }
        }
    }

    isPixelOff(x, y) {
        x *= ControlAlgorithm.LOWRESMAP_SIZERATIO;
        y *= ControlAlgorithm.LOWRESMAP_SIZERATIO;
        for(let j = -1; j <= ControlAlgorithm.LOWRESMAP_SIZERATIO+1; j++)
            for(let i = -1; i <= ControlAlgorithm.LOWRESMAP_SIZERATIO+1; i++)
                if(inBoundsReal(x+i, y+j) && (this.matrix.getValue(x+i, y+j) === true))
                    return false;
        return true;
    }
}