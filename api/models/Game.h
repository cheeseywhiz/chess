#include "Games.h"

namespace drogon_model {
namespace sqlite3 {
class Game : public Games {
    public:
        using Games::Games;
        using Ptr = std::shared_ptr<Game>;

        // get Game by primary id
        static Ptr lookup_game(int game_id);
        static int last_insert_game_id(void);
        static Game last_insert_game(void);
};
}
}
