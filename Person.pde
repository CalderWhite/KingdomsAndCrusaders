class Person {
  PersonType type = PersonType.Breeder;
  int colony;
  
  int lastCol;
  int lastRow;
  
  int row;
  int col;
  
  int rowOffset;
  int colOffset;

  Person(int colonyIn, int rowIn, int colIn) {
    colony = colonyIn;
    row = rowIn;
    col = colIn;
    
    lastCol = -1;
    lastRow = -1;
  }
  
  void setCoords(int rowIn, int colIn) {
    lastRow = row;
    lastCol = col;
    row = rowIn;
    col = colIn;
  }
  
  void updatePerson() {
    setCoords(row + rowOffset, col + colOffset);
  }
}
