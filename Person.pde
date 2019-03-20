class Person {
  PersonType type = PersonType.Breeder;
  int colony;
  int row;
  int col;
  Person(int colonyIn, int rowIn, int colIn) {
    colony = colonyIn;
    row = rowIn;
    col = colIn;
  }
  
  void setCoords(int rowIn, int colIn) {
    row = rowIn;
    col = colIn;
  }
}
