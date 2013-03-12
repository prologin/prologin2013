#ifndef CELL_HH_
#define CELL_HH_

#include <vector>
#include "constant.hh"

class Cell
{
    public:
        Cell(int y, int x, terrain type);

        terrain get_terrain() const;
        int get_or() const;
        int get_player() const;

        void bateau_add(int id);
        bool bateau_remove(int id);
        bool bateau_exists(int id);

    private:
        int x_;
        int y_;
        terrain type_;
        int or_;
        int player_;
        std::vector<int> id_bateaux_;
};

#endif // !CELL_HH_
