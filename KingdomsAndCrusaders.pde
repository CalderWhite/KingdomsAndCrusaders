int n = 100;
PersonGrid grid;
Person p;
Person p2;

void setup() {
  size(500, 500);
  fill(0);
  noStroke();
  
  grid = new PersonGrid(n);

  // random person generation
  for (int i=0; i<10; i++) {
    p = new Person(int(random(0,5)), int(random(0,n-1)), int(random(0,n-1)));
    grid.addPerson(p);
  }
  
  grid.flushGridBuffer();
}

void draw() {
  grid.updatePeople();
  
  clear();
  grid.render();
}
