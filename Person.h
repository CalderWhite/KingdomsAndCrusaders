#pragma once

#include "PersonType.h"

class Person {
public:
    Person(char colony);
    Person();

    void setSails(char row_dir, char col_dir);

    inline void getDirection(char* row, char* col) const {
        *row = m_row_dir;
        *col = m_col_dir;
    }

    void settle();

    inline char getColony() const {
        return m_colony;
    }

    inline void setColony(int c) {
        m_colony = c;
    }

    inline bool getActive() const {
        return m_active;
    }

    inline PersonType getType() const {
        return m_type;
    }

    void setActive(bool s);

private:
    char m_colony;
    
    char m_row_dir = 0;
    char m_col_dir = 0;

    // this is needed since the Person grid is initialized with a bunch of empty people
    bool m_active = false;

    PersonType m_type = PersonType::Breeder;
};
