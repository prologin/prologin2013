#include <istream>
#include <string>
#include <map>
#include <array>

#include <utils/log.hh>
#include <rules/player.hh>

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

    for (int i = 0; i < MAX_JOUEURS; i++)
    {
        s >> start_positions_[i].x >> start_positions_[i].y;
        INFO("Player %d starting position is x=%d, y=%d",
                i, start_positions_[i].x, start_positions_[i].y);
    }

    for (int y = 0; y < TAILLE_TERRAIN; ++y)
    {
        s >> line;

        if (line.length() != (size_t) TAILLE_TERRAIN)
            FATAL("map: line %d is too short or too long "
                  "(is %d long, should be %d)",
                    y + MAX_JOUEURS, line.length(), TAILLE_TERRAIN);

        for (int x = 0; x < TAILLE_TERRAIN; ++x)
        {
            static std::map<char, terrain> type_chars = {
                { '~', TERRAIN_MER },
                { 'o', TERRAIN_ILE },
                { '^', TERRAIN_VOLCAN },
            };

            if (type_chars.find(line[x]) == type_chars.end())
                FATAL("Invalid cell at y=%d x=%d", y, x);

            map_[y][x] = new Cell(y, x, type_chars[line[x]]);
            if (type_chars[line[x]] == TERRAIN_VOLCAN ||
                type_chars[line[x]] == TERRAIN_ILE)
                islands_.push_back({y, x});
        }
    }

    return 0;
}

bool Map::valid_position(position p) const
{
    return 0 <= p.x && p.x < TAILLE_TERRAIN &&
           0 <= p.y && p.y < TAILLE_TERRAIN;
}

Cell* Map::get_cell(position p) const
{
    if (!valid_position(p))
        return NULL;
    return map_[p.y][p.x];
}

std::vector<position> Map::get_islands() const
{
    return islands_;
}

position Map::get_start_position(int i)
{
    if (i < (int) start_positions_.size())
        return start_positions_[i];
    return { -1, -1 };
}
