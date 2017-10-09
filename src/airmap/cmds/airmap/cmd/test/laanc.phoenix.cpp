#include <airmap/cmds/airmap/cmd/test/laanc.phoenix.h>

#include <airmap/codec.h>

namespace laanc = airmap::cmds::airmap::cmd::test::laanc;

namespace {

constexpr const char* component{"laanc::Suite::test"};

}  // namespace

namespace ph = std::placeholders;

void laanc::Suite::run(const std::shared_ptr<Logger>& logger, const std::shared_ptr<::airmap::Client>& client,
                         const std::shared_ptr<::airmap::Context>& context, const ::airmap::Token& token) {
  log_     = util::FormattingLogger{logger};
  client_  = client;
  context_ = context;
  token_   = token;

  query_pilot();
}

void laanc::Suite::query_pilot() {
  Pilots::Authenticated::Parameters parameters;
  parameters.authorization = token_.id();

  client_->pilots().authenticated(parameters, std::bind(&Suite::handle_query_pilot_finished, this, ph::_1));
}

void laanc::Suite::handle_query_pilot_finished(const Pilots::Authenticated::Result& result) {
  if (result) {
    log_.infof(component, "successfully queried pilot profile");
    pilot_ = result.value();
    query_aircrafts();
  } else {
    try {
      std::rethrow_exception(result.error());
    } catch (const std::exception& e) {
      log_.errorf(component, "failed to query pilot profile: %s", e.what());
    } catch (...) {
      log_.errorf(component, "failed to query pilot profile");
    }
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}

void laanc::Suite::query_aircrafts() {
  Pilots::Aircrafts::Parameters parameters;
  parameters.authorization = token_.id();
  parameters.id            = pilot_.get().id;

  client_->pilots().aircrafts(parameters, std::bind(&Suite::handle_query_aircrafts_finished, this, ph::_1));
}

void laanc::Suite::handle_query_aircrafts_finished(const Pilots::Aircrafts::Result& result) {
  if (result) {
    log_.infof(component, "successfully queried pilot profile for aircrafts");
    if (result.value().empty()) {
      log_.errorf(component, "cannot plan flight, no aircraft associated with pilot");
      context_->stop(::airmap::Context::ReturnCode::error);
    } else {
      aircraft_ = result.value().front();
      plan_flight();
    }
  } else {
    try {
      std::rethrow_exception(result.error());
    } catch (const std::exception& e) {
      log_.errorf(component, "failed to query pilot profile for aircrafts: %s", e.what());
    } catch (...) {
      log_.errorf(component, "failed to query pilot profile for aircrafts");
    }
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}

void laanc::Suite::plan_flight() {
  client_->flight_plans().create_by_polygon(parameters(),
                                            std::bind(&Suite::handle_plan_flight_finished, this, ph::_1));
}

void laanc::Suite::handle_plan_flight_finished(const FlightPlans::Create::Result& result) {
  if (result) {
    log_.infof(component, "successfully created flight plan");
    render_briefing(result.value().id);
  } else {
    try {
      std::rethrow_exception(result.error());
    } catch (const std::exception& e) {
      log_.errorf(component, "failed to create flight: %s", e.what());
    } catch (...) {
      log_.errorf(component, "failed to create flight");
    }
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}

void laanc::Suite::render_briefing(const FlightPlan::Id& id) {
  FlightPlans::RenderBriefing::Parameters parameters;
  parameters.id            = id;
  parameters.authorization = token_.id();

  client_->flight_plans().render_briefing(parameters,
                                          std::bind(&Suite::handle_render_briefing_finished, this, ph::_1, id));
}

void laanc::Suite::handle_render_briefing_finished(const FlightPlans::RenderBriefing::Result& result,
                                                     const FlightPlan::Id& id) {
  if (result) {
    log_.infof(component, "successfully rendered flight brief");
    submit_flight_plan(id);
  } else {
    try {
      std::rethrow_exception(result.error());
    } catch (const std::exception& e) {
      log_.errorf(component, "failed to render flight briefing: %s", e.what());
    } catch (...) {
      log_.errorf(component, "failed to render flight briefing");
    }
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}

void laanc::Suite::submit_flight_plan(const FlightPlan::Id& id) {
  FlightPlans::Submit::Parameters parameters;
  parameters.id            = id;
  parameters.authorization = token_.id();

  client_->flight_plans().submit(parameters, std::bind(&Suite::handle_submit_flight_plan_finished, this, ph::_1));
}

void laanc::Suite::handle_submit_flight_plan_finished(const FlightPlans::Submit::Result& result) {
  if (result) {
    log_.infof(component, "successfully submitted flight plan");
    context_->stop();
  } else {
    try {
      std::rethrow_exception(result.error());
    } catch (const std::exception& e) {
      log_.errorf(component, "failed to submit flight plan: %s", e.what());
    } catch (...) {
      log_.errorf(component, "failed to submit flight plan");
    }
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}

airmap::FlightPlans::Create::Parameters laanc::Phoenix::parameters() {
  static constexpr const char* json          = R"_(
    {
        "takeoff_longitude": -118.364180570977,
        "takeoff_latitude": 34.0168307437243,
        "max_altitude_agl": 100,
        "min_altitude_agl": 1,
        "geometry": {
            "type": "Polygon",
            "coordinates": [
                [
                    [
                        -118.364180570977,
                        34.0168307437243
                    ],
                    [
                        -118.365628044777,
                        34.0026580160048
                    ],
                    [
                        -118.370366665279,
                        33.9889926626396
                    ],
                    [
                        -118.378212913785,
                        33.9763595096654
                    ],
                    [
                        -118.388864275762,
                        33.9652435148409
                    ],
                    [
                        -118.401911015266,
                        33.9560712113537
                    ],
                    [
                        -118.416851982014,
                        33.9491944147928
                    ],
                    [
                        -118.433113834775,
                        33.9448768046356
                    ],
                    [
                        -118.450072947987,
                        33.9432838806463
                    ],
                    [
                        -118.467079178993,
                        33.9444766667622
                    ],
                    [
                        -118.483480612439,
                        33.9484093948682
                    ],
                    [
                        -118.498648367065,
                        33.954931253147
                    ],
                    [
                        -118.512000548545,
                        33.9637921333616
                    ],
                    [
                        -118.523024460256,
                        33.9746521633944
                    ],
                    [
                        -118.531296241358,
                        33.9870946705171
                    ],
                    [
                        -118.536497187531,
                        34.0006420919084
                    ],
                    [
                        -118.538426122695,
                        34.0147742363959
                    ],
                    [
                        -118.537007327878,
                        34.0289482094587
                    ],
                    [
                        -118.532293692779,
                        34.0426192459108
                    ],
                    [
                        -118.524464932309,
                        34.055261654524
                    ],
                    [
                        -118.513820898597,
                        34.0663890684601
                    ],
                    [
                        -118.500770211903,
                        34.0755732160792
                    ],
                    [
                        -118.48581462341,
                        34.0824604786282
                    ],
                    [
                        -118.469529700366,
                        34.086785583271
                    ],
                    [
                        -118.45254258058,
                        34.0883818892682
                    ],
                    [
                        -118.435507670481,
                        34.0871878577985
                    ],
                    [
                        -118.419081251578,
                        34.0832494465733
                    ],
                    [
                        -118.403896008951,
                        34.0767183326201
                    ],
                    [
                        -118.390536499309,
                        34.0678460332763
                    ],
                    [
                        -118.379516534899,
                        34.056974159122
                    ],
                    [
                        -118.371259375595,
                        34.0445211860435
                    ],
                    [
                        -118.366081499672,
                        34.030966270202
                    ],
                    [
                        -118.364180570977,
                        34.0168307437243
                    ]
                ]
            ]
        },
        "buffer": 100,
        "rulesets": ["usa_part_107"]
    }
  )_";
  FlightPlans::Create::Parameters parameters = nlohmann::json::parse(json);
  parameters.authorization                   = token_.id();
  parameters.pilot                           = pilot_.get();
  parameters.aircraft                        = aircraft_.get();
  parameters.start_time                      = Clock::universal_time();
  parameters.end_time                        = parameters.start_time + Minutes{5};
  return parameters;
}