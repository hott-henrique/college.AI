#include <algorithm>
#include <deque>
#include <iostream>
#include <thread>
#include <tuple>
#include <vector>

#include "State.hpp"


void bfs_step(int boat_sz, std::deque<State *>& border);

std::tuple<bool, std::vector<State>> search_for_solution(std::deque<State *>& b1, std::deque<State *>& b2);

void remove_expandeds(std::deque<State *>& b, std::vector<State *>& m, int n);

void free_memory(std::deque<State *>& border, std::vector<State *>& mem);

int main() {
    const int boat_sz = 2;

    State s0 {0, 0, 3, 3, Right};
    State sf {3, 3, 0, 0, Left};

    std::deque<State *> down_border, up_border;
    std::vector<State *> down_mem, up_mem;

    down_border.push_back(new State(s0));
    up_border.push_back(new State(sf));

    /* while(true) with counter. */
    for (int i = 0; true; i = i + 1) {
        int n_down = down_border.size();
        int n_up = up_border.size();

        std::thread down(bfs_step, boat_sz, std::ref(down_border));
        std::thread up(bfs_step, boat_sz, std::ref(up_border));

        down.join();
        up.join();

        auto search_result = search_for_solution(down_border, up_border);

        if (std::get<0>(search_result)) {
            std::cout << ">>> Solution found at " << i + 1 << "th iteration." << std::endl;

            auto path = std::get<1>(search_result);

            std::cout << "M C|M C|B"   << std::endl
                      << "---------"   << std::endl;

            for (int j = 0; j < path.size(); j = j + 1) {
                std::cout << path[j] << std::endl;
            }

            break;
        }

        remove_expandeds(down_border, down_mem, n_down);
        remove_expandeds(up_border, up_mem, n_up);
    }

    free_memory(down_border, down_mem);
    free_memory(up_border, up_mem);

    return 0;
}

void bfs_step(int boat_sz, std::deque<State*>& border) {
    int n = border.size();

    /* Expand all border. */
    for (int i = 0; i < n; i = i + 1) {
        State * s = border[i];

        /* For each possible transition. */
        for (int m = 0; m <= boat_sz and m <= s->get_max_m(); m = m + 1) {
            int c = (m == 0 ? 1 : 0);

            /* For each possible transition. */
            for (; c <= (boat_sz - m) and c <= s->get_max_c(); c = c + 1) {
                State new_s = s->to_transite(m, c);

                if (not new_s.is_valid() or new_s.is_cycle(*s)) {
                    continue;
                }

                new_s.previous = s;

                border.push_back(new State(new_s));
            }
        }
    }
}

std::tuple<bool, std::vector<State>> search_for_solution(std::deque<State *>& b1, std::deque<State *>& b2) {
    std::vector<State> path;

    for (State * s: b1) {
        auto r = std::find_if(b2.begin(), b2.end(), [s](State * a){ return *s == *a; });

        if (r != b2.end()) {
            auto path_down = s->get_path();

            auto path_up = (*r)->get_path();
            std::reverse(path_up.begin(), path_up.end());

            path.insert(path.end(), path_down.begin(), path_down.end());
            path.insert(path.end(), path_up.begin(), path_up.end());

            path.erase(std::unique(path.begin(), path.end()), path.end());

            return std::make_tuple(true, path);
        }
    }

    return std::make_tuple(false, path);
}

void remove_expandeds(std::deque<State *>& b, std::vector<State *>& m, int n) {
    for (int i = 0; i < n; i = i + 1) {
        State * s = b.front();
        b.pop_front();
        m.push_back(s);
    }
}

void free_memory(std::deque<State *>& border, std::vector<State *>& mem) {
    while (not border.empty()) {
        State * s = border.front();
        border.pop_front();
        delete s;
    }

    while (not mem.empty()) {
        State * s = mem.back();
        mem.pop_back();
        delete s;
    }
}

