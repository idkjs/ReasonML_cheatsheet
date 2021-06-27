#!/bin/bash
dune build @all && _build/default/cheatsheet.exe && echo '-----' && ocamlformat -m 100 -i cheatsheet.re && dune build @all && _build/default/cheatsheet.exe
