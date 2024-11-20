#pragma once

#include <string>
#include <discord.h>
#include <json.hpp>

struct JsonSerializable {
protected:
	JsonSerializable() = default;
	virtual ~JsonSerializable() = default;
};

struct ConfigFileData : JsonSerializable {
	discord::ClientId applicationId;
	std::string encoding;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(
		ConfigFileData,
		applicationId, encoding
	)
};

struct DiscordPresenceData : JsonSerializable {
	discord::ActivityType type;
	std::string details;
	std::string state;
	std::string largeImageUrl;
	std::string largeImageText;
	std::string smallImageUrl;
	std::string smallImageText;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(
		DiscordPresenceData,
		type, details, state, largeImageUrl, largeImageText, smallImageUrl, smallImageText
	)
};
