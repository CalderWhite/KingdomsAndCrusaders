Grid grid;

boolean[][] generateTerrain(int n) {
  boolean[][] terrain = new boolean[n][n];
  for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
      if ( i < n/2 || j == 4 || j == 6) {
        terrain[i][j] = false;
      } else {
        terrain[i][j] = true;
      }
    }
  }
  
  return terrain;
}

void setup() {
  // CONSTANTS
  int n = 10;
  
  // WINDOW/GRAPHICS SETUP
  size(750, 750);
  fill(0);
  noStroke();
  
  // GAME OBJECTS
  grid = new Grid(n, generateTerrain(n));

  // random person generation
  //for (int i=0; i<5; i++) {
  //  Person p = new Person(i, int(random(0,n-1)), int(random(0,n-1)));
  //  grid.addPerson(p);
  //  Person p2 = new Person(i, int(random(0,n-1)), int(random(0,n-1)));
  //  grid.addPerson(p2);
  //}
  Person p = new Person(0, n-1, n/2);
  grid.addPerson(p);
  
  // INIT
  // flush the added game objects
  grid.flushBuffers();
}

void draw() {
  grid.updatePeople();
  
  clear();
  grid.render();
}
