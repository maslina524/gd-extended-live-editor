#define GLZ_ACTION_META(Type)                                                                                          \
    template <>                                                                                                        \
    struct glz::meta<Type> {                                                                                           \
        using T = Type;                                                                                                \
                                                                                                                       \
        static constexpr auto limit_action = [](const T&, const std::string& action) {                                 \
            return action == T::ACTION_NAME;                                                                           \
        };                                                                                                             \
                                                                                                                       \
        static constexpr auto modify =                                                                                 \
                glz::object("action", glz::read_constraint<&T::action, limit_action, "Action does not match">);        \
    };


#define CHECK_ACTION(Type, hdl)                                                                                        \
    if (auto runner = glz::read_json<Type>(msgStr); runner.has_value()) {                                              \
        if (Type::EDITOR_ACTION) {                                                                                     \
            if (!g_inEditor) {                                                                                         \
                g_wsServer->get_con_from_hdl(hdl)->send(                                                               \
                        std::string("{\"status\":\"error\",\"error\":\"Enter the level editor to run this action\"}"), \
                        websocketpp::frame::opcode::text);                                                             \
                if ((*runner).close) {                                                                                 \
                    geode::log::info("CLOSING");                                                                       \
                    g_wsServer->get_con_from_hdl(hdl)->close(1000, "");                                                \
                }                                                                                                      \
                return;                                                                                                \
            }                                                                                                          \
            std::lock_guard lock(g_actionsMutex);                                                                      \
            g_actions.emplace_back(hdl, pro::make_proxy<Runnable>(*runner), runner->close);                            \
        } else {                                                                                                       \
            log::info("not editor action, call without ptr");                                                          \
            auto resp = runner->run(nullptr);                                                                          \
            if (auto json = glz::write_json(resp)) {                                                                   \
                g_wsServer->send(hdl, *json, websocketpp::frame::opcode::text);                                        \
            } else {                                                                                                   \
                g_wsServer->send(hdl, std::string("{\"status\":\"error\",\"error\":\"Could not serialize response\"}"),\
                                 websocketpp::frame::opcode::text);                                                    \
            }                                                                                                          \
            if ((*runner).close) {                                                                                     \
                g_wsServer->close(hdl, 1000, "");                                                                      \
            }                                                                                                          \
            return;                                                                                                    \
        }                                                                                                              \
    }