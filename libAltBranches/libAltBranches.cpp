
#include <curl/curl.h>
#include <string>
#include <set>
#include <map>

#include <nlohmann/json.hpp>
#include <rpm/rpmlib.h>

#include <iostream>

using json = nlohmann::json;

/* #region Запрос в API*/

static std::string response_data;

size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                     std::string *output) {
  size_t totalSize = size * nmemb;
  output->append((char *)contents, totalSize);
  return totalSize;
}

extern "C" {

    const char *make_rest_request(const char *url) {
      response_data.clear();
    
      CURL *curl = curl_easy_init();
      if (!curl)
        return nullptr;
    
      curl_easy_setopt(curl, CURLOPT_URL, url);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);
    
      CURLcode res = curl_easy_perform(curl);
      curl_easy_cleanup(curl);
    
      if (res != CURLE_OK)
        return nullptr;
    
      return response_data.c_str();
    }
        
    }

    /* #endregion */

struct PackageInfo {
    std::string version;
    std::string release;

    std::string vr() const {
        return version + "-" + release;
    }
};

const char *compare_branches_impl(const char* branch1, const char* branch2) {
    const std::string apiUrl = "https://rdb.altlinux.org/api/export/branch_binary_packages/";
    std::string url1 = apiUrl + branch1;
    std::string url2 = apiUrl + branch2;

    const std::string resp1 = make_rest_request(url1.c_str());
    const std::string resp2 = make_rest_request(url2.c_str());

    if (resp1.empty() || resp2.empty())
    return nullptr;

    auto j1 = json::parse(resp1);
    auto j2 = json::parse(resp2);

    using ArchMap = std::map<std::string, std::map<std::string, PackageInfo>>;
    ArchMap pkgs1, pkgs2;    
    std::set<std::pair<std::string, std::string>> all_keys;

        auto loadPackages = [&](const json& arr, ArchMap& target) {
        if (!arr.contains("packages") || !arr["packages"].is_array()) {
            std::cerr << "Invalid or missing 'packages' in API response\n";
            return;
        }
    
        for (const auto& pkg : arr["packages"]) {
            if (!pkg.contains("arch") || !pkg.contains("name") || 
                !pkg.contains("version") || !pkg.contains("release")) {
                std::cerr << "Skipping incomplete package entry\n";
                continue;
            }
            std::string arch = pkg["arch"];
            std::string name = pkg["name"];
            std::string version = pkg["version"];
            std::string release = pkg["release"];
            target[arch][name] = {version, release};
            all_keys.emplace(arch, name);
        }
    };

loadPackages(j1, pkgs1);
loadPackages(j2, pkgs2);

json result;


for (const auto& [arch, name] : all_keys) {
    bool has1 = pkgs1.count(arch) && pkgs1[arch].count(name);
    bool has2 = pkgs2.count(arch) && pkgs2[arch].count(name);

    if (has1 && !has2) {
        const auto &pak =pkgs1[arch][name];

        result["only_in_first"][arch].push_back(name + " " + pak.vr());
    } else if (!has1 && has2) {
        const auto &pak =pkgs2[arch][name];
        result["only_in_second"][arch].push_back(name + " " + pak.vr());
    } else if (has1 && has2) {
        const auto& p1 = pkgs1[arch][name];
        const auto& p2 = pkgs2[arch][name];
        
        int cmpVersion = rpmvercmp(p1.version.c_str(), p2.version.c_str());
        const auto &pak =pkgs1[arch][name];
        if (cmpVersion == 0) {
            int cmpRelease = rpmvercmp(p1.release.c_str(), p2.release.c_str());
            if (cmpRelease > 0) {
                result["newer_in_first"][arch].push_back(name + " " + pak.vr());
            }
        } else if (cmpVersion > 0) {
            result["newer_in_first"][arch].push_back(name + " " + pak.vr());
        }
    }
}

static std::string resultStr;
resultStr = result.dump(4);
return resultStr.c_str();

};

extern "C" {   
    const char *compare_branches(const char* branch1, const char* branch2){
        return compare_branches_impl(branch1, branch2);
    }
}