function findPath(controlAlgo) {

    // This algorithm is awful I'm ashamed
    // BUT ! It uses only n integers, n being the number of pixels of the lowresmap. So it fits my usage

    function get(x, y) {
        return controlAlgo.lowResMap[y*ControlAlgorithm.LOWRESMAP_SIZE + x];
    }

    function set(x, y, val) {
        controlAlgo.lowResMap[y*ControlAlgorithm.LOWRESMAP_SIZE + x] = val;
    }

    //Resets the pixels that are not walls in lowResMap (sets them to 254)
    for(let y = 0; y < ControlAlgorithm.LOWRESMAP_SIZE; y++)
        for(let x = 0; x < ControlAlgorithm.LOWRESMAP_SIZE; x++)
            if(get(x, y) !== 255)
                set(x, y, 254);

    const target = controlAlgo.target.divide(ControlAlgorithm.LOWRESMAP_SIZERATIO).applyFunc(Math.floor);
    set(target.x, target.y, 0);

    //List of all possible movements
    const offsets = [  [1,0], [0,1], [-1,0], [0,-1], [1,1], [-1,1], [-1,-1], [1,-1]  ];

    //Calculates the distances from the target
    let changed = true;
    while(changed) {

        changed = false;
        for(let y = 0; y < ControlAlgorithm.LOWRESMAP_SIZE; y++) {
            for(let x = 0; x < ControlAlgorithm.LOWRESMAP_SIZE; x++) {
                if(get(x, y) === 255)
                    continue;
                for(let offset of offsets) {
                    let newX = x+offset[0];
                    let newY = y+offset[1];
                    if(inBoundsLowRes(newX, newY) && get(newX, newY)+1 < get(x,y)) {
                        set(x, y, get(newX, newY)+1);
                        changed = true;
                    }
                }
            }
        }
    }

    controlAlgo.needsPathUpdate = false;
}