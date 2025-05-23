
#include <tuple>
#include <ranges>
#include <array>
#include <map>
#include <string>
#include <ostream>
#include <iostream>
#include <fcntl.h>
#include <unordered_set>
#include <set>
#include <vector>

using namespace std;
namespace sr = std::ranges;
using namespace std::string_literals;

inline constexpr wchar_t space = L' '; //L'\u3000';
enum class Player{
    White,
    Black,
    Pink,
    Cyan,
    None,
};

enum class PieceType{
    Pawn,
    Rook,
    Knight,
    Bishop,
    Queen,
    King,
    None
};
PieceType piece_type(wchar_t c){
    switch(c){
        case L'♙':
        case L'♟':
            return PieceType::Pawn;
        case L'♖':
        case L'♜':
            return PieceType::Rook;
        case L'♘':
        case L'♞':
            return PieceType::Knight;
        case L'♗':
        case L'♝':
            return PieceType::Bishop;
        case L'♕':
        case L'♛':
            return PieceType::Queen;
        case L'♔':
        case L'♚':
            return PieceType::King;
        default:
            return PieceType::None;
    }
}
auto getpotentialmoves(int x, int y, PieceType pt) -> vector<tuple<int,int>>{
   switch(pt){
    case PieceType::Pawn:
        if(y == 1){
            return {{x, y+1}, {x, y+2}};
        }
        else{
            return {{x, y+1}};
        }
    case PieceType::Rook:
        return {{0, y}, {1,y},{2,y},{3,y},{4,y},{5,y},{6,y},{7,y},{x,0},{x,1},{x,2},{x,3},{x,4},{x,5},{x,6},{x,7}};
    case PieceType::Knight:
        return {{x+1, y+2}, {x+2, y+1}, {x+2, y-1}, {x+1, y-2}, {x-1, y-2}, {x-2, y-1}, {x-2, y+1}, {x-1, y+2}};
    case PieceType::Bishop:
        return {{x+1, y+1}, {x+2, y+2}, {x+3, y+3}, {x+4, y+4}, {x+5, y+5}, {x+6, y+6}, {x+7, y+7}, {x-1, y-1}, {x-2, y-2}, {x-3, y-3}, {x-4, y-4}, {x-5, y-5}, {x-6, y-6}, {x-7, y-7}, {x+1, y-1}, {x+2, y-2}, {x+3, y-3}, {x+4, y-4}, {x+5, y-5}, {x+6, y-6}, {x+7, y-7}, {x-1, y+1}, {x-2, y+2}, {x-3, y+3}, {x-4, y+4}, {x-5, y+5}, {x-6, y+6}, {x-7, y+7}};
    case PieceType::Queen:
        return {{0, y}, {1,y},{2,y},{3,y},{4,y},{5,y},{6,y},{7,y},{x,0},{x,1},{x,2},{x,3},{x,4},{x,5},{x,6},{x,7},{x+1, y+1}, {x+2, y+2}, {x+3, y+3}, {x+4, y+4}, {x+5, y+5}, {x+6, y+6}, {x+7, y+7}, {x-1, y-1}, {x-2, y-2}, {x-3, y-3}, {x-4, y-4}, {x-5, y-5}, {x-6, y-6}, {x-7, y-7}, {x+1, y-1}, {x+2, y-2}, {x+3, y-3}, {x+4, y-4}, {x+5, y-5}, {x+6, y-6}, {x+7, y-7}, {x-1, y+1}, {x-2, y+2}, {x-3, y+3}, {x-4, y+4}, {x-5, y+5}, {x-6, y+6}, {x-7, y+7}};
    case PieceType::King:
        return {{x+1, y+1}, {x+1, y}, {x+1, y-1}, {x, y+1}, {x, y-1}, {x-1, y+1}, {x-1, y}, {x-1, y-1}};
    default:
        return {};
    }
   
return {};
}



class Piece{
    static constexpr array<wchar_t,6> white_pieces{{L'♙',L'♕',L'♔',L'♗',L'♘',L'♖'}};
    static constexpr array<wchar_t,6> black_pieces{{L'♟',L'♜',L'♞',L'♝',L'♛',L'♚'}};
    wchar_t visual;
    public:
    Player owner;

    Piece(wchar_t visual) : visual{visual} {
        if (visual == space){
            owner = Player::None;
        }
        else if (sr::find(white_pieces, visual) != std::end(white_pieces)){
            owner = Player::White;
        }
        else if (sr::find(black_pieces, visual) != std::end(black_pieces)){
            owner = Player::Black;
        }

    }
    friend wostream &operator<< (wostream &ost, const Piece &p){
        ost << p.visual;
        return ost;
    }
    bool operator==(wchar_t c) const{
        return visual == c;
    }
};

class ChessGame{
    Player turn;

    std::array<
        std::array<
        Piece,
        8>
        ,8
    > board;

    std::array<int, 256> position_index;
    public:
    ChessGame()
    :
    turn{Player::White},
    board{
        {L'♜',L'♞',L'♝',L'♛',L'♚',L'♝',L'♞',L'♜'
        ,L'♟',L'♟',L'♟',L'♟',L'♟',L'♟',L'♟',L'♟'
        ,space,space,space,space,space,space,space,space
        ,space,space,space,space,space,space,space,space
        ,space,space,space,space,space,space,space,space
        ,space,space,space,space,space,space,space,space
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

    auto switch_players(){
        if (turn == Player::White){
            turn = Player::Black;
        }
        else if (turn == Player::Black){
            turn = Player::White;
        }
    }

    auto move(string from, string to){
        auto &f = get_square(from);
        auto &t = get_square(to);
        t = f;
        f = space;
    }

    void play(string from, string to){
        auto &f = get_square(from);
        if(f.owner != turn){
            wcout << "Wrong Piece\n";
            return;
        }
        auto &t = get_square(to);
        t = f;
        f = space;
        switch_players();
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
        int cnt = 0;
        for (const auto &col : board){
            for (const auto &piece : col){
                if( piece == L'♚' || piece == L'♔'){
                    cnt += 1;
                }
            }
        }
        return cnt != 2;
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
        game.play(from, to);

        wcout  << game;
        done = game.done();
    }
    wcout << L"Game Over\n"sv;
    return 0;
}

