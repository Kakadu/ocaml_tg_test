### OCaml+Telegram TDLib experiment

Compile with OCaml 4.13.x.

* `opam pin add td_cpp .` will install Telegram's [TDlib](https://github.com/tdlib/td) (implemented in C++). It will probably ask to install some system-wide dependecies (like `cmake`, `pkg-config` etc.)
* Install OCaml dependecies via opam

    opam install ppx_inline_test ppx_deriving

* There are some hardcoded paths in `lib/dune` (like `-I/home/kakadu/.opam/...`). I'm going to fix them soon
* Run `dune exec bin/main.exe`. It will compile an OCaml+C++ executable, which is a modified version of [official example](https://github.com/tdlib/td/blob/master/example/cpp/td_example.cpp).
  * It will ask for your phone
  * You will receive a code via telegram client.
  * The program will start, it will receive a few updates via telegram API and will be responsive for a few commands (try `c+Enter, u+Enter`: it will recieve updates from recent chats)
