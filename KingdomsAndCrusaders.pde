Grid grid;

// GAME CONSTANTS
int n = 200;
int SPAWNS_PER_COLONY = 2;
int NUM_COLONIES = 5; // max of 5
int FRAME_RATE = 10; // decreasing this can be quite fascinating!

// if you are interest in solely the colony vs colony behaviour, turn this off
boolean ISLANDS = true;

// PROCEDURAL ISLAND GENERATION CONSTANTS
float NOISE_INCREMENT = 0.05*(200/float(n));
float ISLAND_MIN = 0;
float ISLAND_MAX = 70;

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
  // WINDOW/GRAPHICS SETUP
  size(800, 800);
  fill(0);
  noStroke();
  frameRate(FRAME_RATE);

  // GAME OBJECTS
  boolean[][] terrain;
  if (ISLANDS) {
    terrain = generateTerrain(n);
  } else {
    terrain = new boolean[n][n];
    for (int i=0; i<n; i++) {
      for (int j=0; j<n; j++) {
        terrain[i][j] = true;
      }
    }
  }
  grid = new Grid(n, terrain);

  // gather the land coordinates for spawning
  ArrayList<int[]> landCoords = new ArrayList<int[]>();
  for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
      if (terrain[i][j]) {
        int[] pos = {i, j};
        landCoords.add(pos);
      }
    }
  }

  // random person generation
  for (int i=0; i<NUM_COLONIES; i++) {
    for (int j=0; j< SPAWNS_PER_COLONY; j++) {
      // randomly get position from available land positions
      if (landCoords.size() >= 1) {
        int index = int(random(0, landCoords.size()));
        int[] pos = landCoords.get(index);

        Person p = new Person(i, pos[0], pos[1]);
        p.settle();
        grid.addPerson(p);

        landCoords.remove(index);
      }
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
