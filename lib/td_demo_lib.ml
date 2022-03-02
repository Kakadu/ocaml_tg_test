open Format

let () = Format.set_margin 1000

type id_t = int [@@deriving show]

module MessageText = struct
  type t = { text : string [@printer Format.pp_print_string] }
  [@@deriving show { with_path = false }]
end

module MessageAudio = struct
  type t = { caption : string } [@@deriving show { with_path = false }]
end

module MessageCall = struct
  type t = { duration : int } [@@deriving show { with_path = false }]
end

module MessageDocument = struct
  type t = { stub : unit } [@@deriving show { with_path = false }]
end

module MessagePhoto = struct
  type t = { stub : unit } [@@deriving show { with_path = false }]
end

module MessageChatChangeTitle = struct
  type t = { stub : unit } [@@deriving show { with_path = false }]
end

module MessageChatDeleteMember = struct
  type t = { user_id : id_t } [@@deriving show { with_path = false }]
end

module MessageSupergroupChatCreate = struct
  type t = { stub : unit } [@@deriving show { with_path = false }]
end

module MessageSticker = struct
  type t = { stub : unit } [@@deriving show { with_path = false }]
end

module MessageVideo = struct
  type t = { stub : unit } [@@deriving show { with_path = false }]
end

module MessageChatJoinByLink = struct
  type t = { stub : unit } [@@deriving show { with_path = false }]
end

module MessageChatAddMembers = struct
  type t = { stub : unit } [@@deriving show { with_path = false }]
end

type message_content =
  | MessageUnsupported
  | MessageText of MessageText.t
  | MessageAudio of MessageAudio.t
  | MessageCall of MessageCall.t
  | MessageDocument of MessageDocument.t
  | MessageChatDeleteMember of MessageChatDeleteMember.t
  | MessagePhoto of MessagePhoto.t
  | MessageChatChangeTitle of MessageChatChangeTitle.t
  | MessageSupergroupChatCreate of MessageSupergroupChatCreate.t
  | MessageSticker of MessageSticker.t
  | MessageVideo of MessageVideo.t
  | MessageChatJoinByLink of MessageChatJoinByLink.t
  | MessageChatAddMembers of MessageChatAddMembers.t
[@@deriving show { with_path = false }]

module Message = struct
  type t = { content : message_content; id : id_t; chat_id : id_t }
  [@@deriving show { with_path = false }]
end

module UpdateUser = struct
  type t = {
    id : id_t;
    first_name : string; [@printer Format.pp_print_string]
    last_name : string; [@printer Format.pp_print_string]
    username : string;
    phone_number : string;
  }
  [@@deriving show { with_path = false }]
end

module Update_New_Message = struct
  type t = { msg : Message.t } [@@deriving show { with_path = false }]
end

module Chat_Last_Message = struct
  type t = { chat_id : id_t; msg : Message.t }
  [@@deriving show { with_path = false }]
end

module Update_Chat_Title = struct
  type t = { chat_id : id_t; ttile : string }
  [@@deriving show { with_path = false }]
end

module UpdateSuperGroup = struct
  type t = { id : id_t; username : string }
  [@@deriving show { with_path = false }]
end

module UpdateNewChat = struct
  type t = { id : id_t; title : string } [@@deriving show { with_path = false }]
end

(** https://core.telegram.org/tdlib/docs/classtd_1_1td__api_1_1update_chat_position.html *)
module UpdateChatPosition = struct
  type t = { id : id_t } [@@deriving show { with_path = false }]
end

module UpdateUserStatus = struct
  type t = { user_id : id_t } [@@deriving show { with_path = false }]
end

type chat_last_message

type event_info =
  | UpdateUser of UpdateUser.t
  | Update_New_Message of Update_New_Message.t
  | Chat_Last_Message of Chat_Last_Message.t
  | Update_Chat_Title of Update_Chat_Title.t
  | UpdateSuperGroup of UpdateSuperGroup.t
  | UpdateNewChat of UpdateNewChat.t
  | UpdateChatPosition of UpdateChatPosition.t
  | UpdateUserStatus of UpdateUserStatus.t
[@@deriving show { with_path = false }]

let (x : event_info React.event), send_x = React.E.create ()

let _ =
  React.E.map
    (function
      | Update_New_Message
          { msg = { content = MessageText { MessageText.text; _ }; _ } } ->
          Format.printf "text = %S\n%!" text
      | UpdateSuperGroup _ | UpdateChatPosition _ -> ()
      | e -> print_endline (show_event_info e))
    x

let () = Callback.register "td_spawn_event" (fun e -> send_x e)

external tg_loop : unit -> unit = "caml_loop"
