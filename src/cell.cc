#include <map>

#include "constant.hh"
#include "tools.hh"
#include "cell.hh"

Cell::Cell(int y, int x, terrain type)
    : x_(x),
      y_(y),
      type_(type),
      gold_(-1),
      player_(-1)
{
}

terrain Cell::get_type() const
{
    return type_;
}

int Cell::get_gold() const
{
    return gold_;
}

void Cell::set_gold(int gold)
{
    gold_ = gold;
}

int Cell::get_player() const
{
    return player_;
}

void Cell::set_player(int player)
{
    player_ = player;
}

void Cell::add_boat(int boat_id)
{
    boat_ids_.insert(boat_id);
}

bool Cell::remove_boat(int boat_id)
{
    return boat_ids_.erase(boat_id) != 0;
}

bool Cell::exists_boat(int boat_id)
{
    return boat_ids_.count(boat_id);
}

std::set<int> Cell::get_id_boats()
{
    return boat_ids_;
}

void Cell::resolve_fight(std::map<int, bateau>& boats, int id_attacker)
{
    int id_p1 = -1;
    int id_p2 = -1;

    int galions_p1 = 0;
    int galions_p2 = 0;

    int galions_lost = 0;

    int caravelle_winner = -1;

    int id_winner = -1;
    int id_loser = -1;

    int i;

    for (std::set<int>::iterator it = boat_ids_.begin(); it != boat_ids_.end();
            it++)
    {
        i = *it;
        if (id_p1 == -1)
            id_p1 = boats[i].joueur;
        else if (id_p2 == -1 && boats[i].joueur != id_p1)
            id_p2 = boats[i].joueur;

        if (boats[i].btype == BATEAU_GALION)
        {
            if (boats[i].joueur == id_p1)
                galions_p1++;
            if (boats[i].joueur == id_p2)
                galions_p2++;
        }
    }


    if (!galions_p1 || !galions_p2)
        return;

    if (galions_p1 > galions_p2 ||
            (galions_p1 == galions_p2 &&
                ((player_ == -1 && id_p1 == id_attacker) ||
                id_p1 == player_)
            )
       )
    {
        id_winner = id_p1;
        id_loser = id_p2;
        galions_lost = galions_p2 - 1; /* The winner loses galions_enemy-1 */
    }
    else
    {
        id_winner = id_p2;
        id_loser = id_p1;
        galions_lost = galions_p1 - 1; /* The winner loses galions_enemy-1 */
    }

    if (player_ == id_loser)
        player_ = id_winner;

    for (std::set<int>::iterator i = boat_ids_.begin(); i != boat_ids_.end();
            i++)
        if (boats[*i].btype == BATEAU_CARAVELLE &&
            boats[*i].joueur == id_winner)
            caravelle_winner = boats[*i].id;

    int gold_move;

    for (std::set<int>::iterator it = boat_ids_.begin(); it != boat_ids_.end();
            it++)
    {
        i = *it;
        if (boats[i].btype == BATEAU_CARAVELLE)
        {
            if (boats[i].joueur != id_winner)
            {
                gold_move = boats[i].nb_or;
                boats.erase(i);
                remove_boat(i);
                if (caravelle_winner != -1)
                    boats[caravelle_winner].nb_or += gold_move;
            }
        }
        else
        {
            if (boats[i].joueur == id_winner && galions_lost)
            {
                boats.erase(i);
                remove_boat(i);
                galions_lost--;
            }
            else if (boats[i].joueur == id_loser)
            {
                boats.erase(i);
                remove_boat(i);
            }
        }
    }
}
