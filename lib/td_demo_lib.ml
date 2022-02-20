open Format

type id_t = int [@@deriving show]

module MessageText = struct
  type t = { text : string } [@@deriving show { with_path = false }]
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

(*
let pp_message_content ppf = function
  | MessageUnsupported -> fprintf ppf "MessageUnsupported"
  | MessageText { MessageText.text } -> fprintf ppf "MessageText _"
 *)
module Message = struct
  type t = { content : message_content; id : id_t; chat_id : id_t }
  [@@deriving show { with_path = false }]

  (* let pp ppf msg =
     printf "is_block = %b\n%!" Obj.(is_block @@ repr msg);
     printf "size = %d\n%!" Obj.(size @@ repr msg);
     let { content; id; chat_id } = msg in
     fprintf ppf " { id = %d, chat_id = %d } " id chat_id *)
end

module UpdateUser = struct
  type t = {
    id : id_t;
    first_name : string;
    last_name : string;
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
    (fun e ->
      print_endline (show_event_info e);
      (* let () =
           match e with
           | A _ -> ()
           | B m -> Update_New_Message.chat_id m
           | C _ -> ()
           | D _ -> ()
           | UpdateSuperGroup _ -> ()
         in *)
      ())
    x

let () = Callback.register "td_spawn_event" (fun e -> send_x e)

external tg_loop : unit -> unit = "caml_loop"
