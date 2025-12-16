#include "../../../cpp-httplib/httplib.h"
#include "../open_render.hpp"
#include "../../../json/single_include/nlohmann/json.hpp"
#include <string.h>
using json = nlohmann::json;

typedef std::pair<std::pair<openrender::Point, openrender::Point>,
        std::map<std::string, std::pair<std::string,
        std::pair<std::vector<openrender::Point>, openrender::Color>>>>
        request;

int main() {
	httplib::Server svr;

    void *bytes;
    size_t size = HEIGHT * WIDTH * 4;

    svr
    .Options("/(.*)", [&](const httplib::Request&, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type, Accept");
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Max-Age", "86400");
        res.status = 204;
    })
	.Post("/demo", [&](const auto& req, auto& res) {
        res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type, Accept");
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Max-Age", "86400");
		auto request_data = json::parse(req.body);

        request list;
        request_data.at("transformations").at("x").get_to(list.first.first.x);
        request_data.at("transformations").at("y").get_to(list.first.first.y);
        request_data.at("transformations").at("z").get_to(list.first.first.z);
        request_data.at("transformations").at("roll").get_to(list.first.second.x);
        request_data.at("transformations").at("pitch").get_to(list.first.second.y);
        request_data.at("transformations").at("yaw").get_to(list.first.second.z);

        for (auto& [key, val] : request_data.at("requests").items()) {
            list.second.insert({key, {val.at("type"), {{}, {val.at("color").at("r"), val.at("color").at("g"), val.at("color").at("b"), val.at("color").at("a")}}}});
            for (auto& [kp, pnt] : val.at("points").items()) {
                list.second[key].second.first.push_back({pnt.at("x"), pnt.at("y"), pnt.at("z")});
            }
        }

        const double x = list.first.first.x;
        const double y = list.first.first.y;
        const double z = list.first.first.z;
        const double roll = list.first.second.x;
        const double pitch = list.first.second.y;
        const double yaw = list.first.second.z;

        std::map<std::string, openrender::Object> objectList = {};
        try {
            objectList = {};
            openrender::clearCanvas();
            for (auto& [key, val] : list.second) {
                std::vector<openrender::Point> points = val.second.first;
                if (val.first == "line") {
                    objectList.insert({key, openrender::drawLine(points[0].x, points[0].y, points[0].z, points[1].x, points[1].y, points[1].z, val.second.second)});
                } else if (val.first == "triangle") {
                    objectList.insert({key, openrender::drawTriangle(points[0].x, points[0].y, points[0].z, points[1].x, points[1].y, points[1].z, points[2].x, points[2].y, points[2].z, val.second.second)});
                } else if (val.first == "recPrOut") {
                    objectList.insert({key, openrender::drawRecPrOut(points[0].x, points[0].y, points[0].z, points[1].x, points[1].y, points[1].z, val.second.second, points[2].x, points[2].y, points[2].z)});
                } else if (val.first == "recPrFill") {
                    objectList.insert({key, openrender::drawRecPrFill(points[0].x, points[0].y, points[0].z, points[1].x, points[1].y, points[1].z, val.second.second, points[2].x, points[2].y, points[2].z)});
                } else if (val.first == "circle") {
                    objectList.insert({key, openrender::drawCircle(points[0].x, points[0].y, points[0].z, points[1].x, val.second.second)});
                } else if (val.first == "sphere") {
                    objectList.insert({key, openrender::drawSphere(points[0].x, points[0].y, points[0].z, points[1].x, val.second.second)});
                }
            }
        } catch(...) {
            res.status = 500;
            res.set_content("Error: Failed to draw content", "text/plain");
        }
        for (auto& [key, val] : objectList) {
            std::cout << key << std::endl;
        }
        std::cout << std::endl;

        openrender::projToFlat(x, y, z, &objectList, roll, pitch, yaw);

        bytes = openrender::convToBytes();

        if (bytes && size > 0) {
            res.set_content(static_cast<const char*>(bytes), size, "application/octet-stream");
        } else {
            res.status = 500;
            res.set_content("Error: Failed to draw content", "text/plain");
        }
	})
	.listen("0.0.0.0", 8080);
}