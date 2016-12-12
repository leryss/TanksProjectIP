#pragma once

#include "SFML\Graphics.hpp"
#include <memory>
#include <map>
#include <exception>
#include <string>

template<typename Resource, typename Identifier>
class ResourceHolder {
public:
	void											load(Identifier id, const std::string& pathfile);
	Resource&										get(Identifier id);
	void											insertResource(Identifier id, std::unique_ptr<Resource> res);
private:
	std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;
};

template<typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string& pathfile) {
	std::unique_ptr<Resource> resource(new Resource());

	if (!resource->loadFromFile(pathfile)) {
		throw std::runtime_error("Resource missing.\n");
	}
}

template<typename Resource, typename Identifier>
Resource& ResourceHolder<Resource, Identifier>::get(Identifier id) {

}

template<typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::insertResource(Identifier id, std::unique_ptr<Resource> res) {

}