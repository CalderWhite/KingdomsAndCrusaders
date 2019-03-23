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
    
    rowOffset = -1;
    colOffset = 0;
  }
  
  void setCoords(int rowIn, int colIn) {
    lastRow = row;
    lastCol = col;
    row = rowIn;
    col = colIn;
  }
  
  void updatePos() {
    setCoords(row + rowOffset, col + colOffset);
  }
  
  void setSails(int rowDirection, int colDirection) {
    rowOffset = rowDirection;
    colOffset = colDirection;
    
    type = PersonType.Sailor;
  }
  
  void settle() {
    type = PersonType.Settler;
  }
}
