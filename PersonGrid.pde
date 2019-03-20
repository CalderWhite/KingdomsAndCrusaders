static int MAX_COLONIES = 20;

class PersonGrid {
  int gridLength;
  int blockLength;
  color[] colonyColorList = {color(255, 0, 0), color(238, 0, 255), color(255, 250, 0), color(0, 255, 0), color(255, 255, 255)};

  Person[][] grid;
  Person[][] gridNext;
  
  int[] colonyCount = new int[MAX_COLONIES];
  
  PersonGrid(int lengthIn) {
    grid = new Person[lengthIn][lengthIn];
    gridNext = new Person[lengthIn][lengthIn];

    gridLength = lengthIn;
    blockLength = width/lengthIn;
  }
  
  void movePerson(Person p, int row, int col) {
    gridNext[row][col] = null;
    
    p.setCoords(row, col);
    
    gridNext[row][col] = p;
  }
  
  void addPerson(Person p) {
    gridNext[p.row][p.col] = p;
    
    ++colonyCount[p.colony];
  }
  
  void removePerson(Person p) {
    gridNext[p.row][p.col] = null;
    
    --colonyCount[p.colony];
  }
  
  void asd() {
    // deep copy just in case the pointers don't change in processing
    for (int i=0; i<gridLength; i++) {
      for (int j=0; j<gridLength; j++) {
        grid[i][j] = gridNext[i][j];
      }
    }
  }
  
  void refreshGridBuffer() {
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
  
  void attemptReproduction(Person p) {
    // 30% chance that the person breeds to a random square that is free
    // doesn't breed if none are free
    // this probability will increase with a colony's population, as they acquire more resources
    // the more people there are, the more land they own.
    // the correlation between population count and birth rate directly affects 
    // a population's "aggressiveness", as the way they attack is really by having a child
    // in an open space. This also affects the speed at which the conquest.
    // This behaviour simulates how "colonies" or "kingdoms" progress
    float colonyPower = colonyCount[p.colony];
    
    // by using the total grid size as the denominator, this simulates the finite resources, and that once
    // a colony has all the squares covered, they will complete power
    float birthProbability = 0.1 + 0.1*(colonyPower/(gridLength*gridLength));
    
    
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
            
            if (other == null && inBounds) {
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
  
  void render() {
    // maybe make this faster by keeping a list of people later on
    for (int i=0; i<gridLength; i++) {
      for (int j=0; j<gridLength; j++) {
        Person p = safeGetPerson(i, j);
        
        if (p != null) {
          fill(colonyColorList[p.colony]);
          rect(p.row * blockLength, p.col * blockLength, blockLength, blockLength);
        }
      }
    }
    
    fill(0);
  }
  
  void updatePeople() {
    // copy 
    refreshGridBuffer();
    
    // maybe make this faster by keeping a list of people later on
    for (int i=0; i<gridLength; i++) {
      for (int j=0; j<gridLength; j++) {
        Person p = safeGetPerson(i, j);
        
        if (p != null) {
          boolean isDead = killOneEnemyNeighbor(p);
          
          if (!isDead) {
            attemptReproduction(p);
          }
        }
      }
    }
    
    // flush the grid buffer
    flushGridBuffer();
  }
}
