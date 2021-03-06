// AirMap Platform SDK
// Copyright © 2018 AirMap, Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//   http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#ifndef AIRMAP_CODEC_JSON_DATE_TIME_H_
#define AIRMAP_CODEC_JSON_DATE_TIME_H_

#include <airmap/date_time.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <nlohmann/json.hpp>

#include <string>

namespace airmap {

void from_json(const nlohmann::json& j, airmap::DateTime& dt);
void to_json(nlohmann::json& j, const airmap::DateTime& dt);

}  // namespace airmap

namespace boost {
namespace posix_time {

void from_json(const nlohmann::json& j, seconds& s);
void from_json(const nlohmann::json& j, ptime& dt);
void to_json(nlohmann::json& j, const ptime& dt);

}  // namespace posix_time
}  // namespace boost

namespace nlohmann {
template <>
struct adl_serializer<airmap::Seconds> {
  static airmap::Seconds from_json(const json& j);
};
}  // namespace nlohmann

#endif  // AIRMAP_CODEC_JSON_DATE_TIME_H_
