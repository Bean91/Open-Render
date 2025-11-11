#include "../../../cpp-httplib/httplib.h"
#include "../open_render.hpp"
#include "../../../json/single_include/nlohmann/json.hpp"
#include <string>
using json = nlohmann::json;

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

void to_json(json& j, const openrender::Object& obj) {
    json *points = new json[obj.points.size()];

    for (int i = 0; i < obj.points.size(); i++) {
        points[i] = json{
            {"x", obj.points[i].x},
            {"y", obj.points[i].y},
            {"z", obj.points[i].z}
        };
    }

    j = json{
        {"rgba", {
            {"r", obj.rgba.r},
            {"g", obj.rgba.g},
            {"b", obj.rgba.b},
            {"a", obj.rgba.a}
        }},
        {"rendered", obj.rendered}
    };

    for (json pnt : *points) {
        j["points"].push_back(pnt);
    }
}

void from_json(const json& j, openrender::Object& obj) {
    size_t size = j.count("points");
    obj.points.resize(size);

    for (int i = 0; i < size; i++) {
        j.at("points").at(i).at("x").get_to(obj.points[i].x);
        j.at("points").at(i).at("z").get_to(obj.points[i].y);
        j.at("points").at(i).at("z").get_to(obj.points[i].z);
    }

    j.at("rgba").at("r").get_to(obj.rgba.r);
    j.at("rgba").at("g").get_to(obj.rgba.g);
    j.at("rgba").at("b").get_to(obj.rgba.b);
    j.at("rgba").at("a").get_to(obj.rgba.a);
    j.at("rendered").get_to(obj.rendered);
}

void to_json(json& j, const std::map<std::string, openrender::Object>& list) {
    for (auto [key, obj] : list) {
        json *points = new json[obj.points.size()];

        for (int i = 0; i < obj.points.size(); i++) {
            points[i] = json{
                {"x", obj.points[i].x},
                {"y", obj.points[i].y},
                {"z", obj.points[i].z}
            };
        }

        json temp = json{
            {"rgba", {
                {"r", obj.rgba.r},
                {"g", obj.rgba.g},
                {"b", obj.rgba.b},
                {"a", obj.rgba.a}
            }},
            {"rendered", obj.rendered}
        };

        for (json pnt : *points) {
            temp["points"].push_back(pnt);
        }
        j[key] = temp;
    }
}

void from_json(const json& j, std::map<std::string, openrender::Object>& list) {
    size_t size = j.size();
    for (auto& [key, val] : j.items()) {
        std::vector<openrender::Point> temppoints;
        openrender::Color tempcolor;
        openrender::Object obj(temppoints, tempcolor, false);
        list.insert({key, obj});
        list.at(key).points.resize(size);

        for (int i = 0; i < size; i++) {
            val.at("points").at(i).at("x").get_to(list[key].points[i].x);
            val.at("points").at(i).at("z").get_to(list[key].points[i].y);
            val.at("points").at(i).at("z").get_to(list[key].points[i].z);
        }

        val.at("rgba").at("r").get_to(list[key].rgba.r);
        val.at("rgba").at("g").get_to(list[key].rgba.g);
        val.at("rgba").at("b").get_to(list[key].rgba.b);
        val.at("rgba").at("a").get_to(list[key].rgba.a);
        val.at("rendered").get_to(list[key].rendered);
    }
}

void to_json(json& j, const std::pair<std::pair<openrender::Point, openrender::Point>, std::map<std::string, openrender::Object>>& list) {
    j["x"] = list.first.first.x;
    j["y"] = list.first.first.y;
    j["z"] = list.first.first.z;
    j["roll"] = list.first.second.x;
    j["pitch"] = list.first.second.y;
    j["yaw"] = list.first.second.z;
    for (auto [key, obj] : list.second) {
        json *points = new json[obj.points.size()];

        for (int i = 0; i < obj.points.size(); i++) {
            points[i] = json{
                {"x", obj.points[i].x},
                {"y", obj.points[i].y},
                {"z", obj.points[i].z}
            };
        }

        json temp = json{
            {"rgba", {
                {"r", obj.rgba.r},
                {"g", obj.rgba.g},
                {"b", obj.rgba.b},
                {"a", obj.rgba.a}
            }},
            {"rendered", obj.rendered}
        };

        for (json pnt : *points) {
            temp["points"].push_back(pnt);
        }
        j[key] = temp;
    }
}

void from_json(const json& j, std::pair<std::pair<openrender::Point, openrender::Point>, std::map<std::string, openrender::Object>>& list) {
    size_t size = j.size()-6;
    for (auto& [key, val] : j.items()) {
        if (key == "x" || key == "y" || key == "x" || key == "roll" || key == "pitch" || key == "yaw") {
            continue;
        }
        std::vector<openrender::Point> temppoints;
        openrender::Color tempcolor;
        openrender::Object obj(temppoints, tempcolor, false);
        list.second.insert({key, obj});
        list.second.at(key).points.resize(size);

        for (int i = 0; i < size; i++) {
            val.at("points").at(i).at("x").get_to(list.second[key].points[i].x);
            val.at("points").at(i).at("z").get_to(list.second[key].points[i].y);
            val.at("points").at(i).at("z").get_to(list.second[key].points[i].z);
        }

        val.at("rgba").at("r").get_to(list.second[key].rgba.r);
        val.at("rgba").at("g").get_to(list.second[key].rgba.g);
        val.at("rgba").at("b").get_to(list.second[key].rgba.b);
        val.at("rgba").at("a").get_to(list.second[key].rgba.a);
        val.at("rendered").get_to(list.second[key].rendered);
    }
    j.at("x").get_to(list.first.first.x);
    j.at("y").get_to(list.first.first.y);
    j.at("z").get_to(list.first.first.z);
    j.at("roll").get_to(list.first.second.x);
    j.at("pitch").get_to(list.first.second.y);
    j.at("yaw").get_to(list.first.second.z);
}
//End nlomann/json helper functions

int main() {
	httplib::Server svr;

	svr
	.Get("/demo", [&](const auto& req, auto& res) {
		auto request_data = json::parse(req.body);

        std::pair<std::pair<openrender::Point, openrender::Point>, std::map<std::string, openrender::Object>> list;
        from_json(request_data, list);
	})
	.listen("0.0.0.0", 8080);
}