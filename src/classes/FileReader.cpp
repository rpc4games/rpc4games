#include "FileReader.hpp"

FileReader::FileReader(std::filesystem::path path)
	: FileReader(path, "") {
}

FileReader::FileReader(std::filesystem::path path, const std::string& encoding)
	: path(std::move(path)), filename(this->path.filename().string()) {
	if (!encoding.empty()) {
		converter.emplace(encoding, "UTF-8");
	}
}

bool FileReader::isFileReady() const {
	return std::filesystem::exists(this->path);
}

std::string FileReader::getPathString(std::vector<std::string>& path) const {
	std::string result;
	for (const auto& loc : path) {
		result += "/" + loc;
	}
	return result;
}
