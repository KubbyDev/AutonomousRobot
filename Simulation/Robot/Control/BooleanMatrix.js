class BooleanMatrix {

    //This class is not that useful but in the C implementation it will (because each boolean will be encoded on 1 bit)

    values;
    sizeX;
    sizeY;

    constructor(sizeX, sizeY) {
        this.sizeX = sizeX;
        this.sizeY = sizeY;
        this.values = new Array(sizeX);
        for(let i = 0; i < sizeX; i++)
            this.values[i] = new Array(sizeY);
    }

    getValue(x, y) {
        if(x >= 0 && y >= 0 && x < this.sizeX && y < this.sizeY)
            return this.values[x][y];
        return false;
    }

    setValue(x, y, value) {
        if(x >= 0 && y >= 0 && x < this.sizeX && y < this.sizeY)
            this.values[x][y] = value;
    }

    clear() {
        this.fill(0);
    }

    fill(value) {
        for(let y = 0; y < this.sizeY; y++)
            for(let x = 0; x < this.sizeX; x++)
                this.values[x][y] = value;
    }

}