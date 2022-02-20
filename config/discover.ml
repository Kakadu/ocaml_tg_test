[@@@warning "-26-27"]
open Format
module C = Configurator.V1

let () =
  C.main ~name:"foo" (fun c ->
    (* let has_clock_gettime =
      C.c_test c clock_gettime_code ~link_flags:["-lrt"] in *)

    (* C.C_define.gen_header_file c ~fname:"config.h"
      [ "HAS_CLOCK_GETTIME", Switch has_clock_gettime ]; *)

    let open C.Pkg_config in
    let pkgc = Option.get (get c) in
    let {libs; cflags} = (query pkgc ~package:"tdapi tdclient") |> Option.get in
    let prefix =
      (C.Process.run c "opam" ["var";"prefix"]).stdout |> String.trim
    in
    let lib =
      (C.Process.run c "opam" ["var";"lib"]).stdout |> String.trim
    in
    (* let cflags = Format.sprintf "-I%s" prefix :: cflags in *)
    (* C.Flags.write_sexp "td.libs.sexp" libs; *)
    C.Flags.write_lines "Iprefix.txt" [String.trim @@ sprintf "-I%s/include" prefix];
    C.Flags.write_lines "Llib.txt" [String.trim @@ sprintf "-L%s" lib];
    C.Flags.write_lines "lib.txt" [lib];
    (* C.Flags.write_lines "prefix.txt" [String.trim prefix^"/include"]; *)
    C.Flags.write_lines "libs.txt" libs;
    (* C.Flags.write_sexp "td.cflags.sexp" cflags; *)
    ()
  );
