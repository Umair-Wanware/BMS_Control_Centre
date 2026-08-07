#include "application/application.hpp"
#include "esp_err.h"

extern "C" void app_main(void)
{
    control_hub::application::Application application;
    ESP_ERROR_CHECK(application.initialize());
    application.run();
}
