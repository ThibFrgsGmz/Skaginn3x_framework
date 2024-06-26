#pragma once

#include <array>
#include <bitset>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <span>
#include <vector>

#include <tfc/ec/devices/base.hpp>
#include <tfc/ipc/details/type_description.hpp>
#include <tfc/ipc_fwd.hpp>
#include <tfc/stx/basic_fixed_string.hpp>
#include <tfc/utils/asio_fwd.hpp>

namespace tfc::ec::devices::beckhoff {

namespace asio = boost::asio;

template <typename manager_client_type,
          std::size_t size,
          std::array<std::size_t, size> entries,
          uint32_t pc,
          tfc::stx::basic_fixed_string name_v>
class el2xxx final : public base<el2xxx<manager_client_type, size, entries, pc, name_v>> {
public:
  static_assert(size == 4 || size == 8 || size == 16, "Invalid size");
  el2xxx(asio::io_context& ctx, manager_client_type& client, uint16_t slave_index);
  using output_pdo = std::array<std::uint8_t, (size / 9) + 1>;

  static constexpr auto size_v = size;
  static constexpr auto entries_v = entries;
  static constexpr auto name = name_v;
  static constexpr auto product_code = pc;
  static constexpr uint32_t vendor_id = 0x2;

  void pdo_cycle(std::span<std::uint8_t>, output_pdo& output) noexcept;

  auto set_output(size_t position, bool value) -> void { output_states_.set(position, value); }

private:
  std::bitset<size> output_states_;
  std::vector<std::shared_ptr<ipc::slot<ipc::details::type_bool, manager_client_type&>>> bool_receivers_;
};

template <typename manager_client_type>
using el2794 = el2xxx<manager_client_type, 4, std::to_array<std::size_t>({ 1, 5, 4, 8 }), 0xaea3052, "EL2794">;

template <typename manager_client_type>
using el2004 = el2xxx<manager_client_type, 4, std::to_array<std::size_t>({ 1, 5, 4, 8 }), 0x7d43052, "EL2004">;

template <typename manager_client_type>
using el2008 = el2xxx<manager_client_type, 8, std::to_array<std::size_t>({ 1, 5, 2, 6, 3, 7, 4, 8 }), 0x7d83052, "EL2008">;

template <typename manager_client_type>
using el2809 = el2xxx<manager_client_type,
                      16,
                      std::to_array<std::size_t>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }),
                      0xaf93052,
                      "EL2809">;

using imc = tfc::ipc_ruler::ipc_manager_client;
extern template class el2xxx<imc, el2794<imc>::size_v, el2794<imc>::entries_v, el2794<imc>::product_code, el2794<imc>::name>;
extern template class el2xxx<imc, el2004<imc>::size_v, el2004<imc>::entries_v, el2004<imc>::product_code, el2004<imc>::name>;
extern template class el2xxx<imc, el2008<imc>::size_v, el2008<imc>::entries_v, el2008<imc>::product_code, el2008<imc>::name>;
extern template class el2xxx<imc, el2809<imc>::size_v, el2809<imc>::entries_v, el2809<imc>::product_code, el2809<imc>::name>;

}  // namespace tfc::ec::devices::beckhoff
