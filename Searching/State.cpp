#include "State.hpp"

#include <algorithm>
#include <iostream>


State::State(int _Ml, int _Cl, int _Mr, int _Cr, BoatSide _boat_side)
            : Ml(_Ml), Cl(_Cl), Mr(_Mr), Cr(_Cr),
              boat_side(_boat_side),
              m(0), c(0), previous(nullptr)
{ }

State State::to_transite(int _m, int _c) {
    BoatSide new_boat_side = (boat_side == Left ? Right: Left);

    int newMl = Ml,
        newCl = Cl,
        newMr = Mr,
        newCr = Cr;

    if (new_boat_side == Left) {
        newMr = newMr - _m;
        newCr = newCr - _c;
        newMl = newMl + _m;
        newCl = newCl + _c;
    }

    if (new_boat_side == Right) {
        newMr = newMr + _m;
        newCr = newCr + _c;
        newMl = newMl - _m;
        newCl = newCl - _c;
    }

    State s= State(newMl, newCl, newMr, newCr, new_boat_side);
    s.m = _m,
    s.c = _c;
    return s;
}

int State::get_max_m() {
    return (boat_side == Left ? Ml : Mr);
}

int State::get_max_c() {
    return (boat_side == Left ? Cl : Cr);
}

bool State::is_cycle(State s) {
    return boat_side != s.boat_side and
           m == s.m and
           c == s.c;
}

std::vector<State> State::get_path() {
    std::vector<State> path;

    State * current = this;

    while (current) {
        path.push_back(*current);
        current = current->previous;
    }

    std::reverse(path.begin(), path.end());

    return path;
}

bool State::is_valid() {
    bool base_cond = Ml >= 0 and
                     Mr >= 0 and
                     Cl >= 0 and
                     Cr >= 0;

    return base_cond and not((Ml > 0 && Cl > Ml) or (Mr > 0 && Cr > Mr));
}

bool State::operator==(const State& s) {
    return boat_side == s.boat_side and
           Ml == s.Ml and
           Cl == s.Cl and
           Mr == s.Mr and
           Cr == s.Cr;
}

std::ostream& operator<<(std::ostream& os, const State& s) {
    char B = (s.boat_side == Left ? 'L': 'R');

    os << s.Ml << ' '
       << s.Cl << '|'
       << s.Mr << ' '
       << s.Cr << '|'
       << B;

    return os;
}

