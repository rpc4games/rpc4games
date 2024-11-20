#pragma once

#include <fstream>
#include <optional>
#include <filesystem>
#include <string>
#include <vector>
#include <json.hpp>

#include "CodePageConverter.hpp"
#include "../utils/utils.hpp"

// structs.hpp
struct JsonSerializable;

class FileReader {
public:
	FileReader(std::filesystem::path path);
	FileReader(std::filesystem::path path, const std::string& encoding);

	bool isFileReady() const;
	std::string getPathString(std::vector<std::string>& path) const;

	template <typename T>
	inline bool readFile(T* js) {
		static_assert(std::is_base_of_v<JsonSerializable, T>, "T must inherit from JsonSerializable");

		std::ifstream inputFile{ this->path };
		if (!inputFile) {
			utils::logging::Info(std::format("Searching for file '{}'...", this->filename));
			return false;
		}

		std::string fileContents((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

		if (converter.has_value()) {
			fileContents = converter.value().convert(fileContents);
		}

		try {
			this->data = nlohmann::json::parse(fileContents);
		}
		catch (nlohmann::detail::parse_error& e) {
			utils::logging::Warning(std::format("Failed to parse '{}': \n{}", this->filename, e.what()));
			return false;
		}

		try {
			*js = this->data.get<T>();
		} catch (nlohmann::detail::out_of_range& e) {
			utils::logging::Warning(std::format("Failed to map '{}': \n{}", this->filename, e.what()));
			return false;
		}

		return true;
	}

	template <typename T>
	inline T loadValue(nlohmann::json& json, std::vector<std::string> location) {
		try {
			const nlohmann::json* obj = &json;
			for (const auto& path : location) {
				if (!obj->contains(path)) {
					throw nlohmann::json::out_of_range::create(404, "Path not found: " + getPathString(location), obj);
				}
				obj = &(*obj)[path];
			}
			return obj->get<T>();
		}
		catch (nlohmann::json::exception& e) {
			utils::logging::Warning("Error retrieving value: \n{}", e.what());
			return T{};
		}
	}

private:
	std::filesystem::path path{};
	std::string filename{};
	nlohmann::json data{};
	std::optional<CodePageConverter> converter{};
};
