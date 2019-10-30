#include "PersonType.h"
#include "Person.h"

Person::Person(int c) : m_colony(c) {
    m_active = true;
}
Person::Person() {}

void Person::setSails(int row_dir, int col_dir) {
    m_row_dir = row_dir;
    m_col_dir = col_dir;
}

void Person::settle() {
    m_type = PersonType::Breeder;
}

void Person::setActive(bool s) {
    m_active = s;
}