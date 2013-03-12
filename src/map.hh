#ifndef MAP_HH_
# define MAP_HH_

# include <array>

# include "constant.hh"
# include "cell.hh"

inline bool operator==(const position& a, const position& b)
{
    return a.x == b.x && a.y == b.y;
}

inline bool operator!=(const position& a, const position& b)
{
    return !(a == b);
}

class Map
{
    public:
        Map();
        ~Map();

        int load(std::istream& s);

        Cell* get_cell(position p) const;

    private:
        std::array<std::array<Cell*, TAILLE_TERRAIN>, TAILLE_TERRAIN> map_;
        std::array<position, MAX_JOUEURS> start_positions_;
};

#endif // !MAP_HH_
