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
#ifndef AIRMAP_QT_TELEMETRY_H_
#define AIRMAP_QT_TELEMETRY_H_

#include <airmap/client.h>
#include <airmap/qt/dispatcher.h>
#include <airmap/telemetry.h>

#include <memory>

namespace airmap {
namespace qt {

class Telemetry : public airmap::Telemetry, public std::enable_shared_from_this<Telemetry> {
 public:
  static std::shared_ptr<Telemetry> create(const std::shared_ptr<Dispatcher>& dispatcher,
                                           const std::shared_ptr<airmap::Client>& client);

  void submit_updates(const Flight& flight, const std::string& key,
                      const std::initializer_list<Update>& updates) override;

 private:
  explicit Telemetry(const std::shared_ptr<Dispatcher>& dispatcher, const std::shared_ptr<airmap::Client>& client);

  std::shared_ptr<Dispatcher> dispatcher_;
  std::shared_ptr<airmap::Client> client_;
};

}  // namespace qt
}  // namespace airmap

#endif  // AIRMAP_QT_TELEMETRY_H_
