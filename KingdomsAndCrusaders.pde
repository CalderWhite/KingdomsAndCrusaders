Grid grid;

// GAME CONSTANTS
int n = 100;
int SPAWNS_PER_COLONY = 2;
int NUM_COLONIES = 5; // do note, sometimes spawns sail off the map and die. So, you will not always see this many colonies
int FRAME_RATE = 60; // decreasing this can be quite fascinating!

// PROCEDURAL ISLAND GENERATION CONSTANTS
float NOISE_INCREMENT = 0.05;
float ISLAND_MIN = 0;
float ISLAND_MAX = 50;

// Processing has a perlin noise function built in!
// This allows us to create procedurally generated islands very easily, as all we need are boolean values
// I did this by setting a range in which the noise must be in for land to occur there.
// I scale up the perlin value to 1 byte (255). This allows the ISLAND_MIN and ISLAND_MAX to be more user friendly,
// as opposed to extremely small floating point values.
boolean[][] generateTerrain(int terrainSize) {
  boolean[][] terrain = new boolean[terrainSize][terrainSize];
  float detail = map(10, 0, width, 0.1, 0.6);
  noiseDetail(8, detail);
  
  // This goes through the generic perlin noise map in 2D where the 
  // value of the noise at each row,column position determines whether or not it is land.
  // I found it easier to think about it like a topographical map, and then taking a cross section of that map.
  float rowOff = 0.0;
  for (int i=0; i<terrainSize; i++) {
    rowOff += NOISE_INCREMENT;
    float colOff = 0.0;
    for (int j=0; j<terrainSize; j++) {
      colOff += NOISE_INCREMENT;
      
      float value = noise(rowOff, colOff)*255;
      if (ISLAND_MIN < value && value < ISLAND_MAX) {
        terrain[i][j] = true;
      }
    }
  }
  
  return terrain;
}

void setup() {
  // CONSTANTS
  int n = 100;
  
  // WINDOW/GRAPHICS SETUP
  size(750, 750);
  fill(0);
  noStroke();
  frameRate(FRAME_RATE);
  
  // GAME OBJECTS
  grid = new Grid(n, generateTerrain(n));

  // random person generation
  for (int i=0; i<NUM_COLONIES; i++) {
    for (int j=0; j< SPAWNS_PER_COLONY; j++) {
      Person p = new Person(i, int(random(0,n-1)), int(random(0,n-1)));
      grid.addPerson(p);
    }
  }
  
  // INIT
  // this flush adds the people we just added into the map
  grid.flushBuffers();
}

void draw() {
  grid.updatePeople();
  
  clear();
  grid.render();
}
