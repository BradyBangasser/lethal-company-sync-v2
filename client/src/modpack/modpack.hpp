#ifndef MODPACK_HPP
#define MODPACK_HPP

#include "../mod/mod.hpp"
#include "../constants.hpp"

#include <string>
#include <vector>
#include <stack>
#include <stdint.h>
#include <mutex>

// This represents the .modpack.lcs file
class ModPack {
    private:
        std::string name;
        std::string id;
        std::string gameId;
        std::string slashSepartedId;
        std::vector<std::string> modIds;
        uint16_t revisionNumber;

        ModPack(const std::string name, const std::string id, const std::vector<std::string> mods, uint16_t revision);
    
    public:
        ModPack();

        static ModPack fromLSF(const std::string path);
        int writeLSF(const std::string path);

        static ModPack fromJSON(std::string json);
        static ModPack fetch(const std::string id);

        // The download fn for the threads
        Status _downloader(
            const uint16_t id, 
            std::mutex &jobMutex,
            std::mutex &downloadedMutex,
            std::stack<std::string> &jobList,
            std::stack<Mod> &downloaded
        );
        // The install fn for the threads
        Status _installer(const uint16_t id, std::mutex &mutex, volatile std::stack<Mod> &jobList);

        /**
         * @brief Removes a mod from the modpack
         * @note THIS ONLY CHANGES LOCAL MODPACK, IT DOES NOT UPLOAD IT TO THE SERVER
         * 
         * @note if modpack is installed it will remove the mod from the install
         * 
         * @param id Id of the mod
         * @return LSC_OK on ok, else error code
         */
        int removeMod(const std::string id) noexcept;

        /**
         * @brief Add a mod to the modpack
         * @note THIS ONLY CHANGES LOCAL MODPACK, IT DOES NOT UPLOAD IT TO THE SERVER
         * @todo Added mod id verification
         * 
         * @note This will add the mod to the install if the pack is installed
         * 
         * @param id The Id of the mod
         * @return LCS_OK on success, otherwise an error code
         */
        int addMod(const std::string id);
        Status install();

        /**
         * @brief 
         * 
         * @return 
         */
        Status isInstalled();
        int verifyInstall();

        inline const std::string getId() const noexcept { return id; }
        inline const std::vector<std::string> getModIds() const noexcept { return modIds; }
        inline const std::string getName() const noexcept { return name; }
        inline const uint16_t getRevisionNumber() const noexcept { return revisionNumber; }
};

class ModPackError : public std::exception {
    public:
        ModPackError(std::string msg);
        ~ModPackError();
        std::string what();
};

#endif