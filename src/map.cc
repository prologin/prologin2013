#include <istream>
#include <string>
#include <map>

#include <utils/log.hh>

#include "constant.hh"
#include "cell.hh"
#include "map.hh"

Map::Map()
{
}

Map::~Map()
{
    for (int y = 0; y < TAILLE_TERRAIN; ++y)
        for (int x = 0; x < TAILLE_TERRAIN; ++x)
            delete map_[y][x];
}

int Map::load(std::istream& s)
{
    INFO("Loading map");
    std::string line;
    int pos_x;
    int pos_y;

    for (int i = 0; i < MAX_JOUEURS; i++)
        s >> start_positions_[i].pos_x >> start_positions_[i].pos_y;

    for (int y = 0; y < TAILLE_TERRAIN; ++y)
    {
        s >> line;

        if (line.length() != (size_t) TAILLE_TERRAIN)
            FATAL("map: line %d is too short or too long "
                  "(is %d long, should be %d)",
                    y + MAX_JOUEURS, line.length(), TAILLE_TERRAIN);

        for (int x = 0; x < width_; ++x)
        {
            static std::map<char, zone_type> type_chars = {
                { '~', TERRAIN_MER },
                { 'o', TERRAIN_ILE },
                { '^', TERRAIN_VOLCAN },
            };

            if (type_chars.find(line[x]) == type_chars.end())
                FATAL("Invalid cell at y=%d x=%d", y, x);

            map_[y][x] = new Cell(y, x, type_chars[line[x]]);
        }
    }
    return 0;
}

Cell* Map::get_cell(position p) const
{
    return map_[p.y][p.x];
}