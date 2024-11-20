#include "classes/Console.hpp"
#include "classes/DiscordManager.hpp"
#include "classes/FileReader.hpp"
#include "settings/structs.hpp"

void PrintValue(std::string key, const std::string& value) {
	key.resize(20, ' ');
	PRINT_GREEN("[.]");
	std::cout << " " << key << ": " << value << std::endl;
}

unsigned long WINAPI MainThread(void* instance) {
	// Console related actions
	Console::attach();
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleTitleA("rpc4games");

	// Initialize objects
	FileReader cfgReader{ "rpc4games.cfg" };

	ConfigFileData cfgData{};
	if (!cfgReader.readFile<ConfigFileData>(&cfgData)) {
		utils::logging::ThrowError("Unable to read configuration file.");
		// return;
	}

	FileReader dataReader{ std::filesystem::temp_directory_path().append("rpc4games.json"), cfgData.encoding };

	DiscordManager discordManager{};
	discordManager.addTimestamp();

	while (true) {
		// TODO: bad practice
		system("cls");

		if (!discordManager.isReady()) {
			utils::logging::Info("Searching for Discord...");
			discordManager.create(cfgData.applicationId);
			utils::threads::wait(2500);
			continue;
		}

		DiscordPresenceData presenceData{};
		if (!dataReader.readFile<DiscordPresenceData>(&presenceData)) {
			utils::threads::wait(2500);
			continue;
		}

		discordManager.setType(presenceData.type);
		discordManager.setDetails(presenceData.details);
		discordManager.setState(presenceData.state);
		discordManager.setLargeImage(presenceData.largeImageUrl, presenceData.largeImageText);
		discordManager.setSmallImage(presenceData.smallImageUrl, presenceData.smallImageText);
		discordManager.update([&](discord::Result result) {
			if (result == discord::Result::Ok) {
				PrintValue("Activity Type", std::to_string(std::int32_t(presenceData.type)));
				PrintValue("Details", presenceData.details);
				PrintValue("State", presenceData.state);
				PrintValue("Large Image URL", presenceData.largeImageUrl);
				PrintValue("Large Image Text", presenceData.largeImageText);
				PrintValue("Small Image URL", presenceData.smallImageUrl);
				PrintValue("Small Image Text", presenceData.smallImageText);
			} else {
				utils::logging::Warning("Failed to update Discord presence: " + std::int32_t(result));
			}
		});

		utils::threads::wait(2000);
	}
}

BOOL APIENTRY DllMain(const HMODULE hModule, unsigned long callReason, void* reserved) {
	DisableThreadLibraryCalls(hModule);

	HANDLE handle = nullptr;

	switch (callReason) {
	case DLL_PROCESS_ATTACH:
		handle = CreateThread(nullptr, NULL, MainThread, hModule, NULL, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		break;
	}

	if (handle != nullptr) {
		CloseHandle(handle);
	}

	return TRUE;
}
