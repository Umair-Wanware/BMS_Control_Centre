#pragma once

#include <stddef.h>

namespace control_hub::dashboard {

struct EmbeddedAsset {
    const char* path;
    const char* content;
    size_t size;
    const char* contentType;
};

const EmbeddedAsset* findEmbeddedAsset(const char* path) noexcept;

} // namespace control_hub::dashboard
