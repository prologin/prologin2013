/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2013 Prologin
*/

#include "api.hh"

#include <iostream>
#include <sstream>
#include <vector>

// from api.cc
extern Api* api;

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& arr)
{
  os << "[";
  typename std::vector<T>::const_iterator it;
  for (it = arr.begin(); it != arr.end(); ++it)
  {
    if (it != arr.begin())
      os << ", ";
    os << *it;
  }
  os << "]";
  return os;
}


// todo avoir un ostringstream a la place de std::string

std::string convert_to_string(int i){
  std::ostringstream s;
  s << i;
  std::string result = s.str();
  return result;
}
std::string convert_to_string(std::string i){
  return i;
}
std::string convert_to_string(bool i){
  return i?"true":"false";
}
std::string convert_to_string(std::vector<int> in){
  if (in.size()){
    std::string s = "[" + convert_to_string(in[0]);
    for (int i = 1, l = in.size(); i < l; i++){
      s = s + ", " + convert_to_string(in[i]);
    }
    return s + "]";
  }else{
    return "[]";
  }
}
std::string convert_to_string(bateau_type in){
  switch (in)
  {
    case BATEAU_CARAVELLE: return "\"bateau_caravelle\"";
    case BATEAU_GALION: return "\"bateau_galion\"";
    case BATEAU_ERREUR: return "\"bateau_erreur\"";
  }
  return "bad value";
}
std::string convert_to_string(std::vector<bateau_type> in){
  if (in.size()){
    std::string s = "[" + convert_to_string(in[0]);
    for (int i = 1, l = in.size(); i < l; i++){
      s = s + ", " + convert_to_string(in[i]);
    }
    return s + "]";
  }else{
    return "[]";
  }
}
std::string convert_to_string(terrain in){
  switch (in)
  {
    case TERRAIN_ILE: return "\"terrain_ile\"";
    case TERRAIN_VOLCAN: return "\"terrain_volcan\"";
    case TERRAIN_MER: return "\"terrain_mer\"";
    case TERRAIN_RECIF: return "\"terrain_recif\"";
    case TERRAIN_ERREUR: return "\"terrain_erreur\"";
  }
  return "bad value";
}
std::string convert_to_string(std::vector<terrain> in){
  if (in.size()){
    std::string s = "[" + convert_to_string(in[0]);
    for (int i = 1, l = in.size(); i < l; i++){
      s = s + ", " + convert_to_string(in[i]);
    }
    return s + "]";
  }else{
    return "[]";
  }
}
std::string convert_to_string(erreur in){
  switch (in)
  {
    case OR_INSUFFISANT: return "\"or_insuffisant\"";
    case ILE_INVALIDE: return "\"ile_invalide\"";
    case CHEMIN_INVALIDE: return "\"chemin_invalide\"";
    case BATEAUX_INSUFFISANTS: return "\"bateaux_insuffisants\"";
    case ILE_COLONISEE: return "\"ile_colonisee\"";
    case AUCUNE_CARAVELLE: return "\"aucune_caravelle\"";
  }
  return "bad value";
}
std::string convert_to_string(std::vector<erreur> in){
  if (in.size()){
    std::string s = "[" + convert_to_string(in[0]);
    for (int i = 1, l = in.size(); i < l; i++){
      s = s + ", " + convert_to_string(in[i]);
    }
    return s + "]";
  }else{
    return "[]";
  }
}
std::string convert_to_string(position in){
  std::string x = convert_to_string(in.x);
  std::string y = convert_to_string(in.y);
  std::string out = "{";
  out += "x:" + x;
  out += ", ";
  out += "y:" + y;
  return out + "}";
}

std::string convert_to_string(std::vector<position> in){
  if (in.size()){
    std::string s = "[" + convert_to_string(in[0]);
    for (int i = 1, l = in.size(); i < l; i++){
      s = s + ", " + convert_to_string(in[i]);
    }
    return s + "]";
  }else{
    return "[]";
  }
}
std::string convert_to_string(bateau in){
  std::string id = convert_to_string(in.id);
  std::string pos = convert_to_string(in.pos);
  std::string equipe = convert_to_string(in.equipe);
  std::string type = convert_to_string(in.type);
  std::string or = convert_to_string(in.or);
  std::string deplacable = convert_to_string(in.deplacable);
  std::string vivant = convert_to_string(in.vivant);
  std::string out = "{";
  out += "id:" + id;
  out += ", ";
  out += "pos:" + pos;
  out += ", ";
  out += "equipe:" + equipe;
  out += ", ";
  out += "type:" + type;
  out += ", ";
  out += "or:" + or;
  out += ", ";
  out += "deplacable:" + deplacable;
  out += ", ";
  out += "vivant:" + vivant;
  return out + "}";
}

std::string convert_to_string(std::vector<bateau> in){
  if (in.size()){
    std::string s = "[" + convert_to_string(in[0]);
    for (int i = 1, l = in.size(); i < l; i++){
      s = s + ", " + convert_to_string(in[i]);
    }
    return s + "]";
  }else{
    return "[]";
  }
}
///
// Retourne la taille de la carte.
//
extern "C" position api_taille_carte()
{
  return api->taille_carte();
}

///
// Retourne la nature du terrain désigné par ``pos``.
//
extern "C" terrain api_info_terrain(position pos)
{
  return api->info_terrain(pos);
}

///
// Retourne l'équipe qui possède l'île à l'emplacement ``pos``. Retourne 0 si l'île est libre. Retourne -1 si la position indiquée n'est pas une île
//
extern "C" int api_info_ile_equipe(position pos)
{
  return api->info_ile_equipe(pos);
}

///
// Retourne l'or contenu sur l'île à l'emplacement ``pos``. Retourne 0 si la case spécifiée n'est pas une île.
//
extern "C" int api_info_ile_or(position pos)
{
  return api->info_ile_or(pos);
}

///
// Retourne le bateau ayant pour identifiant ``id``
//
extern "C" bateau api_info_bateau(int id)
{
  return api->info_bateau(id);
}

///
// Retourne la liste de bateaux à la position ``pos``
//
extern "C" std::vector<bateau> api_liste_bateaux_position(position pos)
{
  return api->liste_bateaux_position(pos);
}

///
// Retourne la liste des ID des bateaux à la position ``pos``
//
extern "C" std::vector<int> api_liste_id_bateaux_position(position pos)
{
  return api->liste_id_bateaux_position(pos);
}

///
// Retourne la liste des positions des îles
//
extern "C" std::vector<position> api_liste_iles()
{
  return api->liste_iles();
}

///
// Retourne une liste de positions correspondant au plus court chemin d'un point à un autre en passant seulement par des cases accessibles
//
extern "C" std::vector<position> api_chemin()
{
  return api->chemin();
}

///
// Construire un bateau de type ``btype`` sur l'île à la position ``pos``
//
extern "C" erreur api_construire(bateau_type btype, position pos)
{
  return api->construire(btype, pos);
}

///
// Déplace le bateau représenté par l'identifiant ``id`` suivant une liste ``liste_pos`` de positions
//
extern "C" erreur api_deplacer(int id, std::vector<position> liste_pos)
{
  return api->deplacer(id, liste_pos);
}

///
// Colonise l'île à la position ``pos``
//
extern "C" erreur api_coloniser(position pos)
{
  return api->coloniser(pos);
}

///
// Charge la caravelle identifiée par ``id`` de ``nb_or`` d'or.
//
extern "C" erreur api_charger(int id, int nb_or)
{
  return api->charger(id, nb_or);
}

///
// Décharge la caravelle identifiée par ``id`` de ``nb_or`` d'or.
//
extern "C" erreur api_decharger(int id, int nb_or)
{
  return api->decharger(id, nb_or);
}

///
// Retourne le numéro de votre joueur
//
extern "C" int api_mon_joueur()
{
  return api->mon_joueur();
}

///
// Retourne les scores de chaque joueur
//
extern "C" std::vector<int> api_scores()
{
  return api->scores();
}

///
// Retourne le numéro du tour actuel
//
extern "C" int api_tour_actuel()
{
  return api->tour_actuel();
}

///
// Retourne le nombre total de tours de toute la partie
//
extern "C" int api_nombre_tours()
{
  return api->nombre_tours();
}

///
// Affiche le contenu d'une valeur de type bateau_type
//
std::ostream& operator<<(std::ostream& os, bateau_type v)
{
  switch (v) {
  case BATEAU_CARAVELLE: os << "BATEAU_CARAVELLE"; break;
  case BATEAU_GALION: os << "BATEAU_GALION"; break;
  case BATEAU_ERREUR: os << "BATEAU_ERREUR"; break;
  }
  return os;
}
extern "C" void api_afficher_bateau_type(bateau_type v)
{
  std::cerr << v << std::endl;
}

///
// Affiche le contenu d'une valeur de type terrain
//
std::ostream& operator<<(std::ostream& os, terrain v)
{
  switch (v) {
  case TERRAIN_ILE: os << "TERRAIN_ILE"; break;
  case TERRAIN_VOLCAN: os << "TERRAIN_VOLCAN"; break;
  case TERRAIN_MER: os << "TERRAIN_MER"; break;
  case TERRAIN_RECIF: os << "TERRAIN_RECIF"; break;
  case TERRAIN_ERREUR: os << "TERRAIN_ERREUR"; break;
  }
  return os;
}
extern "C" void api_afficher_terrain(terrain v)
{
  std::cerr << v << std::endl;
}

///
// Affiche le contenu d'une valeur de type erreur
//
std::ostream& operator<<(std::ostream& os, erreur v)
{
  switch (v) {
  case OR_INSUFFISANT: os << "OR_INSUFFISANT"; break;
  case ILE_INVALIDE: os << "ILE_INVALIDE"; break;
  case CHEMIN_INVALIDE: os << "CHEMIN_INVALIDE"; break;
  case BATEAUX_INSUFFISANTS: os << "BATEAUX_INSUFFISANTS"; break;
  case ILE_COLONISEE: os << "ILE_COLONISEE"; break;
  case AUCUNE_CARAVELLE: os << "AUCUNE_CARAVELLE"; break;
  }
  return os;
}
extern "C" void api_afficher_erreur(erreur v)
{
  std::cerr << v << std::endl;
}

///
// Affiche le contenu d'une valeur de type position
//
std::ostream& operator<<(std::ostream& os, position v)
{
  os << "{ ";
  os << "x" << "=" << v.x;
  os << ", ";
  os << "y" << "=" << v.y;
  os << " }";
  return os;
}
extern "C" void api_afficher_position(position v)
{
  std::cerr << v << std::endl;
}

///
// Affiche le contenu d'une valeur de type bateau
//
std::ostream& operator<<(std::ostream& os, bateau v)
{
  os << "{ ";
  os << "id" << "=" << v.id;
  os << ", ";
  os << "pos" << "=" << v.pos;
  os << ", ";
  os << "equipe" << "=" << v.equipe;
  os << ", ";
  os << "type" << "=" << v.type;
  os << ", ";
  os << "or" << "=" << v.or;
  os << ", ";
  os << "deplacable" << "=" << v.deplacable;
  os << ", ";
  os << "vivant" << "=" << v.vivant;
  os << " }";
  return os;
}
extern "C" void api_afficher_bateau(bateau v)
{
  std::cerr << v << std::endl;
}
