file(GLOB_RECURSE app_sources "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE control_hub_sources "${CMAKE_SOURCE_DIR}/lib/*.cpp")

idf_component_register(
    SRCS ${app_sources} ${control_hub_sources}
    INCLUDE_DIRS "." "../lib"
    REQUIRES
        driver
        esp_event
        esp_http_server
        esp_netif
        esp_wifi
        nvs_flash
)