#ifndef MAP_HH_
#define MAP_HH_

#include <array>
#include <vector>

#include "cell.hh"
#include "constant.hh"

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
    Map(const Map& map);
    ~Map();

    int load(std::istream& s);

    bool valid_position(position p) const;
    Cell* get_cell(position p) const;

    std::vector<position> get_islands() const;

    position get_start_position(int i);

private:
    std::array<std::array<Cell*, TAILLE_TERRAIN>, TAILLE_TERRAIN> map_;
    std::array<position, MAX_JOUEURS> start_positions_;

    std::vector<position> islands_;
};

#endif // !MAP_HH_
