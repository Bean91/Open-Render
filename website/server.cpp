#include "../../../cpp-httplib/httplib.h"
#include "../open_render.hpp"
#include "../../../json/single_include/nlohmann/json.hpp"
#include <string>
using json = nlohmann::json;

typedef std::pair<std::pair<openrender::Point, openrender::Point>,
        std::map<std::string, std::pair<std::string,
        std::pair<std::vector<openrender::Point>, openrender::Color>>>>
        request;

//Start nlomann/json helper functions
void to_json(json& j, const openrender::Point& p) {
    j = json{
        {"x", p.x},
        {"y", p.y},
        {"z", p.z}
    };
}

void from_json(const json& j, openrender::Point& p) {
    j.at("x").get_to(p.x);
    j.at("y").get_to(p.y);
	j.at("z").get_to(p.z);
}

void to_json(json& j, const openrender::Color& c) {
    j = json{
        {"r", c.r},
        {"g", c.g},
        {"b", c.b},
        {"a", c.a}
    };
}

void from_json(const json& j, openrender::Color& c) {
    j.at("r").get_to(c.r);
    j.at("g").get_to(c.g);
    j.at("b").get_to(c.b);
    j.at("a").get_to(c.a);
}

void to_json(json& j, const request& list) {
    j["transformations"]["x"] = list.first.first.x;
    j["transformations"]["y"] = list.first.first.y;
    j["transformations"]["z"] = list.first.first.z;
    j["transformations"]["roll"] = list.first.second.x;
    j["transformations"]["pitch"] = list.first.second.y;
    j["transformations"]["yaw"] = list.first.second.z;

    for (const auto [key, obj] : list.second) {
        j["requests"][key]["type"] = obj.first;
        j["requests"][key]["color"]["r"] = obj.second.second.r;
        j["requests"][key]["color"]["g"] = obj.second.second.g;
        j["requests"][key]["color"]["b"] = obj.second.second.b;
        j["requests"][key]["color"]["a"] = obj.second.second.a;
        int index = 0;
        for (const openrender::Point pnt : obj.second.first) {
            j["requests"][key]["points"].push_back(json{index, {{"x", pnt.x}, {"y", pnt.y}, {"z", pnt.z}}});
            index++;
        }
    }
}

void from_json(const json& j, request& list) {
    j.at("transformations").at("x").get_to(list.first.first.x);
    j.at("transformations").at("y").get_to(list.first.first.y);
    j.at("transformations").at("z").get_to(list.first.first.z);
    j.at("transformations").at("roll").get_to(list.first.second.x);
    j.at("transformations").at("pitch").get_to(list.first.second.y);
    j.at("transformations").at("yaw").get_to(list.first.second.z);

    for (auto& [key, val] : j.at("requests").items()) {
        list.second.insert({key, {val.at("type"), {{}, {val.at("color").at("r"), val.at("color").at("g"), val.at("color").at("b"), val.at("color").at("a")}}}});
        if (val.at("points").size()%3 == 0) {
            for (int i = 0; i < val.at("points").size()/3; i++) {
                list.second[key].second.first.push_back({val.at("points").at(i).at("x"), val.at("points").at(i).at("y"), val.at("points").at(i).at("z")});
            }
        } else {
            list.second[key].second.first.push_back({val.at("points").at(0), val.at("points").at(1), val.at("points").at(2)});
            list.second[key].second.first.push_back({val.at("points").at(3), 0.0, 0.0});
        }
    }
}
//End nlomann/json helper functions

int main() {
	httplib::Server svr;

	svr
	.Post("/demo", [&](const auto& req, auto& res) {
		auto request_data = json::parse(req.body);

        request list;
        from_json(request_data, list);

        const double x = list.first.first.x;
        const double y = list.first.first.y;
        const double z = list.first.first.z;
        const double roll = list.first.second.x;
        const double pitch = list.first.second.y;
        const double yaw = list.first.second.z;

        std::map<std::string, openrender::Object> objectList;
        for (auto& [key, val] : list.second) {
            std::vector<openrender::Point> points = val.second.first;
            if (val.first == "line") {
                objectList.insert({key, openrender::drawLine(points[0].x, points[0].y, points[0].z, points[1].x, points[1].y, points[1].z, val.second.second)});
            } else if (val.first == "triangle") {
                objectList.insert({key, openrender::drawTriangle(points[0].x, points[0].y, points[0].z, points[1].x, points[1].y, points[1].z, points[2].x, points[2].y, points[2].z, val.second.second)});
            } else if (val.first == "recPrOut") {
                if (points.size() == 3) {
                    objectList.insert({key, openrender::drawRecPrOut(points[0].x, points[0].y, points[0].z, points[1].x, points[1].y, points[1].z, val.second.second, points[2].x, points[2].y, points[2].z)});
                } else if (points.size() == 2) {
                    objectList.insert({key, openrender::drawRecPrOut(points[0].x, points[0].y, points[0].z, points[1].x, points[1].y, points[1].z, val.second.second)});
                }
            } else if (val.first == "recPrFill") {
                if (points.size() == 3) {
                    objectList.insert({key, openrender::drawRecPrOut(points[0].x, points[0].y, points[0].z, points[1].x, points[1].y, points[1].z, val.second.second, points[2].x, points[2].y, points[2].z)});
                } else if (points.size() == 2) {
                    objectList.insert({key, openrender::drawRecPrOut(points[0].x, points[0].y, points[0].z, points[1].x, points[1].y, points[1].z, val.second.second)});
                }
            } else if (val.first == "circle") {
                objectList.insert({key, openrender::drawCircle(points[0].x, points[0].y, points[0].z, points[1].x, val.second.second)});
            } else if (val.first == "sphere") {
                objectList.insert({key, openrender::drawSphere(points[0].x, points[0].y, points[0].z, points[1].x, val.second.second)});
            }
        }
        openrender::projToFlat(x, y, z, &objectList, roll, pitch, yaw);

        void *bytes = openrender::convToBytes();
        size_t size = HEIGHT * WIDTH;

        if (bytes && size > 0) {
            res.set_content(static_cast<const char*>(bytes), size, "application/octet-stream");
        } else {
            res.status = 500;
            res.set_content("Error: Failed to draw content", "text/plain");
        }
	})
	.listen("0.0.0.0", 8080);
}