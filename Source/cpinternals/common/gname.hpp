#pragma once
#include "stringpool.hpp"
#include <nlohmann/json.hpp>

namespace cp {

// One global name type to rule them all.
struct gname
{
  gname()
  {
    m_idx = gnames_pool().register_string("<gname:uninitialized>");
  }

  gname(const char* s)
    : gname(std::string_view(s)) {}

  gname(std::string_view s)
  {
    m_idx = gnames_pool().register_string(s);
  }

  gname(const std::string& s)
    : gname(std::string_view(s))
  {
  }

  gname(const gname&) = default;
  gname& operator=(const gname&) = default;

  friend inline bool operator<(const gname& a, const gname& b) {
    return a.strv() < b.strv();
  }

  friend inline bool operator==(const gname& a, const gname& b) {
    return a.m_idx == b.m_idx;
  }

  friend inline bool operator!=(const gname& a, const gname& b) {
    return !(a == b);
  }

  // Returned string_view has static storage duration
  std::string_view strv() const
  {
    return gnames_pool().at(m_idx);
  }

  const char* c_str() const
  {
    // string_pool guarantees a null terminator after the view.
    return gnames_pool().at(m_idx).data();
  }

  //std::string str() const
  //{
  //  return std::string(strv());
  //}

  uint32_t idx() const { return m_idx; }

protected:
  static stringpool& gnames_pool()
  {
    static stringpool instance;
    return instance;
  }

  uint32_t m_idx;
};


inline void to_json(nlohmann::json& j, const gname& csn)
{
  j = csn.strv();
}

inline void from_json(const nlohmann::json& j, gname& csn)
{
  csn = gname(j.get<std::string>());
}


} // namespace cp

namespace std {

template <>
struct hash<cp::gname>
{
  std::size_t operator()(const cp::gname& k) const
  {
    return hash<uint32_t>()(k.idx());
  }
};

} // namespace std

