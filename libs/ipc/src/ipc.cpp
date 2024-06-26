#include "tfc/ipc.hpp"

#include <magic_enum.hpp>

namespace tfc::ipc::details {

auto enum_name(mass_error_e err) -> std::string_view {
  return magic_enum::enum_name(err);
}

auto enum_name(sensor_error_e err) -> std::string_view {
  return magic_enum::enum_name(err);
}

auto enum_name(voltage_error_e err) -> std::string_view {
  return magic_enum::enum_name(err);
}

auto enum_name(current_error_e err) -> std::string_view {
  return magic_enum::enum_name(err);
}

}  // namespace tfc::ipc::details
