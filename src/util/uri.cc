/**
 * This file is part of the "FnordMetric" project
 *   Copyright (c) 2011-2014 Paul Asmuth, Google Inc.
 *
 * FnordMetric is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include <fnordmetric/util/runtimeexception.h>
#include <fnordmetric/util/uri.h>

namespace fnordmetric {
namespace util {

URI::URI(const std::string& uri_str) : port_(0) {
  parse(uri_str);
}

void URI::parse(const std::string& uri_str) {
  const char* begin = uri_str.c_str();
  const char* end = begin + uri_str.size();

  /* scheme */
  for (const char* cur = begin; cur < end; ++cur) {
    if (cur[0] == ':') {
      scheme_ = std::string(begin, cur - begin);
      begin = cur + 1;
      break;
    }
  }

  if (scheme_.size() == 0) {
    RAISE(util::RuntimeException, "invalid URI: must begin with scheme:");
  }

  /* authority */
  if (begin < end - 2 && begin[0] == '/' && begin[1] == '/') {
    begin += 2;
    const char* cur = begin;
    for (; cur < end && *cur != '/' && *cur != '?' && *cur != '#'; ++cur);
    if (cur > begin) {
      parseAuthority(std::string(begin, cur - begin));
    }
    begin = cur;
  }

  /* path */
  if (begin < end) {
    const char* cur = begin;
    for (; cur < end && *cur != '?' && *cur != '#'; ++cur);
    if (cur > begin) {
      path_ = std::string(begin, cur - begin);
    }
    begin = cur;
  }

  /* query */
  if (begin < end && *begin == '?') {
    const char* cur = ++begin;
    for (; cur < end && *cur != '#'; ++cur);
    if (cur > begin) {
      query_ = std::string(begin, cur - begin);
    }
    begin = cur;
  }

  /* fragment */
  if (begin < end - 1 && *begin == '#') {
    fragment_ = std::string(begin + 1, end - begin - 1);
  }
}

void URI::parseAuthority(const std::string& authority) {
  const char* begin = authority.c_str();
  const char* end = begin + authority.size();

  /* userinfo */
  for (const char* cur = begin; cur < end; ++cur) {
    if (*cur == '/' || *cur == '?' || *cur == '#') {
      break;
    }

    if (*cur == '@') {
      userinfo_ = std::string(begin, cur - begin);
      begin = cur + 1;
      break;
    }
  }

  /* host */
  const char* cur = begin;
  for (; cur < end &&
        *cur != '/' &&
        *cur != '?' &&
        *cur != '#' &&
        *cur != ':'; ++cur);
  host_ = std::string(begin, cur - begin);

  /* port */
  if (cur < end - 1 && *cur == ':') {
    begin = ++cur;
    for (; *cur >= '0' && *cur <= '9'; ++cur);
    if (cur > begin) {
      port_ = std::stoi(std::string(begin, cur - begin));
    }
  }
}

const std::string& URI::scheme() const {
  return scheme_;
}

const std::string& URI::userinfo() const {
  return userinfo_;
}

const std::string& URI::host() const {
  return host_;
}

const unsigned URI::port() const {
  return port_;
}

const std::string& URI::path() const {
  return path_;
}

const std::string& URI::query() const {
  return query_;
}

const std::string& URI::fragment() const {
  return fragment_;
}

std::string URI::toString() const {
  std::string uri_str;

  uri_str.append(scheme());
  uri_str.append(":");

  if (host_.size() > 0) {
    uri_str.append("//");

    if (userinfo_.size() > 0) {
      uri_str.append(userinfo_);
      uri_str.append("@");
    }

    uri_str.append(host_);

    if (port_ > 0) { // FIXPAUL hasPort
      uri_str.append(":");
      uri_str.append(std::to_string(port_));
    }
  }

  uri_str.append(path_);

  if (query_.size() > 0) {
    uri_str.append("?");
    uri_str.append(query_);
  }

  if (fragment_.size() > 0) {
    uri_str.append("#");
    uri_str.append(fragment_);
  }

  return uri_str;
}

}
}