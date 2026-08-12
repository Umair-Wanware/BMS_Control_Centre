Import("env")
import hashlib
import os

PROJECT_DIR = env.subst("$PROJECT_DIR")
WEB_DIR = os.path.join(PROJECT_DIR, "web")
OUTPUT_CPP = os.path.join(PROJECT_DIR, "lib", "dashboard", "embedded_web_assets.cpp")
OUTPUT_HPP = os.path.join(PROJECT_DIR, "lib", "dashboard", "embedded_web_assets.hpp")


def content_type_for_path(path):
    extension = os.path.splitext(path)[1].lower()
    return {
        ".html": "text/html",
        ".css": "text/css",
        ".js": "application/javascript",
        ".json": "application/json",
        ".svg": "image/svg+xml",
        ".png": "image/png",
        ".ico": "image/x-icon",
    }.get(extension, "application/octet-stream")


def make_delimiter(path):
    digest = hashlib.sha1(path.encode("utf-8")).hexdigest()[:10]
    return f"WEB_{digest}"


def generate_embedded_web_assets(source, target, env):
    if not os.path.isdir(WEB_DIR):
        print("generate_embedded_web_assets: web/ directory not found")
        return

    assets = []
    for entry in sorted(os.listdir(WEB_DIR)):
        source_path = os.path.join(WEB_DIR, entry)
        if not os.path.isfile(source_path):
            continue

        with open(source_path, "r", encoding="utf-8") as handle:
            content = handle.read()

        uri_path = f"/{entry}"
        delimiter = make_delimiter(uri_path)
        assets.append(
            {
                "path": uri_path,
                "delimiter": delimiter,
                "content": content,
                "content_type": content_type_for_path(entry),
                "symbol": entry.replace(".", "_").replace("-", "_"),
            }
        )

    hpp_lines = [
        "#pragma once",
        "",
        "#include <stddef.h>",
        "",
        "namespace control_hub::dashboard {",
        "",
        "struct EmbeddedAsset {",
        "    const char* path;",
        "    const char* content;",
        "    size_t size;",
        "    const char* contentType;",
        "};",
        "",
        "const EmbeddedAsset* findEmbeddedAsset(const char* path) noexcept;",
        "",
        "} // namespace control_hub::dashboard",
        "",
    ]

    cpp_lines = [
        '#include "embedded_web_assets.hpp"',
        "",
        "#include <string.h>",
        "",
        "namespace control_hub::dashboard {",
        "namespace {",
        "",
    ]

    table_rows = []
    for asset in assets:
        symbol = f"kAsset_{asset['symbol']}"
        cpp_lines.extend(
            [
                f"static const char {symbol}[] = R\"{asset['delimiter']}(",
                asset["content"],
                f"){asset['delimiter']}\";",
                "",
            ]
        )
        table_rows.append(
            "    {"
            f"\"{asset['path']}\", {symbol}, sizeof({symbol}) - 1U, \"{asset['content_type']}\""
            "},"
        )

    cpp_lines.extend(
        [
            "static const EmbeddedAsset kEmbeddedAssets[] = {",
            *table_rows,
            "};",
            "",
            "static constexpr size_t kEmbeddedAssetCount = sizeof(kEmbeddedAssets) / sizeof(kEmbeddedAssets[0]);",
            "",
            "} // namespace",
            "",
            "const EmbeddedAsset* findEmbeddedAsset(const char* path) noexcept",
            "{",
            "    if (path == nullptr) {",
            "        return nullptr;",
            "    }",
            "",
            "    for (size_t index = 0U; index < kEmbeddedAssetCount; ++index) {",
            "        if (strcmp(kEmbeddedAssets[index].path, path) == 0) {",
            "            return &kEmbeddedAssets[index];",
            "        }",
            "    }",
            "",
            "    return nullptr;",
            "}",
            "",
            "} // namespace control_hub::dashboard",
            "",
        ]
    )

    os.makedirs(os.path.dirname(OUTPUT_CPP), exist_ok=True)
    with open(OUTPUT_HPP, "w", encoding="utf-8") as handle:
        handle.write("\n".join(hpp_lines))
    with open(OUTPUT_CPP, "w", encoding="utf-8") as handle:
        handle.write("\n".join(cpp_lines))

    print(f"generate_embedded_web_assets: embedded {len(assets)} web files into firmware")


generate_embedded_web_assets(None, None, env)
env.AddPreAction("$BUILD_DIR/${PROGNAME}.elf", generate_embedded_web_assets)
