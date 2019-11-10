#include "PersonType.h"
#include "Person.h"

Person::Person(char c) : m_colony(c) {
    m_active = true;
}
Person::Person() {}

void Person::setSails(char row_dir, char col_dir) {
    m_row_dir = row_dir;
    m_col_dir = col_dir;

    m_type = PersonType::Sailor;
}

void Person::settle() {
    m_type = PersonType::Breeder;
}

void Person::setActive(bool s) {
    m_active = s;

    if (m_active == false) {
        m_row_dir = 0;
        m_col_dir = 0;
        m_type = PersonType::Breeder;
    }
}
