
#include <tuple>
#include <ranges>
#include <array>
#include <map>
#include <string>
#include <ostream>
#include <iostream>
#include <fcntl.h>

using namespace std;
namespace sr = std::ranges;
using namespace std::string_literals;

class ChessGame{
    static constexpr wchar_t empty_space = L' '; //L'\u3000';
    enum Turn{
        White,
        Black
    } turn;

    std::array<
        std::array<
        wchar_t,
        8>
        ,8
    > board;

    std::array<int, 256> position_index;
    public:
    ChessGame()
    :
    turn{White},
    board{
        {L'♜',L'♞',L'♝',L'♛',L'♚',L'♝',L'♞',L'♜'
        ,L'♟',L'♟',L'♟',L'♟',L'♟',L'♟',L'♟',L'♟'
        ,empty_space,empty_space,empty_space,empty_space,empty_space,empty_space,empty_space,empty_space
        ,empty_space,empty_space,empty_space,empty_space,empty_space,empty_space,empty_space,empty_space
        ,empty_space,empty_space,empty_space,empty_space,empty_space,empty_space,empty_space,empty_space
        ,empty_space,empty_space,empty_space,empty_space,empty_space,empty_space,empty_space,empty_space
        ,L'♙',L'♙',L'♙',L'♙',L'♙',L'♙',L'♙',L'♙'
        ,L'♖',L'♘',L'♗',L'♕',L'♔',L'♗',L'♘',L'♖'}
    },
    position_index{56} {
        for(int i = 0; i < 8; ++i){
            position_index ['a' + i + 1] = i +1;
        }
    }
    auto &get_square(string pos){
        return board[8 - pos[1] + '0'][position_index[pos[0]]];
    }

    auto move(string from, string to){
        auto &f = get_square(from);
        auto &t = get_square(to);
        t = f;
        f = empty_space;
    }
    friend auto &operator <<(wostream &ot, const ChessGame &game){
        for(const auto&col : game.board){
            for( auto wc : col){
                ot << wc;
            }
            ot << L"\n"sv;
        }
        return ot;
    }
    bool done() const{
        return false;
    }
};


int main(){
    setlocale(LC_ALL, "en_US.UTF-8");
    ChessGame game;
    wcout << game;
    bool done = false;
    while(!done){
        wcout << L"Enter move: "sv;
        string from, to;
        cin >> from >> to;
        wcout << L"\x1b[1A                     \n";
        for(int i =0; i< 9;++i){
            wcout << "\x1b[1A";
        }
        game.move(from, to);

        wcout  << game;
        done = game.done();
    }
    return 0;
}
