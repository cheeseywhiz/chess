#pragma once
#include <drogon/HttpController.h>
using drogon::HttpRequestPtr, drogon::HttpResponsePtr;
using drogon::Get, drogon::Post;

class AdvCtrl : public drogon::HttpController<AdvCtrl> {
    public:
        METHOD_LIST_BEGIN
        //use METHOD_ADD to add your custom processing function here;
        //METHOD_ADD(AdvCtrl::get,"/{2}/{1}",Get);//path is /AdvCtrl/{arg2}/{arg1}
        //METHOD_ADD(AdvCtrl::your_method_name,"/{1}/{2}/list",Get);//path is /AdvCtrl/{arg1}/{arg2}/list
        //ADD_METHOD_TO(AdvCtrl::your_method_name,"/absolute/path/{1}/{2}/list",Get);//path is /absolute/path/{arg1}/{arg2}/list
        ADD_METHOD_TO(AdvCtrl::getAdv, "/adv/{int}/{string}/", Get);
        ADD_METHOD_TO(AdvCtrl::login, "/login", Get, Post);
        ADD_METHOD_TO(AdvCtrl::logout, "/logout", Get, Post);
        METHOD_LIST_END

        // your declaration of processing function maybe like this:
        void getAdv(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr&)>&& callback, int p1, const std::string& p2);
        void login(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr&)>&& callback);
        void logout(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr&)>&& callback);
};
