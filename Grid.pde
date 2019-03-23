static int MAX_COLONY_COUNT = 5;

class Grid {
  int gridLength;
  int blockLength;
  color[] colonyColorList = {color(255, 0, 0), color(238, 0, 255), color(255, 250, 0), color(0, 255, 0), color(255, 255, 255)};

  Person[][] grid;
  Person[][] gridNext;
  
  boolean[][] terrainGrid;
  int landSize;
  
  int[] colonyCount = new int[MAX_COLONY_COUNT];

  Grid(int lengthIn, boolean[][] terrain) {
    grid = new Person[lengthIn][lengthIn];
    gridNext = new Person[lengthIn][lengthIn];
    
    terrainGrid = terrain;
    for (int i=0; i<lengthIn; i++) {
      for (int j=0; j<lengthIn; j++) {
        if (terrain[i][j]) {
          ++landSize;
        }
      }
    }

    gridLength = lengthIn;
    blockLength = width/lengthIn;
  }
  
  void updatePersonPosition(Person p) {
    if (p.lastRow >= 0) {
      gridNext[p.lastRow][p.lastCol] = null;
    }
    
    gridNext[p.row][p.col] = p;
  }
  
  void addPerson(Person p) {
    gridNext[p.row][p.col] = p;
  }
  
  void removePerson(Person p) {
    gridNext[p.row][p.col] = null;
  }
  
  void flushBuffers() {
    // deep copy just in case the pointers don't change in processing
    for (int i=0; i<gridLength; i++) {
      for (int j=0; j<gridLength; j++) {
        grid[i][j] = gridNext[i][j];
      }
    }
  }
  
  void refreshBuffers() {
    // deep copy just in case the pointers don't change in processing
    for (int i=0; i<gridLength; i++) {
      for (int j=0; j<gridLength; j++) {
        gridNext[i][j] = grid[i][j];
      }
    }
  }
  
  Person safeGetPerson(int row, int col) {
    Person p = null;
    try {
      p = grid[row][col];
    } catch (ArrayIndexOutOfBoundsException e) {
      return null;
    };
    
    return p;
  }
  
  boolean positionOnLand(int row, int col) {
    boolean onLand = false;
    try {
      onLand = terrainGrid[row][col];
    } catch (ArrayIndexOutOfBoundsException e) {}; 
    
    return onLand;
  }
  
  void updateColonyCount() {
    colonyCount = new int[MAX_COLONY_COUNT];
    
    for (int i=0; i<gridLength; i++) {
      for (int j=0; j<gridLength; j++) {
        Person p = safeGetPerson(i, j);
        
        if (p != null) {
          colonyCount[p.colony]++;
        }
      }
    }
  }
  
  void attemptReproduction(Person p) {
    // this probability will increase with a colony's population, as they acquire more resources
    // the more people there are, the more land they own.
    // the correlation between population count and birth rate directly affects 
    // a population's "aggressiveness", as the way they attack is really by having a child
    // in an open space. This also affects the speed at which the conquest.
    // This behaviour simulates how "colonies" or "kingdoms" progress
    float colonyPower = colonyCount[p.colony];
    
    // by using the total grid size as the denominator, this simulates the finite resources, and that once
    // a colony has all the squares covered, they will complete power
    float birthProbability = 0.1 + 0.1*(colonyPower/(landSize));
    
    
    if (random(0,1) <= birthProbability) {
      ArrayList<int[]> freeSquares = new ArrayList<int[]>();
      for (int i=-1; i<=1; i++) {
        for (int j=-1; j<=1; j++) {
          if (i != 0 || j != 0) {
            Person other = safeGetPerson(p.row + i, p.col + j);
            
            // we also have to check that this is in bounds, since if it is out of bounds
            // safeGetPerson returns null, since there was no person there
            boolean inBounds = true;
            try {
              Person inBoundsTestPerson = grid[p.row + i][p.col + j];
            } catch (ArrayIndexOutOfBoundsException e) {
              inBounds = false;
            }

            if (other == null && inBounds && positionOnLand(p.row + i, p.col + j)) {
              int[] freePos = {p.row + i, p.col + j};
              freeSquares.add(freePos);
            }
          }
        }
      }
    
      if (freeSquares.size() > 0) {
        int index = int(random(0, freeSquares.size()));
        int[] childPos = freeSquares.get(index);
        
        // create the new person of the same colony at the coords at index
        Person child = new Person(p.colony, childPos[0], childPos[1]);
        
        addPerson(child);
      }
    }
  }
  
  // returns true if an enemy was killed (meaning it is now dead as well)
  boolean killOneEnemyNeighbor(Person p) {
    for (int i=-1; i<=1; i++) {
      for (int j=-1; j<=1; j++) {
        if (i != 0 || j != 0) {
          Person neighbor = safeGetPerson(p.row + i, p.col + j);
          
          // if the neighbor has a different colony, delete both people
          if (neighbor != null && neighbor.colony != p.colony) {
            removePerson(p);
            removePerson(neighbor);
            
            return true;
          }
        }
      }
    }
    
    return false;
  }
  
  void updatePersonState(Person p) {
    ArrayList<int[]> directions = new ArrayList<int[]>();
    switch(p.type) {
      case Breeder:
        for (int i=-1; i<=1; i++) {
          for (int j=-1; j<=1; j++) {
            if (i != 0 || j != 0) {
              try {
                if (!terrainGrid[p.row + i][p.col + j]) {
                  int[] pos = {i, j};
                  directions.add(pos);
                }
              } catch (ArrayIndexOutOfBoundsException e) {
              }
            }
          }
        }
        
        if (directions.size() >= 1) {
          int randomIndex = int(random(0,directions.size()));
          
          // update the person's properties
          int row = directions.get(randomIndex)[0];
          int col = directions.get(randomIndex)[1];
          p.setSails(row, col);
        }
        break;
      case Sailor:
        if (positionOnLand(p.row, p.col)) {
          p.settle();
        }
        break;
      case Settler:
        break;
    }
  }
  
  void render() {
    // maybe make this faster by keeping a list of people later on
    for (int i=0; i<gridLength; i++) {
      for (int j=0; j<gridLength; j++) {
        Person p = safeGetPerson(i, j);
        
        if (p != null) {
          fill(colonyColorList[p.colony]);
          rect(p.col * blockLength, p.row * blockLength, blockLength, blockLength);
        } else {
          if (terrainGrid[i][j]) {
            fill(237, 232, 175);
            rect(j * blockLength, i * blockLength, blockLength, blockLength);
          } else {
            fill(0, 0, 255);
            rect(j * blockLength, i * blockLength, blockLength, blockLength);
          }
        }
      }
    }
    
    fill(0);
  }
  
  void updatePeople() {
    // copy the buffers
    refreshBuffers();
    
    // maybe make this faster by keeping a list of people later on
    for (int i=0; i<gridLength; i++) {
      for (int j=0; j<gridLength; j++) {
        Person p = safeGetPerson(i, j);
        
        if (p != null) {
          boolean isDead = killOneEnemyNeighbor(p);
          
          if (!isDead) {
            updatePersonState(p);

            switch (p.type) {
              case Breeder:
                attemptReproduction(p);
                break;
              case Sailor:
                p.updatePos();
                
                // if they fall off the map, may they rest in peace.
                try {
                  updatePersonPosition(p);
                } catch (ArrayIndexOutOfBoundsException e) {
                }
                break;
              case Settler:
                attemptReproduction(p);
                break;
            }
          }
        }
      }
    }
    
    // update the colony populations
    updateColonyCount();
    
    // flush the grid buffer
    flushBuffers();
  }
}
