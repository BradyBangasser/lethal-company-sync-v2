#include "modpack.hpp"
#include "../constants.hpp"
#include "../network/network.hpp"
#include "../fs/lsf.h"
#include "../mod/mod.hpp"

#include <string>
#include <algorithm>
#include <vector>
#include <http.hpp>
#include <json.hpp>
#include <stdio.h>
#include <stack>
#include <thread>
#include <mutex>
#include <fmt/core.h>

ModPack::ModPack() {}
ModPack::ModPack(const std::string name, const std::string id, const std::vector<std::string> modIds, uint16_t revision) {
    if (id[0] != 'P') throw ModPackError("");
    this->name = name;
    this->id = id;
    this->modIds = modIds;
    this->revisionNumber = revision;

    this->gameId = id.substr(1, id.find(':'));
    this->slashSepartedId = id;
    // std::replace(id.begin(), id.end(), ':', PATH_SLASH);
}

int ModPack::addMod(const std::string id) {
    this->modIds.push_back(id);
    return Status::ALL_GOOD;
}

int ModPack::removeMod(std::string id) noexcept {
    std::vector<std::string>::iterator idIter = std::find(this->modIds.begin(), this->modIds.end(), id);

    if (idIter == this->modIds.end()) {
        return -1; // Mod not found
    }

    this->modIds.erase(idIter);

    return Status::ALL_GOOD;
}

ModPack ModPack::fetch(const std::string id) {
    struct blib_http::Response res; 
    std::string requestUrl = serverUrl(fmt::format("/{}/", id));
    res = blib_http::request<const struct blib_http::Response>(requestUrl, NULL);

    switch (res.status) {
        case 404:
            throw network::NetworkError(404, "Mod Not found");
        case 502:
            throw network::NetworkError(502, "Server Unreachable");
        case 200:
            return ModPack::fromJSON(res.content);
        default:
            throw network::NetworkError(res.status, "Network error");
    }
}

ModPack ModPack::fromJSON(std::string json) {
    throw Status::NOT_IMPLEMENTED_YET;
}

Status ModPack::install() {
    if (isInstalled() == Status::INSTALLED) {
        // Verify that all mods are correctly installed 
    } else {
        std::string modpackInstallPath = installPath(this->slashSepartedId);
        this->writeLSF(modpackInstallPath);

        uint8_t i;

        std::stack<std::string> downloadJobs;
        std::stack<Mod> installJobs;

        // load jobs
        for (std::string id : this->modIds) {
            downloadJobs.push(id);
        }

        // Make an algorithm for this
        const uint8_t numThreads = 4;

        std::mutex m_downloads;

        std::stack<Mod> installJobs;
        std::mutex m_install;

        std::thread downloadThreads[numThreads];
        std::thread installerThreads[numThreads];
        
        for (i = 0; i < numThreads; i++) {
            downloadThreads[i] = std::thread();
        }

        for (i = 0; i < numThreads; i++) {

        }
    }

    return Status::NOT_IMPLEMENTED_YET;
}

Status ModPack::_downloader(
    const uint16_t id,
    std::mutex &jobMutex,
    std::mutex &downloadMutex,
    std::stack<std::string> &jobs,
    std::stack<Mod> &downloaded
) {
    std::string modId;
    Mod mod;

    while (1) {
        // Grab the top job off the stack
        jobMutex.lock();

        if (jobs.size() <= 0) {
            return ALL_GOOD;
        } else {
            modId = jobs.top();
            jobs.pop();
        }

        jobMutex.unlock();

        // Attempt to fetch the mod

        try {
            mod = Mod::fetch(modId);
        } catch (blib_http::RequestError &err) {
            if (err.code == 404) {
                // Report the mod and modpack to the server so it knows to update the modpack or something
                std::string surl = serverUrl(fmt::format("{}/{}/", this->id, modId));
                blib_http::RequestOptions opts;
                memset(&opts, 0, sizeof(opts));
                opts.method = "POST";
                int res = blib_http::request<int>(surl, &opts);

                // Make this a little nicer
                throw NOT_IMPLEMENTED_YET;
            } else if (err.code == 502) {
                throw network::NetworkError(-5, "Server is down");
            } else {
                // try harder 
                throw NOT_IMPLEMENTED_YET;
            }
        }

        // Attempt to download the mod
        throw NOT_IMPLEMENTED_YET;

        downloadMutex.lock();

        downloaded.push(mod);

        downloadMutex.unlock();

        fmt::println("Downloader {x} successfully downloaded {}", id, modId);
    }
}

Status ModPack::_installer() {

}

int ModPack::writeLSF(const std::string path) {
    struct LSFValue *vals = createLSFValue("id", this->id.c_str());

    if (vals == NULL) {
        return -1;
    }

    if (writeLSFFile(path.c_str(), vals) != 0) {
        freeLSFValues(vals);
        return -1;
    }

    freeLSFValues(vals);
    return 0;
}

ModPack ModPack::fromLSF(const std::string path) {
    throw Status::NOT_IMPLEMENTED_YET;
}

Status ModPack::isInstalled() {
    throw Status::NOT_IMPLEMENTED_YET;
}

ModPackError::ModPackError(std::string msg) {

}

ModPackError::~ModPackError() {

}