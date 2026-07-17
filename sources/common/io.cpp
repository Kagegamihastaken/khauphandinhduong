#include "common/io.hpp"
#include <physfs.h>
#include <stdexcept>
#include <vector>
#include <fmt/color.h>

namespace MFCPP {
    void IO::Loadbyte(std::vector<uint8_t>& vec, const std::filesystem::path &fileName) {
        if (PHYSFS_isInit()) {
            if (!PHYSFS_exists(fileName.string().c_str())) throw std::runtime_error(fmt::format("PhysFS: File {} doesn't exist", fileName.string()));
            if (PHYSFS_File* fp = PHYSFS_openRead(fileName.string().c_str())) {
                std::vector<uint8_t> buffer(1024);
                do {
                    const PHYSFS_sint64 rc = PHYSFS_readBytes(fp, buffer.data(), buffer.size());
                    vec.insert(vec.end(), buffer.begin(), buffer.begin() + rc);
                } while (!PHYSFS_eof(fp));
                PHYSFS_close(fp);
            }
        }
        else throw std::runtime_error("PhysFS: Load data before initializing, call IOInit() first");
    }
    void IO::Init() {
        PHYSFS_init(nullptr);
        if (PHYSFS_mount("data.zip", "/", 1) == 0) throw std::runtime_error("PhysFS: Cannot mount data.zip");
    }

    void IO::Deinit() {
        PHYSFS_deinit();
    }

    void IO::LoadJSON(std::string &inp, const std::filesystem::path &path) {
        std::vector<uint8_t> vec;
        Loadbyte(vec, path);
        inp.assign(vec.begin(), vec.end());
    }
}
