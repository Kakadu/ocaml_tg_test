#include <td/telegram/Client.h>
#include <map>
#include <functional>
#include <sstream>
#include <iostream>
#include <string.h>
#include <cassert>

extern "C" {
#include "caml/memory.h"
#include "caml/mlvalues.h"
#include "caml/alloc.h"
#include "caml/callback.h"
#include "caml/threads.h"
#include "caml/custom.h"
}


namespace td_api = td::td_api;

value caml_set_verbosity_level(value level)
{
  CAMLparam1(level);
  td::ClientManager::execute(td_api::make_object<td_api::setLogVerbosityLevel>( Int_val(level) ));
  CAMLreturn(Val_unit);
}

value caml_new_client_manager(value unit)
{
  CAMLparam1(unit);
  CAMLlocal1(ans);
  auto _ans = (value) new td::ClientManager();
  ans = caml_alloc_small(1, Abstract_tag);
  Store_field(ans, 0, _ans);
  CAMLreturn(ans);
}

// overloaded
namespace detail {
template <class... Fs>
struct overload;

template <class F>
struct overload<F> : public F {
  explicit overload(F f) : F(f) {
  }
};
template <class F, class... Fs>
struct overload<F, Fs...>
    : public overload<F>
    , public overload<Fs...> {
  overload(F f, Fs... fs) : overload<F>(f), overload<Fs...>(fs...) {
  }
  using overload<F>::operator();
  using overload<Fs...>::operator();
};
}  // namespace detail

template <class... F>
auto overloaded(F... f) {
  return detail::overload<F...>(f...);
}


/* ****************************************************************** */

#define MAKE_WRAPPER2(ns, tname, ops_name) \
  value caml_wrap_ ## tname(td::td_api::object_ptr<td_api::tname> &_obj)\
  {\
    CAMLparam0();\
    CAMLlocal1(ans);\
    ans = caml_alloc_custom(&ops_name, sizeof( td_api::tname ), 0, 1);\
    memcpy( Data_custom_val(ans), _obj.get(), sizeof(td_api::tname) ); \
    *((td_api::tname)Data_custom_val(ans)) = std::move( *_obj );\
    CAMLreturn(ans);\
  }
#define MAKE_WRAPPER(tname, ops_name) \
  value caml_wrap_ ## tname(td_api::tname &_obj)\
  {\
    CAMLparam0();\
    CAMLlocal1(ans);\
    ans = caml_alloc_custom(&ops_name, sizeof(_obj), 0, 1);\
    /* *((td_api::tname*)Data_custom_val(ans)) = _obj; */ \
    *((td_api::tname*)Data_custom_val(ans)) = std::move( _obj );\
    CAMLreturn(ans);\
  }
#define MAKE_WRAPPER3(tname, sz, tag, extra) \
  value caml_wrap_ ## tname(td_api::tname &_obj)\
  {\
    CAMLparam0();\
    CAMLlocal2(ans, arg);\
    ans = caml_alloc_small(1, tag);\
    arg = caml_alloc_tuple(sz);\
    Store_field(ans, 0, arg);\
    if (true) extra\
    CAMLreturn(ans);\
  }

#define UCONSTR_MSG_CONTENT_MSG_TEXT 0
#define UCONSTR_MSG_CONTENT_MSG_AUDIO 1
#define UCONSTR_MSG_CONTENT_MSG_CALL 2
#define UCONSTR_MSG_CONTENT_MSG_DOCUMENT 3
#define UCONSTR_MSG_CONTENT_MSG_DELETE_MEMBNER 4
#define UCONSTR_MSG_CONTENT_MSG_PHOTO 5
#define UCONSTR_MSG_CONTENT_CHAT_CHANGE_TITLE 6
#define UCONSTR_MSG_CONTENT_SUPERGROUP_CHAT_CREATE 7
#define UCONSTR_MSG_CONTENT_STICKER 8
#define UCONSTR_MSG_CONTENT_VIDEO 9
#define UCONSTR_MSG_CONTENT_CHAT_JOINED_BY_LINK 10
#define UCONSTR_MSG_CONTENT_CHAT_ADD_MEMBERS 11



MAKE_WRAPPER3(messageText, 1, UCONSTR_MSG_CONTENT_MSG_TEXT, {
  Store_field(arg, 0, caml_copy_string( _obj.text_->text_.c_str() ) );
})
MAKE_WRAPPER3(messageAudio, 1, UCONSTR_MSG_CONTENT_MSG_AUDIO, {
  Store_field(arg, 0, caml_copy_string( _obj.caption_->text_.c_str() ) );
})
MAKE_WRAPPER3(messageCall, 1, UCONSTR_MSG_CONTENT_MSG_CALL, {
  Store_field(arg, 0, Val_int( _obj.duration_ ) );
})
MAKE_WRAPPER3(messageDocument, 1, UCONSTR_MSG_CONTENT_MSG_DOCUMENT, {
  Store_field(arg, 0, Val_unit );
})
MAKE_WRAPPER3(messageChatDeleteMember, 1, UCONSTR_MSG_CONTENT_MSG_DELETE_MEMBNER, {
  Store_field(arg, 0, Val_int( _obj.user_id_ ) );
})
MAKE_WRAPPER3(messagePhoto, 1, UCONSTR_MSG_CONTENT_MSG_PHOTO, {
  Store_field(arg, 0, Val_unit );
})
MAKE_WRAPPER3(messageChatChangeTitle, 1, UCONSTR_MSG_CONTENT_CHAT_CHANGE_TITLE, {
  Store_field(arg, 0, Val_unit );
})
MAKE_WRAPPER3(messageSupergroupChatCreate, 1, UCONSTR_MSG_CONTENT_SUPERGROUP_CHAT_CREATE, {
  Store_field(arg, 0, Val_unit );
})
MAKE_WRAPPER3(messageSticker, 1, UCONSTR_MSG_CONTENT_STICKER, {
  Store_field(arg, 0, Val_unit );
})
MAKE_WRAPPER3(messageVideo, 1, UCONSTR_MSG_CONTENT_VIDEO, {
  Store_field(arg, 0, Val_unit );
})
MAKE_WRAPPER3(messageChatJoinByLink, 1, UCONSTR_MSG_CONTENT_CHAT_JOINED_BY_LINK, {
  Store_field(arg, 0, Val_unit );
})

MAKE_WRAPPER3(messageChatAddMembers, 1, UCONSTR_MSG_CONTENT_CHAT_ADD_MEMBERS, {
  Store_field(arg, 0, Val_unit );
})

/* *                *                  * ******************** */
value caml_wrap_message(td_api::object_ptr< td_api::message > &_msg)
{
  CAMLparam0();
  CAMLlocal2(ans, content);
  caml_acquire_runtime_system();
  // std::cout << "Unsupported: " << to_string(_msg) << "\n";
  ans = caml_alloc_tuple(3);
  Store_field( ans, 0, Val_int(0) );
  Store_field( ans, 1, Val_int( _msg->id_ ) );
  Store_field( ans, 2, Val_int( _msg->chat_id_ ) );

  #define WRAP_MSG(tag, typ) \
    content = caml_alloc_small(1, tag); \
    Store_field( content, 0, caml_wrap_ ## typ( msg ));

  td_api::downcast_call(
    *(_msg->content_),
    overloaded(
      [&_msg, &content](td_api::messageText &msg) {
        // std::cout << "SUPPORTED: " << to_string(_msg) << "\n";
        WRAP_MSG(UCONSTR_MSG_CONTENT_MSG_TEXT, messageText)
      },
      [&_msg, &content](td_api::messageAudio &msg) {
        // std::cout << "SUPPORTED: " << to_string(_msg) << "\n";
        WRAP_MSG(UCONSTR_MSG_CONTENT_MSG_AUDIO, messageAudio)
      },
      [&_msg, &content](td_api::messageCall &msg) {
        // std::cout << "SUPPORTED: " << to_string(_msg) << "\n";
        WRAP_MSG(UCONSTR_MSG_CONTENT_MSG_CALL, messageCall)
      },
      [&_msg, &content](td_api::messageDocument &msg) {
        // std::cout << "SUPPORTED: " << to_string(_msg) << "\n";
        WRAP_MSG(UCONSTR_MSG_CONTENT_MSG_DOCUMENT, messageDocument)
      },
      [&_msg, &content](td_api::messageChatDeleteMember &msg) {
        // std::cout << "SUPPORTED: " << to_string(_msg) << "\n";
        WRAP_MSG(UCONSTR_MSG_CONTENT_MSG_DELETE_MEMBNER, messageChatDeleteMember)
      },
      [&_msg, &content](td_api::messagePhoto &msg) {
        // std::cout << "SUPPORTED: " << to_string(_msg) << "\n";
        WRAP_MSG(UCONSTR_MSG_CONTENT_MSG_PHOTO, messagePhoto)
      },
      [&_msg, &content](td_api::messageChatChangeTitle &msg) {
        // std::cout << "SUPPORTED: " << to_string(_msg) << "\n";
        WRAP_MSG(UCONSTR_MSG_CONTENT_CHAT_CHANGE_TITLE, messageChatChangeTitle)
      },
      [&_msg, &content](td_api::messageSupergroupChatCreate  &msg) {
        // std::cout << "SUPPORTED: " << to_string(_msg) << "\n";
        WRAP_MSG(UCONSTR_MSG_CONTENT_SUPERGROUP_CHAT_CREATE, messageSupergroupChatCreate)
      },
      [&_msg, &content](td_api::messageSticker  &msg) {
        // std::cout << "SUPPORTED: " << to_string(_msg) << "\n";
        WRAP_MSG(UCONSTR_MSG_CONTENT_STICKER, messageSticker)
      },
      [&_msg, &content](td_api::messageVideo  &msg) {
        // std::cout << "SUPPORTED: " << to_string(_msg) << "\n";
        WRAP_MSG(UCONSTR_MSG_CONTENT_VIDEO, messageVideo)
      },
      [&_msg, &content](td_api::messageChatJoinByLink  &msg) {
        // std::cout << "SUPPORTED: " << to_string(_msg) << "\n";
        WRAP_MSG(UCONSTR_MSG_CONTENT_CHAT_JOINED_BY_LINK, messageChatJoinByLink)
      },
      [&_msg, &content](td_api::messageChatAddMembers  &msg) {
        // std::cout << "SUPPORTED: " << to_string(_msg) << "\n";
        WRAP_MSG(UCONSTR_MSG_CONTENT_CHAT_ADD_MEMBERS, messageChatAddMembers)
      },
      [&_msg, &content](auto &) {
        content = Val_int(0);
        std::cout << "Unsupported: " << to_string(_msg) << "\n";
      }
    ));
  Store_field( ans, 0, content );
  assert(Is_block(ans));
  assert(Is_block(ans));
  caml_release_runtime_system();
  CAMLreturn(ans);
}

#define UCONSTR_UPDATE_USER 0
#define UCONSTR_NEW_MSG 1
#define UCONSTR_CHAT_LAST_MESSAGE 2
#define UCONSTR_UPDATE_CHAT_TITLE 3
#define UCONSTR_UPDATE_SUPERGROUP 4
#define UCONSTR_UPDATE_NEW_CHAT 5
#define UCONSTR_UPDATE_CHAT_POSITION 6
#define UCONSTR_UPDATE_USER_STATUS 7

MAKE_WRAPPER3(updateUser, 5, UCONSTR_UPDATE_USER, {
  Store_field(arg, 0, Val_int( _obj.user_->id_) );
  Store_field(arg, 1, caml_copy_string( _obj.user_->first_name_.c_str() ) );
  Store_field(arg, 2, caml_copy_string( _obj.user_->last_name_.c_str() ) );
  Store_field(arg, 3, caml_copy_string( _obj.user_->username_.c_str() ) );
  Store_field(arg, 4, caml_copy_string( _obj.user_->phone_number_.c_str() ) );
})
MAKE_WRAPPER3(updateNewMessage, 1, UCONSTR_NEW_MSG, {
  Store_field(arg, 0, caml_wrap_message( _obj.message_ ) );
})
MAKE_WRAPPER3(updateChatLastMessage, 2, UCONSTR_CHAT_LAST_MESSAGE, {
  Store_field(arg, 0, Val_int( _obj.chat_id_ ) );
  Store_field(arg, 1, caml_wrap_message( _obj.last_message_ ) );
})
MAKE_WRAPPER3(updateChatTitle, 2, UCONSTR_UPDATE_CHAT_TITLE, {
  Store_field(arg, 0, Val_int( _obj.chat_id_) );
  Store_field(arg, 1, caml_copy_string( _obj.title_.c_str() ) );
})
MAKE_WRAPPER3(updateSupergroup, 2, UCONSTR_UPDATE_SUPERGROUP, {
  // std::cout << to_string(_obj) << "\n";
  Store_field(arg, 0, Val_int( _obj.supergroup_->id_) );
  Store_field(arg, 1, caml_copy_string( _obj.supergroup_->username_.c_str() ) );
})
MAKE_WRAPPER3(updateNewChat, 2, UCONSTR_UPDATE_NEW_CHAT, {
  // std::cout << to_string(_obj) << "\n";
  Store_field(arg, 0, Val_int( _obj.chat_->id_) );
  Store_field(arg, 1, caml_copy_string( _obj.chat_->title_.c_str() ) );
})
MAKE_WRAPPER3(updateChatPosition, 1, UCONSTR_UPDATE_CHAT_POSITION, {
  Store_field(arg, 0, Val_int( _obj.chat_id_) );
})

MAKE_WRAPPER3(updateUserStatus, 1, UCONSTR_UPDATE_USER_STATUS, {
  Store_field(arg, 0, Val_int( _obj.user_id_) );
})

/* ****************************************************** */
class Hack
{
private:
  using Object = td_api::object_ptr<td_api::Object>;
  std::unique_ptr<td::ClientManager> client_manager_;
  std::int32_t client_id_{0};

  td_api::object_ptr<td_api::AuthorizationState> authorization_state_;
  bool are_authorized_{false};
  bool need_restart_{false};
  std::uint64_t current_query_id_{0};
  std::uint64_t authentication_query_id_{0};

  std::map<std::uint64_t, std::function<void(Object)>> handlers_;
  std::map<std::int64_t, td_api::object_ptr<td_api::user>> users_;
  std::map<std::int64_t, std::string> chat_title_;

public:
  static Hack* instance() {
    static Hack* instance_;
    if (instance_ == nullptr)
      instance_ = new Hack();
    return instance_;
  }

  Hack() {
    td::ClientManager::execute(td_api::make_object<td_api::setLogVerbosityLevel>(1));
    client_manager_ = std::make_unique<td::ClientManager>();
    client_id_ = client_manager_->create_client_id();
    send_query(td_api::make_object<td_api::getOption>("version"), {});
  }

  void send_query(td_api::object_ptr<td_api::Function> f, std::function<void(Object)> handler) {
    auto query_id = next_query_id();
    if (handler) {
      handlers_.emplace(query_id, std::move(handler));
    }
    client_manager_->send(client_id_, query_id, std::move(f));
  }

  std::uint64_t next_query_id() {
    return ++current_query_id_;
  }

  #define PREPARE_SINGLE_ARG(fname, arg)\
    CAMLparam0();\
    caml_acquire_runtime_system(); \
    const value* cb = caml_named_value("td_spawn_event");\
    assert(cb);\
    /* CAMLlocal2(ans, custom1); */ \
    CAMLlocal1(ans); \
    /* ans = caml_alloc_small(1, cname); */ \
    ans = caml_wrap_ ## fname(arg);\
    /* Store_field (ans, 0, custom1); */ \
    caml_callback(*cb, ans);\
    caml_release_runtime_system();\
    CAMLreturn0;

  void process_update(td_api::object_ptr<td_api::Object> root_update) {

    td_api::downcast_call(
      *root_update, overloaded(
        [this](td_api::updateAuthorizationState &update) {
          authorization_state_ = std::move(update.authorization_state_);
          on_authorization_state_update();
        },
        [this](td_api::updateHavePendingNotifications &) {        },
        [this](td_api::updateUnreadMessageCount &) {        },
        [this](td_api::updateChatThemes &) {        },
        [this](td_api::updateSelectedBackground &) {        },
        [this](td_api::updateAnimationSearchParameters &) {        },
        [this](td_api::updateDiceEmojis &) {        },
        [this](td_api::updateChatFilters &) {        },
        [this](td_api::updateScopeNotificationSettings &) {        },
        [this](td_api::updateUnreadChatCount &) {        },
        [this](td_api::updateBasicGroup &) {        },
        [this](td_api::updateSupergroup &arg) {
          // std::cout << __func__ << " " << to_string(update) << "\n";
          PREPARE_SINGLE_ARG(updateSupergroup, arg);
        },
        [this](td_api::updateSupergroupFullInfo &) {        },

        [this](td_api::updateConnectionState &) {        },
        [this](td_api::updateOption &update_new_chat) {        },
        [this](td_api::updateNewChat &update_new_chat) {        },
        [this](td_api::updateChatPosition &arg) {
          // chat_title_[update_new_chat.chat_->id_] = update_new_chat.chat_->title_;
          // std::cout << __func__ << " " << to_string(update) << "\n";
          PREPARE_SINGLE_ARG(updateChatPosition, arg);
        },
        [this](td_api::updateChatTitle &arg) {
          PREPARE_SINGLE_ARG(updateChatTitle, arg);
          // std::cout << to_string(update_chat_title) << "\n";
        },
        [this](td_api::updateChatLastMessage &arg) {
          PREPARE_SINGLE_ARG(updateChatLastMessage, arg);
        },
        [this](td_api::updateUserStatus &arg) {
          PREPARE_SINGLE_ARG(updateUserStatus, arg);
        },
        [this](td_api::updateUser &arg) {
          PREPARE_SINGLE_ARG(updateUser, arg);
          // auto user_id = update_user.user_->id_;
          // users_[user_id] = std::move(update_user.user_);
        },
        [this](td_api::updateMessageEdited &arg) { },
        [this](td_api::updateNewMessage &arg) {
          PREPARE_SINGLE_ARG(updateNewMessage, arg);
  /*         auto chat_id = update_new_message.message_->chat_id_;
          std::string sender_name;
          td_api::downcast_call(*update_new_message.message_->sender_id_,
                                overloaded(
                                    [this, &sender_name](td_api::messageSenderUser &user) {
                                      sender_name = get_user_name(user.user_id_);
                                    },
                                    [this, &sender_name](td_api::messageSenderChat &chat) {
                                      sender_name = get_chat_title(chat.chat_id_);
                                    }));
          std::string text;
          if (update_new_message.message_->content_->get_id() == td_api::messageText::ID) {
            text = static_cast<td_api::messageText &>(*update_new_message.message_->content_).text_->text_;
          }
          std::cout << "Got message: [chat_id:" << chat_id << "] [from:" << sender_name << "] [" << text
                    << "]" << std::endl; */
        },
        [root_update = root_update.get()](td::TlObject &u) {
          std::cout << "Unhandled update, id = " << u.get_id() << std::endl;
          std::cout << __func__ << " " << to_string(*root_update) << "\n";
        }));
  }

  void process_response(td::ClientManager::Response response) {
    if (!response.object) {
      return;
    }
    /* std::cout << response.request_id << " " << to_string(response.object) << std::endl; */
    if (response.request_id == 0) {
      return process_update(std::move(response.object));
    }
    auto it = handlers_.find(response.request_id);
    if (it != handlers_.end()) {
      it->second(std::move(response.object));
      handlers_.erase(it);
    }
  }

  auto create_authentication_query_handler() {
      return [this, id = authentication_query_id_](Object object) {
        if (id == authentication_query_id_) {
          check_authentication_error(std::move(object));
        }
      };
    }

  void on_authorization_state_update() {
    std::cout << __func__ << "\n";
    authentication_query_id_++;
    td_api::downcast_call(
    *authorization_state_,
    overloaded(
        [this](td_api::authorizationStateReady &) {
          are_authorized_ = true;
          std::cout << "Got authorization" << std::endl;
        },
        [this](td_api::authorizationStateLoggingOut &) {
          are_authorized_ = false;
          std::cout << "Logging out" << std::endl;
        },
        [this](td_api::authorizationStateClosing &) { std::cout << "Closing" << std::endl; },
        [this](td_api::authorizationStateClosed &) {
          are_authorized_ = false;
          need_restart_ = true;
          std::cout << "Terminated" << std::endl;
        },
        [this](td_api::authorizationStateWaitCode &) {
          std::cout << "Enter authentication code: \n";
          std::string code;
          std::cin >> code;
          send_query(td_api::make_object<td_api::checkAuthenticationCode>(code),
                      create_authentication_query_handler());
        },
        [this](td_api::authorizationStateWaitRegistration &) {
          std::string first_name;
          std::string last_name;
          std::cout << "Enter your first name: \n";
          std::cin >> first_name;
          std::cout << "Enter your last name: \n";
          std::cin >> last_name;
          send_query(td_api::make_object<td_api::registerUser>(first_name, last_name),
                      create_authentication_query_handler());
        },
        [this](td_api::authorizationStateWaitPassword &) {
          std::cout << "Enter authentication password: \n";
          std::string password;
          std::getline(std::cin, password);
          send_query(td_api::make_object<td_api::checkAuthenticationPassword>(password),
                      create_authentication_query_handler());
        },
        [this](td_api::authorizationStateWaitOtherDeviceConfirmation &state) {
          std::cout << "Confirm this login link on another device: " << state.link_ << std::endl;
        },
        [this](td_api::authorizationStateWaitPhoneNumber &) {
          std::cout << "Enter phone number: \n";
          std::string phone_number;
          std::cin >> phone_number;
          send_query(td_api::make_object<td_api::setAuthenticationPhoneNumber>(phone_number, nullptr),
                      create_authentication_query_handler());
        },
        [this](td_api::authorizationStateWaitEncryptionKey &) {
          std::string key = "";
          std::cout << "Enter encryption key or DESTROY: \n";

          // std::getline(std::cin, key);
          // if (key == "DESTROY") {
          if (false) {
            send_query(td_api::make_object<td_api::destroy>(), create_authentication_query_handler());
          } else {
            send_query(td_api::make_object<td_api::checkDatabaseEncryptionKey>(std::move(key)),
                        create_authentication_query_handler());
          }
        },
        [this](td_api::authorizationStateWaitTdlibParameters &) {
          auto parameters = td_api::make_object<td_api::tdlibParameters>();
          parameters->database_directory_ = "tdlib";
          parameters->use_message_database_ = true;
          parameters->use_secret_chats_ = true;
          parameters->api_id_ = 94575;
          parameters->api_hash_ = "a3406de8d171bb422bb6ddf3bbd800e2";
          parameters->system_language_code_ = "en";
          parameters->device_model_ = "Desktop";
          parameters->application_version_ = "1.0";
          parameters->enable_storage_optimizer_ = true;
          send_query(td_api::make_object<td_api::setTdlibParameters>(std::move(parameters)),
                      create_authentication_query_handler());
        }));
  }

  void check_authentication_error(Object object) {
    if (object->get_id() == td_api::error::ID) {
      auto error = td::move_tl_object_as<td_api::error>(object);
      std::cout << "Error: " << to_string(error) << "\n";
      on_authorization_state_update();
    }
  }

  void loop() {
    CAMLparam0();
    caml_release_runtime_system();

    while (true) {
      if (!are_authorized_) {
        process_response(client_manager_->receive(10));
      } else {
        std::cout << "Enter action [q] quit [u] check for updates and request results [c] show chats [m <chat_id> "
                     "<text>] send message [me] show self [l] logout: "
                  << std::endl;
        std::string line;
        std::getline(std::cin, line);
        std::istringstream ss(line);
        std::string action;
        if (!(ss >> action)) {
          continue;
        }
        if (action == "q") {
          return;
        }
        if (action == "u") {
          std::cout << "Checking for updates..." << std::endl;
          while (true) {
            auto response = client_manager_->receive(0);
            if (response.object) {
              process_response(std::move(response));
            } else {
              break;
            }
          }
        } else if (action == "close") {
          std::cout << "Closing..." << std::endl;
          send_query(td_api::make_object<td_api::close>(), {});
        } else if (action == "me") {
          send_query(td_api::make_object<td_api::getMe>(),
                     [this](Object object) { std::cout << to_string(object) << std::endl; });
        } else if (action == "l") {
          std::cout << "Logging out..." << std::endl;
          send_query(td_api::make_object<td_api::logOut>(), {});
        } else if (action == "m") {
          std::int64_t chat_id;
          ss >> chat_id;
          ss.get();
          std::string text;
          std::getline(ss, text);

          std::cout << "Sending message to chat " << chat_id << "..." << std::endl;
          auto send_message = td_api::make_object<td_api::sendMessage>();
          send_message->chat_id_ = chat_id;
          auto message_content = td_api::make_object<td_api::inputMessageText>();
          message_content->text_ = td_api::make_object<td_api::formattedText>();
          message_content->text_->text_ = std::move(text);
          send_message->input_message_content_ = std::move(message_content);

          send_query(std::move(send_message), {});
        } else if (action == "c") {
          std::cout << "Getting chat list..." << std::endl;
          send_query(td_api::make_object<td_api::getChats>(nullptr, 20), [this](Object object) {
            std::cout << "Got response with chat list" << std::endl;
            if (object->get_id() == td_api::error::ID) {
              std::cout << "an error" << std::endl;
              return;
            }
            auto chats = td::move_tl_object_as<td_api::chats>(object);
            for (auto chat_id : chats->chat_ids_) {
              std::cout << "[chat_id:" << chat_id << "] [title:" << chat_title_[chat_id] << "]" << std::endl;
            }
          });
        } else if (action == "lc") {
          std::cout << "Loading chat list..." << std::endl;
          send_query(td_api::make_object<td_api::loadChats>(nullptr, 20), [this](Object object) {
            std::cout << "Got response with loaded chat list" << std::endl;
            if (object->get_id() == td_api::error::ID) {
              std::cout << "an error" << std::endl;
              return;
            }
          });
        }
      }
    }
  }
};

extern "C" value caml_loop(value manager)
{
  CAMLparam1(manager);
  // auto _man = (td::ClientManager*) Field(manager, 0);

  Hack h;
  h.loop();
  CAMLreturn(Val_unit);
}

// https://github.com/tdlib/td/blob/master/example/cpp/td_example.cpp
