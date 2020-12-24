#include <cassert>
#include "User.h"
#include <drogon/HttpAppFramework.h>

namespace drogon_model {
namespace sqlite3 {
User::Ptr User::lookup_user(const std::string& username) {
    const auto& db = drogon::app().getDbClient();
    const auto& rows = db->execSqlSync(sqlForFindingByPrimaryKey(), username);
    // not found?
    if (rows.size() != 1)
        return nullptr;
    return std::make_shared<User>(rows[0]);
}
}
}
