#include "../../cpp-httplib/httplib.h"
#include "../open_render.hpp"
#include "../../json/single_include/nlohmann/json.hpp"
#include <string>
using json = nlohmann::json;

void to_json(json& j, const openrender::point& p) {
    j = json{{"x", p.x}, {"y", p.y}, {"z", p.z}};
}

void from_json(const json& j, openrender::point& p) {
    j.at("x").get_to(p.x);
    j.at("y").get_to(p.y);
	j.at("z").get_to(p.z);
}

void to_json(json& j, const openrender::color& c) {
    j = json{{"r", c.r}, {"g", c.g}, {"b", c.b}, {"a", c.a}};
}

void from_json(const json& j, openrender::color& c) {
    j.at("r").get_to(c.r);
    j.at("g").get_to(c.g);
    j.at("b").get_to(c.b);
    j.at("a").get_to(c.a);
}

void to_json(json& j, const openrender::object& obj) {
    j = json{{"points", obj.points}, {"rgba", obj.rgba}, {"rendered", obj.rendered}};
}

void from_json(const json& j, openrender::object& obj) {
    j.at("points").get_to(obj.points);
    j.at("rgba").get_to(obj.rgba);
    j.at("rendered").get_to(obj.rendered);
}

int main() {
	httplib::Server svr;

	svr
	.Post("/demo", [&](const auto& req, auto& res) {
		auto request_data = json::parse(req.body);

//		res.set_content();
	})
	.listen("localhost", 8080);
}