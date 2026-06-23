#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <iostream>
#include <conio.h>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <vector>

int main(int argc, char *argvs[])
{

    httplib::Client cli("https://apim-udp-dev-sea-001.azure-api.net");

    std::vector<nlohmann::json> columns;

    columns.push_back(
        {{"data_type", "INT"},
         {"default_value", ""},
         {"description", ""},
         {"field_name", "Id"},
         {"pii_tag", "No"}});
    columns.push_back(
        {{"data_type", "STRING"},
         {"default_value", ""},
         {"description", ""},
         {"field_name", "DocumentCode"},
         {"pii_tag", "No"}});

    nlohmann::json jColumns = {
        {"columns", columns}};

    nlohmann::json j = {
        {"system_name", "wineco"},
        {"business_unit", "wineco"},
        {"system_owner", "test@example.com"},
        {"table_name", "test_wineco_101"},
        {"data_owner", "test@example.com"},
        {"partition_information", ""},
        {"synchronization_type", "Batch"},
        {"data_schema", jColumns}};

    auto body = j.dump();

    httplib::Request req;
    req.method = "POST";
    req.path = "/fa-udp-data-ingestion-services-sub/v2/create_table";
    req.body = body;
    req.set_header("Content-Type", "application/json");
    req.set_header("ocp-apim-subscription-key", "f1eaa07fb21340b198d88ae00df77db3");

    auto res = cli.send(req);

    if (res)
    {
        std::cout << "Status: " << res->status << "\n";
        std::cout << "Body: " << res->body << "\n";
    }
    else
    {
        std::cout << "Request failed!\n";
    }
    std::cout << "Press any key to exit...";
    _getch();
    return 0;
}
