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

    int caravelle_p1 = -1;
    int caravelle_p2 = -1;

    int winner = -1;
    int id_winner = -1;

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
        else
        {
            if (boats[i].joueur == id_p1 && caravelle_p1 == -1)
                caravelle_p1 = boats[i].id;
            if (boats[i].joueur == id_p2 && caravelle_p2 == -1)
                caravelle_p2 = boats[i].id;
        }
    }

    if (!galions_p1 || !galions_p2)
        return;

    if (galions_p1 > galions_p2)
    {
        id_winner = id_p1;
        winner = 1;
        galions_p2--; /* The winner loses galions_enemy - 1 */
    }
    else if (galions_p2 > galions_p1)
    {
        id_winner = id_p2;
        winner = 2;
        galions_p1--;
    }
    else
    {
        if (id_p1 == player_)
        {
            id_winner = id_p1;
            winner = 1;
        }
        else if (id_p2 == player_)
        {
            id_winner = id_p2;
            winner = 2;
        }
        else if (id_p1 == id_attacker)
        {
            id_winner = id_p1;
            winner = 1;
        }
        else if (id_p2 == id_attacker)
        {
            id_winner = id_p2;
            winner = 2;
        }
        else
            winner = 0;
    }


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
                if (winner == 1 && caravelle_p1 != -1)
                    boats[caravelle_p1].nb_or += gold_move;
                else if (winner == 2 && caravelle_p2 != -1)
                    boats[caravelle_p2].nb_or += gold_move;
            }
        }
        else
        {
            if (boats[i].joueur == id_p1 && galions_p2)
            {
                boats.erase(i);
                remove_boat(i);
                galions_p2--;
            }

            if (boats[i].joueur == id_p2 && galions_p1)
            {
                boats.erase(i);
                remove_boat(i);
                galions_p1--;
            }
        }
    }
}
