#pragma once

#include "PersonType.h"
#include "Position.h"

class Person {
public:
    Person(int colony);
    Person();

    void setSails(int row_dir, int col_dir);

    inline Position getDirection() const {
        return Position{m_row_dir, m_col_dir};
    }

    void settle();

    inline int getColony() const {
        return m_colony;
    }

    inline bool getActive() const {
        return m_active;
    }

    inline PersonType getType() const {
        return m_type;
    }

    void setActive(bool s);

private:
    int m_colony;
    
    int m_row_dir = 0;
    int m_col_dir = 0;

    // this is needed since the Person grid is initialized with a bunch of empty people
    bool m_active = false;

    PersonType m_type = PersonType::Breeder;
};