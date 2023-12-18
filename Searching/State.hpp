#include <ostream>
#include <vector>


enum BoatSide { Left, Right };

class State {
    int Ml, Cl, Mr, Cr;
    BoatSide boat_side;

    int m, c;

    public:
        State * previous = nullptr;

        State(int _Ml, int _Cl, int _Mr, int _Cr, BoatSide _boat_side);

        State to_transite(int _m, int _c);

        int get_max_m();
        int get_max_c();

        std::vector<State> get_path();

        bool is_valid();
        bool is_cycle(State s);

        bool operator==(const State& s);

        friend std::ostream& operator<<(std::ostream& os, const State& s);
};

