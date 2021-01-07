#pragma once
#include "auto/Users.h"

namespace drogon_model {
namespace sqlite3 {
class User : public Users {
    public:
        using Users::Users;
        using Ptr = std::shared_ptr<User>;

        // get User by primary key
        static Ptr lookup_user(const std::string& username);
};
}
}
