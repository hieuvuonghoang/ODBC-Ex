
#include "dbr_api_service.h"

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>

#include <nlohmann/json.hpp>

using namespace nlohmann;
using namespace databricks;

DBRApiService::DBRApiService(const Config& config) {
    apiKey = config.GetAPIKey();
    baseUrl = config.GetBaseUrl();
    createTableEndpoint = config.GetCreateTableAPI();
}

void DBRApiService::CreateTable(Table& table) const {
    httplib::Client cli(baseUrl);
    httplib::Request req;
    req.method = "POST";
    req.path = createTableEndpoint;
    req.body = table.GetBody();
    req.set_header("Content-Type", "application/json");
    req.set_header("ocp-apim-subscription-key", apiKey);
    auto res = cli.send(req);
    table.SetSuccess(false);
    if (res)
    {
        if (res->status == 200)
        {
            table.SetSuccess(true);
            nlohmann::json j = nlohmann::json::parse(res->body);
            table.SetTableId(j.at("table_id").get<std::string>());
            table.SetMes(j.at("message").get<std::string>());
        }
        else if (res->status == 500)
        {
            nlohmann::json j = nlohmann::json::parse(res->body);
            table.SetMes(j.at("detail").get<std::string>());
        }
        else
        {
            std::cout << "Status: " << res->status << "\n";
            nlohmann::json j = nlohmann::json::parse(res->body);
            std::cout << "Error: " << j.dump(2) << "\n";
        }
    }
    else
    {
        table.SetMes("Request failed!");
    }
};