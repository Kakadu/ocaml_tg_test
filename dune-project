(lang dune 2.9)

(generate_opam_files true)

(use_standard_c_and_cxx_flags true)

(cram enable)

(name td_demo)

(package
 (name td_demo)
 (synopsis "demo")
 (description "demo")
 (depends td_cpp))

(package
 (name td_cpp)
 (maintainers "Kakadu")
 (synopsis "The td library"))
